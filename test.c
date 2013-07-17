#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "3dm.h"

#define assert_vec4d_equal(u, v) do { \
  if (!vec4d_equal(u, v)) { \
    fprintf(stderr, "assert_vec4d_equal: %d\n", __LINE__); \
    vec4_print(u); \
    vec4_print(v); \
    abort(); \
  } \
} while (0)

#define assert_mat4d_equal(m, n) do { \
  if (!mat4d_equal(m, n)) { \
    fprintf(stderr, "assert_mat4d_equal: %d\n", __LINE__); \
    mat4_print(m); \
    mat4_print(n); \
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

vec4d u = {1, 2, 3, 4};
vec4d v = {7, 7, 7, 7};
vec4d z = {0, 0, 0, 0};
mat4d m = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
mat4d n = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};
mat4d I = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

double u_sum = 10;
double v_sum = 28;

double uu_dot_product = 30;
double vv_dot_product = 196;
double uv_dot_product = 70;
double vu_dot_product = 70;

mat4d u_cross_matrix = {0, -3, 2, 0, 3, 0, -1, 0, -2, 1, 0, 0, 0, 0, 0, 0};
mat4d v_cross_matrix = {0, -7, 7, 0, 7, 0, -7, 0, -7, 7, 0, 0, 0, 0, 0, 0};

vec4d uu_cross_product = {0, 0, 0, 0};
vec4d vv_cross_product = {0, 0, 0, 0};
vec4d uv_cross_product = {2*7-3*7, 3*7-1*7, 1*7-2*7, 0};
vec4d vu_cross_product = {7*3-7*2, 7*1-7*3, 7*2-7*1, 0};

mat4d uu_tensor_product = {1, 2, 3, 4, 2, 4, 6, 8, 3, 6, 9, 12, 4, 8, 12, 16};
mat4d vv_tensor_product = {49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49};
mat4d uv_tensor_product = {7, 7, 7, 7, 14, 14, 14, 14, 21, 21, 21, 21, 28, 28, 28, 28};
mat4d vu_tensor_product = {7, 14, 21, 28, 7, 14, 21, 28, 7, 14, 21, 28, 7, 14, 21, 28};

vec4d m_row0 = {1, 2, 3, 4};
vec4d m_row1 = {5, 6, 7, 8};
vec4d m_row2 = {9, 10, 11, 12};
vec4d m_row3 = {13, 14, 15, 16};

vec4d m_col0 = {1, 5, 9, 13};
vec4d m_col1 = {2, 6, 10, 14};
vec4d m_col2 = {3, 7, 11, 15};
vec4d m_col3 = {4, 8, 12, 16};

mat4d m_transpose = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};
mat4d n_transpose = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7};

mat4d mn_multiply = {70, 70, 70, 70, 182, 182, 182, 182, 294, 294, 294, 294, 406, 406, 406, 406};
mat4d nm_multiply = {196, 224, 252, 280, 196, 224, 252, 280, 196, 224, 252, 280, 196, 224, 252, 280};

vec4d mu_multiply = {30, 70, 110, 150};
vec4d mv_multiply = {70, 182, 294, 406};

mat4d I123_scale = {1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1};
mat4d I123_translate = {1, 0, 0, 1, 0, 1, 0, 2, 0, 0, 1, 3, 0, 0, 0, 1};
mat4d I123_scale_123_translate = {1, 0, 0, 1, 0, 2, 0, 2, 0, 0, 3, 3, 0, 0, 0, 1};

mat4d r_frustum = {0.40000000000000002220446049250313, 0,
  0.20000000000000001110223024625157, 0, 0,
  0.40000000000000002220446049250313,
  -0.20000000000000001110223024625157, 0, 0, 0,
  -1.00100050025012499155252498894697,
  -2.00100050025012521359712991397828, 0, 0, -1, 0};
mat4d r_ortho = {0.40000000000000002220446049250313, 0, 0,
  -0.20000000000000001110223024625157, 0,
  0.40000000000000002220446049250313, 0,
  0.20000000000000001110223024625157, 0, 0,
  -0.00100050025012506245934706949896,
  -1.00100050025012499155252498894697, 0, 0, 0, 1};
mat4d r_look_at = {-0.57735026918962573105886804114562,
  0.57735026918962573105886804114562,
  -0.57735026918962573105886804114562,
  0.57735026918962573105886804114562,
  -0.40824829046386301723003953156876,
  0.40824829046386301723003953156876,
  0.81649658092772603446007906313753,
  -0.81649658092772603446007906313753,
  0.70710678118654746171500846685376,
  0.70710678118654746171500846685376, 0,
  -1.41421356237309492343001693370752, 0, 0, 0, 1};

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

  assert_vec4d_equal(vec4d_normalize(u), (u / vec4d_length(u)));
  assert_vec4d_equal(vec4d_normalize(v), (v / vec4d_length(v)));
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

  assert_mat4d_equal(mat4d_frustum(-2, 3, -3, 2, 1, 2000), r_frustum);
  assert_mat4d_equal(mat4d_ortho(-2, 3, -3, 2, 1, 2000), r_ortho);
  assert_mat4d_equal(mat4d_look_at((vec4d){1}, (vec4d){1}, (vec4d){0,1}), I);
  assert_mat4d_equal(mat4d_look_at((vec4d){1,1,1,0}, (vec4d){0,0,1,0}, (vec4d){0,0.5,0.5,0}), r_look_at);

  test_end("test_mat4");
}

int main(int argc, const char *argv[])
{
  test_equal();
  test_vec4();
  test_mat4();
  return 0;
}
