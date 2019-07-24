#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livro_codigos.h"
#include "livros_dados.h"

int main()
{
    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));

 
     FILE *teste;
    teste = fopen("bd.bin", "wb");
    cria_lista_vazia(teste);
    imprimi_lista_livro(teste);
    fclose(teste);

    FILE *teste5;
    teste5 = fopen("bd.bin", "rb+");
    imprimi_lista_livro(teste5);
    fclose(teste5); 

  FILE *teste56;
    teste56 = fopen("bdcodigos.bin", "wb");
    cria_arvore_vazia_codigo(teste56);
    fclose(teste56);


 adiciona_livro();

FILE *teste59;
    teste59 = fopen("bd.bin", "rb+");
    imprimi_lista_livro(teste59);
    fclose(teste59); 

    FILE *teste3;
    teste3 = fopen("bdcodigos.bin", "rb+");
    cab = le_cabecalho_codigos(teste3);
    imprimi_lista(teste3);
    imprimir_arvore_binaria_na_notacao(teste3, cab->pos_raiz); 
    fclose(teste3);


    
  
  
}