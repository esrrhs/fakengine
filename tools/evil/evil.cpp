#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_write etc */
#include <sys/reg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/user.h>
#include <stdarg.h>
#include <string>

#define WORDSIZE sizeof(long)

#include "tdr/tdr.h"

#define MYLOG(...) MyLog(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);

void MyLog(const char * file, const char * func, int pos, const char *fmt, ...)
{
	FILE *pLog = NULL;
	time_t clock1;
	struct tm * tptr;
	va_list ap;

	pLog = fopen("evil.log", "a+");
	if (pLog == NULL)
	{
		return;
	}

	clock1 = time(0);
	tptr = localtime(&clock1);

	fprintf(pLog, "===========================[%d.%d.%d, %d.%d.%d]%s:%d,%s:===========================\n", 
		tptr->tm_year+1990,tptr->tm_mon+1,
		tptr->tm_mday,tptr->tm_hour,tptr->tm_min,
		tptr->tm_sec,file,pos,func);

	va_start(ap, fmt);
	vfprintf(pLog, fmt, ap);
	fprintf(pLog, "\n\n");
	va_end(ap);

	fclose(pLog);
}

LPTDRMETALIB g_pstLib = NULL;
LPTDRMETA g_pstMeta = NULL;

int net_init()
{
	MYLOG("net_init");

	LPTDRMETALIB pstLib = NULL;
	LPTDRMETA pstMeta;
	int iRet;

	FILE * tdrdirfp = fopen("tdrdir.txt", "r");
	if (!tdrdirfp)
	{
		MYLOG("no tdrdirfp");
		return -1;
	}
	char tdrdir[1024];
	memset(tdrdir, 0, sizeof(tdrdir));
	fread(tdrdir, 1, sizeof(tdrdir), tdrdirfp);
	MYLOG("tdrdir %s", tdrdir);

	char tdrexportdir[1024];
	strcpy(tdrexportdir, tdrdir);
	strcat(tdrexportdir, "/TDRExport.txt");
	MYLOG("tdrexportdir %s", tdrexportdir);

	FILE * tdrexportfp = fopen(tdrexportdir, "r");
	if (!tdrexportfp)
	{
		MYLOG("no tdrexportfp %s", tdrexportdir);
		return -1;
	}
	char filelist[1024];
	memset(filelist, 0, sizeof(filelist));
	fread(filelist, 1, sizeof(filelist), tdrexportfp);
	
	MYLOG("filelist %s", filelist);

	char filefir[1024][100];
	char * xmlfile[1024];
	char * p = strtok(filelist, " ");
	int i = 0;
	while (p)
	{
		strcpy(filefir[i], tdrdir);
		strcat(filefir[i], "/");
		strcat(filefir[i], p);
		xmlfile[i] = filefir[i];
		p = strtok(NULL, " ");
		i++;
	}
	int num = i;

	for (int i = 0; i < num; i++)
	{
		MYLOG("file: %s", xmlfile[i]);
	}

	/*根据xml文件创建元数据库*/
	iRet = tdr_create_lib_multifile(&pstLib, (const char **)xmlfile, num, 1, stderr);
	if (TDR_ERR_IS_ERROR(iRet))
	{
		MYLOG("tdr_create_lib_file failed by file %s, for %s\n", "net_protocol.xml", tdr_error_string(iRet));
		return iRet;
	}
	pstMeta = tdr_get_meta_by_name(pstLib, "CSMsg");
	if (TDR_ERR_IS_ERROR(iRet))
	{
		MYLOG("tdr_get_meta_by_name Pkg, for %s\n", tdr_error_string(iRet));
		return iRet;
	}

	g_pstLib = pstLib;
	g_pstMeta = pstMeta;

	MYLOG("net_init ok");

	return 0;
}

