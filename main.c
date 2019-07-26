#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livro_codigos.h"
#include "livros_dados.h"
#include "integration.h"
#include "menu.h"

void teste()
{
  Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));

  FILE *teste;
  teste = fopen("bd.bin", "wb");
  cria_lista_vazia(teste);
  fclose(teste);

  FILE *teste1;
  teste1 = fopen("bdcodigos.bin", "wb");
  cria_arvore_vazia_codigo(teste1);
  fclose(teste1);

  Dados_Livro x;
  Dados_Livro x2;
  Dados_Livro x3;
  Dados_Livro x4;
  Dados_Livro x5;
  Dados_Livro x6;
  Dados_Livro x7;

  strcpy(x.titulo, "teste 1");
  strcpy(x.autor, "teste 1");
  x.codigo = 111;
  x.exemplares = 111;
  strcpy(x2.titulo, "teste 2");
  strcpy(x2.autor, "teste 2");
  x2.codigo = 222;
  x2.exemplares = 222;
  strcpy(x3.titulo, "teste 3");
  strcpy(x3.autor, "teste 3");
  x3.codigo = 333;
  x3.exemplares = 333;
  strcpy(x4.titulo, "teste 4");
  strcpy(x4.autor, "teste 4");
  x4.codigo = 444;
  x4.exemplares = 444;
  strcpy(x5.titulo, "teste 5");
  strcpy(x5.autor, "teste 5");
  x5.codigo = 555;
  x5.exemplares = 555;
  strcpy(x6.titulo, "teste 6");
  strcpy(x6.autor, "teste 6");
  x6.codigo = 666;
  x6.exemplares = 666;
  strcpy(x7.titulo, "teste 7");
  strcpy(x7.autor, "teste 7");
  x7.codigo = 777;
  x7.exemplares = 777;

  adiciona_livro(x5);
  adiciona_livro(x3);
  adiciona_livro(x6);
  adiciona_livro(x7);
  adiciona_livro(x);
  adiciona_livro(x4);
  adiciona_livro(x2);

  FILE *teste3;
  teste3 = fopen("bdcodigos.bin", "rb+");
  cab = le_cabecalho_codigos(teste3);

  imprime_em_ordem_de_codigo(teste3, cab->pos_raiz);
  fclose(teste3);

  atualizar_exmplares();

  FILE *teste4;
  teste4 = fopen("bdcodigos.bin", "rb+");
  cab = le_cabecalho_codigos(teste4);

  imprime_em_ordem_de_codigo(teste3, cab->pos_raiz);
  fclose(teste4);
}

int main()
{
  FILE *codigos = fopen("bdcodigos.bin", "wb");
  FILE *dados = fopen("bd.bin", "wb+");

  cria_arvore_vazia_codigo(codigos);
  cria_lista_vazia(dados);

  fclose(codigos);
  fclose(dados);

  menu_start();
}