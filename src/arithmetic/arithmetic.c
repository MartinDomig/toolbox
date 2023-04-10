#include <glib.h>
#include <math.h>
#include <stdio.h>

#include "arithmetic.h"

int main(int argc, char const *argv[])
{
    g_print("Enter a number: ");
    double num1 = 0;
    scanf("%lf", &num1);

    g_print("Square: U = %lf\n", num1 * 4);
    g_print("Square: A = %lf\n", num1 * num1);

    g_print("Circle: U = %lf\n", 2 * M_PI * num1);
    g_print("Circle: A = %lf\n", M_PI * num1 * num1);

    g_print("Sphere: U = %lf\n", 4 * M_PI * num1 * num1);
    g_print("Sphere: A = %lf\n", 4 * M_PI * num1 * num1 * num1);
    g_print("Sphere: V = %lf\n", 4 * M_PI * num1 * num1 * num1 / 3);

    g_print("Cube: U = %lf\n", 6 * num1 * num1);
    g_print("Cube: A = %lf\n", 4 * num1 * num1);
    g_print("Cube: V = %lf\n", num1 * num1 * num1);

    g_print("Pyramid: U = %lf\n", 4 * num1 * num1);
    g_print("Pyramid: A = %lf\n", 4 * num1 * num1);
    g_print("Pyramid: V = %lf\n", num1 * num1 * num1 / 3);

    return EXIT_SUCCESS;
}
