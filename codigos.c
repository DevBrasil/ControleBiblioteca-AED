#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int Codigo;

typedef struct Cabecalho_Codigo
{
    int pos_raiz;
    int pos_topo;
    int pos_livre;

} Cabecalho_Codigo;

typedef struct No_Codigo
{
    Codigo info;
    int pos_livro;
    int esquerda;
    int direita;
} No_Codigo;

//Escreve um n ́o em uma determinada posi ̧c~ao do arquivo
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
// pos deve ser uma posi ̧c~ao v ́alida do arquivo
//P ́os-condi ̧c~ao: n ́o escrito no arquivo
void escreve_no_codigo(FILE *arq, No_Codigo *x, int pos)
{
    fseek(arq, sizeof(Cabecalho_Codigo) + pos * sizeof(No_Codigo), SEEK_SET);
    fwrite(x, sizeof(No_Codigo), 1, arq);
}

void escreve_cabecalho_codigo(FILE *arq, Cabecalho_Codigo *cab)
{
    fseek(arq, 0, SEEK_SET); //posiciona no in ́ıcio do arquivo
    fwrite(cab, sizeof(Cabecalho_Codigo), 1, arq);
}

void cria_arvore_vazia_codigo(FILE *arq)
{
    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab->pos_raiz = -1;
    cab->pos_topo = 0;
    cab->pos_livre = -1;
    escreve_cabecalho_codigo(arq, cab);
    free(cab);
}

//l^e um n ́o em uma determinada posi ̧c~ao do arquivo
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
// pos deve ser uma posi ̧c~ao v ́alida da lista
//P ́os-condi ̧c~ao: ponteiro para n ́o lido  ́e retornado
No_Codigo *le_no_codigo(FILE *arq, int pos)
{
    No_Codigo *x = malloc(sizeof(No_Codigo));
    fseek(arq, sizeof(Cabecalho_Codigo) + pos * sizeof(No_Codigo), SEEK_SET);
    fread(x, sizeof(No_Codigo), 1, arq);
    return x;
}

//L^e o cabe ̧calho do arquivo contendo as informa ̧c~oes da lista
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
//P ́os-condi ̧c~ao: retorna o ponteiro para o cabe ̧calho lido
Cabecalho_Codigo *le_cabecalho_codigos(FILE *arq)
{
    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    fseek(arq, 0, SEEK_SET); // posiciona no in ́ıcio do arquivo
    fread(cab, sizeof(Cabecalho_Codigo), 1, arq);
    return cab;
}

