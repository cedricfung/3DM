#ifndef _3DM_POLY_H
#define _3DM_POLY_H

enum poly_type {
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
} poly_t;

poly_t *poly_create(enum poly_type type, int n);

void poly_destroy(poly_t *poly);

#endif
