#include "fuck_game_server_engine.h"

#ifdef WIN32
void start_profiler(const char* fname)  
{
}
void stop_profiler()  
{  
}
#else

profiledata g_ProfileData;  
      
void start_profiler(const char* fname)  
{  
    g_ProfileData.start(fname);  
      
    // 定时器  
    struct itimerval timer;  
    timer.it_interval.tv_sec = 0;  
    timer.it_interval.tv_usec = 1000000 / 100; // 0.01秒  
    timer.it_value = timer.it_interval;  
    setitimer(ITIMER_PROF, &timer, 0);  
      
    // 回调  
    struct sigaction sa;  
    sa.sa_sigaction = profiler_signal_handler;  
    sa.sa_flags = SA_RESTART | SA_SIGINFO;  
    sigemptyset(&sa.sa_mask);  
    const int signal_number = SIGPROF;  
    sigaction(signal_number, &sa, NULL);  
}  
      
void stop_profiler()  
{  
    // 定时器  
    struct itimerval timer;  
    memset(&timer, 0, sizeof timer);  
    setitimer(ITIMER_PROF, &timer, 0);  
        
    // 回调  
    struct sigaction sa;  
    sa.sa_handler = SIG_IGN;  
    sa.sa_flags = SA_RESTART;  
    sigemptyset(&sa.sa_mask);  
    const int signal_number = SIGPROF;  
    sigaction(signal_number, &sa, NULL);  
      
    g_ProfileData.stop();  
}  
      
void profiledata::add(int depth, const void* const* stack)   
{  
    if (!enabled())   
    {  
        return;  
    }  
          
    if (depth > kMaxStackDepth)   
    {  
        depth = kMaxStackDepth;  
    }  
      
    // Make hash-value  
    slot h = 0;  
    for (int i = 0; i < depth; i++)   
    {  
        slot s = reinterpret_cast<slot>(stack[i]);  
        h = (h << 8) | (h >> (8*(sizeof(h)-1)));  
        h += (s * 31) + (s * 7) + (s * 3);  
    }  
      
    count_++;  
      
    // See if table already has an entry for this trace  
    bool done = false;  
    bucket* bucket = &hash_[h % kBuckets];  
    for (int a = 0; a < kAssociativity; a++)   
    {  
        entry* e = &bucket->entry_[a];  
        if ((int)e->depth == depth)   
        {  
            bool match = true;  
            for (int i = 0; i < depth; i++)   
            {  
                if (e->stack[i] != reinterpret_cast<slot>(stack[i]))   
                {  
                    match = false;  
                    break;  
                }  
            }  
            if (match)   
            {  
                e->count++;  
                done = true;  
                break;  
            }  
        }  
    }  
      
    if (!done)   
    {  
        // evict entry with smallest count  
        entry* e = &bucket->entry_[0];  
        for (int a = 1; a < kAssociativity; a++)   
        {  
            if (bucket->entry_[a].count < e->count)   
            {  
                e = &bucket->entry_[a];  
            }  
        }  
        if (e->count > 0)   
        {  
            evictions_++;  
            evict(*e);  
        }  
      
        // Use the newly evicted entry  
        e->depth = depth;  
        e->count = 1;  
        for (int i = 0; i < depth; i++)   
        {  
            e->stack[i] = reinterpret_cast<slot>(stack[i]);  
        }  
    }  
}  
      
void profiledata::evict(const entry& entry)   
{  
    const int d = entry.depth;  
    const int nslots = d + 2;     // Number of slots needed in eviction buffer  
    if (num_evicted_ + nslots > kBufferLength)   
    {  
        flushevicted();  
    }  
    evict_[num_evicted_++] = entry.count;  
    evict_[num_evicted_++] = d;  
    memcpy(&evict_[num_evicted_], entry.stack, d * sizeof(slot));  
    num_evicted_ += d;  
}  
      
static void fd_write(int fd, const char* buf, size_t len)   
{  
    while (len > 0)   
    {  
        ssize_t r;  
        r = write(fd, buf, len);  
        if (r == -1)  
        {  
            return;  
        }  
        buf += r;  
        len -= r;  
    }  
}  
      
