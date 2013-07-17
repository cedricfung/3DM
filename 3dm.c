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


#define _GNU_SOURCE
#include <stdio.h>
#include <math.h>
#include "3dm.h"

double vec4d_sum(vec4d v)
{
  return v[0] + v[1] + v[2] + v[3];
}

double vec4d_dot_product(vec4d u, vec4d v)
{
  return vec4d_sum(u * v);
}

double vec4d_length(vec4d v)
{
  return sqrt(vec4d_dot_product(v, v));
}

vec4d vec4d_normalize(vec4d v)
{
  double l = vec4d_length(v);
  return l == 0 ? v : v / l;
}

mat4d vec4d_cross_matrix(vec4d v)
{
  return (mat4d){0, -v[2], v[1], 0, v[2], 0, -v[0], 0, -v[1], v[0], 0, 0, 0, 0, 0, 0};
}

vec4d vec4d_cross_product(vec4d u, vec4d v)
{
  return mat4d_multiply_vec4d(vec4d_cross_matrix(u), v);
}

mat4d vec4d_tensor_product(vec4d u, vec4d v)
{
  return mat4d_from_vec4d(u[0] * v, u[1] * v, u[2] * v, u[3] * v);
}

bool vec4d_equal(vec4d u, vec4d v)
{
  bool e = true;
  for (int i = 0; i < 4 && (e = (u[i] == v[i])); i++);
  return e;
}

mat4d mat4d_identity()
{
  return (mat4d){1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
}

vec4d mat4d_row(mat4d m, int r)
{
  r = r * 4;
  return (vec4d){m[r], m[r+1], m[r+2], m[r+3]};
}

vec4d mat4d_column(mat4d m, int c)
{
  return (vec4d){m[c], m[c+4], m[c+8], m[c+12]};
}

mat4d mat4d_from_vec4d(vec4d r0, vec4d r1, vec4d r2, vec4d r3)
{
  return (mat4d)
  { r0[0],r0[1],r0[2],r0[3],
    r1[0],r1[1],r1[2],r1[3],
    r2[0],r2[1],r2[2],r2[3],
    r3[0],r3[1],r3[2],r3[3] };
}

mat4d mat4d_transpose(mat4d m)
{
  vector(long, 16) mask = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};
  return __builtin_shuffle(m, mask);
}

mat4d mat4d_multiply(mat4d m, mat4d n)
{
  mat4d r = {};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      r[i * 4 + j] = vec4d_dot_product(mat4d_row(m, i), mat4d_column(n, j));
    }
  }
  return r;
}

vec4d mat4d_multiply_vec4d(mat4d m, vec4d v)
{
  return (vec4d){
    vec4d_dot_product(mat4d_row(m, 0), v),
      vec4d_dot_product(mat4d_row(m, 1), v),
      vec4d_dot_product(mat4d_row(m, 2), v),
      vec4d_dot_product(mat4d_row(m, 3), v)
  };
}

mat4d mat4d_scale(mat4d m, double x, double y, double z)
{
  mat4d s = mat4d_identity();
  s[0] = x;
  s[5] = y;
  s[10] = z;
  return mat4d_multiply(s, m);
}

mat4d mat4d_translate(mat4d m, double x, double y, double z)
{
  mat4d t = mat4d_identity();
  t[3] = x;
  t[7] = y;
  t[11] = z;
  return mat4d_multiply(t, m);
}

mat4d mat4d_rotate(mat4d m, vec4d axis, double degree)
{
  double rad = degree * M_PI / 180;
  double s = sin(rad), c = cos(rad);
  vec4d u = vec4d_normalize(axis);
  mat4d rotate = c * mat4d_identity() + s * vec4d_cross_matrix(axis) + (1 - c) * vec4d_tensor_product(u, u);
  rotate[15] = 1;
  return mat4d_multiply(rotate, m);
}

mat4d mat4d_frustum(double l, double r, double b, double t, double n, double f)
{
  mat4d m = {0};
  m[0] = (2 * n) / (r - l);
  m[2] = (r + l) / (r - l);
  m[5] = (2 * n) / (t - b);
  m[6] = (t + b) / (t - b);
  m[10] = -(f + n) / (f - n);
  m[11] = -(2 * f * n) / (f - n);
  m[14] = -1;
  return m;
}

mat4d mat4d_perspective(double fov, double aspect, double n, double f)
{
  double t = n * tan(fov * M_PI / 360);
  double r = t * aspect;
  return mat4d_frustum(-r, r, -t, t, n, f);
}

mat4d mat4d_ortho(double l, double r, double b, double t, double n, double f)
{
  mat4d m = {0};
  m[0] = 2 / (r - l);
  m[3] = -(r + l) / (r - l);
  m[5] = 2 / (t - b);
  m[7] = -(t + b) / (t - b);
  m[10] = -2 / (f - n);
  m[11] = -(f + n) / (f - n);
  m[15] = 1;
  return m;
}

mat4d mat4d_look_at(vec4d eye, vec4d center, vec4d up)
{
  vec4d x, y, z;

  if (vec4d_equal(eye, center)) {
    return mat4d_identity();
  }

  z = vec4d_normalize(eye - center);
  x = vec4d_normalize(vec4d_cross_product(up, z));
  y = vec4d_normalize(vec4d_cross_product(z, x));

  x[3] = -vec4d_dot_product(x, eye);
  y[3] = -vec4d_dot_product(y, eye);
  z[3] = -vec4d_dot_product(z, eye);

  return mat4d_from_vec4d(x, y, z, (vec4d){0,0,0,1});
}

mat4f mat4d_to_mat4f(mat4d m)
{
  mat4f f = {0};
  for (int i = 0; i < 16; i++) {
    f[i] = (float)m[i];
  }
  return f;
}

bool mat4d_equal(mat4d m, mat4d n)
{
  bool e = true;
  for (int i = 0; i < 16 && (e = (m[i] == n[i])); i++);
  return e;
}