int net_visualize(char * inputbuffer, int inputsize, char * outputbuffer, int outputsize)
{
	MYLOG("net_visualize");

	if (!g_pstMeta)
	{
		MYLOG("g_pstMeta is null, should check tdrdir.txt");
		strcpy(outputbuffer, "g_pstMeta is null, should check tdrdir.txt");
		return -1;
	}

	TDRDATA stHostInfo;
	TDRDATA stNetInfo;

	static char unpackbuffer[102400];

	stHostInfo.iBuff = sizeof(unpackbuffer);
	stHostInfo.pszBuff = unpackbuffer;

	/*网络数据解码成本地数据*/
	stNetInfo.iBuff = inputsize;
	stNetInfo.pszBuff = inputbuffer;
	int iRet = tdr_ntoh(g_pstMeta, &stHostInfo, &stNetInfo, tdr_get_meta_current_version(g_pstMeta));
	if (TDR_ERR_IS_ERROR(iRet))
	{
		MYLOG("tdr_ntoh failed , for %s\n", tdr_error_string(iRet));
		sprintf(outputbuffer, "tdr_ntoh failed , for %s\n", tdr_error_string(iRet));
		return iRet;
	}

	/*打印数据*/
	tagTDRData stOut;
	stOut.pszBuff = outputbuffer;
	stOut.iBuff = outputsize;

	iRet = tdr_sprintf(g_pstMeta, &stOut, &stHostInfo, tdr_get_meta_current_version(g_pstMeta));
	if (TDR_ERR_IS_ERROR(iRet))
	{
		MYLOG("tdr_input_file failed , for %s\n", tdr_error_string(iRet));
		sprintf(outputbuffer, "tdr_input_file failed , for %s\n", tdr_error_string(iRet));
		return -1;
	}

	MYLOG("net_visualize ok");

	return 0;
}

void getdata(pid_t child, long addr, char *str, int len)
{   
    char *laddr;
    int i, j;
    union u {
		long val;
		char chars[WORDSIZE];
    }data;

    i = 0;
    j = len / WORDSIZE;
    laddr = str;
    while(i < j) 
	{
        data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * WORDSIZE, NULL);
        memcpy(laddr, data.chars, WORDSIZE);
        ++i;
        laddr += WORDSIZE;
    }
    j = len % WORDSIZE;
    if(j != 0) 
	{
        data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * WORDSIZE, NULL);
        memcpy(laddr, data.chars, j);
    }
}

void putdata(pid_t child, long addr, char *str, int len)
{   
    char *laddr;
    int i, j;
    union u {
		long val;
		char chars[WORDSIZE];
    }data;

    i = 0;
    j = len / WORDSIZE;
    laddr = str;
    while(i < j) 
	{
        memcpy(data.chars, laddr, WORDSIZE);
        ptrace(PTRACE_POKEDATA, child, addr + i * WORDSIZE, data.val);
        ++i;
        laddr += WORDSIZE;
    }
    j = len % WORDSIZE;
    if(j != 0) 
	{
        memcpy(data.chars, laddr, j);
        ptrace(PTRACE_POKEDATA, child, addr + i * WORDSIZE, data.val);
    }
}

const int g_outputsize = 102400;
char g_outputbuffer[g_outputsize];
const int g_inputsize = 102400;
char g_inputbuffer[g_inputsize];

static int findhead(char * p, int size)
{
    int i = 0;
	for (i = 0; i < size; i++)
	{
		int head = *(int*)(p + i);
		head = ((((unsigned int)(head) & 0xff000000) >> 24) | \

			(((unsigned int)(head) & 0x00ff0000) >> 8) | \

			(((unsigned int)(head) & 0x0000ff00) << 8) | \

			(((unsigned int)(head) & 0x000000ff) << 24));
		if (head == size - i)
		{
			return i;
		}
	}
	return -1;
}

int parse_recvfrom(pid_t traced_process, char * buff, int size)
{
	MYLOG("parse_recvfrom %p, %d\n", buff, size);
	
	getdata(traced_process, (long)buff, g_inputbuffer, size);
    
    int evil_header_size = findhead(g_inputbuffer, size);
    if (evil_header_size == -1)
    {
        MYLOG("findhead fail %d %p", size, g_inputbuffer);
        return -1;
    }
	
	net_visualize(g_inputbuffer + evil_header_size, size - evil_header_size, g_outputbuffer, g_outputsize);
	printf("----------recv-----------\n%s\n", g_outputbuffer);
	MYLOG("send msg: %s\n", g_outputbuffer);
		
	return 0;
}

