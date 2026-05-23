#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <math.h>
#include <stdio.h>

double Sred_time(double total_time, int all_times_count){
    if (all_times_count == 0){
        printf("Ошибка вычисления среднего времени! Деление на 0!\n");
        return 0;
    }
    return (double)(total_time / all_times_count) * 1000;
}

double Fraction_percent(double total, int total_request){
    if (total_request == 0){
        printf("Ошибка! Деление на 0! Общее количество запросов не должно быть равно 0!\n");
        return 0;
    }
    if (total_request < 0){
        printf("Ошибка! Общее количество запросов не должно быть меньше 0!\n");
        return 0;
    }
    return (double)total / total_request * 100;
}

double P(double *all_times, int all_times_count, double p){
    if (all_times == NULL){
        printf("Ошибка! Массив времени не инициализирован!\n");
        return 0;
    }
    if (p < 0){
        printf("Ошибка! Множитель не может быть меньше 0! Стандартные значения: 0,95 и 0,99!\n");
        return 0;
    }
    return all_times[(int)(all_times_count * p)] * 1000;
}

static void test_sred_time_normal(void **state) {
    (void) state;
    double result = Sred_time(5.0, 2);
    assert_double_equal(result, 2500.0, 0.001);
}

static void test_sred_time_zero_count(void **state) {
    (void) state;
    double result = Sred_time(10.0, 0);
    assert_double_equal(result, 0.0, 0.001);
}

static void test_sred_time_zero_time(void **state) {
    (void) state;
    double result = Sred_time(0.0, 5);
    assert_double_equal(result, 0.0, 0.001);
}

static void test_fraction_normal(void **state) {
    (void) state;
    double result = Fraction_percent(30.0, 100);
    assert_double_equal(result, 30.0, 0.001);
}

static void test_fraction_zero_total(void **state) {
    (void) state;
    double result = Fraction_percent(10.0, 0);
    assert_double_equal(result, 0.0, 0.001);
}

static void test_fraction_zero_percent(void **state) {
    (void) state;
    double result = Fraction_percent(0.0, 100);
    assert_double_equal(result, 0.0, 0.001);
}

static void test_fraction_hundred_percent(void **state) {
    (void) state;
    double result = Fraction_percent(100.0, 100);
    assert_double_equal(result, 100.0, 0.001);
}

static void test_p95(void **state) {
    (void) state;
    double times[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    double result = P(times, 10, 0.95);
    assert_double_equal(result, 1000.0, 0.001);
}

static void test_p99(void **state) {
    (void) state;
    double times[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    double result = P(times, 10, 0.99);
    assert_double_equal(result, 1000.0, 0.001);
}

static void test_p50(void **state) {
    (void) state;
    double times[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    double result = P(times, 10, 0.5);
    assert_double_equal(result, 600.0, 0.001);
}

static void test_p_null_array(void **state) {
    (void) state;
    double result = P(NULL, 10, 0.95);
    assert_double_equal(result, 0.0, 0.001);
}

static void test_p_negative_p(void **state) {
    (void) state;
    double times[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    double result = P(times, 10, -0.5);
    assert_double_equal(result, 0.0, 0.001);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_sred_time_normal),
        cmocka_unit_test(test_sred_time_zero_count),
        cmocka_unit_test(test_sred_time_zero_time),
        
        cmocka_unit_test(test_fraction_normal),
        cmocka_unit_test(test_fraction_zero_total),
        cmocka_unit_test(test_fraction_zero_percent),
        cmocka_unit_test(test_fraction_hundred_percent),
        
        cmocka_unit_test(test_p95),
        cmocka_unit_test(test_p99),
        cmocka_unit_test(test_p50),
        cmocka_unit_test(test_p_null_array),
        cmocka_unit_test(test_p_negative_p),
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}