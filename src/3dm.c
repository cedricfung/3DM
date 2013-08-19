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
#include "3dm/3dm.h"

double vec4d_sum(vec4d v)
{
  return v.ptr[0] + v.ptr[1] + v.ptr[2] + v.ptr[3];
}

double vec4d_dot_product(vec4d u, vec4d v)
{
  return vec4d_sum(vector_multiply(u, v));
}

double vec4d_length(vec4d v)
{
  return sqrt(vec4d_dot_product(v, v));
}

vec4d vec4d_normalize(vec4d v)
{
  double l = vec4d_length(v);
  return l == 0 ? v : vector_scale(v, 1.0 / l);
}

mat4d vec4d_cross_matrix(vec4d v)
{
  mat4d m = {.vex = {0}};
  m.ptr[1] = -v.ptr[2];
  m.ptr[2] = v.ptr[1];
  m.ptr[4] = v.ptr[2];
  m.ptr[6] = -v.ptr[0];
  m.ptr[8] = -v.ptr[1];
  m.ptr[9] = v.ptr[0];
  return m;
}

vec4d vec4d_cross_product(vec4d u, vec4d v)
{
  return mat4d_multiply_vec4d(vec4d_cross_matrix(u), v);
}

mat4d vec4d_tensor_product(vec4d u, vec4d v)
{
  return mat4d_from_vec4d(vector_scale(v, u.ptr[0]), vector_scale(v, u.ptr[1]), vector_scale(v, u.ptr[2]), vector_scale(v, u.ptr[3]));
}

bool vec4d_equal(vec4d u, vec4d v)
{
  bool e = true;
  for (int i = 0; i < 4 && (e = (u.ptr[i] == v.ptr[i])); i++);
  return e;
}

mat4d mat4d_identity(void)
{
  mat4d m = {.vex = {0}};
  m.ptr[0] = 1;
  m.ptr[5] = 1;
  m.ptr[10] = 1;
  m.ptr[15] = 1;
  return m;
}

vec4d mat4d_row(mat4d m, int r)
{
  vec4d v = {.vex = {0}};
  r = r * 4;
  v.ptr[0] = m.ptr[r];
  v.ptr[1] = m.ptr[r+1];
  v.ptr[2] = m.ptr[r+2];
  v.ptr[3] = m.ptr[r+3];
  return v;
}

vec4d mat4d_column(mat4d m, int c)
{
  vec4d v = {.vex = {0}};
  v.ptr[0] = m.ptr[c];
  v.ptr[1] = m.ptr[c+4];
  v.ptr[2] = m.ptr[c+8];
  v.ptr[3] = m.ptr[c+12];
  return v;
}

mat4d mat4d_from_vec4d(vec4d r0, vec4d r1, vec4d r2, vec4d r3)
{
  mat4d m = {.vex = {0}};
  for (int i = 0; i < 4; i++) { m.ptr[i] = r0.ptr[i]; }
  for (int i = 0; i < 4; i++) { m.ptr[i+4] = r1.ptr[i]; }
  for (int i = 0; i < 4; i++) { m.ptr[i+8] = r2.ptr[i]; }
  for (int i = 0; i < 4; i++) { m.ptr[i+12] = r3.ptr[i]; }
  return m;
}

mat4d mat4d_transpose(mat4d m)
{
  mat4d mt = {.vex = {0}};
  vector(long, 16) mask = {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};
  mt.vex = vector_shuffle(m.vex, mask);
  return mt;
}

mat4d mat4d_multiply(mat4d m, mat4d n)
{
  mat4d r = {.vex = {0}};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      r.ptr[i * 4 + j] = vec4d_dot_product(mat4d_row(m, i), mat4d_column(n, j));
    }
  }
  return r;
}

vec4d mat4d_multiply_vec4d(mat4d m, vec4d v)
{
  vec4d vr = {.vex = {0}};
  vr.ptr[0] = vec4d_dot_product(mat4d_row(m, 0), v);
  vr.ptr[1] = vec4d_dot_product(mat4d_row(m, 1), v);
  vr.ptr[2] = vec4d_dot_product(mat4d_row(m, 2), v);
  vr.ptr[3] = vec4d_dot_product(mat4d_row(m, 3), v);
  return vr;
}