int parse_writev(pid_t traced_process, const struct iovec *iov, int iovcnt)
{
	MYLOG("parse_writev %p, %d\n", iov, iovcnt);
	for (int i = 0; i < iovcnt; i++)
	{
		MYLOG("parse_writev [%d: %p %d]\n", i, iov[i].iov_base, iov[i].iov_len);
	}
		
	if (iovcnt == 2 && iov[1].iov_len > 24)
	{
		getdata(traced_process, (long)iov[1].iov_base, g_inputbuffer, iov[1].iov_len);
	
		net_visualize(g_inputbuffer, iov[1].iov_len, g_outputbuffer, g_outputsize);
		printf("----------send-----------\n%s\n", g_outputbuffer);
		MYLOG("send msg: %s\n", g_outputbuffer);
	}
	else
	{
		MYLOG("parse_writev ERROR iovcnt %d\n", iovcnt);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	if (net_init() != 0)
	{	
		printf("net init fail\n");
		MYLOG("net init fail\n");
		return -1;
	}
	
	pid_t traced_process;
	struct user_regs_struct regs;
    long ins;
    if(argc != 2) {
        printf("Usage: %s <pid to be traced> \n", argv[0], argv[1]);
        exit(1);
    }
	
	int wStatus = 0;
    traced_process = atoi(argv[1]);
	
	// Now set our options
	ptrace(PTRACE_ATTACH, traced_process, NULL, NULL);
    wait(&wStatus);
	ptrace(PTRACE_SETOPTIONS, traced_process, NULL, PTRACE_O_TRACESYSGOOD);
	ptrace(PTRACE_SYSCALL, traced_process, NULL, NULL);
	
	printf("enter loop\n");
	MYLOG("enter loop\n");
	
	// Wait for the child process to stop
	while (1)
	{
		wait(&wStatus);
		    
		// Stopped by our ptrace call
		if(WSTOPSIG(wStatus) == (SIGTRAP | 0x80))
		{
			// We are now entering a system call
			ptrace(PTRACE_GETREGS, traced_process, NULL, &regs);
			long call = regs.orig_rax;
			if (call != SYS_epoll_wait && call != SYS_gettimeofday)
			{
				//printf("syscall:  %4ld \n", call);
				MYLOG("syscall:  %4ld \n", call);
			}
			if(call == SYS_writev)
			{
				/* Syscall entry */
				MYLOG("Write called with %ld, %ld, %ld \n", regs.rdi, regs.rsi, regs.rdx);
				iovec buff[1024];
				getdata(traced_process, regs.rsi, (char*)buff, regs.rdx * sizeof(iovec));
				parse_writev(traced_process, buff, regs.rdx);
			}
			char * recvbuff = 0;
			if(call == SYS_recvfrom)
			{
				/* Syscall entry */
				MYLOG("Read called with %ld, %ld, %ld \n", regs.rdi, regs.rsi, regs.rdx);
				recvbuff = (char *)regs.rsi;
			}
			
			// Wait until we're exiting the system call
			ptrace(PTRACE_SYSCALL, traced_process, NULL, NULL);
			wait(&wStatus);
			
			ptrace(PTRACE_GETREGS, traced_process, NULL, &regs);
			/* Syscall exit */
			if(call == SYS_writev)
			{
				MYLOG("Write returned with %ld \n", regs.rax);
			}
			if(call == SYS_recvfrom)
			{
				parse_recvfrom(traced_process, recvbuff, regs.rax);
			}
		}
		// Stopped for some other reason
		else
		{
			MYLOG("child stopped but not for system call.\n");
		}
		fflush(stdout); // flush the output
		ptrace(PTRACE_SYSCALL, traced_process, NULL, NULL);
	}
	ptrace(PTRACE_DETACH, traced_process, NULL, NULL);
	return 0;
}

