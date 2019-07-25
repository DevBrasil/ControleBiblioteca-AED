#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livro_codigos.h"

typedef struct Livro
{
    int codigo;
    char titulo[100];
    char autor[100];
    int exemplares;

} Dados_Livro;

// cabecalho do arquivo
typedef struct
{
    int pos_cabeca; //posi ̧c~ao do in ́ıcio da lista
    int pos_topo;   // 1a posi ̧c~ao n~ao usada no fim do arquivo
    int pos_livre;  // posi ̧c~ao do in ́ıcio da lista de n ́os livres
} Cabecalho_livros_dados;

// estrutura de no para lista encadeada
typedef struct
{
    Dados_Livro livro;
    int ant;
    int prox;
} No_livro;

//Escreve no arquivo o cabe ̧calho contendo as informa ̧c~oes da lista
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
//P ́os-condi ̧c~ao: cabe ̧calho escrito no arquivo
void escreve_cabecalho_livro(FILE *arq, Cabecalho_livros_dados *cab)
{
    fseek(arq, 0, SEEK_SET); //posiciona no in ́ıcio do arquivo
    fwrite(cab, sizeof(Cabecalho_livros_dados), 1, arq);
}

//Cria uma lista nova em arquivo
//Pr ́e-condi ̧c~ao: arquivo aberto para leitura/escrita
//P ́os-condi ̧c~ao: arquivo  ́e inicializado com uma lista vazia
void cria_lista_vazia(FILE *arq)
{
    Cabecalho_livros_dados *cab = (Cabecalho_livros_dados *)malloc(sizeof(Cabecalho_livros_dados));
    cab->pos_cabeca = -1;
    cab->pos_topo = 0;
    cab->pos_livre = -1;
    escreve_cabecalho_livro(arq, cab);
    free(cab);
}

//L^e o cabe ̧calho do arquivo contendo as informa ̧c~oes da lista
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
//P ́os-condi ̧c~ao: retorna o ponteiro para o cabe ̧calho lido
Cabecalho_livros_dados *le_cabecalho_livro(FILE *arq)
{
    Cabecalho_livros_dados *cab = (Cabecalho_livros_dados *)malloc(sizeof(Cabecalho_livros_dados));
    fseek(arq, 0, SEEK_SET); // posiciona no in ́ıcio do arquivo
    fread(cab, sizeof(Cabecalho_livros_dados), 1, arq);
    return cab;
}

//l^e um n ́o em uma determinada posi ̧c~ao do arquivo
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
// pos deve ser uma posi ̧c~ao v ́alida da lista
//P ́os-condi ̧c~ao: ponteiro para n ́o lido  ́e retornado
No_livro *le_no_livro(FILE *arq, int pos)
{
    No_livro *x = malloc(sizeof(No_livro));
    fseek(arq, sizeof(Cabecalho_livros_dados) + pos * sizeof(No_livro), SEEK_SET);
    fread(x, sizeof(No_livro), 1, arq);
    return x;
}

//Escreve um n ́o em uma determinada posi ̧c~ao do arquivo
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
// pos deve ser uma posi ̧c~ao v ́alida do arquivo
//P ́os-condi ̧c~ao: n ́o escrito no arquivo
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
        printf("colocabdo ant = %d , prox = %d\n", x.ant, x.prox);
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

void adiciona_livro_entradas()
{
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
    __fpurge(stdin);
    printf("\nInsira um titulo: ");
    scanf("%[^\n]", livro.titulo);
    __fpurge(stdin);
    printf("\nInsira um autor: ");
    scanf("%[^\n]", livro.autor);
    printf("\nInsira o numero de exemplares: ");
    scanf("%d", &livro.exemplares);
    fclose(arquivo_codigo);

    adiciona_livro(livro);
}
void adiciona_livro(Dados_Livro livro)
{
    int posicao, aux = 0, aux_codigo;

    FILE *arquivo_codigo;
    arquivo_codigo = fopen("bdcodigos.bin", "rb+");
    FILE *arq_livro;
    arq_livro = fopen("bd.bin", "rb+");

    insere_codigo(arquivo_codigo, livro.codigo);
    posicao = insere_livro(arq_livro, livro);                         //Insere dados do livro
    adiciona_posicao_do_livro(arquivo_codigo, posicao, livro.codigo); //adiciona a posicao dos dados do livro
    fclose(arquivo_codigo);
    fclose(arq_livro);
}

