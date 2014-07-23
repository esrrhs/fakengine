#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <time.h>

#include <epan/packet.h>
#include <epan/prefs.h>
#include <epan/expert.h>

#define MYLOG(...) printf(__VA_ARGS__);\
	MyLog(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);
	
void MyLog( const char * file, const char * func, int pos, const char *fmt, ...)
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


static int
dissect_evil(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void *data _U_)
{
    int offset = 0;//偏移变量，记录偏移位置
    int ret = 0;
    proto_item * ti = NULL;//方便添加结点而定义
    proto_tree * tt = NULL; //方便添加结点而定义
    int size = 0;
    
    MYLOG("!!!!!!!!!!!!!!!!! dissect_evil start !!!!!!!!!!!!!!!!!");

    col_set_str(pinfo->cinfo, COL_PROTOCOL, "Evil");//显示协议
    col_clear(pinfo->cinfo, COL_INFO);

    ret = tvb_length(tvb);

    if (!tree) 
    {
        return ret;
    }

    ti = proto_tree_add_item(tree, proto_evil, tvb, 0, -1, FALSE);//添加结点
    tt = proto_item_add_subtree(ti, ett_evil);//添加子树，用以显示数据

    size = 26;
    proto_tree_add_string(tt, hf_evil_hdr_msg_header, tvb, ret, size, "header");
    offset += size;

    size = tvb_length(tvb) - 26;
    proto_tree_add_string(tt, hf_evil_hdr_msg_body, tvb, ret, size, "body todo---------");
    offset += size;

    MYLOG("dissect_evil ok");

    return ret;
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
    evil_handle = create_dissector_handle(dissect_evil, proto_evil);
	MYLOG("create_dissector_handle evil_handle = %p", evil_handle);
    dissector_add_uint("tcp.port", 8000, evil_handle);

	MYLOG("proto_reg_handoff_evil ok");
}
