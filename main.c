#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct matrix
{
  struct matrix *right;
  struct matrix *below;
  int line;
  int column;
  float info;
};
typedef struct matrix Matrix;

struct matrix_size
{
  int totalLines;
  int totalColumns;
};
typedef struct matrix_size MatrixSize;

Matrix *create_matrix_node(int line, int column, float info);
Matrix *matrix_create(void);
void matrix_print(Matrix *m);
void matrix_destroy(Matrix *m);
float matrix_getelem(Matrix *m, int x, int y);
void matrix_setelem(Matrix *m, int x, int y, float elem);
Matrix *matrix_empty(int m, int n);
Matrix *matrix_add(Matrix *m, Matrix *n);
Matrix *matrix_multiply(Matrix *m, Matrix *n);
Matrix *matrix_create_bigsizeList(int m);
MatrixSize *size_check(Matrix *m);
Matrix *matrix_transpose(Matrix *m);
float** matrix_create_bigsizeNormal(int m);
float** matrix_multiplyNormal(float** mat1, float** mat2, int size);

int main()
{
  clock_t inicio, fim;
  double tempoDecorrido;

  inicio = clock();
  Matrix *A = matrix_create();
  matrix_print(A);
  Matrix *B = matrix_create();
  matrix_print(B);
  printf("Matriz ADD \n");
  Matrix *C = matrix_add(A, B);
  matrix_print(C);
  matrix_destroy(C);
  printf("Matriz mult \n");
  C = matrix_multiply(A, B);
  matrix_print(C);
  matrix_destroy(C);
  printf("Matriz trampose \n");
  C = matrix_transpose(A);
  matrix_print(C);
  matrix_destroy(C);
  matrix_destroy(A);
  matrix_destroy(B);
  fim = clock();
  tempoDecorrido = (double)(fim - inicio) / CLOCKS_PER_SEC;

  printf("Tempo decorrido: %f segundos\n", tempoDecorrido);
  
  return 0;
}

Matrix *create_matrix_node(int line, int column, float info){
  Matrix *node = (Matrix *)malloc(sizeof(Matrix));
  if (node)
  {
    node->right = node->below = NULL;
    node->line = line;
    node->column = column;
    node->info = info;
  }
  return node;
}

Matrix *matrix_empty(int m, int n){
  Matrix *cabecalho = create_matrix_node(-1, -1, 0);
  Matrix *aux = cabecalho;
  cabecalho->right = cabecalho;
  cabecalho->below = cabecalho;

  for (int j = 0; j < n; j++)
  {
    Matrix *cabecalho_coluna = create_matrix_node(-1, j, 0);
    cabecalho_coluna->right = aux->right;
    aux->right = cabecalho_coluna;
    aux = cabecalho_coluna;
    cabecalho_coluna->below = cabecalho_coluna;
  }
  aux = cabecalho;
  for (int i = 0; i < m; i++)
  {
    Matrix *cabecalho_linha = create_matrix_node(i, -1, 0);
    cabecalho_linha->below = aux->below;
    aux->below = cabecalho_linha;
    aux = cabecalho_linha;
    cabecalho_linha->right = cabecalho_linha;
  }
  return cabecalho;
}

Matrix *matrix_create(){
  int m, n;
  printf("Insira a quantidade de linhas: ");
  scanf("%d", &m);
  printf("Insira a quantidade de colunas: ");
  scanf("%d", &n);

  Matrix *cabecalho = create_matrix_node(-1, -1, 0);
  Matrix *aux = cabecalho;
  cabecalho->right = cabecalho;
  cabecalho->below = cabecalho;

  for (int j = 0; j < n; j++)
  {
    Matrix *cabecalho_coluna = create_matrix_node(-1, j, 0);
    cabecalho_coluna->right = aux->right;
    aux->right = cabecalho_coluna;
    aux = cabecalho_coluna;
    cabecalho_coluna->below = cabecalho_coluna;
  }

  aux = cabecalho;

  for (int i = 0; i < m; i++)
  {
    Matrix *cabecalho_linha = create_matrix_node(i, -1, 0);
    cabecalho_linha->below = aux->below;
    aux->below = cabecalho_linha;
    aux = cabecalho_linha;
    cabecalho_linha->right = cabecalho_linha;
  }

  int i, j;
  float value;
  printf("Informe a linha, coluna e valor (ou -1 -1 -1 para finalizar): \n");
  while (scanf("%d %d %f", &i, &j, &value) == 3)
  {
    if (i == -1 && j == -1 && value == -1)
    {
      break;
    }
    if (i > -1 && i < m && j > -1 && j < n)
    {
      matrix_setelem(cabecalho, i, j, value);
      printf("Inserido elemento em (%d, %d) com valor %.2f\n", i, j, value);
    }
    else
    {
      printf("Coordenadas fora dos limites da matriz. Ignorando entrada.\n");
    }

    printf("Informe a linha, coluna e valor (ou -1 -1 -1 para finalizar): \n");
  }

  return cabecalho;
}
void matrix_print(Matrix *m){
  MatrixSize *sizeMatrix;
  float valor;
  sizeMatrix = size_check(m);

  for (int i = 0; i < sizeMatrix->totalLines; i++)
  {

    for (int j = 0; j < sizeMatrix->totalColumns; j++)
    {
      valor = matrix_getelem(m, i, j);
      printf("%.2f\t", valor);
    }
    printf("\n");
  }
  printf("\n");

  free(sizeMatrix);
}

