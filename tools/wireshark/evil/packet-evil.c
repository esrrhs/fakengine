#include "config.h"

#include <glib.h>

#include <epan/packet.h>
#include <epan/prefs.h>
#include <epan/expert.h>

/* IF PROTO exposes code to other dissectors, then it must be exported
   in a header file. If not, a header file is not needed at all. */
#include "packet-evil.h"

static int proto_evil = -1;

/* Initialize the protocol and registered fields */
static int hf_evil_hdr_msg_size = -1;

/* Initialize the subtree pointers */
static gint ett_evil = -1;

/* Preferences */
static guint evil_tcp_port = 0;


static int
dissect_evil(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree, void *data _U_)
{
    return tvb_length(tvb);
}

void
proto_register_evil(void)
{
    module_t *evil_module;

    static hf_register_info hf[] = {
            {
                &hf_evil_hdr_msg_size,
                {
                    "EVIL Msg Size",
                    "evil.msgsize",
                    FT_UINT32,
                    BASE_DEC,
                    NULL,
                    0xfc,
                    NULL,
                    HFILL
                }
            },
		};
			
        /* Protocol subtree array */
    static gint *ett[] = {
        &ett_evil,
    };

    /* Register the protocol name and description */
    proto_evil = proto_register_protocol(
        "I am Evil",
        "Evil",
        "evil");

    /* Required function calls to register the header fields and subtrees
     * used */
    proto_register_field_array(proto_evil, hf, array_length(hf));
    proto_register_subtree_array(ett, array_length(ett));

    /* Register preferences module (See Section 2.6 for more on
     * preferences) */
    evil_module = prefs_register_protocol(
        proto_evil,
        proto_reg_handoff_evil);

    prefs_register_uint_preference(
        evil_module, "tcp.port",
        "EVIL TCP Port",
        "EVIL TCP port",
        8000,
        &evil_tcp_port);

}

void
proto_reg_handoff_evil(void)
{
    static guint              old_evil_tcp_port = 0;
    static gboolean           inited                   = FALSE;
    static dissector_handle_t evil_handle;

    if (!inited) {
        evil_handle = new_create_dissector_handle(dissect_evil, proto_evil);
        dissector_add_handle("tcp.port", evil_handle); /* for 'decode as' */
        inited = TRUE;
    }

    /* Register tcp port for dissection */
    if (old_evil_tcp_port != 0 && old_evil_tcp_port != evil_tcp_port) {
        dissector_delete_uint("tcp.port", old_evil_tcp_port, evil_handle);
    }

    if (evil_tcp_port != 0 && old_evil_tcp_port != evil_tcp_port) {
        dissector_add_uint("tcp.port", evil_tcp_port, evil_handle);
    }
}
