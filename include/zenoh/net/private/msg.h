/*
 * Copyright (c) 2017, 2020 ADLINK Technology Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Apache License, Version 2.0
 * which is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
 *
 * Contributors:
 *   ADLINK zenoh team, <zenoh@adlink-labs.tech>
 */

#ifndef ZENOH_C_NET_MSG_H
#define ZENOH_C_NET_MSG_H

#include "zenoh/net/private/internal.h"

/* Message ID */

#define _ZN_SCOUT           0x01
#define _ZN_HELLO           0x02

#define _ZN_OPEN            0x03
#define _ZN_ACCEPT          0x04
#define _ZN_CLOSE           0x05

#define _ZN_DECLARE         0x06

#define _ZN_COMPACT_DATA    0x07
#define _ZN_STREAM_DATA     0x1a
#define _ZN_BATCH_DATA      0x08
#define _ZN_WRITE_DATA      0x09

#define _ZN_QUERY           0x0a
#define _ZN_PULL            0x0b

#define _ZN_PING_PONG       0x0c

#define _ZN_SYNCH           0x0e
#define _ZN_ACKNACK         0x0f

#define _ZN_KEEP_ALIVE      0x10
#define _ZN_CONDUIT_CLOSE   0x11
#define _ZN_FRAGMENT        0x12
#define _ZN_CONDUIT         0x13
#define _ZN_MIGRATE         0x14

#define _ZN_REPLY           0x19

#define _ZN_RSPACE          0x18

/* Zenoh Rust Session FLAGS */
#define _ZN_S_C  1 << 6 // 0x40 Count         if C==1 then number of unacknowledged messages is present
#define _ZN_S_D  1 << 5 // 0x80 LeasePeriod   if D==1 then the lease period is present
#define _ZN_S_E  1 << 7 // 0x80 End           if E==1 then it is the last FRAME fragment
#define _ZN_S_F  1 << 6 // 0x40 Fragment      if F==1 then the FRAME is a fragment
#define _ZN_S_I  1 << 5 // 0x20 PeerID        if I==1 then the PeerID is present
#define _ZN_S_K  1 << 6 // 0x40 CloseLink     if K==1 then close the transport link only
#define _ZN_S_L  1 << 7 // 0x20 Locators      if L==1 then Locators are present
#define _ZN_S_M  1 << 5 // 0x20 Mask          if M==1 then a Mask is present
#define _ZN_S_O  1 << 5 // 0x20 Options       if O==1 then options are present
#define _ZN_S_P  1 << 5 // 0x20 PingOrPong    if P==1 then the message is Ping, otherwise is Pong
#define _ZN_S_R  1 << 5 // 0x20 Reliable      if R==1 then it concerns the reliable channel, best-effort otherwise
#define _ZN_S_S  1 << 6 // 0x40 SN Resolution if S==1 then the SN Resolution is present
#define _ZN_S_T  1 << 7 // 0x80 SN Resolution if T==1 then the scouter is asking for forwarded hello messages 
#define _ZN_S_W  1 << 6 // 0x40 WhatAmI       if W==1 then WhatAmI is indicated

/* Message Header _FLAGs */
#define _ZN_S_FLAG  0x20
#define _ZN_M_FLAG  0x20
#define _ZN_P_FLAG  0x20

#define _ZN_R_FLAG  0x40
#define _ZN_N_FLAG  0x40
#define _ZN_C_FLAG  0x40
#define _ZN_E_FLAG  0x40

#define _ZN_A_FLAG  0x80
#define _ZN_U_FLAG  0x80

#define _ZN_Z_FLAG  0x80
#define _ZN_L_FLAG  0x20
#define _ZN_H_FLAG  0x40

#define _ZN_G_FLAG  0x80
#define _ZN_I_FLAG  0x20
#define _ZN_F_FLAG  0x80
#define _ZN_O_FLAG  0x20

#define _ZN_MID_MASK 0x1f
#define _ZN_FLAGS_MASK = 0xe0

#define _ZN_HAS_FLAG(h, f) ((h & f) != 0)
#define _ZN_MID(h) (_ZN_MID_MASK & h)
#define _ZN_FLAGS(h) (_ZN_FLAGS_MASK & h)

/* WHATAMI Flags */
#define _ZN_BROKER 0x01
#define _ZN_ROUTER 0x02
#define _ZN_PEER 0x04
#define _ZN_CLIENT 0x08


/* Declaration Id */