int adiciona_codigo_no_bd_codigos(FILE *arq, Codigo info, int pos)
{
    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab = le_cabecalho_codigos(arq);
    fseek(arq, sizeof(Cabecalho_Codigo) + pos * sizeof(No_Codigo), SEEK_SET);
    No_Codigo *no_aux = (No_Codigo *)malloc(sizeof(No_Codigo));
    fread(no_aux, sizeof(No_Codigo), 1, arq);

    No_Codigo *novo = (No_Codigo *)malloc(sizeof(No_Codigo));
    novo->direita = -1;
    novo->esquerda = -1;
    novo->info = info;

    if (no_aux->info < novo->info)
    { //adição a direita
        if (no_aux->direita != -1)
        { //ele ja tem filho da direita
            printf("Ele tem filho a direita\n");
            adiciona_codigo_no_bd_codigos(arq, novo->info, no_aux->direita);
        }
        else
        { //nao tem filho
            printf("Ele nao tem filho a direita\n");
            if (cab->pos_livre != -1)
            { //adicionar na posicao livre
                No_Codigo *aux = (No_Codigo *)malloc(sizeof(No_Codigo));
                aux = le_no_codigo(arq, cab->pos_livre);

                no_aux->direita = cab->pos_livre;

                if (aux->info == -1)
                { //so tem um no livre
                    cab->pos_livre = -1;
                }
                else
                { //tem mais de um no livre
                    cab->pos_livre = aux->info;
                }

                no_aux->direita = cab->pos_livre;
                escreve_no_codigo(arq, no_aux, pos);
                escreve_no_codigo(arq, novo, no_aux->direita);
                escreve_cabecalho_codigo(arq, cab);
            }
            else
            { //nao tem posicao livre
                printf("Ele nao tem posicao livre\n");
                no_aux->direita = cab->pos_topo;
                escreve_no_codigo(arq, no_aux, pos);
                escreve_no_codigo(arq, novo, no_aux->direita);
                cab->pos_topo++;
                escreve_cabecalho_codigo(arq, cab);
            }
        }
    }
    else if (no_aux->info > info)
    { //adição a esquerda
        if (no_aux->esquerda != -1)
        { //ele ja tem filho na esquerda
            printf("Ele tem filho a esquerda\n");
            adiciona_codigo_no_bd_codigos(arq, novo->info, no_aux->esquerda);
        }
        else
        { //nao tem filho
            printf("Ele nao tem filho a esquerda\n");
            if (cab->pos_livre != -1)
            { //adicionar na posicao livre
                No_Codigo *aux = (No_Codigo *)malloc(sizeof(No_Codigo));
                aux = le_no_codigo(arq, cab->pos_livre);

                no_aux->esquerda = cab->pos_livre;

                if (aux->info == -1)
                { //so tem um no livre
                    cab->pos_livre = -1;
                }
                else
                { //tem mais de um no livre
                    cab->pos_livre = aux->info;
                }

                no_aux->esquerda = cab->pos_livre;
                escreve_no_codigo(arq, no_aux, pos);
                escreve_no_codigo(arq, novo, no_aux->esquerda);
                escreve_cabecalho_codigo(arq, cab);
            }
            else
            { //nao tem posicao livre
                printf("Ele nao tem posicao livre\n");
                no_aux->esquerda = cab->pos_topo;
                escreve_no_codigo(arq, no_aux, pos);
                escreve_no_codigo(arq, novo, no_aux->esquerda);
                cab->pos_topo++;
                escreve_cabecalho_codigo(arq, cab);
            }
        }
    }
    else
    {
        printf("Codigo ja existente");
        return 0;
    }
}

int insere(FILE *arq, Codigo info)
{
    Cabecalho_Codigo *cab = le_cabecalho_codigos(arq);
    No_Codigo x;
    x.info = info;

    if (cab->pos_raiz == -1)
    { //nao tem raiz ainda
        printf("entrou aqui onde tem um monte de coisa\n");
        x.direita = -1;
        x.esquerda = -1;
        escreve_no_codigo(arq, &x, cab->pos_topo);
        cab->pos_raiz = 0;
        cab->pos_topo++;
        escreve_cabecalho_codigo(arq, cab);
        return 1;
    }
    else
    {
        return adiciona_codigo_no_bd_codigos(arq, info, cab->pos_raiz);
    }
    return 0;
}

int maximo_codigo(FILE *arq, int pos)
{
    No_Codigo *no_aux = (No_Codigo *)malloc(sizeof(No_Codigo));
    no_aux = le_no_codigo(arq, pos);

    if (no_aux->direita != -1)
    {
        return maximo_codigo(arq, no_aux->direita);
    }
    else
        return no_aux->info;
}

int minimo_codigo(FILE *arq, int pos)
{
    No_Codigo *no_aux = (No_Codigo *)malloc(sizeof(No_Codigo));
    no_aux = le_no_codigo(arq, pos);

    if (no_aux->esquerda != -1)
    {
        return minimo_codigo(arq, no_aux->esquerda);
    }
    else
        return no_aux->info;
}

void imprimi_lista(FILE *arq)
{
    No_Codigo x;
    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab = le_cabecalho_codigos(arq);

    printf("Raiz %d  Topo %d Livre %d\n", cab->pos_raiz, cab->pos_topo, cab->pos_livre);
    int i = 0;
    while (fread(&x, sizeof(No_Codigo), 1, arq))
    {

        printf("\n %d - Codigo:  %d , esquerda: %d , direita: %d  \n", i, x.info, x.esquerda, x.direita);
        i++;
    }
}
void imprimir_arvore_binaria_na_notacao(FILE *arq, int pos)
{
    fseek(arq, sizeof(Cabecalho_Codigo) + pos * sizeof(No_Codigo), SEEK_SET);
    No_Codigo *no_aux = (No_Codigo *)malloc(sizeof(No_Codigo));
    fread(no_aux, sizeof(No_Codigo), 1, arq);

    printf("[");
    if (no_aux->info != -1)
    {
        printf("%d,", no_aux->info);

        if (no_aux->esquerda != -1)
        {
            imprimir_arvore_binaria_na_notacao(arq, no_aux->esquerda);
            printf(",");
        }
        else
        {
            printf("[],");
        }

        if (no_aux->direita != -1)
        {
            imprimir_arvore_binaria_na_notacao(arq, no_aux->direita);
            // printf("");
        }
        else
        {
            printf("[]");
        }
    }
    printf("]");
}