static void dump_proc_selfmaps(int fd)   
{  
    proc_maps_iterator::buffer iterbuf;  
    proc_maps_iterator it(0, &iterbuf);   // 0 means "current pid"  
      
    uint64_t start, end, offset;  
    int64_t inode;  
    char *flags, *filename;  
    proc_maps_iterator::buffer linebuf;  
    while (it.next(&start, &end, &flags, &offset, &inode, &filename))   
    {  
        int written = it.formatline(linebuf.buf_, sizeof(linebuf.buf_),  
                                    start, end, flags, offset, inode, filename,  
                                    0);  
        fd_write(fd, linebuf.buf_, written);  
    }  
}  
      
void profiledata::flushevicted()   
{  
    if (num_evicted_ > 0)   
    {  
        const char* buf = reinterpret_cast<char*>(evict_);  
        size_t bytes = sizeof(evict_[0]) * num_evicted_;  
        total_bytes_ += bytes;  
        fd_write(out_, buf, bytes);  
    }  
    num_evicted_ = 0;  
}  
      
void profiledata::stop()   
{  
    if (!enabled())   
    {  
        return;  
    }  
      
    // Move data from hash table to eviction buffer  
    for (int b = 0; b < kBuckets; b++)   
    {  
        bucket* bucket = &hash_[b];  
        for (int a = 0; a < kAssociativity; a++)   
        {  
            if (bucket->entry_[a].count > 0)   
            {  
                evict(bucket->entry_[a]);  
            }  
        }  
    }  
      
    if (num_evicted_ + 3 > kBufferLength)   
    {  
        // Ensure there is enough room for end of data marker  
        flushevicted();  
    }  
      
    // Write end of data marker  
    evict_[num_evicted_++] = 0;         // count  
    evict_[num_evicted_++] = 1;         // depth  
    evict_[num_evicted_++] = 0;         // end of data marker  
    flushevicted();  
      
    // Dump "/proc/self/maps" so we get list of mapped shared libraries  
    dump_proc_selfmaps(out_);  
      
    reset();  
    fprintf(stderr, "PROFILE: interrupts/evictions/bytes = %d/%d/%zu\n",  
            count_, evictions_, total_bytes_);  
}  
      
void profiledata::reset()   
{  
    if (!enabled())   
    {  
        return;  
    }  
      
    // Don't reset count_, evictions_, or total_bytes_ here.  They're used  
    // by stop to print information about the profile after reset, and are  
    // cleared by start when starting a new profile.  
    close(out_);  
    memset(hash_, 0, sizeof(hash_));  
    memset(evict_, 0, sizeof(evict_)); 
    num_evicted_ = 0;  
    fname_.clear();  
    start_time_ = 0;  
      
    out_ = -1;  
}  
      
profiledata::profiledata()  
    : num_evicted_(0),  
        out_(-1),  
        count_(0),  
        evictions_(0),  
        total_bytes_(0),  
        start_time_(0)   
{  
}  
      
profiledata::~profiledata()   
{  
    stop();  
}  
      
bool profiledata::start(const char* fname)   
{  
    if (enabled())   
    {  
        return false;  
    }  
        
    // Open output file and initialize various data structures  
    int fd = open(fname, O_CREAT | O_WRONLY | O_TRUNC, 0666);  
    if (fd < 0)   
    {  
        // Can't open outfile for write  
        return false;  
    }  
      
    start_time_ = time(NULL);  
    fname_ = fname;  
      
    // reset counters  
    num_evicted_ = 0;  
    count_       = 0;  
    evictions_   = 0;  
    total_bytes_ = 0;  
      
    memset(hash_, 0, sizeof(hash_[0]) * kBuckets);  
      
    // Record special entries  
    evict_[num_evicted_++] = 0;                     // count for header  
    evict_[num_evicted_++] = 3;                     // depth for header  
    evict_[num_evicted_++] = 0;                     // Version number  
          
    int period = 1000000 / 100;  
    evict_[num_evicted_++] = period;                // Period (microseconds)  
    evict_[num_evicted_++] = 0;                     // Padding  
      
    out_ = fd;  
      
    return true;  
}  
      
void profiler_signal_handler(int sig, siginfo_t* sinfo, void* ucontext)  
{  
    const int maxLevel = 200;      
    void* buffer[maxLevel];      
    int level = backtrace(buffer, maxLevel);  
      
    g_ProfileData.add(level - 2, buffer + 2);  
}  
      
