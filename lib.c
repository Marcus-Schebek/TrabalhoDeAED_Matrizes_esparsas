#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

Matrix *matrix_create(void)
{
    int m, n;
    printf("Informe o número de linhas da matriz: ");
    scanf("%d", &m);
    printf("Informe o número de colunas da matriz: ");
    scanf("%d", &n);

    Matrix *head = NULL; // Cabeça da matriz esparsa
    Matrix *current_row = NULL;
    Matrix *current_col = NULL;

    printf("Informe os elementos da matriz no formato (linha coluna valor).\n");
    printf("Use 0 0 0 para finalizar a entrada.\n");

    while (1)
    {
        int i, j;
        float value;
        printf("Informe a linha, coluna e valor (ou 0 0 0 para finalizar): ");
        if (scanf("%d %d %f", &i, &j, &value) != 3)
        {
            break; // Sai do loop quando a leitura falhar
        }

        if (i == 0 && j == 0 && value == 0)
        {
            break; // Sai do loop quando o marcador de fim for encontrado
        }

        if (i > m || j > n)
        {
            fprintf(stderr, "Erro: Índices fora dos limites da matriz\n");
            continue;
        }

        Matrix *new_element = (Matrix *)malloc(sizeof(Matrix));
        if (!new_element)
        {
            fprintf(stderr, "Erro: Falha na alocação de memória\n");
            exit(1);
        }

        new_element->line = i;
        new_element->column = j;
        new_element->info = value;
        new_element->right = NULL;
        new_element->below = NULL;

        if (!head)
        {
            head = new_element;
            current_row = new_element;
            current_col = new_element;
        }
        else
        {
            if (i == current_row->line)
            {
                // insere na mesma linha
                while (current_col->right && current_col->right->column < j)
                {
                    current_col = current_col->right;
                }
                new_element->right = current_col->right;
                current_col->right = new_element;
                current_col = new_element;
            }
            else if (i > current_row->line)
            {
                // Insere numa nova linha
                Matrix *prev_row = NULL;
                while (current_row && current_row->line < i)
                {
                    prev_row = current_row;
                    current_row = current_row->below;
                }

                new_element->below = current_row;
                if (prev_row)
                {
                    prev_row->below = new_element;
                }
                else
                {
                    head->below = new_element;
                }

                Matrix *prev_col = NULL;
                Matrix *col_current = prev_row ? prev_row->right : head;
                while (col_current && col_current->column < j)
                {
                    prev_col = col_current;
                    col_current = col_current->right;
                }

                new_element->right = col_current;
                if (prev_col)
                {
                    prev_col->right = new_element;
                }
                else
                {
                    if (prev_row)
                    {
                        prev_row->right = new_element;
                    }
                    else
                    {
                        head->right = new_element;
                    }
                }

                current_row = new_element;
                current_col = new_element;
            }
            else
            {
                //insere numa linha anterior
                Matrix *prev_row = NULL;
                Matrix *row_current = head;
                while (row_current && row_current->line < i)
                {
                    prev_row = row_current;
                    row_current = row_current->below;
                }
                new_element->below = row_current;
                if (prev_row)
                {
                    prev_row->below = new_element;
                }
                else
                {
                    head = new_element;
                }

                Matrix *prev_col = NULL;
                Matrix *col_current = row_current; //começa uma nova linha
                while (col_current && col_current->column < j)
                {
                    prev_col = col_current;
                    col_current = col_current->right;
                }
                new_element->right = col_current;
                if (prev_col)
                {
                    prev_col->right = new_element;
                }
                else
                {
                    row_current = new_element;
                }

                current_row = new_element;
                current_col = new_element;
            }
        }
    }
    return head;
}

void matrix_print(Matrix *head)
{
    // Encontrar o número de linhas e colunas percorrendo a matriz
    int rows = 0, cols = 0;
    Matrix *current_row = head;
    Matrix *current_col = head;

    while (current_row)
    {
        if (current_row->line > rows)
        {
            rows = current_row->line;
        }
        current_row = current_row->below;
    }

    while (current_col)
    {
        if (current_col->column > cols)
        {
            cols = current_col->column;
        }
        current_col = current_col->right;
    }

    for (int i = 1; i <= rows; i++)
    {
        current_row = head;
        current_col = head;

        printf("%d |", i);

        for (int j = 1; j <= cols; j++)
        {
            if (current_col && current_col->line == i && current_col->column == j)
            {
                printf(" %.2f,", current_col->info);
                current_col = current_col->right;
            }
            else
            {
                printf(" 0,");
            }
        }

        printf("\b |\n");
    }
}
void matrix_destroy(Matrix *m)
{
    Matrix *current = m;
    while (current != NULL)
    {
        Matrix *temp = current;
        current = current->right;
        free(temp);
    }
}