mat4d mat4d_scale(mat4d m, double x, double y, double z)
{
  mat4d s = mat4d_identity();
  s.ptr[0] = x;
  s.ptr[5] = y;
  s.ptr[10] = z;
  return mat4d_multiply(s, m);
}

mat4d mat4d_translate(mat4d m, double x, double y, double z)
{
  mat4d t = mat4d_identity();
  t.ptr[3] = x;
  t.ptr[7] = y;
  t.ptr[11] = z;
  return mat4d_multiply(t, m);
}

mat4d mat4d_rotate(mat4d m, vec4d axis, double degree)
{
  double rad = degree * M_PI / 180;
  double s = sin(rad), c = cos(rad);
  vec4d u = vec4d_normalize(axis);
  mat4d m1 = vector_scale(mat4d_identity(), c);
  mat4d m2 = vector_scale(vec4d_cross_matrix(axis), s);
  mat4d m3 = vector_scale(vec4d_tensor_product(u, u), 1 - c);
  mat4d rotate = vector_add(vector_add(m1, m2), m3);
  rotate.ptr[15] = 1;
  return mat4d_multiply(rotate, m);
}

mat4d mat4d_frustum(double l, double r, double b, double t, double n, double f)
{
  mat4d m = {.vex = {0}};
  m.ptr[0] = (2 * n) / (r - l);
  m.ptr[2] = (r + l) / (r - l);
  m.ptr[5] = (2 * n) / (t - b);
  m.ptr[6] = (t + b) / (t - b);
  m.ptr[10] = -(f + n) / (f - n);
  m.ptr[11] = -(2 * f * n) / (f - n);
  m.ptr[14] = -1;
  return m;
}

mat4d mat4d_perspective(double fov, double aspect, double n, double f)
{
  double t = n * tan(fov * M_PI / 360);
  double r = t * aspect;
  return mat4d_frustum(-r, r, -t, t, n, f);
}

mat4d mat4d_frustum_ortho(double l, double r, double b, double t, double n, double f)
{
  mat4d m = {.vex = {0}};
  m.ptr[0] = 2 / (r - l);
  m.ptr[3] = -(r + l) / (r - l);
  m.ptr[5] = 2 / (t - b);
  m.ptr[7] = -(t + b) / (t - b);
  m.ptr[10] = -2 / (f - n);
  m.ptr[11] = -(f + n) / (f - n);
  m.ptr[15] = 1;
  return m;
}

mat4d mat4d_ortho(double fov, double aspect, double n, double f)
{
  double t = n * tan(fov * M_PI / 360);
  double r = t * aspect;
  return mat4d_frustum_ortho(-r, r, -t, t, n, f);
}

mat4d mat4d_look_at(vec4d eye, vec4d center, vec4d up)
{
  vec4d x = {.vex = {0}}, y = {.vex = {0}}, z = {.vex = {0}}, w = {.vex = {0}};

  if (vec4d_equal(eye, center)) {
    return mat4d_identity();
  }

  z = vec4d_normalize(vector_add(eye, vector_scale(center, -1)));
  x = vec4d_normalize(vec4d_cross_product(up, z));
  y = vec4d_normalize(vec4d_cross_product(z, x));

  x.ptr[3] = -vec4d_dot_product(x, eye);
  y.ptr[3] = -vec4d_dot_product(y, eye);
  z.ptr[3] = -vec4d_dot_product(z, eye);
  w.ptr[3] = 1;

  return mat4d_from_vec4d(x, y, z, w);
}

mat4f mat4d_to_mat4f(mat4d m)
{
  mat4f f = {.vex = {0}};
  for (int i = 0; i < 16; i++) {
    f.ptr[i] = (float)m.ptr[i];
  }
  return f;
}

bool mat4d_equal(mat4d m, mat4d n)
{
  bool e = true;
  for (int i = 0; i < 16 && (e = (m.ptr[i] == n.ptr[i])); i++);
  return e;
}
