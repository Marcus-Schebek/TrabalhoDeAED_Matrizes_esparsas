#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

Matrix* create_matrix_node(int line, int column, float info) {
    Matrix* node = (Matrix*)malloc(sizeof(Matrix));
    if (node) {
        node->right = node->below = NULL;
        node->line = line;
        node->column = column;
        node->info = info;
    }
    return node;
}
Matrix* insert_node(Matrix *head, Matrix *node) {
    // Encontrar o local adequado para inserir na linha
    Matrix* prev_row = head;
    while (prev_row->right && (prev_row->right->line < node->line || (prev_row->right->line == node->line && prev_row->right->column < node->column))) {
        prev_row = prev_row->right;
    }
    node->right = prev_row->right;
    prev_row->right = node;

    // Encontrar o local adequado para inserir na coluna
    Matrix* prev_column = head;
    while (prev_column->below && (prev_column->below->column < node->column || (prev_column->below->column == node->column && prev_column->below->line < node->line))) {
        prev_column = prev_column->below;
    }
    node->below = prev_column->below;
    prev_column->below = node;

    return head;
}

Matrix* matrix_create() {
    int m, n;
    printf("Insira a quantidade de linhas: ");
    scanf("%d", &m);
    printf("Insira a quantidade de colunas: ");
    scanf("%d", &n);

    Matrix* head = create_matrix_node(-1, -1, 0);

    int i, j;
    float value;
    printf("Informe a linha, coluna e valor (ou 0 0 0 para finalizar): ");
    while (scanf("%d %d %f", &i, &j, &value) == 3) {
        if (i == 0 && j == 0 && value == 0) {
            break;  // Encerrar o loop ao fornecer (0, 0, 0)
        }

        Matrix* new_node = create_matrix_node(i, j, value);
        head = insert_node(head, new_node);

        printf("Inserido elemento em (%d, %d) com valor %.2f\n", i, j, value);
        printf("Informe a linha, coluna e valor (ou 0 0 0 para finalizar): ");
    }

    return head;
}


void matrix_print(Matrix* m) {
    if (m == NULL) {
        printf("Matrix esta vazia.\n");
        return;
    }

    Matrix* current_row = m->right;
    while (current_row != m) {
        printf("%d %d %.2f\n", current_row->line, current_row->column, current_row->info);
        
        if (current_row->right == NULL) {
            break; // Encerrar o loop se o próximo right for nulo
        }

        current_row = current_row->right;
    }
    if (current_row->below != NULL) {
        current_row = current_row->below;
    } else {
        return; // Encerrar a função se o próximo abaixo também for nulo
    }
}


