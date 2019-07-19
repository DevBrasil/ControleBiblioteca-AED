#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Codigo_livro
{
    int codigo;

} Codigo_livro;

typedef struct No_Codigo_Livro
{
    struct Codigo_livro *codigo;
    int posicao_esquerda;
    int posicao_direita;
    int posicao_pai;

} No_Codigo_Livro;

typedef struct Livro
{
    int codigo;
    char titulo[100];
    char autor[100];
    int exemplares;

} Dados_Livro;


typedef struct Cabecalho_livros_codigos
{
    int livre;
    int raiz;
    int topo;

} Cabecalho_livros_codigos;


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
    int prox;
} No_livro;

//Escreve no arquivo o cabe ̧calho contendo as informa ̧c~oes da lista
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
//P ́os-condi ̧c~ao: cabe ̧calho escrito no arquivo
void escreve_cabecalho(FILE *arq, Cabecalho_livros_dados *cab)
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
    escreve_cabecalho(arq, cab);
    free(cab);
}

//L^e o cabe ̧calho do arquivo contendo as informa ̧c~oes da lista
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
//P ́os-condi ̧c~ao: retorna o ponteiro para o cabe ̧calho lido
Cabecalho_livros_dados *le_cabecalho(FILE *arq)
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
No_livro *le_no(FILE *arq, int pos)
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
void escreve_no(FILE *arq, No_livro *x, int pos)
{
    fseek(arq, sizeof(Cabecalho_livros_dados) + pos * sizeof(No_livro), SEEK_SET);
    fwrite(x, sizeof(No_livro), 1, arq);
}

void insere(FILE *arq, Dados_Livro livro)
{
    Cabecalho_livros_dados *cab = le_cabecalho(arq);
    No_livro x;
    x.livro = livro;
    x.prox = cab->pos_cabeca;
    if (cab->pos_livre == -1)
    { // n~ao h ́a n ́os livres, ent~ao usar o topo
        escreve_no(arq, &x, cab->pos_topo);
        cab->pos_cabeca = cab->pos_topo;
        cab->pos_topo++;
    }
    else
    { // usar n ́o da lista de livres
        No_livro *aux = le_no(arq, cab->pos_livre);
        escreve_no(arq, &x, cab->pos_livre);
        cab->pos_cabeca = cab->pos_livre;
        cab->pos_livre = aux->prox;
        free(aux);
    }
    escreve_cabecalho(arq, cab);
    free(cab);
}

//Retira um n ́o da lista
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
//P ́os-condi ̧c~ao: n ́o retirado da lista caso perten ̧ca a ela
void retira(FILE *arq, int codigo)
{
    Cabecalho_livros_dados *cab = le_cabecalho(arq);
    int pos_aux = cab->pos_cabeca;
    int pos_ant = cab->pos_cabeca;
    No_livro *aux = NULL;
    while (pos_aux != -1 && // procura o elemento a ser retirado
           ((aux = le_no(arq, pos_aux)) != NULL) &&
           aux->livro.codigo != codigo)
    {
        pos_ant = pos_aux;
        pos_aux = aux->prox;
        free(aux);
        aux = NULL;
    }
    if (pos_aux != -1)
    { //encontrou o elemento
        if (pos_ant == pos_aux)
        { // remo ̧c~ao na cabe ̧ca
            cab->pos_cabeca = aux->prox;
        }
        else
        { // remo ̧c~ao no meio
            No_livro *ant = le_no(arq, pos_ant);
            ant->prox = aux->prox;
            escreve_no(arq, ant, pos_ant);
            free(ant);
        }
        aux->prox = cab->pos_livre; // torna o n ́o removido um n ́o livre
        cab->pos_livre = pos_aux;
        escreve_no(arq, aux, pos_aux);
        escreve_cabecalho(arq, cab);
        free(aux);
    }
    free(cab);
}

