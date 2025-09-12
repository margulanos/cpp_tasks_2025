#include "my_math.hpp"
#include <iostream>


double factorial(int n)
{
    double result = 1.0;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

double power(double base, int exponent)
{
    if (exponent == 0) return 1.0;
    if (exponent == 1) return base;
    
    double result = 1.0;
    bool negative_exponent = exponent < 0;
    exponent = negative_exponent ? exponent : -exponent;
    
    for (int i = 0; i < exponent; ++i) {
        result *= base;
    }
    
    return negative_exponent ? 1.0 / result : result;
}

double normalize_angle(double x) {
    while (x > 2 * PI || x < -2 * PI)
    {
        if (x > PI) x -= 2 * PI;
        else if (x < -PI) x += 2 * PI;
    }
    
    return x;
}

AnswerHandler sin(double x) {
    x = normalize_angle(x);
    
    double result = 0.0;
    double term;
    double term_abs;
    int n = 0;
    
    do
    {
        term = power(-1, n) * power(x, 2 * n + 1) / factorial(2 * n + 1);
        term_abs = term >= 0 ? term : -term;
        result += term;
        n++;
    }
    while (term_abs  > EPSILON && n < MAX_ITERATIONS);
    
    return {x, result};
}

AnswerHandler cos(double x) {
    x = normalize_angle(x);
    
    double result = 0.0;
    double term;
    double term_abs;
    int n = 0;
    
    do 
    {
        term = power(-1, n) * power(x, 2 * n) / factorial(2 * n);
        term_abs = term >= 0 ? term : -term;
        result += term;
        n++;
    }
    while (term_abs > EPSILON && n < MAX_ITERATIONS);
    
    return {x, result};
}

AnswerHandler tan(double x) {
    AnswerHandler sin_result = sin(x);
    AnswerHandler cos_result = cos(x);
    double cos_result_abs = cos_result.answer >= 0 ? cos_result.answer : -cos_result.answer;
    
    if (cos_result_abs < EPSILON)
    {
        std::cout<<("Tangent is undefined for this angle");
    }
    
    return {x, sin_result.answer / cos_result.answer};
}

AnswerHandler ctan(double x) {
    AnswerHandler tan_result = tan(x);
    double tan_result_abs = tan_result.answer >= 0 ? tan_result.answer : -tan_result.answer;
    
    if (tan_result_abs < EPSILON)
    {
        std::cout<<("Cotangent is undefined for this angle");
    }
    
    return {x, 1.0 / tan_result.answer};
}
