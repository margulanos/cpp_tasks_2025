#include "ans_handler.hpp"


const double PI = 3.14159265358979323846;
const double EPSILON = 1e-10;
const int MAX_ITERATIONS = 100;

double factorial(int n);
double power(double base, int exponent);
double normalize_angle(double x);

AnswerHandler sin(double x);
AnswerHandler cos(double x);
AnswerHandler tan(double x);
AnswerHandler ctan(double x);