static void constructfilename(const char* spec, pid_t pid, char* buf, int buf_size)  
{  
    snprintf(buf, buf_size, spec, static_cast<int>(pid ? pid : getpid()));  
}  
      
proc_maps_iterator::proc_maps_iterator(pid_t pid, buffer *buffer)   
{  
    init(pid, buffer, false);  
}  
      
void proc_maps_iterator::init(pid_t pid, buffer *b, bool use_maps_backing)   
{  
    pid_ = pid;  
    using_maps_backing_ = use_maps_backing;  
    dynamic_buffer_ = NULL;  
    if (!b)   
    {  
        // If the user didn't pass in any buffer storage, allocate it  
        // now. This is the normal case; the signal handler passes in a  
        // static buffer.  
        b = dynamic_buffer_ = new buffer;  
    }   
    else   
    {  
        dynamic_buffer_ = NULL;  
    }  
      
    ibuf_ = b->buf_;  
      
    stext_ = etext_ = nextline_ = ibuf_;  
    ebuf_ = ibuf_ + buffer::kBufSize - 1;  
    nextline_ = ibuf_;  
      
    if (use_maps_backing)   
    {  // don't bother with clever "self" stuff in this case  
        constructfilename("/proc/%d/maps_backing", pid, ibuf_, buffer::kBufSize);  
    }   
    else if (pid == 0)   
    {  
        // We have to kludge a bit to deal with the args constructfilename  
        // expects.  The 1 is never used -- it's only impt. that it's not 0.  
        constructfilename("/proc/self/maps", 1, ibuf_, buffer::kBufSize);  
    }   
    else   
    {  
        constructfilename("/proc/%d/maps", pid, ibuf_, buffer::kBufSize);  
    }  
          
    // No error logging since this can be called from the crash dump  
    // handler at awkward moments. Users should call Valid() before  
    // using.  
    fd_ = open(ibuf_, O_RDONLY);  
          
}  
      
proc_maps_iterator::~proc_maps_iterator()   
{  
    if (fd_ >= 0)   
    {  
        close(fd_);  
    }  
    delete dynamic_buffer_;  
}  
      
bool proc_maps_iterator::next(uint64_t *start, uint64_t *end, char **flags, uint64_t *offset, int64_t *inode, char **filename)   
{  
    return nextext(start, end, flags, offset, inode, filename, NULL, NULL, NULL, NULL, NULL);  
}  
      
      
// Finds |c| in |text|, and assign '\0' at the found position.  
// The original character at the modified position should be |c|.  
// A pointer to the modified position is stored in |endptr|.  
// |endptr| should not be NULL.  
static bool extractuntilchar(char *text, int c, char **endptr)   
{  
    char *found;  
    found = strchr(text, c);  
    if (found == NULL)   
    {  
        *endptr = NULL;  
        return false;  
    }  
      
    *endptr = found;  
    *found = '\0';  
    return true;  
}  
      
template<class T>  
static T stringtointeger(char *text, char **endptr, int base)   
{  
    assert(false);  
    return T();  
}  
      
template<>  
int stringtointeger<int>(char *text, char **endptr, int base)   
{  
    return strtol(text, endptr, base);  
}  
      
template<>  
int64_t stringtointeger<int64_t>(char *text, char **endptr, int base)   
{  
    return strtoll(text, endptr, base);  
}  
      
template<>  
uint64_t stringtointeger<uint64_t>(char *text, char **endptr, int base)   
{  
    return strtoull(text, endptr, base);  
}  
      
// Increments |*text_pointer| while it points a whitespace character.  
// It is to follow sscanf's whilespace handling.  
static void skipwhilewhitespace(char **text_pointer, int c)   
{  
    if (isspace(c))   
    {  
        while (isspace(**text_pointer) && isspace(*((*text_pointer) + 1)))   
        {  
            ++(*text_pointer);  
        }  
    }  
}  
      
