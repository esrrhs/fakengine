#pragma once

/************************************************************************/
/* 

start_trace
while
	if wait_syscall_begin

		get_syscall_begin

		wait_syscall_end

		get_syscall_end
	end

	continue_trace
end
stop_trace
                                                                     */
/************************************************************************/

#ifndef WIN32

class fptrace
{
public:
	fptrace()
	{
	}
	~fptrace()
	{
	}

	force_inline void set_traced_process(pid_t traced_process)
	{
		m_traced_process = traced_process;
	}

	force_inline bool start_trace()
	{
		int32_t status = 0;
		SAFE_DIFFER_TEST_RET_VAL(ptrace(PTRACE_ATTACH, m_traced_process, NULL, NULL), 0, false);
		wait(&status);
		SAFE_DIFFER_TEST_RET_VAL(ptrace(PTRACE_SETOPTIONS, m_traced_process, NULL, PTRACE_O_TRACESYSGOOD), 0, false);
		SAFE_DIFFER_TEST_RET_VAL(ptrace(PTRACE_SYSCALL, m_traced_process, NULL, NULL), 0, false);
		return true;
	}

	force_inline bool wait_syscall_begin()
	{
		int32_t status = 0;

		wait(&status);

		if (WSTOPSIG(status) != (SIGTRAP | 0x80))
		{
			return false;
		}

		return true;
	}

	force_inline bool get_syscall_begin(uint64_t call, uint64_t p1, uint64_t p2, uint64_t p3)
	{
		struct user_regs_struct regs;
		// We are now entering a system call
		SAFE_DIFFER_TEST_RET_VAL(ptrace(PTRACE_GETREGS, m_traced_process, NULL, &regs), 0, false);
		call = regs.orig_rax;
		p1 = regs.rdi;
		p2 = regs.rsi;
		p3 = regs.rdx;
		return true;
	}

	force_inline bool wait_syscall_end()
	{
		int32_t status = 0;
		// Wait until we're exiting the system call
		SAFE_DIFFER_TEST_RET_VAL(ptrace(PTRACE_SYSCALL, m_traced_process, NULL, NULL), 0, false);
		wait(&status);

		return true;
	}

	force_inline bool get_syscall_end(uint64_t ret)
	{
		struct user_regs_struct regs;
		/* Syscall exit */
		SAFE_DIFFER_TEST_RET_VAL(ptrace(PTRACE_GETREGS, m_traced_process, NULL, &regs), 0, false);

		ret = regs.rax;

		return true;
	}

	force_inline bool continue_trace()
	{
		SAFE_DIFFER_TEST_RET_VAL(ptrace(PTRACE_SYSCALL, m_traced_process, NULL, NULL), 0, false);
		return true;
	}

	force_inline bool stop_trace()
	{
		SAFE_DIFFER_TEST_RET_VAL(ptrace(PTRACE_DETACH, m_traced_process, NULL, NULL), 0, false);
		return true;
	}

private:
	pid_t m_traced_process;
};

#endif
