#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <time.h>

#include <epan/packet.h>
#include <epan/prefs.h>
#include <epan/expert.h>

/* IF PROTO exposes code to other dissectors, then it must be exported
   in a header file. If not, a header file is not needed at all. */
#include "packet-evil.h"

static int proto_evil = -1;

/* Initialize the protocol and registered fields */
static int hf_evil_hdr_msg_header = -1;
static int hf_evil_hdr_msg_body = -1;

/* Initialize the subtree pointers */
static gint ett_evil = -1;

/* Preferences */
static guint evil_tcp_port = 0;

#define evil_buffer_size 102400
static char evil_buffer[evil_buffer_size];

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

/* This method dissects fully reassembled messages */
static int dissect_evil_message(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void* data _U_)
{
    int offset = 0;//偏移变量，记录偏移位置
    int ret = 0;
    proto_item * ti = NULL;//方便添加结点而定义
    proto_tree * tt = NULL; //方便添加结点而定义
    int size = 0;
    char * p = NULL;
    int evil_header_size = 0;
    int packet_size = 0;
    const guint8* buffer = 0;
    int buffersize = 0;
    
    MYLOG("!!!!!!!!!!!!!!!!! dissect_evil_message start !!!!!!!!!!!!!!!!!");

    MYLOG("tvb_length %d tvb_reported_length %d", tvb_length(tvb), tvb_reported_length(tvb));
    	
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "Evil");//显示协议
    col_clear(pinfo->cinfo, COL_INFO);

    if (!tree) 
    {
        return tvb_length(tvb);
    }

    ti = proto_tree_add_item(tree, proto_evil, tvb, 0, -1, FALSE);//添加结点
    tt = proto_item_add_subtree(ti, ett_evil);//添加子树，用以显示数据
    
    buffer = tvb_get_ptr(tvb, 0, -1);
    buffersize = tvb_length_remaining(tvb, 0);
    MYLOG("tvb buffer %d %p", buffersize, buffer);
    
    evil_header_size = findhead(buffer, buffersize);
    if (evil_header_size == -1)
    {
        MYLOG("findhead fail %d %p", buffersize, buffer);
        proto_tree_add_string(tt, hf_evil_hdr_msg_header, tvb, 0, buffersize, "findhead fail");
        return tvb_length(tvb);
    }
    
    offset = 0;
    size = evil_header_size;
    proto_tree_add_string(tt, hf_evil_hdr_msg_header, tvb, offset, size, "header");
    offset += size;

    size = buffersize - evil_header_size;
    ret = cs_visualize(buffer + evil_header_size, size, evil_buffer, evil_buffer_size);
    p = strtok(evil_buffer, "\n");
    while(p)
    {
        proto_tree_add_string(tt, hf_evil_hdr_msg_body, tvb, offset, size, p);
        p = strtok(NULL, "\n");
    }
    offset += size;

    MYLOG("dissect_evil ok");

    return tvb_length(tvb);
}

#define FRAME_HEADER_LEN 22
#define FRAME_HEADER_SIZE_POS 17

/* determine PDU length of protocol foo */
static guint get_evil_message_len(packet_info *pinfo, tvbuff_t *tvb, int offset)
{
    // length is at offset FRAME_HEADER_SIZE_POS    
    guint len = 0;
    guint16 head = 0;
    head = tvb_get_ntohs(tvb, offset);
    if (head != 0x3366)
    {
        MYLOG("get_evil_message_len error head %d", (int)head);
        return tvb_length(tvb);
    }
    
    len = (guint)tvb_get_ntohl(tvb, offset + FRAME_HEADER_SIZE_POS);
    len = len + FRAME_HEADER_LEN;
    MYLOG("get_evil_message_len len %d", (int)len);
    return len;
}

/* The main dissecting routine */
static int dissect_evil(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void* data)
{
    tcp_dissect_pdus(tvb, pinfo, tree, TRUE, FRAME_HEADER_LEN,
                     get_evil_message_len, dissect_evil_message, data);
    return tvb_length(tvb);
}

void
proto_register_evil(void)
{
    module_t *evil_module;

    static hf_register_info hf[] = {
            {
                &hf_evil_hdr_msg_header,
                {
                    "Msg Header",
                    "evil.msghead",
                    FT_STRING,
                    BASE_NONE,
                    NULL,
                    0,
                    NULL,
                    HFILL
                }
            },
            {
                &hf_evil_hdr_msg_body,
                {
                    "Msg Body",
                    "evil.msgbody",
                    FT_STRING,
                    BASE_NONE,
                    NULL,
                    0,
                    NULL,
                    HFILL
                }
            },
		};
			
        /* Protocol subtree array */
    static gint *ett[] = {
        &ett_evil,
    };

    MYLOG("!!!!!!!!!!!!!!!!! evil start !!!!!!!!!!!!!!!!!");

    if (cs_init() != 0)
    {
        MYLOG("cs_init fail");
    }
    
	MYLOG("proto_register_evil");

    /* Register the protocol name and description */
    proto_evil = proto_register_protocol(
        "I am Evil",
        "Evil",
        "evil");

	MYLOG("proto_register_protocol proto_evil = %d", proto_evil);

    /* Required function calls to register the header fields and subtrees
     * used */
    proto_register_field_array(proto_evil, hf, array_length(hf));
	MYLOG("proto_register_field_array");
    proto_register_subtree_array(ett, array_length(ett));
	MYLOG("proto_register_subtree_array");
	
    /* Register preferences module (See Section 2.6 for more on
     * preferences) */
    evil_module = prefs_register_protocol(
        proto_evil,
        proto_reg_handoff_evil);

	MYLOG("prefs_register_protocol evil_module = %d", evil_module);
	
    MYLOG("proto_register_evil ok");
}

void
proto_reg_handoff_evil(void)
{
    dissector_handle_t evil_handle;
    FILE * portfp = NULL;
    int port = 8000;
    
    evil_handle = create_dissector_handle(dissect_evil, proto_evil);
	MYLOG("create_dissector_handle evil_handle = %p", evil_handle);

	// 读port
	portfp = fopen("port.txt", "r");
	if (!portfp)
	{
		MYLOG("no portfp %s", "port.txt");
	}
	else
	{
    	char strport[10];
    	memset(strport, 0, sizeof(strport));
    	fread(strport, 1, sizeof(strport), portfp);
    	port = atoi(strport);
	}
	MYLOG("port=%d", port);
	
    dissector_add_uint("tcp.port", port, evil_handle);

	MYLOG("proto_reg_handoff_evil ok");
}