template<typename T>  
static T stringtointegeruntilchar(char *text, int base, int c, char **endptr_result)   
{  
    *endptr_result = NULL;  
      
    char *endptr_extract;  
    if (!extractuntilchar(text, c, &endptr_extract))  
        return 0;  
      
    T result;  
    char *endptr_strto;  
    result = stringtointeger<T>(text, &endptr_strto, base);  
    *endptr_extract = c;  
      
    if (endptr_extract != endptr_strto)  
        return 0;  
      
    *endptr_result = endptr_extract;  
    skipwhilewhitespace(endptr_result, c);  
      
    return result;  
}  
      
template<typename T>  
static bool stringtoIntegeruntilcharwithcheck(T *outptr, char *text, int base, int c, char **endptr)   
{  
    *outptr = stringtointegeruntilchar<T>(*endptr, base, c, endptr);  
    if (*endptr == NULL || **endptr == '\0') return false;  
        ++(*endptr);  
    return true;  
}  
      
static char *copystringuntilchar(char *text, unsigned out_len, int c, char *out)  
{  
    char *endptr;  
    if (!extractuntilchar(text, c, &endptr))  
        return NULL;  
      
    strncpy(out, text, out_len);  
    out[out_len-1] = '\0';  
    *endptr = c;  
      
    skipwhilewhitespace(&endptr, c);  
    return endptr;  
}  
      
static bool parseprocmapsline(char *text, uint64_t *start, uint64_t *end,  
                                char *flags, uint64_t *offset,  
                                int *major, int *minor, int64_t *inode,  
                                unsigned *filename_offset)   
{  
    /* 
    * It's similar to: 
    * sscanf(text, "%"SCNx64"-%"SCNx64" %4s %"SCNx64" %x:%x %"SCNd64" %n", 
    *        start, end, flags, offset, major, minor, inode, filename_offset) 
    */  
    char *endptr = text;  
    if (endptr == NULL || *endptr == '\0')  return false;  
      
    if (!stringtoIntegeruntilcharwithcheck(start, endptr, 16, '-', &endptr))  
    return false;  
      
    if (!stringtoIntegeruntilcharwithcheck(end, endptr, 16, ' ', &endptr))  
    return false;  
      
    endptr = copystringuntilchar(endptr, 5, ' ', flags);  
    if (endptr == NULL || *endptr == '\0')  return false;  
    ++endptr;  
      
    if (!stringtoIntegeruntilcharwithcheck(offset, endptr, 16, ' ', &endptr))  
    return false;  
      
    if (!stringtoIntegeruntilcharwithcheck(major, endptr, 16, ':', &endptr))  
    return false;  
      
    if (!stringtoIntegeruntilcharwithcheck(minor, endptr, 16, ' ', &endptr))  
    return false;  
      
    if (!stringtoIntegeruntilcharwithcheck(inode, endptr, 10, ' ', &endptr))  
    return false;  
      
    *filename_offset = (endptr - text);  
    return true;  
}  
      