float matrix_getelem(Matrix *m, int x, int y){
  Matrix *finder;
  finder = m->right;
  while (finder->column != -1 && finder->column < y)
  {
    finder = finder->right;
  }
  finder = finder->below;
  while (finder->line != -1 && finder->line < x)
  {
    finder = finder->below;
  }

  if (finder->line == x && finder->column == y)
  {
    return finder->info;
  }
  else
  {
    return 0.0;
  }
}

void matrix_setelem(Matrix *m, int x, int y, float elem)
{

  Matrix *cabecalhoLinha = m->below;
  Matrix *cabecalhoColuna = m->right;

  while (cabecalhoLinha->line != x)
  {
    cabecalhoLinha = cabecalhoLinha->below;
  }

  while (cabecalhoColuna->column != y)
  {
    cabecalhoColuna = cabecalhoColuna->right;
  }

  Matrix *NoExistente = cabecalhoLinha->right;
  Matrix *prev_node = cabecalhoLinha;

  while (NoExistente != cabecalhoLinha && NoExistente->column < y)
  {
    prev_node = NoExistente;
    NoExistente = NoExistente->right;
  }

  Matrix *nodeCol = cabecalhoColuna->below;
  Matrix *prevCol = cabecalhoColuna;

  while (nodeCol != cabecalhoColuna && nodeCol->line <= x)
  {
    prevCol = nodeCol;
    nodeCol = nodeCol->below;
  }

  if (NoExistente == cabecalhoLinha || NoExistente->column > y)
  {
    if (elem != 0.0)
    {
      Matrix *NovoNo = create_matrix_node(x, y, elem);
      NovoNo->right = prev_node->right;
      prev_node->right = NovoNo;
      NovoNo->below = prevCol->below;
      prevCol->below = NovoNo;
    }
  }
  else if (NoExistente->column == y)
  {
    if (elem != 0.0)
    {
      NoExistente->info = elem;
    }
    else
    {
      prev_node->right = NoExistente->right;
      prevCol->below = NoExistente->below;
      free(NoExistente);
    }
  }
}

MatrixSize *size_check(Matrix *m){
  MatrixSize *size = malloc(sizeof(MatrixSize));
  Matrix *checkCol, *checkLin;
  checkCol = m->right;
  checkLin = m->below;
  int sizeCol = 0, sizeLin = 0;
  while (checkCol->column != -1)
  {
    checkCol = checkCol->right;
    sizeCol++;
  }
  while (checkLin->line != -1)
  {
    checkLin = checkLin->below;
    sizeLin++;
  }
  size->totalLines = sizeLin;
  size->totalColumns = sizeCol;
  return size;
}

Matrix *matrix_add(Matrix *m, Matrix *n){
  int soma = 0;
  int NumLinA, NumLinB, NumColA, NumColB;
  MatrixSize *sizeA, *sizeB;

  sizeA = size_check(m);
  sizeB = size_check(n);
  Matrix *resultado = matrix_empty(sizeA->totalLines, sizeB->totalColumns);

  if (sizeA->totalColumns != sizeB->totalColumns || sizeA->totalLines != sizeB->totalLines)
  {
    resultado = matrix_empty(0, 0);
    printf("Incapaz de fazer soma de matriz de ordem diferente \n");
    return resultado;
  }

  for (int i = 0; i < sizeA->totalLines; i++)
  {
    for (int j = 0; j < sizeA->totalColumns; j++)
    {
      float elemA = matrix_getelem(m, i, j);
      float elemB = matrix_getelem(n, i, j);
      soma = elemA + elemB;
      matrix_setelem(resultado, i, j, soma);
    }
  }

  free(sizeA);
  free(sizeB);
  return resultado;
}