#define _ZN_RESOURCE_DECL  0x01
#define _ZN_PUBLISHER_DECL  0x02
#define _ZN_SUBSCRIBER_DECL  0x03
#define _ZN_SELECTION_DECL  0x04
#define _ZN_BINDING_DECL  0x05
#define _ZN_COMMIT_DECL  0x06
#define _ZN_RESULT_DECL  0x07
#define _ZN_FORGET_RESOURCE_DECL  0x08
#define _ZN_FORGET_PUBLISHER_DECL  0x09
#define _ZN_FORGET_SUBSCRIBER_DECL  0x0a
#define _ZN_FORGET_SELECTION_DECL  0x0b
#define _ZN_STORAGE_DECL  0x0c
#define _ZN_FORGET_STORAGE_DECL  0x0d
#define _ZN_EVAL_DECL  0x0e
#define _ZN_FORGET_EVAL_DECL  0x0f

/* Close Reasons */
#define _ZN_PEER_CLOSE 0
#define _ZN_ERROR_CLOSE 1

/* Payload Header */
#define _ZN_SRC_ID 0x01
#define _ZN_SRC_SN 0x02
#define _ZN_BRK_ID 0x04
#define _ZN_BRK_SN 0x08
#define _ZN_T_STAMP ZN_T_STAMP
#define _ZN_KIND ZN_KIND
#define _ZN_ENCODING ZN_ENCODING

#define _HAS_PROPERTIES (m) (m.properties != 0)
/*---------------------------------------------------
                    Session Messages
-----------------------------------------------------*/                    

/*------------------ Scout Message ------------------*/
typedef struct {
  z_int_t what;
} _zn_scout_t;

/*------------------ Hello Message ------------------*/
typedef struct {
  z_uint8_array_t pid;
  z_int_t whatami;
  z_vec_t locators;
} _zn_hello_t;

/*------------------ Open Message ------------------*/
typedef struct {
  uint8_t version;
  z_int_t whatami;
  z_uint8_array_t pid;
  z_int_t lease;
  z_int_t sn0;
  z_int_t sn_resolution;
  // z_vec_t *locators;
} _zn_open_t;

/*------------------ Accept Message ------------------*/
typedef struct {
  z_int_t whatami;
  z_uint8_array_t client_pid;
  z_uint8_array_t broker_pid;
  z_int_t sn0;  
  z_int_t sn_resolution;
  z_int_t lease;
  // z_vec_t *locators;
} _zn_accept_t;

/*------------------ Close Message ------------------*/
typedef struct {
  z_uint8_array_t pid;
  uint8_t reason;
} _zn_close_t;

/*------------------  Resource Declaration Message ------------------*/
// in types.h

/*------------------ Delcare Publisher ------------------*/
typedef struct {
  z_int_t rid;
} _zn_pub_decl_t;

/*------------------ Forget Publisher Message ------------------*/
typedef struct {
  z_int_t rid;
} _zn_forget_pub_decl_t;

/*------------------ Declare Storage ------------------*/
typedef struct {
  z_int_t rid;
} _zn_storage_decl_t;

/*------------------ Forget Storage Message ------------------*/
typedef struct {
  z_int_t rid;
} _zn_forget_sto_decl_t;

/*------------------ Declare Eval ------------------*/
typedef struct {
  z_int_t rid;
} _zn_eval_decl_t;

/*------------------ Forget Eval Message ------------------*/
typedef struct {
  z_int_t rid;
} _zn_forget_eval_decl_t;

/*------------------ Declare Subscriber Message ------------------*/
typedef struct {
  z_int_t rid;
  zn_sub_mode_t sub_mode;
} _zn_sub_decl_t;

/*------------------ Forget Subscriber Message ------------------*/
typedef struct {
  z_int_t rid;
} _zn_forget_sub_decl_t;

/*------------------ Declaration Commit Message ------------------*/
typedef struct {
  uint8_t cid;
} _zn_commit_decl_t;

/*------------------ Declaration Result  Message ------------------*/
typedef struct {
  uint8_t cid;
  uint8_t status;
} _zn_result_decl_t;

/**
 *  On the wire this is represented as:
 *     |header|payload|properties|
 *  The declaration of the structure does not follow this
 *  convention for alignement purposes.
 */
