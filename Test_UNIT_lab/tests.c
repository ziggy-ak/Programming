#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "matrix_operations.h"

static void test_DSize_valid(void **state) {
    int size = 3;
    double **matrix = DSize(size);
    assert_non_null(matrix);
    for (int i = 0; i < size; i++) {
        assert_non_null(matrix[i]);
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 0.0;
        }
    }
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

static void test_sum_valid(void **state) {
    int size = 2;
    double **m1 = DSize(size);
    double **m2 = DSize(size);
    m1[0][0] = 1.0; m1[0][1] = 2.0; m1[1][0] = 3.0; m1[1][1] = 4.0;
    m2[0][0] = 5.0; m2[0][1] = 6.0; m2[1][0] = 7.0; m2[1][1] = 8.0;
    
    double **result = sum(m1, m2, size);
    
    assert_float_equal(result[0][0], 6.0, 0.001);
    assert_float_equal(result[0][1], 8.0, 0.001);
    assert_float_equal(result[1][0], 10.0, 0.001);
    assert_float_equal(result[1][1], 12.0, 0.001);
    
    for (int i = 0; i < size; i++) {
        free(m1[i]); free(m2[i]); free(result[i]);
    }
    free(m1); free(m2); free(result);
}

static void test_substract_valid(void **state) {
    int size = 2;
    double **m1 = DSize(size);
    double **m2 = DSize(size);
    m1[0][0] = 10.0; m1[0][1] = 20.0; m1[1][0] = 30.0; m1[1][1] = 40.0;
    m2[0][0] = 1.0; m2[0][1] = 2.0; m2[1][0] = 3.0; m2[1][1] = 4.0;
    
    double **result = substract(m1, m2, size);
    
    assert_float_equal(result[0][0], 9.0, 0.001);
    assert_float_equal(result[0][1], 18.0, 0.001);
    assert_float_equal(result[1][0], 27.0, 0.001);
    assert_float_equal(result[1][1], 36.0, 0.001);
    
    for (int i = 0; i < size; i++) {
        free(m1[i]); free(m2[i]); free(result[i]);
    }
    free(m1); free(m2); free(result);
}

static void test_multiply_valid(void **state) {
    int size = 2;
    double **m1 = DSize(size);
    double **m2 = DSize(size);
    m1[0][0] = 1.0; m1[0][1] = 2.0; m1[1][0] = 3.0; m1[1][1] = 4.0;
    m2[0][0] = 5.0; m2[0][1] = 6.0; m2[1][0] = 7.0; m2[1][1] = 8.0;
    
    double **result = multiply(m1, m2, size);
    
    assert_float_equal(result[0][0], 19.0, 0.001);
    assert_float_equal(result[0][1], 22.0, 0.001);
    assert_float_equal(result[1][0], 43.0, 0.001);
    assert_float_equal(result[1][1], 50.0, 0.001);
    
    for (int i = 0; i < size; i++) {
        free(m1[i]); free(m2[i]); free(result[i]);
    }
    free(m1); free(m2); free(result);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_DSize_valid),
        cmocka_unit_test(test_sum_valid),
        cmocka_unit_test(test_substract_valid),
        cmocka_unit_test(test_multiply_valid),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}