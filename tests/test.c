#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "3dm/3dm.h"
#include "3dm/poly.h"

#define assert_vec4d_equal(u, v) do { \
  if (!vec4d_equal(u, v)) { \
    fprintf(stderr, "assert_vec4d_equal: %d\n", __LINE__); \
    vector_print(u, 4); \
    vector_print(v, 4); \
    abort(); \
  } \
} while (0)

#define assert_mat4d_equal(m, n) do { \
  if (!mat4d_equal(m, n)) { \
    fprintf(stderr, "assert_mat4d_equal: %d\n", __LINE__); \
    vector_print(m, 16); \
    vector_print(n, 16); \
    abort(); \
  } \
} while (0)

#define test_begin(name) \
  struct timespec ts; do { \
    clock_gettime(CLOCK_REALTIME, &ts); \
    printf("TEST BEGIN: %s\n", name); \
  } while (0)
#define test_end(name) do { \
  struct timespec tp; \
  clock_gettime(CLOCK_REALTIME, &tp); \
  long us = (tp.tv_sec - ts.tv_sec) * 1000000 + (tp.tv_nsec - ts.tv_nsec) / 1000; \
  printf("TEST END: %s\t TIME(us): %ld\n\n", name, us); \
} while (0)

vec4d u = vector_new(1, 2, 3, 4);
vec4d v = vector_new(7, 7, 7, 7);
vec4d z = vector_new(0, 0, 0, 0);
mat4d m = vector_new(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
mat4d n = vector_new(7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7);
mat4d I = vector_new(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

double u_sum = 10;
double v_sum = 28;

double uu_dot_product = 30;
double vv_dot_product = 196;
double uv_dot_product = 70;
double vu_dot_product = 70;

vec4d uu_add = vector_new(2, 4, 6, 8);
vec4d uv_add = vector_new(8, 9, 10, 11);
vec4d vv_add = vector_new(14, 14, 14, 14);
vec4d vu_add = vector_new(8, 9, 10, 11);

vec4d u1_scale = vector_new(1, 2, 3, 4);
vec4d u2_scale = vector_new(2, 4, 6, 8);

vec4d uu_multiply = vector_new(1, 4, 9, 16);
vec4d uv_multiply = vector_new(7, 14, 21, 28);
vec4d vv_multiply = vector_new(49, 49, 49, 49);
vec4d vu_multiply = vector_new(7, 14, 21, 28);

mat4d u_cross_matrix = vector_new(0, -3, 2, 0, 3, 0, -1, 0, -2, 1, 0, 0, 0, 0, 0, 0);
mat4d v_cross_matrix = vector_new(0, -7, 7, 0, 7, 0, -7, 0, -7, 7, 0, 0, 0, 0, 0, 0);

vec4d uu_cross_product = vector_new(0, 0, 0, 0);
vec4d vv_cross_product = vector_new(0, 0, 0, 0);
vec4d uv_cross_product = vector_new(2*7-3*7, 3*7-1*7, 1*7-2*7, 0);
vec4d vu_cross_product = vector_new(7*3-7*2, 7*1-7*3, 7*2-7*1, 0);

mat4d uu_tensor_product = vector_new(1, 2, 3, 4, 2, 4, 6, 8, 3, 6, 9, 12, 4, 8, 12, 16);
mat4d vv_tensor_product = vector_new(49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49);
mat4d uv_tensor_product = vector_new(7, 7, 7, 7, 14, 14, 14, 14, 21, 21, 21, 21, 28, 28, 28, 28);
mat4d vu_tensor_product = vector_new(7, 14, 21, 28, 7, 14, 21, 28, 7, 14, 21, 28, 7, 14, 21, 28);

vec4d m_row0 = vector_new(1, 2, 3, 4);
vec4d m_row1 = vector_new(5, 6, 7, 8);
vec4d m_row2 = vector_new(9, 10, 11, 12);
vec4d m_row3 = vector_new(13, 14, 15, 16);

vec4d m_col0 = vector_new(1, 5, 9, 13);
vec4d m_col1 = vector_new(2, 6, 10, 14);
vec4d m_col2 = vector_new(3, 7, 11, 15);
vec4d m_col3 = vector_new(4, 8, 12, 16);

mat4d m_transpose = vector_new(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16);
mat4d n_transpose = vector_new(7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7);

mat4d mn_multiply = vector_new(70, 70, 70, 70, 182, 182, 182, 182, 294, 294, 294, 294, 406, 406, 406, 406);
mat4d nm_multiply = vector_new(196, 224, 252, 280, 196, 224, 252, 280, 196, 224, 252, 280, 196, 224, 252, 280);

vec4d mu_multiply = vector_new(30, 70, 110, 150);
vec4d mv_multiply = vector_new(70, 182, 294, 406);

mat4d I123_scale = vector_new(1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1);
mat4d I123_translate = vector_new(1, 0, 0, 1, 0, 1, 0, 2, 0, 0, 1, 3, 0, 0, 0, 1);
mat4d I123_scale_123_translate = vector_new(1, 0, 0, 1, 0, 2, 0, 2, 0, 0, 3, 3, 0, 0, 0, 1);
mat4d I001_10_rotate = vector_new(0.98480775301220802031565426659654,
    -0.17364817766693033118663436198403, 0, 0,
    0.17364817766693033118663436198403,
    0.98480775301220802031565426659654,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
mat4d I001_n10_rotate = vector_new(0.98480775301220802031565426659654,
    0.17364817766693033118663436198403, 0, 0,
    -0.17364817766693033118663436198403,
    0.98480775301220802031565426659654,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

mat4d r_frustum = vector_new(0.40000000000000002220446049250313, 0,
    0.20000000000000001110223024625157, 0, 0,
    0.40000000000000002220446049250313,
    -0.20000000000000001110223024625157, 0, 0, 0,
    -1.00100050025012499155252498894697,
    -2.00100050025012521359712991397828, 0, 0, -1, 0);
mat4d r_ortho = vector_new(0.40000000000000002220446049250313, 0, 0,
    -0.20000000000000001110223024625157, 0,
    0.40000000000000002220446049250313, 0,
    0.20000000000000001110223024625157, 0, 0,
    -0.00100050025012506245934706949896,
    -1.00100050025012499155252498894697, 0, 0, 0, 1);
mat4d r_look_at = vector_new(-0.57735026918962584208117050366127,
    0.57735026918962584208117050366127,
    -0.57735026918962584208117050366127,
    0.57735026918962584208117050366127,
    -0.40824829046386301723003953156876,
    0.40824829046386301723003953156876,
    0.81649658092772603446007906313753,
    -0.81649658092772603446007906313753,
    0.70710678118654746171500846685376,
    0.70710678118654746171500846685376, 0,
    -1.41421356237309492343001693370752, 0, 0, 0, 1);

void test_vector()
{
  test_begin("test_vector");
  vector(double, 4) _v = {1.0, 2.01, 3, 4.5};
  assert(_v[0] == 1.0);
  assert(_v[1] == 2.01);
  assert(_v[2] == 3);
  assert(_v[3] == 4.5);

  assert_vec4d_equal(vector_add(u, u), uu_add);
  assert_vec4d_equal(vector_add(u, v), uv_add);
  assert_vec4d_equal(vector_add(v, v), vv_add);
  assert_vec4d_equal(vector_add(v, u), vu_add);

  assert_vec4d_equal(vector_scale(u, 1), u1_scale);
  assert_vec4d_equal(vector_scale(u, 2), u2_scale);

  assert_vec4d_equal(vector_multiply(u, u), uu_multiply);
  assert_vec4d_equal(vector_multiply(u, v), uv_multiply);
  assert_vec4d_equal(vector_multiply(v, v), vv_multiply);
  assert_vec4d_equal(vector_multiply(v, u), vu_multiply);
  test_end("test_vector");
}

void test_equal()
{
  test_begin("test_equal");
  assert(vec4d_equal(u, u) == true);
  assert(vec4d_equal(v, v) == true);
  assert(vec4d_equal(u, v) == false);
  assert(vec4d_equal(v, u) == false);
  assert(mat4d_equal(m, m) == true);
  assert(mat4d_equal(n, n) == true);
  assert(mat4d_equal(m, n) == false);
  assert(mat4d_equal(n, m) == false);
  test_end("test_equal");
}

void test_vec4()
{
  test_begin("test_vec4");
  assert(vec4d_sum(u) == u_sum);
  assert(vec4d_sum(v) == v_sum);

  assert(vec4d_dot_product(u, u) == uu_dot_product);
  assert(vec4d_dot_product(v, v) == vv_dot_product);
  assert(vec4d_dot_product(u, v) == uv_dot_product);
  assert(vec4d_dot_product(v, u) == vu_dot_product);

  assert(vec4d_length(u) == sqrt(uu_dot_product));
  assert(vec4d_length(v) == sqrt(vv_dot_product));
  assert(vec4d_length(z) == 0);

  assert_vec4d_equal(vec4d_normalize(u), vector_scale(u, 1.0 / vec4d_length(u)));
  assert_vec4d_equal(vec4d_normalize(v), vector_scale(v, 1.0 / vec4d_length(v)));
  assert_vec4d_equal(vec4d_normalize(z), z);

  assert_mat4d_equal(vec4d_cross_matrix(u), u_cross_matrix);
  assert_mat4d_equal(vec4d_cross_matrix(v), v_cross_matrix);

  assert_vec4d_equal(vec4d_cross_product(u, u), uu_cross_product);
  assert_vec4d_equal(vec4d_cross_product(v, v), vv_cross_product);
  assert_vec4d_equal(vec4d_cross_product(u, v), uv_cross_product);
  assert_vec4d_equal(vec4d_cross_product(v, u), vu_cross_product);

  assert_mat4d_equal(vec4d_tensor_product(u, u), uu_tensor_product);
  assert_mat4d_equal(vec4d_tensor_product(v, v), vv_tensor_product);
  assert_mat4d_equal(vec4d_tensor_product(u, v), uv_tensor_product);
  assert_mat4d_equal(vec4d_tensor_product(v, u), vu_tensor_product);

  test_end("test_vec4");
}

void test_mat4()
{
  test_begin("test_mat4");
  assert_mat4d_equal(mat4d_identity(), I);

  assert_vec4d_equal(mat4d_row(m, 0), m_row0);
  assert_vec4d_equal(mat4d_row(m, 1), m_row1);
  assert_vec4d_equal(mat4d_row(m, 2), m_row2);
  assert_vec4d_equal(mat4d_row(m, 3), m_row3);

  assert_vec4d_equal(mat4d_column(m, 0), m_col0);
  assert_vec4d_equal(mat4d_column(m, 1), m_col1);
  assert_vec4d_equal(mat4d_column(m, 2), m_col2);
  assert_vec4d_equal(mat4d_column(m, 3), m_col3);

  assert_mat4d_equal(mat4d_from_vec4d(m_row0, m_row1, m_row2, m_row3), m);
  assert_mat4d_equal(mat4d_from_vec4d(m_col0, m_col1, m_col2, m_col3), m_transpose);

  assert_mat4d_equal(mat4d_transpose(I), I);
  assert_mat4d_equal(mat4d_transpose(m), m_transpose);
  assert_mat4d_equal(mat4d_transpose(n), n_transpose);

  assert_mat4d_equal(mat4d_multiply(m, I), m);
  assert_mat4d_equal(mat4d_multiply(I, m), m);
  assert_mat4d_equal(mat4d_multiply(n, I), n);
  assert_mat4d_equal(mat4d_multiply(I, n), n);
  assert_mat4d_equal(mat4d_multiply(I, I), I);
  assert_mat4d_equal(mat4d_multiply(m, n), mn_multiply);
  assert_mat4d_equal(mat4d_multiply(n, m), nm_multiply);

  assert_vec4d_equal(mat4d_multiply_vec4d(I, u), u);
  assert_vec4d_equal(mat4d_multiply_vec4d(I, v), v);
  assert_vec4d_equal(mat4d_multiply_vec4d(m, u), mu_multiply);
  assert_vec4d_equal(mat4d_multiply_vec4d(m, v), mv_multiply);

  assert_mat4d_equal(mat4d_scale(I, 1, 2, 3), I123_scale);
  assert_mat4d_equal(mat4d_scale(m, 1, 2, 3), mat4d_multiply(I123_scale, m));
  assert_mat4d_equal(mat4d_scale(n, 1, 2, 3), mat4d_multiply(I123_scale, n));

  assert_mat4d_equal(mat4d_translate(I, 1, 2, 3), I123_translate);
  assert_mat4d_equal(mat4d_translate(m, 1, 2, 3), mat4d_multiply(I123_translate, m));
  assert_mat4d_equal(mat4d_translate(n, 1, 2, 3), mat4d_multiply(I123_translate, n));

  assert_mat4d_equal(mat4d_translate(mat4d_scale(I, 1, 2, 3), 1, 2, 3), I123_scale_123_translate);
  assert_mat4d_equal(mat4d_translate(mat4d_scale(m, 1, 2, 3), 1, 2, 3), mat4d_multiply(I123_scale_123_translate, m));

  assert_mat4d_equal(mat4d_rotate(I, (vec4d)vector_new(0, 0, 1), 10), I001_10_rotate);
  assert_mat4d_equal(mat4d_rotate(I, (vec4d)vector_new(0, 0, 1), -10), I001_n10_rotate);

  assert_mat4d_equal(mat4d_frustum(-2, 3, -3, 2, 1, 2000), r_frustum);
  assert_mat4d_equal(mat4d_frustum_ortho(-2, 3, -3, 2, 1, 2000), r_ortho);
  assert_mat4d_equal(mat4d_look_at((vec4d)vector_new(1), (vec4d)vector_new(1), (vec4d)vector_new(0,1)), I);
  assert_mat4d_equal(mat4d_look_at((vec4d)vector_new(1,1,1), (vec4d)vector_new(0,0,1), (vec4d)vector_new(0,0.5,0.5)), r_look_at);

  test_end("test_mat4");
}

void test_poly_icosahedron()
{
  test_begin("test_poly_icosahedron");
  poly_t *poly = poly_create(POLY_ICOSAHEDRON, 64*64);
  assert(poly == NULL);
  poly = poly_create(POLY_ICOSAHEDRON, 7);
  assert(poly != NULL);
  poly_destroy(poly);
  test_end("test_poly_icosahedron");
}

void test_poly_cube()
{
  test_begin("test_poly_cube");
  poly_t *poly = poly_create(POLY_CUBE, 0);
  assert(poly != NULL);
  poly_destroy(poly);
  poly = poly_create(POLY_CUBE, 7);
  assert(poly != NULL);
  poly_destroy(poly);
  test_end("test_poly_cube");
}

int main(int argc, const char *argv[])
{
  test_vector();
  test_equal();
  test_vec4();
  test_mat4();
  test_poly_icosahedron();
  test_poly_cube();
  return 0;
}
