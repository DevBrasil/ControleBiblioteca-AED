#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livro_codigos.h"
#include "livros_dados.h"

void adiciona_livro(Dados_Livro livro)
{
  int posicao, aux = 0, aux_codigo;

  FILE *arquivo_codigo;
  arquivo_codigo = fopen("bdcodigos.bin", "rb+");
  FILE *arq_livro;
  arq_livro = fopen("bd.bin", "rb+");

  insere_codigo(arquivo_codigo, livro.codigo);
  posicao = insere_livro(arq_livro, livro);                                   //Insere dados do livro
  adiciona_posicao_do_livro_no_codigo(arquivo_codigo, posicao, livro.codigo); //adiciona a posicao dos dados do livro
  fclose(arquivo_codigo);
  fclose(arq_livro);
}

void adiciona_livro_entradas()
{
  printf("\n \t Adicionar Livro\n\n");

  FILE *arquivo_codigo;
  arquivo_codigo = fopen("bdcodigos.bin", "rb+");

  Dados_Livro livro;

  Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
  cab = le_cabecalho_codigos(arquivo_codigo);

  int aux_codigo, aux = 1;
  printf("Insira um codigo: ");
  scanf("%d", &aux_codigo);

  aux = existe_codigo(arquivo_codigo, aux_codigo, cab->pos_raiz); //Insere o codigo dos livros

  while (aux == 1)
  {
    aux = existe_codigo(arquivo_codigo, aux_codigo, cab->pos_raiz); //Insere o codigo dos livros
    printf("%d", aux);
    printf("\nInsira um codigo novamente: ");
    scanf("%d", &aux_codigo);
  }

  livro.codigo = aux_codigo;
  //__fpurge(stdin);
  printf("\nInsira um titulo: ");
  scanf("%[^\n]", livro.titulo);
  //__fpurge(stdin);
  printf("\nInsira um autor: ");
  scanf("%[^\n]", livro.autor);
  printf("\nInsira o numero de exemplares: ");
  scanf("%d", &livro.exemplares);
  fclose(arquivo_codigo);

  adiciona_livro(livro);
}

void exclui_livro(int codigo)
{
  FILE *arquivo_codigo;
  arquivo_codigo = fopen("bdcodigos.bin", "rb+");

  Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
  cab = le_cabecalho_codigos(arquivo_codigo);

  int posicao = posicao_do_livro(arquivo_codigo, cab->pos_raiz, codigo);

  excluir_codigo(arquivo_codigo, cab->pos_raiz, codigo);
  fclose(arquivo_codigo);

  FILE *teste2;
  teste2 = fopen("bd.bin", "rb+");
  retira(teste2, posicao);
  fclose(teste2);
  fclose(arquivo_codigo);
}

void exclui_livro_entradas()
{
  printf("\n \t Excluir Livro\n\n");

  FILE *arquivo_codigo;
  arquivo_codigo = fopen("bdcodigos.bin", "rb+");

  Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
  cab = le_cabecalho_codigos(arquivo_codigo);

  if (cab->pos_raiz == -1)
  {
    printf("Nao existem livros cadastrados \n");
    fclose(arquivo_codigo);
  }
  else
  {
    int aux = 1;
    Codigo aux_codigo;
    printf("\nInsira o codigo: ");
    scanf("%d", &aux_codigo);

    aux = existe_codigo(arquivo_codigo, aux_codigo, cab->pos_raiz); //Insere o codigo dos livros
    if (aux == 0)
    {
      printf("Livro nao existente no sistema\n");
      fclose(arquivo_codigo);
    }
    else
    {
      fclose(arquivo_codigo);

      exclui_livro(aux_codigo);
    }
  }
}

void imprime_em_ordem_de_codigo(FILE *arq, int pos)
{
  No_Codigo *no = (No_Codigo *)malloc(sizeof(No_Codigo));
  no = le_no_codigo(arq, pos);

  if (no->esquerda != -1)
  {
    imprime_em_ordem_de_codigo(arq, no->esquerda);
  }
  imprime_livro(no->pos_livro);

  if (no->direita != -1)
  {
    imprime_em_ordem_de_codigo(arq, no->direita);
  }
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
    if (file == NULL){
        printf("Problemas na CRIACAO do arquivo\n");
        
    }else{
        printf("ARQUIVO ABERTO COM SUCESSO LEITURA SENDO FEITA ...\n");
        while(!feof(file)){
            fscanf(file,"%d%*c",&livro.codigo);
                fscanf(file,"%[^;]%*c",livro.titulo);
                fscanf(file,"%[^;]%*c",livro.autor);
                fscanf(file,"%d%*c",&livro.exemplares);
                adiciona_livro(livro);  
            }
        }
        printf("LEITURA FEITA COM SUCESSO\n");
    
    
    fclose(file);

}