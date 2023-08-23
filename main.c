#include <stdio.h>
#include "lib.h"

int main(void) {
    Matrix *A = matrix_create();
    printf("Matriz A:\n");
    matrix_print(A);

    int x, y;
    printf("Informe as coordenadas (linha coluna) para obter o elemento: ");
    scanf("%d %d", &x, &y);
    float elem = matrix_getelem(A, x, y);
    printf("Elemento na posição (%d, %d) = %.2f\n", x, y, elem);

    printf("Informe as coordenadas (linha coluna) para atribuir um elemento: ");
    scanf("%d %d", &x, &y);
    float new_elem;
    printf("Informe o novo valor: ");
    scanf("%f", &new_elem);
    matrix_setelem(A, x, y, new_elem);

    printf("\nMatriz A após atribuição:\n");
    matrix_print(A);


    return 0;
}