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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "3dm.h"
#include "poly.h"

#define M_PHI 1.618033988749895  //((1 + sqrt(5)) / 2)

static bool texcoords_calculate(poly_t *poly)
{
  float x, y, z;
  poly->t_len = poly->v_len / 3 * 2;
  poly->texcoords = malloc(poly->t_len * sizeof(float));
  if (poly->texcoords == NULL) {
    return false;
  }

  for (int i = 0; i < poly->v_len / 3; i++) {
    x = poly->vertices[3*i];
    y = poly->vertices[3*i+1];
    z = poly->vertices[3*i+2];
    poly->texcoords[2*i] = (1.0f + atan2f(y, x) / M_PI) * 0.5f;
    poly->texcoords[2*i+1] = (1.0f - z) * 0.5f;
  }

  return true;
}

static bool icosahedron_recur(poly_t *poly)
{
  vec4d v1, v2, v3, v12, v23, v31;
  int i1, i2, i3, i12, i23, i31;
  int vdi = poly->v_len - 1, idi = poly->i_len - 1;
  int *indices = realloc(poly->indices, (poly->i_len + poly->i_len * 4) * sizeof(int));
  float *vertices = realloc(poly->vertices, (poly->v_len + poly->i_len * 3) * sizeof(float));
  if (indices == NULL || vertices == NULL) {
    return false;
  }

  for (int i = 0; i < poly->i_len; i += 3) {
    i1 = indices[i]; i2 = indices[i+1]; i3 = indices[i+2];
    v1 = (vec4d){vertices[3*i1], vertices[3*i1+1], vertices[3*i1+2]};
    v2 = (vec4d){vertices[3*i2], vertices[3*i2+1], vertices[3*i2+2]};
    v3 = (vec4d){vertices[3*i3], vertices[3*i3+1], vertices[3*i3+2]};
    v12 = vec4d_normalize(v1 + v2);
    v23 = vec4d_normalize(v2 + v3);
    v31 = vec4d_normalize(v3 + v1);
    vertices[++vdi] = v12[0]; vertices[++vdi] = v12[1]; vertices[++vdi] = v12[2]; i12 = (vdi + 1) / 3 - 1;
    vertices[++vdi] = v23[0]; vertices[++vdi] = v23[1]; vertices[++vdi] = v23[2]; i23 = (vdi + 1) / 3 - 1;
    vertices[++vdi] = v31[0]; vertices[++vdi] = v31[1]; vertices[++vdi] = v31[2]; i31 = (vdi + 1) / 3 - 1;
    indices[++idi] = i1; indices[++idi] = i12; indices[++idi] = i31;
    indices[++idi] = i2; indices[++idi] = i23; indices[++idi] = i12;
    indices[++idi] = i3; indices[++idi] = i31; indices[++idi] = i23;
    indices[++idi] = i12; indices[++idi] = i23; indices[++idi] = i31;
  }

  poly->vertices = vertices;
  poly->indices = indices;
  poly->v_len = poly->v_len + poly->i_len * 3;
  poly->i_len = poly->i_len + poly->i_len * 4;
  printf("VERTICES: %d, \tTRIANGLES: %d\n", poly->v_len / 3, poly->i_len / 3);

  return true;
}

static poly_t *icosahedron_create(int n)
{
  const float icosahedron_vertices[36] = {
    // [(0,+/-1, +/-phi), (+/-1, +/-phi, 0), (+/-phi, 0, +/-1)]
    0, 1, M_PHI, 0, 1, -M_PHI, 0, -1, M_PHI, 0, -1, -M_PHI,
    1, M_PHI, 0, 1, -M_PHI, 0, -1, M_PHI, 0, -1, -M_PHI, 0,
    M_PHI, 0, 1, M_PHI, 0, -1, -M_PHI, 0, 1, -M_PHI, 0, -1,
  };
  const int icosahedron_indices[60] = {
    0, 2, 8, 0, 2, 10, 0, 4, 6, 0, 4, 8, 0, 6, 10,
    1, 3, 9, 1, 3, 11, 1, 4, 6, 1, 4, 9, 1, 6, 11,
    2, 5, 7, 2, 5, 8, 2, 7, 10, 3, 5, 7, 3, 5, 9,
    3, 7, 11, 4, 8, 9, 5, 8, 9, 6, 10, 11, 7, 10, 11
  };
  const float radius = vec4d_length((vec4d){1, M_PHI});
  int ret = 0;

  poly_t *poly = calloc(1, sizeof(poly_t));
  if (poly == NULL) {
    return NULL;
  }
  poly->type = POLY_ICOSAHEDRON;

  poly->vertices = malloc(sizeof(icosahedron_vertices));
  if (poly->vertices == NULL) {
    ret = -1;
    goto end;
  }
  poly->v_len = sizeof(icosahedron_vertices) / sizeof(float);
  memcpy(poly->vertices, icosahedron_vertices, sizeof(icosahedron_vertices));
  for (int i = 0; i < poly->v_len; i++) {
    poly->vertices[i] /= radius;
  }

  poly->indices = malloc(sizeof(icosahedron_indices));
  if (poly->indices == NULL) {
    ret = -1;
    goto end;
  }
  poly->i_len = sizeof(icosahedron_indices) / sizeof(int);
  memcpy(poly->indices, icosahedron_indices, sizeof(icosahedron_indices));

  for (int i = 1; i < n; i++) {
    if (!icosahedron_recur(poly)) {
      ret = -1;
      goto end;
    }
  }

  if (!texcoords_calculate(poly)) {
    ret = -1;
    goto end;
  }

end:
  if (ret != 0) {
    poly_destroy(poly);
    return NULL;
  }
  return poly;
}

poly_t *poly_create(enum poly_type type, int n)
{
  switch (type) {
    case POLY_ICOSAHEDRON:
      return icosahedron_create(n);
    default:
      return NULL;
  }
}

void poly_destroy(poly_t *poly)
{
  free(poly->vertices);
  free(poly->indices);
  free(poly->texcoords);
  free(poly);
}
