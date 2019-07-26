#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livro_codigos.h"

//Estrutura para armazenar o informacoes de um livro;
typedef struct Livro
{
    int codigo;
    char titulo[100];
    char autor[100];
    int exemplares;

} Dados_Livro;

//Eabecalho do arquivo de dados
typedef struct
{
    int pos_cabeca; //posi ̧c~ao do in ́ıcio da lista
    int pos_topo;   // 1a posi ̧c~ao n~ao usada no fim do arquivo
    int pos_livre;  // posi ̧c~ao do in ́ıcio da lista de n ́os livres
} Cabecalho_livros_dados;

//Estrutura de no de livro para lista encadeada
typedef struct
{
    Dados_Livro livro;
    int ant;
    int prox;
} No_livro;

void escreve_cabecalho_livro(FILE *arq, Cabecalho_livros_dados *cab)
{
    fseek(arq, 0, SEEK_SET); //posiciona no in ́ıcio do arquivo
    fwrite(cab, sizeof(Cabecalho_livros_dados), 1, arq);
}

void cria_lista_vazia(FILE *arq)
{
    Cabecalho_livros_dados *cab = (Cabecalho_livros_dados *)malloc(sizeof(Cabecalho_livros_dados));
    cab->pos_cabeca = -1;
    cab->pos_topo = 0;
    cab->pos_livre = -1;
    escreve_cabecalho_livro(arq, cab);
    free(cab);
}

Cabecalho_livros_dados *le_cabecalho_livro(FILE *arq)
{
    Cabecalho_livros_dados *cab = (Cabecalho_livros_dados *)malloc(sizeof(Cabecalho_livros_dados));
    fseek(arq, 0, SEEK_SET); // posiciona no in ́ıcio do arquivo
    fread(cab, sizeof(Cabecalho_livros_dados), 1, arq);
    return cab;
}

No_livro *le_no_livro(FILE *arq, int pos)
{
    No_livro *x = malloc(sizeof(No_livro));
    fseek(arq, sizeof(Cabecalho_livros_dados) + pos * sizeof(No_livro), SEEK_SET);
    fread(x, sizeof(No_livro), 1, arq);
    return x;
}

void escreve_livro_no(FILE *arq, No_livro *x, int pos)
{
    fseek(arq, sizeof(Cabecalho_livros_dados) + pos * sizeof(No_livro), SEEK_SET);
    fwrite(x, sizeof(No_livro), 1, arq);
}

int insere_livro(FILE *arq, Dados_Livro livro)
{
    int aux_pos;
    Cabecalho_livros_dados *cab = le_cabecalho_livro(arq);
    No_livro x;
    x.livro = livro;
    x.ant = cab->pos_cabeca;
    x.prox = -1;

    if (cab->pos_livre == -1)
    { // n~ao h ́a n ́os livres, ent~ao usar o topo
        escreve_livro_no(arq, &x, cab->pos_topo);

        No_livro *ant = le_no_livro(arq, x.ant);
        ant->prox = cab->pos_topo;

        escreve_livro_no(arq, ant, x.ant);

        cab->pos_cabeca = cab->pos_topo;
        aux_pos = cab->pos_topo;
        cab->pos_topo++;
        escreve_cabecalho_livro(arq, cab);
        free(cab);
        return aux_pos;
    }
    else
    { // usar n ́o da lista de livres
        No_livro *livre = le_no_livro(arq, cab->pos_livre);

        escreve_livro_no(arq, &x, cab->pos_livre);
        cab->pos_cabeca = cab->pos_livre;

        No_livro *ant = le_no_livro(arq, x.ant);
        ant->prox = cab->pos_livre;

        printf("no ant = %d \n", x.ant);

        escreve_livro_no(arq, ant, x.ant);

        aux_pos = cab->pos_livre;
        cab->pos_livre = livre->prox;
        free(livre);
        escreve_cabecalho_livro(arq, cab);
        free(cab);
        return aux_pos;
    }
}

void imprime_livro(int pos)
{
    FILE *arquivo_livros;
    arquivo_livros = fopen("bd.bin", "rb+");

    No_livro *x = (No_livro *)malloc(sizeof(No_livro));

    x = le_no_livro(arquivo_livros, pos);

    printf("\nCodigo:  %d , Autor: %s , Titulo: %s , Exemplares: %d \n", x->livro.codigo, x->livro.autor, x->livro.titulo, x->livro.exemplares);
}

void retira(FILE *arq, int pos)
{

    Cabecalho_livros_dados *cab = (Cabecalho_livros_dados *)malloc(sizeof(Cabecalho_livros_dados));

    cab = le_cabecalho_livro(arq);

    No_livro *aux = (No_livro *)malloc(sizeof(No_livro));
    aux = le_no_livro(arq, pos);

    printf("caralho %d\n", pos);

    if (pos == cab->pos_cabeca)
    { // remo ̧c~ao na cabe ̧ca
        No_livro *ant = (No_livro *)malloc(sizeof(No_livro));
        ant = le_no_livro(arq, aux->ant);
        ant->prox = -1;

        escreve_livro_no(arq, ant, aux->ant);

        cab->pos_cabeca = aux->ant;

        escreve_cabecalho_livro(arq, cab);

        free(ant);
    }
    else
    { // remo ̧c~ao no meio
        No_livro *ant = (No_livro *)malloc(sizeof(No_livro));
        No_livro *prox = (No_livro *)malloc(sizeof(No_livro));

        ant = le_no_livro(arq, aux->ant);
        prox = le_no_livro(arq, aux->prox);

        ant->prox = aux->prox;
        prox->ant = aux->ant;

        escreve_livro_no(arq, ant, aux->ant);
        escreve_livro_no(arq, prox, aux->prox);

        free(ant);
        free(prox);
    }

    aux->prox = cab->pos_livre; // torna o n ́o removido um n ́o livre
    aux->ant = -1;
    aux->livro.codigo = -1;
    cab->pos_livre = pos;
    escreve_livro_no(arq, aux, pos);
    escreve_cabecalho_livro(arq, cab);
    free(aux);

    free(cab);
}