Matrix *matrix_add(Matrix *m, Matrix *n)
{
    if (m == NULL || n == NULL)
    {
        return NULL; // Retorna NULL se uma das matrizes for vazia
    }

    Matrix *result_head = NULL; // Cabeça da matriz resultante
    Matrix *result_prev_col = NULL;
    Matrix *result_prev_row = NULL;

    while (m != NULL && n != NULL)
    {
        // Comparar as posições (linha, coluna) das duas matrizes
        if (m->line < n->line || (m->line == n->line && m->column < n->column))
        {
            // Inserir elemento da matriz m na matriz resultante
            Matrix *new_element = (Matrix *)malloc(sizeof(Matrix));
            if (!new_element)
            {
                fprintf(stderr, "Erro: Falha na alocação de memória\n");
                exit(1);
            }

            new_element->line = m->line;
            new_element->column = m->column;
            new_element->info = m->info + 0; // Copia o valor de m->info
            new_element->right = NULL;
            new_element->below = NULL;

            if (result_prev_col)
            {
                result_prev_col->right = new_element;
            }
            result_prev_col = new_element;

            if (!result_prev_row || result_prev_row->line != m->line)
            {
                if (result_head == NULL)
                {
                    result_head = new_element;
                }
                result_prev_row = new_element;
            }

            m = m->right;
        }
        else if (m->line > n->line || (m->line == n->line && m->column > n->column))
        {
            // Inserir elemento da matriz n na matriz resultante
            Matrix *new_element = (Matrix *)malloc(sizeof(Matrix));
            if (!new_element)
            {
                fprintf(stderr, "Erro: Falha na alocação de memória\n");
                exit(1);
            }

            new_element->line = n->line;
            new_element->column = n->column;
            new_element->info = n->info + 0; // Copia o valor de n->info
            new_element->right = NULL;
            new_element->below = NULL;

            if (result_prev_col)
            {
                result_prev_col->right = new_element;
            }
            result_prev_col = new_element;

            if (!result_prev_row || result_prev_row->line != n->line)
            {
                if (result_head == NULL)
                {
                    result_head = new_element;
                }
                result_prev_row = new_element;
            }

            n = n->right;
        }
        else
        {
            // As posições (linha, coluna) são iguais em ambas as matrizes
            // Soma os valores e insere o elemento resultante na matriz resultante
            Matrix *new_element = (Matrix *)malloc(sizeof(Matrix));
            if (!new_element)
            {
                fprintf(stderr, "Erro: Falha na alocação de memória\n");
                exit(1);
            }

            new_element->line = m->line;
            new_element->column = m->column;
            new_element->info = m->info + n->info;
            new_element->right = NULL;
            new_element->below = NULL;

            if (result_prev_col)
            {
                result_prev_col->right = new_element;
            }
            result_prev_col = new_element;

            if (!result_prev_row || result_prev_row->line != m->line)
            {
                if (result_head == NULL)
                {
                    result_head = new_element;
                }
                result_prev_row = new_element;
            }

            m = m->right;
            n = n->right;
        }
    }

    // Caso ainda haja elementos restantes em m ou n, insere na matriz resultante
    while (m != NULL)
    {
        // Inserir elemento da matriz m na matriz resultante
        Matrix *new_element = (Matrix *)malloc(sizeof(Matrix));
        if (!new_element)
        {
            fprintf(stderr, "Erro: Falha na alocação de memória\n");
            exit(1);
        }

        new_element->line = m->line;
        new_element->column = m->column;
        new_element->info = m->info + 0; // Copia o valor de m->info
        new_element->right = NULL;
        new_element->below = NULL;

        if (result_prev_col)
        {
            result_prev_col->right = new_element;
        }
        result_prev_col = new_element;

        if (!result_prev_row || result_prev_row->line != m->line)
        {
            if (result_head == NULL)
            {
                result_head = new_element;
            }
            result_prev_row = new_element;
        }

        m = m->right;
    }

    while (n != NULL)
    {
        // Inserir elemento da matriz n na matriz resultante
        Matrix *new_element = (Matrix *)malloc(sizeof(Matrix));
        if (!new_element)
        {
            fprintf(stderr, "Erro: Falha na alocação de memória\n");
            exit(1);
        }

        new_element->line = n->line;
        new_element->column = n->column;
        new_element->info = n->info + 0; // Copia o valor de n->info
        new_element->right = NULL;
        new_element->below = NULL;

        if (result_prev_col)
        {
            result_prev_col->right = new_element;
        }
        result_prev_col = new_element;

        if (!result_prev_row || result_prev_row->line != n->line)
        {
            if (result_head == NULL)
            {
                result_head = new_element;
            }
            result_prev_row = new_element;
        }

        n = n->right;
    }

    return result_head;
}

