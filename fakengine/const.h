#pragma once

// ����
const uint32_t c_main_version = 1;
const int32_t c_DefaultFps = 30;	// Ĭ��֡��

// ��ѧ
const double c_pi = 3.1415926535;
const double c_min_float = 1.0E-9;

// ����
const uint32_t c_ip_size = 24;
const uint32_t c_socket_container_max_process_per_frame = 100;
const uint32_t c_socket_link_max_process_per_frame = 1000;
const uint32_t c_msg_header = 2012;		// ��ͷ
const uint32_t c_msg_header_size = 2;	// ��ͷ����
const uint32_t c_buffer_size_size = 2;		// ��ʾ���������ȵĳ���

// ���̿���
const int32_t c_DefaultCmdKey = 20140510;	// Ĭ��cmd����key
const int32_t c_CmdControlCmdSize =	32;
const int32_t c_CmdControlParamSize = 10240;
const int8_t c_CmdExit[] = "byebye";

// ��־
const int32_t c_LogBuffer = 102400;
#ifdef WIN32
const PRINTF_CLOLOR_TYPE c_log_print_color[FLOGT_MAX] = {
	0 | FOREGROUND_INTENSITY,
	FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	FOREGROUND_RED | FOREGROUND_INTENSITY,
	FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
};
#else
const PRINTF_CLOLOR_TYPE c_log_print_color[FLOGT_MAX] = {
	"\e[0m",
	"\e[34m\e[1m",//hight blue
	"\e[31m", //red
	"\e[32m", //green
};
#endif

// �ַ���
const int32_t c_DefaultStringBuffer = 256;
const int32_t c_IniFileBufferSize = 10240;

// �ڴ�
const size_t c_falloc_max_size = 100 * 1024;
const size_t c_falloc_hashstep = 32;
const size_t c_falloc_hasharray = c_falloc_max_size / c_falloc_hashstep;
const uint32_t c_falloc_magic_head = 0xDEADBEEF;
const uint32_t c_falloc_magic_tail = 0xBADDDADD;
const uint32_t c_falloc_pagesize = 4096;

// ���л�
const uint32_t c_marshal_buffer_size = 1024 * 1024;

