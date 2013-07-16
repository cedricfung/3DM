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


#ifndef _3DM_H
#define _3DM_H
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef float vec4f __attribute__((vector_size (16)));
typedef float mat4f __attribute__((vector_size (64)));
typedef double vec4d __attribute__((vector_size (32)));
typedef double mat4d __attribute__((vector_size (128)));

double vec4d_sum(vec4d v);

double vec4d_dot_product(vec4d u, vec4d v);

double vec4d_length(vec4d v);

vec4d vec4d_normalize(vec4d v);

mat4d vec4d_cross_matrix(vec4d v);

vec4d vec4d_cross_product(vec4d u, vec4d v);

mat4d vec4d_tensor_product(vec4d u, vec4d v);

bool vec4d_equal(vec4d u, vec4d v);

mat4d mat4d_identity();

vec4d mat4d_row(mat4d m, int r);

vec4d mat4d_column(mat4d m, int c);

mat4d mat4d_from_vec4d(vec4d r0, vec4d r1, vec4d r2, vec4d r3);

mat4d mat4d_transpose(mat4d m);

mat4d mat4d_multiply(mat4d m, mat4d n);

vec4d mat4d_multiply_vec4d(mat4d m, vec4d v);

mat4d mat4d_scale(mat4d m, double x, double y, double z);

mat4d mat4d_translate(mat4d m, double x, double y, double z);

mat4d mat4d_rotate(mat4d m, vec4d axis, double degree);

mat4d mat4d_frustum(double l, double r, double b, double t, double n, double f);

mat4d mat4d_perspective(double fov, double aspect, double n, double f);

mat4d mat4d_ortho(double l, double r, double b, double t, double n, double f);

mat4d mat4d_look_at(vec4d eye, vec4d center, vec4d up);

mat4f mat4d_to_mat4f(mat4d m);

bool mat4d_equal(mat4d m, mat4d n);

#define vec4_print(v)  do { \
  for (int i = 0; i < 4; i++) { \
    printf("%f\t", v[i]); \
  } \
  printf("\n"); \
} while (0)

#define mat4_print(m) do { \
  for (int i = 0; i < 16; i++) { \
    if (i % 4 == 3) { \
      printf("%f\n", m[i]); \
    } else { \
      printf("%f\t", m[i]); \
    } \
  } \
  printf("\n"); \
} while (0)

#ifdef __cplusplus
}
#endif

#endif