typedef struct {
  z_vec_t* properties;
  union {
    _zn_res_decl_t resource;
    _zn_pub_decl_t pub;
    _zn_sub_decl_t sub;
    _zn_storage_decl_t storage;
    _zn_eval_decl_t eval;
    _zn_forget_pub_decl_t forget_pub;
    _zn_forget_sub_decl_t forget_sub;
    _zn_forget_sto_decl_t forget_sto;
    _zn_forget_eval_decl_t forget_eval;
    _zn_commit_decl_t commit;
    _zn_result_decl_t result;
  } payload;
  uint8_t header;
} _zn_declaration_t;

_ZN_ARRAY_DECLARE(declaration)

/*------------------ Declare Messages ------------------*/
typedef struct  {
  z_int_t sn;
  _zn_declaration_array_t declarations;
} _zn_declare_t;


/*------------------ Compact Data Message ------------------*/
typedef struct {
  z_int_t sn;
  z_int_t rid;
  z_iobuf_t payload;
} _zn_compact_data_t;


/*------------------ Payload Header ------------------*/
typedef struct {
  z_int_t src_sn;
  z_int_t brk_sn;
  z_int_t kind;
  z_int_t encoding;
  uint8_t src_id[16];
  uint8_t brk_id[16];
  uint8_t flags;
  z_timestamp_t tstamp;
  z_iobuf_t payload;
} _zn_payload_header_t;

/*------------------ StreamData Message ------------------*/
typedef struct {
  z_int_t sn;
  z_int_t rid;
  z_iobuf_t payload_header;
} _zn_stream_data_t;

/*------------------ Write Data Message ------------------*/
typedef struct {
  z_int_t sn;
  char* rname;
  z_iobuf_t payload_header;
} _zn_write_data_t;

/*------------------ Pull Message ------------------*/
typedef struct {
  z_int_t sn;
  z_int_t id;
  z_int_t max_samples;
} _zn_pull_t;

/*------------------ Query Message ------------------*/
typedef struct {
  z_uint8_array_t pid;
  z_int_t qid;
  char* rname;
  char* predicate;
} _zn_query_t;

/*------------------ Reply Message ------------------*/
typedef struct {
  z_uint8_array_t qpid;
  z_int_t qid;
  z_uint8_array_t srcid;
  z_int_t rsn;
  char* rname;
  z_iobuf_t payload_header;
} _zn_reply_t;


/**
 *  On the wire this is represented as:
 *     |header|payload|properties|
 *  The declaration of the structure does not follow this
 *  convention for alignement purposes.
 */
typedef struct {
  const z_vec_t* properties;
  union {
    _zn_open_t open;
    _zn_accept_t accept;
    _zn_close_t close;
    _zn_declare_t declare;
    _zn_compact_data_t compact_data;
    _zn_stream_data_t stream_data;
    _zn_write_data_t write_data;
    _zn_pull_t pull;
    _zn_query_t query;
    _zn_reply_t reply;
    _zn_scout_t scout;
    _zn_hello_t hello;
  } payload;
  uint8_t header;
} _zn_message_t;

_ZN_RESULT_DECLARE (_zn_scout_t, scout)
_ZN_RESULT_DECLARE (_zn_hello_t, hello)

_ZN_RESULT_DECLARE (_zn_accept_t, accept)
_ZN_RESULT_DECLARE (_zn_close_t, close)
_ZN_RESULT_DECLARE (_zn_declare_t, declare)
_ZN_RESULT_DECLARE (_zn_declaration_t, declaration)
_ZN_RESULT_DECLARE (_zn_res_decl_t, res_decl)
_ZN_RESULT_DECLARE (_zn_pub_decl_t, pub_decl)
_ZN_RESULT_DECLARE (_zn_sub_decl_t, sub_decl)
_ZN_RESULT_DECLARE (_zn_commit_decl_t, commit_decl)
_ZN_RESULT_DECLARE (_zn_result_decl_t, result_decl)
_ZN_RESULT_DECLARE (_zn_compact_data_t, compact_data)
_ZN_RESULT_DECLARE (_zn_payload_header_t, payload_header)
_ZN_RESULT_DECLARE (_zn_stream_data_t, stream_data)
_ZN_RESULT_DECLARE (_zn_write_data_t, write_data)
_ZN_RESULT_DECLARE (_zn_pull_t, pull)
_ZN_RESULT_DECLARE (_zn_query_t, query)
_ZN_RESULT_DECLARE (_zn_reply_t, reply)
_ZN_P_RESULT_DECLARE (_zn_message_t, message)

#endif /* ZENOH_C_NET_MSG_H */
