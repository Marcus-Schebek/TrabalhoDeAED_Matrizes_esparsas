#include <stdio.h>
#include "lib.h"

int main(void)
{
    /* Inicializacao da aplicacao ... */
    Matrix *A = matrix_create();
    matrix_print(A);
    return 0;
}