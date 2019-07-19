#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int Codigo;

typedef struct 
{
    Codigo codigo;
    int posicao_esquerda;
    int posicao_direita;
    int posicao_pai;

} No_Codigo_Livro;

typedef struct
{
    int pos_raiz; //posi ̧c~ao do in ́ıcio da lista
    int pos_livre;   // 1a posi ̧c~ao n~ao usada no fim do arquivo
    int pos_topo;  // posi ̧c~ao do in ́ıcio da lista de n ́os livres
} Cabecalho_livros_codigos;

void cria_arvore_vazia(FILE *arq)
{
    Cabecalho_livros_codigos *cab = (Cabecalho_livros_codigos *)malloc(sizeof(Cabecalho_livros_codigos));
    cab->pos_raiz = -1;
    cab->pos_topo = 0;
    cab->pos_topo = -1;
    escreve_cabecalho(arq, cab);
    free(cab);
}

void escreve_cabecalho(FILE *arq, Cabecalho_livros_codigos *cab)
{
    fseek(arq, 0, SEEK_SET); //posiciona no in ́ıcio do arquivo
    fwrite(cab, sizeof(Cabecalho_livros_codigos), 1, arq);
}

//L^e o cabe ̧calho do arquivo contendo as informa ̧c~oes da lista
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
//P ́os-condi ̧c~ao: retorna o ponteiro para o cabe ̧calho lido
Cabecalho_livros_codigos *le_cabecalho(FILE *arq)
{
    Cabecalho_livros_codigos *cab = (Cabecalho_livros_codigos *)malloc(sizeof(Cabecalho_livros_codigos));
    fseek(arq, 0, SEEK_SET); // posiciona no in ́ıcio do arquivo
    fread(cab, sizeof(Cabecalho_livros_codigos), 1, arq);
    return cab;
}