void exclui_livro()
{
    FILE *arquivo_codigo;
    arquivo_codigo = fopen("bdcodigos.bin", "rb+");

    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab = le_cabecalho_codigos(arquivo_codigo);

    int aux = 1;
    Codigo aux_codigo;

    printf("\nInsira um codigo: ");
    scanf("%d", &aux_codigo);

    aux = existe_codigo(arquivo_codigo, aux_codigo, cab->pos_raiz); //Insere o codigo dos livros
    while (aux == 0)
    {
        printf("\nInsira um codigo existente: ");
        scanf("%d", &aux_codigo);
        aux = existe_codigo(arquivo_codigo, aux_codigo, cab->pos_raiz); //Insere o codigo dos livros
    }

    int posicao = posicao_do_livro(arquivo_codigo, cab->pos_raiz, aux_codigo);

    excluir_codigo(arquivo_codigo, cab->pos_raiz, aux_codigo);
    fclose(arquivo_codigo);

    FILE *teste2;
    teste2 = fopen("bd.bin", "rb+");
    retira(teste2, posicao);
    fclose(teste2);
}

void imprime_livro(int pos)
{
    FILE *arquivo_livros;
    arquivo_livros = fopen("bd.bin", "rb+");

    No_livro *x = (No_livro *)malloc(sizeof(No_livro));

    x = le_no_livro(arquivo_livros, pos);

    printf("\nCodigo:  %d , Autor: %s , Titulo: %s , Exemplares: %d \n", x->livro.codigo, x->livro.autor, x->livro.titulo, x->livro.exemplares);
}

//Retira um n ́o da lista
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
//P ́os-condi ̧c~ao: n ́o retirado da lista caso perten ̧ca a ela
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
    cab->pos_livre = pos;
    escreve_livro_no(arq, aux, pos);
    escreve_cabecalho_livro(arq, cab);
    free(aux);

    free(cab);
}

void procura_no(FILE *arq, int codigo)
{

    Cabecalho_livros_dados *cab = le_cabecalho_livro(arq);
    int pos_aux = cab->pos_cabeca;
    int pos_ant = cab->pos_cabeca;
    No_livro *aux = NULL;
    while (pos_aux != -1 && /* procura o elemento a ser pesquisado */ ((aux = le_no_livro(arq, pos_aux)) != NULL) && aux->livro.codigo != codigo)
    {
        pos_ant = pos_aux;
        pos_aux = aux->prox;
        free(aux);
        aux = NULL;
    }

    if (pos_aux != -1)
    { //encontrou o elemento
        aux = le_no_livro(arq, pos_aux);

        printf("\nCodigo = %d , Autor = %s , Titulo = %s , Exemplares = %d\n", aux->livro.codigo, aux->livro.autor, aux->livro.titulo, aux->livro.exemplares);
    }
    else
    {
        printf("\nLivro nao encontrado \n");
    }

    free(cab);
}

