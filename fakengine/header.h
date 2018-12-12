#pragma once

#include <vector>
#include <map>
#include <list>
#include <set>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <memory>
#include <stdarg.h>
#include <math.h>
#include <functional>
#include <string.h>
#include <time.h>
#include <stddef.h>

#ifdef WIN32
#include <windows.h>
#include <unordered_map>
#include <shlwapi.h>
#else
#include <execinfo.h>
#include <signal.h>
#include <exception>
#include <setjmp.h>
#include <pthread.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <linux/limits.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <malloc.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/syscall.h> 
#include <sys/reg.h>
#include <sys/user.h>
#endif


