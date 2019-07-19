#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* typedef struct Codigo_livro
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

typedef struct No_Livro
{
    struct Livro *livro;
    int pos_livre;
} No_Livro;

typedef struct Cabecalho_livros_dados
{
    int topo;
    int cabeca;
    int livre;

} Cabecalho_livros_dados;
typedef struct Cabecalho_livros_codigos
{
    int livre;
    int raiz;
    int topo;

} Cabecalho_livros_codigos; */

typedef int TipoItem;

// cabecalho do arquivo
typedef struct
{
    int pos_cabeca; //posi ̧c~ao do in ́ıcio da lista
    int pos_topo;   // 1a posi ̧c~ao n~ao usada no fim do arquivo
    int pos_livre;  // posi ̧c~ao do in ́ıcio da lista de n ́os livres
} cabecalho;

// estrutura de no para lista encadeada
typedef struct
{
    TipoItem info;
    int prox;
} no;

//Escreve no arquivo o cabe ̧calho contendo as informa ̧c~oes da lista
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
//P ́os-condi ̧c~ao: cabe ̧calho escrito no arquivo
void escreve_cabecalho(FILE *arq, cabecalho *cab)
{
    fseek(arq, 0, SEEK_SET); //posiciona no in ́ıcio do arquivo
    fwrite(cab, sizeof(cabecalho), 1, arq);
}

//Cria uma lista nova em arquivo
//Pr ́e-condi ̧c~ao: arquivo aberto para leitura/escrita
//P ́os-condi ̧c~ao: arquivo  ́e inicializado com uma lista vazia
void cria_lista_vazia(FILE *arq)
{
    cabecalho *cab = (cabecalho *)malloc(sizeof(cabecalho));
    cab->pos_cabeca = -1;
    cab->pos_topo = 0;
    cab->pos_livre = -1;
    escreve_cabecalho(arq, cab);
    free(cab);
}

//L^e o cabe ̧calho do arquivo contendo as informa ̧c~oes da lista
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
//P ́os-condi ̧c~ao: retorna o ponteiro para o cabe ̧calho lido
cabecalho *le_cabecalho(FILE *arq)
{
    cabecalho *cab = (cabecalho *)malloc(sizeof(cabecalho));
    fseek(arq, 0, SEEK_SET); // posiciona no in ́ıcio do arquivo
    fread(cab, sizeof(cabecalho), 1, arq);
    return cab;
}

//l^e um n ́o em uma determinada posi ̧c~ao do arquivo
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
// pos deve ser uma posi ̧c~ao v ́alida da lista
//P ́os-condi ̧c~ao: ponteiro para n ́o lido  ́e retornado
no *le_no(FILE *arq, int pos)
{
    no *x = malloc(sizeof(no));
    fseek(arq, sizeof(cabecalho) + pos * sizeof(no), SEEK_SET);
    fread(x, sizeof(no), 1, arq);
    return x;
}

//Escreve um n ́o em uma determinada posi ̧c~ao do arquivo
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
// pos deve ser uma posi ̧c~ao v ́alida do arquivo
//P ́os-condi ̧c~ao: n ́o escrito no arquivo
void escreve_no(FILE *arq, no *x, int pos)
{
    fseek(arq, sizeof(cabecalho) + pos * sizeof(no), SEEK_SET);
    fwrite(x, sizeof(no), 1, arq);
}

void insere(FILE *arq, TipoItem info)
{
    cabecalho *cab = le_cabecalho(arq);
    no x;
    x.info = info;
    x.prox = cab->pos_cabeca;
    if (cab->pos_livre == -1)
    { // n~ao h ́a n ́os livres, ent~ao usar o topo
        escreve_no(arq, &x, cab->pos_topo);
        cab->pos_cabeca = cab->pos_topo;
        cab->pos_topo++;
    }
    else
    { // usar n ́o da lista de livres
        no *aux = le_no(arq, cab->pos_livre);
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
void retira(FILE *arq, TipoItem x)
{
    cabecalho *cab = le_cabecalho(arq);
    int pos_aux = cab->pos_cabeca;
    int pos_ant = cab->pos_cabeca;
    no *aux = NULL;
    while (pos_aux != -1 && // procura o elemento a ser retirado
           ((aux = le_no(arq, pos_aux)) != NULL) &&
           aux->info != x)
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
            no *ant = le_no(arq, pos_ant);
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

void imprimi_lista(FILE *arq){
    no x ;
    cabecalho *cab = (cabecalho *) malloc(sizeof(cabecalho));
    cab = le_cabecalho(arq);

    printf("cabeca= %d , topo=%d , livre= %d\n",cab->pos_cabeca, cab->pos_livre, cab->pos_topo);

    while(fread(&x,sizeof(no),1,arq)){
        printf("numero = %d \n",x.info);
    }

}

int main()
{
    TipoItem x = 5;
    TipoItem x2 = 10;

    FILE *teste;
    teste = fopen("bd.bin", "wb");
    cria_lista_vazia(teste);
    fclose(teste);

    FILE *teste2;
    teste2 = fopen("bd.bin", "rb+");
    insere(teste2, x);
    fclose(teste2);

    FILE *teste3;
    teste3 = fopen("bd.bin", "rb+");
    insere(teste3, x2);
    imprimi_lista(teste3);
    fclose(teste3);


}
