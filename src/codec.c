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


#include <stdio.h>
#include "zenoh/codec.h"
#include "zenoh/private/logging.h"

void
z_int_le_encode(z_iobuf_t* buf, z_int_t v) {
  z_iobuf_write(buf, (v & 0x00ff));
  z_iobuf_write(buf, (v & 0xff00) >> 8);
}

z_int_t
z_int_le_decode(z_iobuf_t* buf) {
  return z_iobuf_read(buf) | (z_iobuf_read(buf) << 8);  
}

void
z_int_encode(z_iobuf_t* buf, z_int_t v) {
  while (v > 0x7f) {
    uint8_t c = (v & 0x7f) | 0x80;
    z_iobuf_write(buf, (uint8_t)c);
    v = v >> 7;
  }
  z_iobuf_write(buf, (uint8_t)v);
}

z_zint_result_t
z_int_decode(z_iobuf_t* buf) {
  z_zint_result_t r;
  r.tag = Z_OK_TAG;
  r.value.zint = 0;

  uint8_t c;
  int i = 0;
  do {
    c = z_iobuf_read(buf);
    _Z_DEBUG_VA("vle c = 0x%x\n",c);
    r.value.zint = r.value.zint | (((z_int_t)c & 0x7f) << i);
    _Z_DEBUG_VA("current vle  = %zu\n",r.value.zint);
    i += 7;
  } while (c > 0x7f);

  _Z_DEBUG_VA("==> Decoded VLE  = %zu\n",r.value.zint);
  return r;
}

void
z_uint8_array_encode(z_iobuf_t* buf, const z_uint8_array_t* bs) {
  z_int_encode(buf, bs->length);
  z_iobuf_write_slice(buf, bs->elem, 0,  bs->length);
}

void
z_uint8_array_decode_na(z_iobuf_t* buf, z_uint8_array_result_t *r) {
  r->tag = Z_OK_TAG;
  z_zint_result_t r_vle = z_int_decode(buf);
  ASSURE_P_RESULT(r_vle, r, Z_INT_PARSE_ERROR)
  r->value.uint8_array.length = (unsigned int)r_vle.value.zint;
  r->value.uint8_array.elem = z_iobuf_read_n(buf, r->value.uint8_array.length);
}

z_uint8_array_result_t
z_uint8_array_decode(z_iobuf_t* buf) {
  z_uint8_array_result_t r;
  z_uint8_array_decode_na(buf, &r);
  return r;
}

void
z_string_encode(z_iobuf_t* buf, const char* s) {
  size_t len = strlen(s);
  z_int_encode(buf, len);
  // Note that this does not put the string terminator on the wire.
  z_iobuf_write_slice(buf, (uint8_t*)s, 0, len);
}

z_string_result_t
z_string_decode(z_iobuf_t* buf) {
  z_string_result_t r;
  r.tag = Z_OK_TAG;
  z_zint_result_t vr = z_int_decode(buf);
  ASSURE_RESULT(vr, r, Z_INT_PARSE_ERROR)
  size_t len = vr.value.zint;
  // Allocate space for the string terminator.
  char* s = (char*)malloc(len + 1);
  s[len] = '\0';
  z_iobuf_read_to_n(buf, (uint8_t*)s, len);
  r.value.string = s;
  return r;
}