Matrix *matrix_multiply(Matrix *m, Matrix *n)
{
    if (m == NULL || n == NULL)
    {
        return NULL; // Retorna NULL se uma das matrizes for vazia
    }

    // Verifica se o número de colunas da matriz m é igual ao número de linhas da matriz n
    if (m->column != n->line)
    {
        fprintf(stderr, "Erro: As dimensões das matrizes não são compatíveis para multiplicação\n");
        return NULL;
    }

    Matrix *result_head = NULL; // Cabeça da matriz resultante

    // Inicializa uma matriz temporária para auxiliar na multiplicação
    float temp_matrix[m->line][n->column];
    for (int i = 0; i < m->line; i++)
    {
        for (int j = 0; j < n->column; j++)
        {
            temp_matrix[i][j] = 0.0;
        }
    }

    // Realiza a multiplicação das matrizes
    for (Matrix *i = m; i != NULL; i = i->right)
    {
        for (Matrix *j = n; j != NULL; j = j->below)
        {
            if (i->column == j->line)
            {
                temp_matrix[i->line - 1][j->column - 1] += i->info * j->info;
            }
        }
    }

    // Cria a matriz resultante com base na matriz temporária
    for (int i = 0; i < m->line; i++)
    {
        for (int j = 0; j < n->column; j++)
        {
            if (temp_matrix[i][j] != 0.0)
            {
                Matrix *new_element = (Matrix *)malloc(sizeof(Matrix));
                if (!new_element)
                {
                    fprintf(stderr, "Erro: Falha na alocação de memória\n");
                    exit(1);
                }

                new_element->line = i + 1;
                new_element->column = j + 1;
                new_element->info = temp_matrix[i][j];
                new_element->right = NULL;
                new_element->below = NULL;

                if (result_head == NULL)
                {
                    result_head = new_element;
                }
                else
                {
                    Matrix *current = result_head;
                    while (current->right != NULL)
                    {
                        current = current->right;
                    }
                    current->right = new_element;
                }
            }
        }
    }

    return result_head;
}

// Function to transpose a matrix
Matrix *matrix_transpose(Matrix *m)
{
    if (m == NULL)
    {
        return NULL; // Retorna NULL se a matriz for vazia
    }

    Matrix *result_head = NULL; // Cabeça da matriz transposta

    // Inicializa uma matriz temporária para armazenar os elementos transpostos
    int num_columns = m->line; // Número de colunas na matriz transposta
    int num_lines = m->column; // Número de linhas na matriz transposta
    Matrix *temp_matrix[num_columns][num_lines];

    // Inicializa a matriz temporária com NULL em todas as posições
    for (int i = 0; i < num_columns; i++)
    {
        for (int j = 0; j < num_lines; j++)
        {
            temp_matrix[i][j] = NULL;
        }
    }

    // Preenche a matriz temporária com os elementos transpostos
    for (Matrix *current = m; current != NULL; current = current->right)
    {
        Matrix *new_element = (Matrix *)malloc(sizeof(Matrix));
        if (!new_element)
        {
            fprintf(stderr, "Erro: Falha na alocação de memória\n");
            exit(1);
        }

        int transposed_line = current->column - 1;
        int transposed_column = current->line - 1;

        new_element->line = transposed_line + 1;
        new_element->column = transposed_column + 1;
        new_element->info = current->info;
        new_element->right = NULL;
        new_element->below = NULL;

        if (temp_matrix[transposed_line][transposed_column] == NULL)
        {
            temp_matrix[transposed_line][transposed_column] = new_element;
        }
        else
        {
            Matrix *temp_current = temp_matrix[transposed_line][transposed_column];
            while (temp_current->right != NULL)
            {
                temp_current = temp_current->right;
            }
            temp_current->right = new_element;
        }
    }

    // Cria a matriz transposta com base na matriz temporária
    for (int i = 0; i < num_columns; i++)
    {
        for (int j = 0; j < num_lines; j++)
        {
            Matrix *temp_current = temp_matrix[i][j];
            if (temp_current != NULL)
            {
                if (result_head == NULL)
                {
                    result_head = temp_current;
                }
                else
                {
                    Matrix *current = result_head;
                    while (current->below != NULL)
                    {
                        current = current->below;
                    }
                    current->below = temp_current;
                }
            }
        }
    }

    return result_head;
}

float matrix_getelem(Matrix *m, int x, int y)
{
    Matrix *current = m;

    while (current != NULL)
    {
        if (current->line == x && current->column == y)
        {
            return current->info;
        }
        current = current->right;
    }

    return 0.0; // Retorna 0.0 se o elemento não for encontrado
}

void matrix_setelem(Matrix *m, int x, int y, float elem)
{
    Matrix *current = m;

    while (current != NULL)
    {
        if (current->line == x && current->column == y)
        {
            current->info = elem;
            return;
        }
        current = current->right;
    }

    // Se o elemento não for encontrado, insere o novo elemento na matriz
    Matrix *new_element = (Matrix *)malloc(sizeof(Matrix));
    if (!new_element)
    {
        fprintf(stderr, "Erro: Falha na alocação de memória\n");
        exit(1);
    }

    new_element->line = x;
    new_element->column = y;
    new_element->info = elem;
    new_element->right = NULL;
    new_element->below = NULL;

    Matrix *prev_col = NULL;
    Matrix *prev_row = NULL;
    current = m;

    while (current != NULL)
    {
        if (current->line == x)
        {
            prev_col = current;
        }
        if (current->column == y)
        {
            prev_row = current;
        }
        current = current->right;
    }

    if (prev_col)
    {
        prev_col->right = new_element;
    }

    if (!prev_row || prev_row->line != x)
    {
        if (m == NULL)
        {
            m = new_element;
        }
        prev_row = new_element;
    }
}
