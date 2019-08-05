#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livro_codigos.h"
#include "livros_dados.h"


char *remover_espacos_frase(char *s)
{
    int n = 0;
    while (n < strlen(s)) {
        if ((s[n] == ' ') && (s[n-1] == ' ')) {
                  while (s[n] == ' ') {
                        s[n] = '\a';
                        n++;
                  }
        }
        n++;
    }
    return s;
}

void adiciona_livro(Dados_Livro livro)
{

  insere_codigo(livro.codigo);

  int posicao;
  FILE *arq_livro = fopen("bd.bin", "rb+");
  posicao = insere_livro(arq_livro, livro);
  fclose(arq_livro);

  FILE *arquivo_codigo = fopen("bdcodigos.bin", "rb+");                       //Insere dados do livro
  adiciona_posicao_do_livro_no_codigo(arquivo_codigo, posicao, livro.codigo); //adiciona a posicao dos dados do livro
  fclose(arquivo_codigo);
}

void adiciona_livro_entradas()
{
  printf("\n \t Adicionar Livro\n\n");

  FILE *arquivo_codigo = fopen("bdcodigos.bin", "rb+");

  Dados_Livro livro;

  Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
  cab = le_cabecalho_codigos(arquivo_codigo);

  int aux_codigo, aux = 0;
  printf("Insira um codigo: ");
  scanf("%d", &aux_codigo);

  aux = existe_codigo(arquivo_codigo, aux_codigo, cab->pos_raiz);

  while (aux == 1)
  {
    printf("\nCodigo existente!!");
    printf("\nInsira um codigo novamente: ");
    scanf("%d", &aux_codigo);
    aux = existe_codigo(arquivo_codigo, aux_codigo, cab->pos_raiz);
  }
  livro.codigo = aux_codigo;

  printf("\nInsira um titulo: ");
  scanf("%*c%[^\n]%*c", livro.titulo);
  
  printf("\nInsira um autor: ");
  scanf("%[^\n]", livro.autor);
  //printf("Livro titulo: %s\n",remover_espacos_frase(livro.titulo));
  

  printf("\nInsira o numero de exemplares: ");
  scanf("%d", &livro.exemplares);

  fclose(arquivo_codigo);

  adiciona_livro(livro);
  printf("Livro cadastrado com sucesso!\n");
}

void exclui_livro()
{

  int codigo;
  printf("\nEscreva o codigo do livro que deseja excluir = ");
  scanf("%d", &codigo);

  FILE *arquivo_codigo;
  arquivo_codigo = fopen("bdcodigos.bin", "rb+");

  Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
  cab = le_cabecalho_codigos(arquivo_codigo);

  if (existe_codigo(arquivo_codigo, codigo, cab->pos_raiz))
  {
    int posicao = posicao_do_livro(arquivo_codigo, cab->pos_raiz, codigo);
    fclose(arquivo_codigo);

    iniciar_exclusao_Codigo(codigo);

    FILE *arquivo_dados = fopen("bd.bin", "rb+");
    retira(arquivo_dados, posicao);
    printf("Removido com sucesso!!\n ");
    fclose(arquivo_dados);
  }
  else
  {
    printf("\nlivro nao existente\n");
    fclose(arquivo_codigo);
  }


  free(cab);
}

void imprime_em_ordem_de_codigo(FILE *arq, int pos)
{

  No_Codigo *no = (No_Codigo *)malloc(sizeof(No_Codigo));
  no = le_no_codigo(arq, pos);

  if (no->esquerda != -1)
  {
    imprime_em_ordem_de_codigo(arq, no->esquerda);
  }

  if (no->pos_livro != -2)
  {
    imprime_livro(no->pos_livro);
  }

  if (no->direita != -1)
  {
    imprime_em_ordem_de_codigo(arq, no->direita);
  }
}

void livros_ordenados_por_codigo()
{
  FILE *arq = fopen("bdcodigos.bin", "rb+");
  Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
  cab = le_cabecalho_codigos(arq);

  if (cab->pos_raiz == -1)
  {
    printf("Nao existem livros cadastrados\n");
  }
  else
  {
    imprime_em_ordem_de_codigo(arq, cab->pos_raiz);
  }

  fclose(arq);
}

void atualizar_exmplares()
{
  printf("\n \t Atualizar exemplares \n\n");

  FILE *arquivo_codigo;
  arquivo_codigo = fopen("bdcodigos.bin", "rb+");

  Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
  cab = le_cabecalho_codigos(arquivo_codigo);

  if (cab->pos_raiz == -1)
  {
    printf("Nao existem livros cadastrados \n");
  }
  else
  {
    int aux = 1;
    Codigo aux_codigo;

    printf("\nInsira um codigo: ");
    scanf("%d", &aux_codigo);

    aux = existe_codigo(arquivo_codigo, aux_codigo, cab->pos_raiz); //Insere o codigo dos livros
    if (aux == 0)
    {
      printf("Livro nao existente no sistema\n");
    }
    else
    {
      int posicao = posicao_do_livro(arquivo_codigo, cab->pos_raiz, aux_codigo);

      FILE *teste2;
      teste2 = fopen("bd.bin", "rb+");
      imprime_livro(posicao);
      No_livro *livro = (No_livro *)malloc(sizeof(No_livro));
      livro = le_no_livro(teste2, posicao);

      printf("\nInsira a nova quantidade de exemplares do livro: ");
      scanf("%d", &livro->livro.exemplares);

      escreve_livro_no(teste2, livro, posicao);

      fclose(teste2);
    }
  }

  fclose(arquivo_codigo);
}

void buscar_dados_do_livro()
{
  printf("\n \t Buscar dados do Livro\n\n");

  FILE *arquivo_codigo;
  arquivo_codigo = fopen("bdcodigos.bin", "rb+");

  Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
  cab = le_cabecalho_codigos(arquivo_codigo);

  if (cab->pos_raiz == -1)
  {
    printf("Nao existem livros cadastrados \n");
  }
  else
  {

    int aux = 1;
    Codigo aux_codigo;

    printf("\nInsira um codigo: ");
    scanf("%d", &aux_codigo);

    aux = existe_codigo(arquivo_codigo, aux_codigo, cab->pos_raiz); //Insere o codigo dos livros

    if (aux == 0)
    {
      printf("Livro nao existente no sistema\n");
    }
    else
    {
      int posicao = posicao_do_livro(arquivo_codigo, cab->pos_raiz, aux_codigo);

      FILE *teste2;
      teste2 = fopen("bd.bin", "rb+");
      imprime_livro(posicao);
      fclose(teste2);
    }
  }

  fclose(arquivo_codigo);
}

void inserir_via_arquivo_txt()
{

  Dados_Livro livro;

  FILE *file;
  file = fopen("dados.txt", "r");

  char type;
  if (file == NULL)
  {
    printf("Problemas na LEITURA do arquivo\n");
  }
  else
  {
    printf("ARQUIVO ABERTO COM SUCESSO LEITURA SENDO FEITA ...\n");
    while (!feof(file))
    {
      fscanf(file, "%d%*c", &livro.codigo);
      fscanf(file, "%[^;]%*c", livro.titulo);
      fscanf(file, "%[^;]%*c", livro.autor);
      fscanf(file, "%d%*c", &livro.exemplares);
      *livro.autor = *remover_espacos_frase(livro.autor);
      *livro.titulo = *remover_espacos_frase(livro.titulo);
      adiciona_livro(livro);
    }
  }
  printf("LEITURA FEITA COM SUCESSO\n");

  fclose(file);
}