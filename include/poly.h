/**
 * 3dm - simple 3D mathematic library
 *
 * Copyright (C) 2013 Cedric Fung <cedric@vec.io>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *    3. Neither the name of the Cedric Fung nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY Cedric Fung "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Cedric Fung BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef _3DM_POLY_H
#define _3DM_POLY_H

#ifdef __cplusplus
extern "C" {
#endif

enum poly_type {
  POLY_CUBE,
  POLY_ICOSAHEDRON,
};

typedef struct {
  enum poly_type type;
  float *vertices; // x, y, z per vertex
  float *texcoords; // u, v per vertex
  int *indices; // for GL_TRIANGLES
  int v_len; // length of vertices
  int t_len; // length of texcoords
  int i_len; // length of indices
  int v_cap;
  int t_cap;
} poly_t;

poly_t *poly_create(enum poly_type type, int n);

void poly_destroy(poly_t *poly);

#ifdef __cplusplus
}
#endif

#endif