void procura_no(FILE *arq, int codigo){
    Cabecalho_livros_dados *cab = le_cabecalho(arq);
    int pos_aux = cab->pos_cabeca;
    int pos_ant = cab->pos_cabeca;
    No_livro *aux = NULL;
    while (pos_aux != -1 &&  /* procura o elemento a ser pesquisado */ ((aux = le_no(arq, pos_aux)) != NULL) && aux->livro.codigo != codigo)
    {
        pos_ant = pos_aux;
        pos_aux = aux->prox;
        free(aux);
        aux = NULL;
    }

    if (pos_aux != -1){ //encontrou o elemento
        aux = le_no(arq,pos_aux);

        printf("\nCodigo = %d , Autor = %s , Titulo = %s , Exemplares = %d\n",aux->livro.codigo, aux->livro.autor, aux->livro.titulo, aux->livro.exemplares);
    }else {
        printf("\nLivro nao encontrado \n");
    }

    free(cab);

}

void imprimi_lista(FILE *arq)
{
    No_livro x;
    Cabecalho_livros_dados *cab = (Cabecalho_livros_dados *)malloc(sizeof(Cabecalho_livros_dados));
    cab = le_cabecalho(arq);

    printf("\ncabeca= %d , topo=%d , livre= %d\n", cab->pos_cabeca, cab->pos_topo, cab->pos_livre);

    while (fread(&x, sizeof(No_livro), 1, arq))
    {
        printf("\nCodigo:  %d , Autor: %s , Titulo: %s , Exemplares: %d , %d\n", x.livro.codigo, x.livro.autor, x.livro.titulo, x.livro.exemplares, x.prox);
    }
}

int main()
{
    Dados_Livro x;
    Dados_Livro x2;
    Dados_Livro x3;
    Dados_Livro x4;
    Dados_Livro x5;
    Dados_Livro x6;
    Dados_Livro x7;
    strcpy(x.titulo, "Rei das gambiarras");strcpy(x.autor,"Fernando brasil"); x.codigo = 155; x.exemplares=1457;
    strcpy(x2.titulo, "So colocar que funciona");strcpy(x2.autor,"Ingo top"); x2.codigo = 156; x2.exemplares=999;
    strcpy(x3.titulo, "Cade o socket?");strcpy(x3.autor,"Felipe Android"); x3.codigo = 157; x3.exemplares=1457;
    strcpy(x4.titulo, "A merda que é o GoogleAds");strcpy(x4.autor,"Filipe"); x4.codigo = 158; x4.exemplares=588;
    strcpy(x5.titulo, "Crescer ainda mais");strcpy(x5.autor,"Dr Barroncas"); x5.codigo = 159; x5.exemplares=1457;
    strcpy(x6.titulo, "Quais sao suas metas?");strcpy(x6.autor,"Coach Anisio"); x6.codigo = 160; x6.exemplares=145557;
    strcpy(x7.titulo, "Faz o narga pitando um Fox!");strcpy(x7.autor,"Alecs Fox"); x7.codigo = 161; x7.exemplares=188457;

    FILE *teste;
    teste = fopen("bd.bin", "wb");
    cria_lista_vazia(teste);
    imprimi_lista(teste);
    fclose(teste);
    printf("-------------------------------------------------------------\n");
    FILE *teste2;
    teste2 = fopen("bd.bin", "rb+");

    insere(teste2, x);
    insere(teste2, x2);
    insere(teste2, x3);
    insere(teste2, x4);
    insere(teste2, x5);
    insere(teste2, x6);
  imprimi_lista(teste2);
    fclose(teste2);

    printf("-------------------------------------------------------------\n");

    FILE *teste4;
    teste4 = fopen("bd.bin", "rb+");
    retira(teste4, 9999);
    imprimi_lista(teste4);
    fclose(teste4);

    printf("---------------------inserindo denovo----------------------------------\n");

    FILE *teste5;
    teste5 = fopen("bd.bin", "rb+");
    insere(teste, x7);
    imprimi_lista(teste5);
    fclose(teste5);

     printf("---------------------Procurando no----------------------------------\n");

    FILE *teste6;
    teste6 = fopen("bd.bin", "rb+");
    procura_no(teste6,158);
    procura_no(teste6,160);
    fclose(teste6);
}