int excluir_codigo(FILE *arq, int pos, Codigo codigo){

    No_Codigo *no = (No_Codigo *)malloc(sizeof(No_Codigo));
    no = le_no_codigo(arq, pos);
   
    if (codigo > no->info){ //navega para direita
        no->direita = excluir_codigo(arq, no->direita, codigo);
        printf(" codigo %d ,direita %d\n",no->info,no->direita);
        return no->direita;
    }

    else if (codigo < no->info){ //navega para esquerda
        printf(" codigo %d ,esquerda %d\n",no->info,no->esquerda);
        

        no->esquerda = excluir_codigo(arq, no->esquerda, codigo);
        
        escreve_no_codigo(arq,no,pos);
        return no->esquerda;
    }
    else{ // encontrou
        if (no->esquerda == -1 && no->direita == -1){ // no folha

            no->esquerda = -1;
            no->pos_livro = -1;
            no->direita = -1;
            Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
            cab = le_cabecalho_codigos(arq);

            no->info = cab->pos_livre;
            cab->pos_livre = pos;
            escreve_cabecalho_codigo(arq, cab);
            escreve_no_codigo(arq, no, pos);

            return -1;
        }
        if (no->esquerda == -1){ //somente filho a direita
            no->info = minimo_codigo(arq, no->direita);
            no->direita = excluir_codigo(arq, no->direita, no->info);
            return no->direita;
        }
        else{ // dois filhos ou 1 a esquerda
            no->info = maximo_codigo(arq, no->esquerda);
            no->esquerda = excluir_codigo(arq, no->esquerda, no->info);
            return no->esquerda;
        }
    }
}

int main()
{
    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    int info;
    FILE *teste;
    teste = fopen("bdcodigos.bin", "wb");
    cria_arvore_vazia_codigo(teste);
    fclose(teste);
    printf("-------------------------------------------------------------\n");

    FILE *teste1;
    teste1 = fopen("bdcodigos.bin", "rb+");
    cab = le_cabecalho_codigos(teste1);
    printf("Raiz %d  Topo %d Livre %d\n", cab->pos_raiz, cab->pos_topo, cab->pos_livre);

    fclose(teste1);
    printf("-------------------------------------------------------------\n");

    FILE *teste2;
    teste2 = fopen("bdcodigos.bin", "rb+");

    insere(teste2, 11);

    insere(teste2, 5);

    insere(teste2, 2);

    insere(teste2, 8);

    insere(teste2, 14);

    insere(teste2, 22);

    insere(teste2, 15);

    insere(teste2, 27);

    insere(teste2, 25);
    fclose(teste2);

    printf("-------------------------------------------------------------\n");

    FILE *teste3;
    teste3 = fopen("bdcodigos.bin", "rb+");
    cab = le_cabecalho_codigos(teste3);
    //printf("Raiz %d  Topo %d Livre %d\n", cab->pos_raiz, cab->pos_topo, cab->pos_livre);
    imprimi_lista(teste3);
    /* imprimir_arvore_binaria_na_notacao(teste3, cab->pos_raiz); */
    printf("\n-------------------------------------------------------------\n");
    imprimir_arvore_binaria_na_notacao(teste3, cab->pos_raiz);
    int i;

    excluir_codigo(teste3, cab->pos_raiz, 11);

    //imprimi_lista(teste3);
    /* imprimir_arvore_binaria_na_notacao(teste3, cab->pos_raiz);
    deleta_o_codigo_na_arvore(teste3,5, cab->pos_raiz); */
    printf("\n-------------------------------------------------------------\n");
    imprimi_lista(teste3);
    imprimir_arvore_binaria_na_notacao(teste3, cab->pos_raiz);
    fclose(teste3);
    printf("\n-------------------------------------------------------------\n");
}