Matrix *matrix_transpose(Matrix *m){
  MatrixSize *size = size_check(m);
  Matrix *transposta = matrix_empty(size->totalColumns, size->totalLines);
  int newRows = size->totalColumns;
  int newCols = size->totalLines;

  for (int i = 0; i < newRows; i++)
  {
    for (int j = 0; j < newCols; j++)
    {
      float elem = matrix_getelem(m, j, i);
      matrix_setelem(transposta, i, j, elem);
    }
  }
  free(size);
  return transposta;
}

Matrix *matrix_multiply(Matrix *m, Matrix *n){
  MatrixSize *sizeMatrixA = size_check(m);
  MatrixSize *sizeMatrixB = size_check(n);
  if (sizeMatrixA->totalColumns != sizeMatrixB->totalLines)
  {
    printf("Impossivel fazer a multiplicação");
    return NULL;
  }
  Matrix *resultado = matrix_empty(sizeMatrixA->totalLines, sizeMatrixB->totalColumns);
  for (int i = 0; i < sizeMatrixA->totalLines; i++)
  {
    for (int j = 0; j < sizeMatrixB->totalColumns; j++)
    {
      float sum = 0.0;
      for (int k = 0; k < sizeMatrixA->totalColumns; k++)
      {
        sum += matrix_getelem(m, i, k) * matrix_getelem(n, k, j);
      }
      matrix_setelem(resultado, i, j, sum);
    }
  }
  free(sizeMatrixA);
  free(sizeMatrixB);
  return resultado;
}
void matrix_destroy(Matrix *m){
  MatrixSize *size = size_check(m);
  Matrix *current_row = m->below;
  Matrix *aux1, *aux2, *save;
  save = m;
  while (save->below != save)
  {
    m = save->below;
    aux1 = m->right;
    aux2 = aux1->right;
    while (aux1->column != -1)
    {
      free(aux1);
      aux1 = aux2;
      aux2 = aux2->right;
    }
    save->below = m->below;
    free(m);
  }
  while (save->right != save)
  {
    m = save->right;
    save->right = m->right;
    free(m);
  }
  free(save);
  free(size);
}

Matrix *matrix_create_bigsizeList(int m){
  srand(time(NULL));
  Matrix *cabecalho = create_matrix_node(-1, -1, 0);
  Matrix *aux = cabecalho;
  cabecalho->right = cabecalho;
  cabecalho->below = cabecalho;

  for (int j = 0; j < m; j++)
  {
    Matrix *cabecalho_coluna = create_matrix_node(-1, j, 0);
    cabecalho_coluna->right = aux->right;
    aux->right = cabecalho_coluna;
    aux = cabecalho_coluna;
    cabecalho_coluna->below = cabecalho_coluna;
  }

  aux = cabecalho;
  for (int i = 0; i < m; i++)
  {
    Matrix *cabecalho_linha = create_matrix_node(i, -1, 0);
    cabecalho_linha->below = aux->below;
    aux->below = cabecalho_linha;
    aux = cabecalho_linha;
    cabecalho_linha->right = cabecalho_linha;
  }
  int numZeros = m * m * 0.5; 
  int atuaisZeros = 0;
  int i, j;

  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < m; j++)
    {
      float value = 0.0; 
      if (atuaisZeros < numZeros)
      {
        if (rand() % 10 < 5)
        { 
          
          value = rand() % 10;
          atuaisZeros++;
        }
      }
      matrix_setelem(cabecalho, i, j, value);
    }
  }
  return cabecalho;
}
float** matrix_create_bigsizeNormal(int m) {
    float** matrizTest;
    srand(time(NULL));
    matrizTest = (float**)malloc(m * sizeof(float*));
    for (int i = 0; i < m; i++) {
        matrizTest[i] = (float*)malloc(m * sizeof(float));
    }
  
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            matrizTest[i][j] = rand()%10;
        }
    }
  
    return matrizTest;
}
float** matrix_multiplyNormal(float** mat1, float** mat2, int size) {
    float** result = (float**)malloc(size * sizeof(float*));
    for (int i = 0; i < size; i++) {
        result[i] = (float*)malloc(size * sizeof(float));
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = 0.0;
            for (int k = 0; k < size; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }

    return result;
}
