#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livro_codigos.h"
#include "livros_dados.h"
#include "integration.h"

void ClearTerminal()
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void menu_start()
{
  ClearTerminal();

  int opcao;

  while (2 == 2)
  {

    printf("----------------------------------------------");
    printf("\n\tMENU\n");
    printf("1. Carregar arquivo de inicializacao:\n");
    printf("2. Inserir livro\n");
    printf("3. Atualizar numero de exemplares de um livro\n");
    printf("4. Remover livro\n");
    printf("5. Buscar dados do livro\n");
    printf("6. Imprimir arvore binaria\n");
    printf("7. Imprimir arvore binaria por nıvel\n");
    printf("8. Imprimir acervo\n");
    printf("9. Gerar listagem de livros ordenados por tıtulo\n");

    printf("0. Sair\n");
    printf("----------------------------------------------\n");

    scanf("%d", &opcao);

    switch (opcao)
    {

    case 1:
      ClearTerminal();
      inserir_via_arquivo_txt();
      printf("Opcao 1\n");
      break;
    case 2:
      ClearTerminal();
      adiciona_livro_entradas();
      break;
    case 3:
      ClearTerminal();
      atualizar_exmplares();
      break;
    case 4:
      ClearTerminal();
      exclui_livro();
     
      break;
    case 5:
      ClearTerminal();
      buscar_dados_do_livro();
      break;
    case 6:
      imprime_tudo_notacao();
      break;
    case 7:
      ClearTerminal();
      printa_arvore_por_nivel();
      break;
    case 8:
      ClearTerminal();
      livros_ordenados_por_codigo();
      break;
    case 9:
      ClearTerminal();
      gerarListagemporTitulo();
      break;
    case 0:
      ClearTerminal();
      printf("\nPrograma finalizado\n");
      exit(0);
      break;
    }
  }
}