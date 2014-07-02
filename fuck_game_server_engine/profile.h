#pragma once

extern void start_profiler(const char* fname);  
extern void stop_profiler();  

#ifndef WIN32

extern void profiler_signal_handler(int sig, siginfo_t* sinfo, void* ucontext);  

class profiledata   
{  
public:      
    static const int kMaxStackDepth = 64;  // Max stack depth stored in profile  
      
    profiledata();  
    ~profiledata();  
      
    // If data collection is not already enabled start to collect data  
    // into fname.  Parameters related to this profiling run are specified  
    // by 'options'.  
    //  
    // Returns true if data collection could be started, otherwise (if an  
    // error occurred or if data collection was already enabled) returns  
    // false.  
    bool start(const char *fname);  
      
    // If data collection is enabled, stop data collection and write the  
    // data to disk.  
    void stop();  
      
    // Stop data collection without writing anything else to disk, and  
    // discard any collected data.  
    void reset();  
      
    // If data collection is enabled, record a sample with 'depth'  
    // entries from 'stack'.  (depth must be > 0.)  At most  
    // kMaxStackDepth stack entries will be recorded, starting with  
    // stack[0].  
    //  
    // This function is safe to call from asynchronous signals (but is  
    // not re-entrant).  
    void add(int depth, const void* const* stack);  
      
    // Is data collection currently enabled?  
    bool enabled() const { return out_ >= 0; }  
      
private:  
    static const int kAssociativity = 4;          // For hashtable  
    static const int kBuckets = 1 << 10;          // For hashtable  
    static const int kBufferLength = 1 << 18;     // For eviction buffer  
      
    // Type of slots: each slot can be either a count, or a PC value  
    typedef uintptr_t slot;  
      
    // Hash-table/eviction-buffer entry (a.k.a. a sample)  
    struct entry   
    {  
        slot count;                  // Number of hits  
        slot depth;                  // Stack depth  
        slot stack[kMaxStackDepth];  // Stack contents  
    };  
      
    // Hash table bucket  
    struct bucket   
    {  
        entry entry_[kAssociativity];  
    };  
      
    bucket        hash_[kBuckets];          // hash table  
    slot          evict_[kBufferLength];         // evicted entries  
    int           num_evicted_;   // how many evicted entries?  
    int           out_;           // fd for output file.  
    int           count_;         // How many samples recorded  
    int           evictions_;     // How many evictions  
    size_t        total_bytes_;   // How much output  
    stringc       fname_;         // Profile file name  
    time_t        start_time_;    // Start time, or 0  
      
    // Move 'entry' to the eviction buffer.  
    void evict(const entry& entry);  
      
    // Write contents of eviction buffer to disk.  
    void flushevicted();  
      
};  
      
class proc_maps_iterator   
{  
public:  
    struct buffer   
    {  
        static const size_t kBufSize = PATH_MAX + 1024;  
        char buf_[kBufSize];  
    };  
      
    // Create an iterator with specified storage (for use in signal  
    // handler). "buffer" should point to a proc_maps_iterator::Buffer  
    // buffer can be NULL in which case a bufer will be allocated.  
    proc_maps_iterator(pid_t pid, buffer *buffer);  
      
    // Writes the "canonical" form of the /proc/xxx/maps info for a single  
    // line to the passed-in buffer. Returns the number of bytes written,  
    // or 0 if it was not able to write the complete line.  (To guarantee  
    // success, buffer should have size at least Buffer::kBufSize.)  
    // Takes as arguments values set via a call to Next().  The  
    // "canonical" form of the line (taken from linux's /proc/xxx/maps):  
    //    <start_addr(hex)>-<end_addr(hex)> <perms(rwxp)> <offset(hex)>   +  
    //    <major_dev(hex)>:<minor_dev(hex)> <inode> <filename> Note: the  
    // eg  
    //    08048000-0804c000 r-xp 00000000 03:01 3793678    /bin/cat  
    // If you don't have the dev_t (dev), feel free to pass in 0.  
    // (Next() doesn't return a dev_t, though NextExt does.)  
    //  
    // Note: if filename and flags were obtained via a call to Next(),  
    // then the output of this function is only valid if Next() returned  
    // true, and only until the iterator is destroyed or Next() is  
    // called again.  (Since filename, at least, points into CurrentLine.)  
    static int formatline(char* buffer, int bufsize,  
                        uint64_t start, uint64_t end, const char *flags,  
                        uint64_t offset, int64_t inode, const char *filename,  
                        dev_t dev);  
      
    // Find the next entry in /proc/maps; return true if found or false  
    // if at the end of the file.  
    //  
    // Any of the result pointers can be NULL if you're not interested  
    // in those values.  
    //  
    // If "flags" and "filename" are passed, they end up pointing to  
    // storage within the proc_maps_iterator that is valid only until the  
    // iterator is destroyed or Next() is called again. The caller may  
    // modify the contents of these strings (up as far as the first NUL,  
    // and only until the subsequent call to Next()) if desired.  
      
    // The offsets are all uint64_t in order to handle the case of a  
    // 32-bit process running on a 64-bit kernel  
    //  
    // IMPORTANT NOTE: see top-of-class notes for details about what  
    // mapped regions Next() iterates over, depending on O/S.  
    // TODO(csilvers): make flags and filename const.  
    bool next(uint64_t *start, uint64_t *end, char **flags,  
            uint64_t *offset, int64_t *inode, char **filename);  
      
    bool nextext(uint64_t *start, uint64_t *end, char **flags,  
                uint64_t *offset, int64_t *inode, char **filename,  
                uint64_t *file_mapping, uint64_t *file_pages,  
                uint64_t *anon_mapping, uint64_t *anon_pages,  
                dev_t *dev);  
      
    ~proc_maps_iterator();  
      
private:  
    void init(pid_t pid, buffer *buffer, bool use_maps_backing);  
      
    char *ibuf_;        // input buffer  
    char *stext_;       // start of text  
    char *etext_;       // end of text  
    char *nextline_;    // start of next line  
    char *ebuf_;        // end of buffer (1 char for a nul)  
      
    int fd_;            // filehandle on /proc/*/maps  
      
    pid_t pid_;  
    char flags_[10];  
    buffer* dynamic_buffer_;  // dynamically-allocated Buffer  
    bool using_maps_backing_; // true if we are looking at maps_backing instead of maps.  
};  

#endif