// This has too many arguments.  It should really be building  
// a map object and returning it.  The problem is that this is called  
// when the memory allocator state is undefined, hence the arguments.  
bool proc_maps_iterator::nextext(uint64_t *start, uint64_t *end, char **flags,  
                                uint64_t *offset, int64_t *inode, char **filename,  
                                uint64_t *file_mapping, uint64_t *file_pages,  
                                uint64_t *anon_mapping, uint64_t *anon_pages,  
                                dev_t *dev)   
{  
    do   
    {  
        // Advance to the start of the next line  
        stext_ = nextline_;  
      
        // See if we have a complete line in the buffer already  
        nextline_ = static_cast<char *>(memchr (stext_, '\n', etext_ - stext_));  
        if (!nextline_)   
        {  
            // Shift/fill the buffer so we do have a line  
            int count = etext_ - stext_;  
      
            // Move the current text to the start of the buffer  
            memmove(ibuf_, stext_, count);  
            stext_ = ibuf_;  
            etext_ = ibuf_ + count;  
      
            int nread = 0;            // fill up buffer with text  
            while (etext_ < ebuf_)   
            {  
                nread = read(fd_, etext_, ebuf_ - etext_);  
                if (nread > 0)  
                    etext_ += nread;  
                else  
                    break;  
            }  
      
            // Zero out remaining characters in buffer at EOF to avoid returning  
            // garbage from subsequent calls.  
            if (etext_ != ebuf_ && nread == 0)   
            {  
                memset(etext_, 0, ebuf_ - etext_);  
            }  
            *etext_ = '\n';   // sentinel; safe because ibuf extends 1 char beyond ebuf  
            nextline_ = static_cast<char *>(memchr (stext_, '\n', etext_ + 1 - stext_));  
        }  
        *nextline_ = 0;                // turn newline into nul  
        nextline_ += ((nextline_ < etext_)? 1 : 0);  // skip nul if not end of text  
        // stext_ now points at a nul-terminated line  
        uint64_t tmpstart, tmpend, tmpoffset;  
        int64_t tmpinode;  
        int major, minor;  
        unsigned filename_offset = 0;  
      
        // for now, assume all linuxes have the same format  
        if (!parseprocmapsline(  
            stext_,  
            start ? start : &tmpstart,  
            end ? end : &tmpend,  
            flags_,  
            offset ? offset : &tmpoffset,  
            &major, &minor,  
            inode ? inode : &tmpinode, &filename_offset)) continue;  
      
        // Depending on the Linux kernel being used, there may or may not be a space  
        // after the inode if there is no filename.  sscanf will in such situations  
        // nondeterministically either fill in filename_offset or not (the results  
        // differ on multiple calls in the same run even with identical arguments).  
        // We don't want to wander off somewhere beyond the end of the string.  
        size_t stext_length = strlen(stext_);  
        if (filename_offset == 0 || filename_offset > stext_length)  
            filename_offset = stext_length;  
      
        // We found an entry  
        if (flags) *flags = flags_;  
        if (filename) *filename = stext_ + filename_offset;  
        if (dev) *dev = minor | (major << 8);  
      
        if (using_maps_backing_)   
        {  
        // Extract and parse physical page backing info.  
        char *backing_ptr = stext_ + filename_offset + strlen(stext_+filename_offset);  
      
        // find the second '('  
        int paren_count = 0;  
        while (--backing_ptr > stext_)   
        {  
            if (*backing_ptr == '(')   
            {  
                ++paren_count;  
                if (paren_count >= 2)   
                {  
                    uint64_t tmp_file_mapping;  
                    uint64_t tmp_file_pages;  
                    uint64_t tmp_anon_mapping;  
                    uint64_t tmp_anon_pages;  
      
                    sscanf(backing_ptr+1, "F %" "llx" " %" "lld" ") (A %" "llx" " %" "lld" ")",  
                            (long long unsigned int*)(file_mapping ? file_mapping : &tmp_file_mapping),  
                            (long long int*)(file_pages ? file_pages : &tmp_file_pages),  
                            (long long unsigned int*)(anon_mapping ? anon_mapping : &tmp_anon_mapping),  
                            (long long int*)(anon_pages ? anon_pages : &tmp_anon_pages));  
                    // null terminate the file name (there is a space  
                    // before the first (.  
                    backing_ptr[-1] = 0;  
                    break;  
                }  
            }  
        }  
    }  
      
    return true;  
    } while (etext_ > ibuf_);  
      
    // We didn't find anything  
    return false;  
}  
      
int proc_maps_iterator::formatline(char* buffer, int bufsize,  
                                    uint64_t start, uint64_t end, const char *flags,  
                                    uint64_t offset, int64_t inode,  
                                    const char *filename, dev_t dev)   
{  
    // We assume 'flags' looks like 'rwxp' or 'rwx'.  
    char r = (flags && flags[0] == 'r') ? 'r' : '-';  
    char w = (flags && flags[0] && flags[1] == 'w') ? 'w' : '-';  
    char x = (flags && flags[0] && flags[1] && flags[2] == 'x') ? 'x' : '-';  
    // p always seems set on linux, so we set the default to 'p', not '-'  
    char p = (flags && flags[0] && flags[1] && flags[2] && flags[3] != 'p')  
        ? '-' : 'p';  
      
    const int rc = snprintf(buffer, bufsize,  
        "%08" "llx" "-%08" "llx" " %c%c%c%c %08" "llx" " %02x:%02x %-11" "lld" " %s\n",  
        (long long int)start, (long long int)end, r,w,x,p, (long long int)offset,  
        static_cast<int>(dev/256), static_cast<int>(dev%256),  
        (long long int)inode, filename);  
              
    return (rc < 0 || rc >= bufsize) ? 0 : rc;  
}  

#endif