void imprime_lista_livro(FILE *arq)
{
    No_livro x;
    Cabecalho_livros_dados *cab = (Cabecalho_livros_dados *)malloc(sizeof(Cabecalho_livros_dados));
    cab = le_cabecalho_livro(arq);

    printf("\ncabeca= %d , topo=%d , livre= %d\n", cab->pos_cabeca, cab->pos_topo, cab->pos_livre);
    int i = 0;
    while (fread(&x, sizeof(No_livro), 1, arq))
    {
        printf("\n[%d] - Codigo:  %d , Autor: %s , Titulo: %s , Exemplares: %d ,ant = %d ,prox = %d\n", i, x.livro.codigo, x.livro.autor, x.livro.titulo, x.livro.exemplares, x.ant, x.prox);
        i++;
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

void buscar_dados_do_livro()
{
    FILE *arquivo_codigo;
    arquivo_codigo = fopen("bdcodigos.bin", "rb+");

    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab = le_cabecalho_codigos(arquivo_codigo);

    int aux = 1;
    Codigo aux_codigo;

    printf("\nInsira um codigo: ");
    scanf("%d", &aux_codigo);

    aux = existe_codigo(arquivo_codigo, aux_codigo, cab->pos_raiz); //Insere o codigo dos livros
    while (aux == 0)
    {
        printf("\nInsira um codigo existente: ");
        scanf("%d", &aux_codigo);
        aux = existe_codigo(arquivo_codigo, aux_codigo, cab->pos_raiz); //Insere o codigo dos livros
    }

    int posicao = posicao_do_livro(arquivo_codigo, cab->pos_raiz, aux_codigo);

    FILE *teste2;
    teste2 = fopen("bd.bin", "rb+");
    imprime_livro(posicao);
    fclose(teste2);
}

void atualizar_exmplares(){
    FILE *arquivo_codigo;
    arquivo_codigo = fopen("bdcodigos.bin", "rb+");

    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab = le_cabecalho_codigos(arquivo_codigo);

    int aux = 1;
    Codigo aux_codigo;

    printf("\nInsira um codigo: ");
    scanf("%d", &aux_codigo);

    aux = existe_codigo(arquivo_codigo, aux_codigo, cab->pos_raiz); //Insere o codigo dos livros
    while (aux == 0)
    {
        printf("\nInsira um codigo existente: ");
        scanf("%d", &aux_codigo);
        aux = existe_codigo(arquivo_codigo, aux_codigo, cab->pos_raiz); //Insere o codigo dos livros
    }

    int posicao = posicao_do_livro(arquivo_codigo, cab->pos_raiz, aux_codigo);

    FILE *teste2;
    teste2 = fopen("bd.bin", "rb+");
    imprime_livro(posicao);
    No_livro *livro = (No_livro *) malloc(sizeof(No_livro));
    livro = le_no_livro(teste2, posicao);

    printf("\nInsira a nova quantidade de exemplares do livro: ");
    scanf("%d", &livro->livro.exemplares);

    escreve_livro_no(teste2,livro,posicao);

    fclose(teste2);
}

/* void teste()
{
    
    FILE *teste;
    teste = fopen("bd.bin", "wb");
    cria_lista_vazia(teste);
    imprimi_lista_livro(teste);
    fclose(teste);
    printf("-------------------------------------------------------------\n");
    FILE *teste2;
    teste2 = fopen("bd.bin", "rb+");

    insere_livro(teste2, x);
    insere_livro(teste2, x2);
    insere_livro(teste2, x3);
    insere_livro(teste2, x4);
    insere_livro(teste2, x5);
    insere_livro(teste2, x6);
    imprimi_lista_livro(teste2);
    fclose(teste2);

    printf("-------------------------------------------------------------\n");

    FILE *teste4;
    teste4 = fopen("bd.bin", "rb+");
    retira(teste4, 9999);
    imprimi_lista_livro(teste4);
    fclose(teste4);

    printf("---------------------inserindo denovo----------------------------------\n");

    FILE *teste5;
    teste5 = fopen("bd.bin", "rb+");
    insere_livro(teste, x7);
    imprimi_lista_livro(teste5);
    fclose(teste5);

    printf("---------------------Procurando no----------------------------------\n");

    FILE *teste6;
    teste6 = fopen("bd.bin", "rb+");
    procura_no(teste6, 158);
    procura_no(teste6, 160);
    fclose(teste6);
} */

/* int main()
{
    teste();
}
 */