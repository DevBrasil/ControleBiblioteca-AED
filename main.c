#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livro_codigos.h"
#include "livros_dados.h"
#include "integration.h"
#include "menu.h"


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