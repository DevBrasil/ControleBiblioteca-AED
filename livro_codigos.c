#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "livros_dados.h"

//Estrutura de cabecalos de arquivo de arvore de codigos;
typedef struct Cabecalho_Codigo
{
    int pos_raiz;
    int pos_topo;
    int pos_livre;

} Cabecalho_Codigo;

//Estrutura para armazenar o codigo de um livro;
typedef int Codigo;

//Estrutura no para representar o livro na arvore de codigos;
typedef struct No_Codigo
{
    Codigo info;
    int pos_livro;
    int esquerda;
    int direita;
} No_Codigo;

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

No_Codigo *le_no_codigo(FILE *arq, int pos)
{
    No_Codigo *x = malloc(sizeof(No_Codigo));
    fseek(arq, sizeof(Cabecalho_Codigo) + pos * sizeof(No_Codigo), SEEK_SET);
    fread(x, sizeof(No_Codigo), 1, arq);
    return x;
}

Cabecalho_Codigo *le_cabecalho_codigos(FILE *arq)
{
    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    fseek(arq, 0, SEEK_SET); // posiciona no in ́ıcio do arquivo
    fread(cab, sizeof(Cabecalho_Codigo), 1, arq);
    return cab;
}

int acha_posicao_do_codigo(FILE *arq, int codigo, int pos)
{

    No_Codigo *no_aux = (No_Codigo *)malloc(sizeof(No_Codigo));
    no_aux = le_no_codigo(arq, pos);

    if (no_aux->info > codigo)
    {
        return acha_posicao_do_codigo(arq, codigo, no_aux->esquerda);
    }
    if (no_aux->info < codigo)
    {
        return acha_posicao_do_codigo(arq, codigo, no_aux->direita);
    }
    if (no_aux->info == codigo)
    {
        return pos;
    }
    return -1;
}

void adiciona_posicao_do_livro_no_codigo(FILE *arq, int posicao_livro, int codigo)
{
    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab = le_cabecalho_codigos(arq);

    int posicao_codigo;
    posicao_codigo = acha_posicao_do_codigo(arq, codigo, cab->pos_raiz);

    No_Codigo *no_aux = (No_Codigo *)malloc(sizeof(No_Codigo));
    no_aux = le_no_codigo(arq, posicao_codigo);

    no_aux->pos_livro = posicao_livro;
    escreve_no_codigo(arq, no_aux, posicao_codigo);
}

int posicao_do_livro(FILE *arq, int pos, int codigo)
{
    No_Codigo *no_aux = (No_Codigo *)malloc(sizeof(No_Codigo));
    no_aux = le_no_codigo(arq, pos);

    if (codigo > no_aux->info)
    {
        return posicao_do_livro(arq, no_aux->direita, codigo);
    }
    else if (codigo < no_aux->info)
    {
        return posicao_do_livro(arq, no_aux->esquerda, codigo);
    }
    else
    {
        return no_aux->pos_livro;
    }
}

int adiciona_codigo_no_bd_codigos(FILE *arq, Codigo info, int pos)
{

    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab = le_cabecalho_codigos(arq);

    No_Codigo *no_aux = (No_Codigo *)malloc(sizeof(No_Codigo));
    no_aux = le_no_codigo(arq, pos);

    No_Codigo *novo = (No_Codigo *)malloc(sizeof(No_Codigo));
    novo->direita = -1;
    novo->esquerda = -1;
    novo->info = info;

    if (no_aux->info < info)
    { //adição a direita
        if (no_aux->direita != -1)
        { //ele ja tem filho da direita

            adiciona_codigo_no_bd_codigos(arq, novo->info, no_aux->direita);
        }
        else
        { //nao tem filho a direita
            if (cab->pos_livre != -1)
            {                                                 //adicionar na posicao livre
                escreve_no_codigo(arq, novo, cab->pos_livre); // escreve o no na posicao livre

                No_Codigo *aux = (No_Codigo *)malloc(sizeof(No_Codigo)); // le a posicao livre
                aux = le_no_codigo(arq, cab->pos_livre);

                no_aux->direita = cab->pos_livre; //color o novo membro como filho da direita
                cab->pos_livre = aux->info;       //encadear a lista de nos livres

                escreve_no_codigo(arq, no_aux, pos);
                escreve_cabecalho_codigo(arq, cab);
            }
            else
            {                                                //nao tem posicao livre
                escreve_no_codigo(arq, novo, cab->pos_topo); // escreve o no na posicao topo

                no_aux->direita = cab->pos_topo; //color o novo membro como filho da direita
                cab->pos_topo++;                 //atualizar o topo da arvore

                escreve_no_codigo(arq, no_aux, pos);
                escreve_cabecalho_codigo(arq, cab);
            }
        }
    }
    else if (no_aux->info > info)
    { //adição a esquerda
        if (no_aux->esquerda != -1)
        { //ele ja tem filho da esquerda

            adiciona_codigo_no_bd_codigos(arq, novo->info, no_aux->esquerda);
        }
        else
        { //nao tem filho a direita
            if (cab->pos_livre != -1)
            {                                                 //adicionar na posicao livre
                escreve_no_codigo(arq, novo, cab->pos_livre); // escreve o no na posicao livre

                No_Codigo *aux = (No_Codigo *)malloc(sizeof(No_Codigo)); // le a posicao livre
                aux = le_no_codigo(arq, cab->pos_livre);

                no_aux->esquerda = cab->pos_livre; //color o novo membro como filho da direita
                cab->pos_livre = aux->info;        //encadear a lista de nos livres

                escreve_no_codigo(arq, no_aux, pos);
                escreve_cabecalho_codigo(arq, cab);
            }
            else
            {                                                //nao tem posicao livre
                escreve_no_codigo(arq, novo, cab->pos_topo); // escreve o no na posicao topo

                no_aux->esquerda = cab->pos_topo; //color o novo membro como filho da direita
                cab->pos_topo++;                  //atualizar o topo da arvore

                escreve_no_codigo(arq, no_aux, pos);
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

int insere_codigo(FILE *arq, Codigo info)
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

void imprimi_lista_codigo(FILE *arq)
{
    No_Codigo x;
    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab = le_cabecalho_codigos(arq);

    printf("Raiz %d  Topo %d Livre %d\n", cab->pos_raiz, cab->pos_topo, cab->pos_livre);
    int i = 0;
    while (fread(&x, sizeof(No_Codigo), 1, arq))
    {

        printf("\n[%d] - Codigo:  %d , esquerda: %d , direita: %d , pos_livro: %d \n", i, x.info, x.esquerda, x.direita, x.pos_livro);
        i++;
    }

    printf("\n");
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

int excluir_codigo(FILE *arq, int pos, Codigo codigo)
{

    No_Codigo *no = (No_Codigo *)malloc(sizeof(No_Codigo));
    no = le_no_codigo(arq, pos);

    printf("CODIGO = %d - ", no->info);

    if (codigo > no->info)
    { //navega para direita
        printf("vai para direita \n");
        no->direita = excluir_codigo(arq, no->direita, codigo);
        escreve_no_codigo(arq, no, pos);
        printf("CODIGO = %d agora tem dir como %d \n", no->info, no->direita);
    }

    else if (codigo < no->info)
    { //navega para esquerda
        printf("vai para esquerda \n");
        no->esquerda = excluir_codigo(arq, no->esquerda, codigo);
        escreve_no_codigo(arq, no, pos);
        printf("CODIGO = %d agora tem esq como %d \n", no->info, no->esquerda);
    }
    else if (codigo == no->info)
    { // encontrou
        printf("excluir \n");

        if (no->esquerda == -1 && no->direita == -1)
        { // no folha

            printf("folha \n");

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
        if (no->esquerda == -1)
        { //somente filho a direita
            printf("so tem filho pra direita \n");
            no->info = minimo_codigo(arq, no->direita);
            printf("teste %d \n", no->info);
            no->direita = excluir_codigo(arq, no->direita, no->info);
            escreve_no_codigo(arq, no, pos);
            return pos;
        }
        else
        { // dois filhos ou 1 a esquerda
            no->info = maximo_codigo(arq, no->esquerda);
            printf("VAI EXCLUIR A BOLITA %d\n", no->info);
            no->esquerda = excluir_codigo(arq, no->esquerda, no->info);
            escreve_no_codigo(arq, no, pos);

            return pos;
        }
    }

    return pos;
}

void printa_no(FILE *arq, int pos)
{
    No_Codigo *no_aux = (No_Codigo *)malloc(sizeof(No_Codigo));
    no_aux = le_no_codigo(arq, pos);

    printf(" %d ", no_aux->info);
}

void printa_nivel(FILE *arq, int pos, int nivel, int final)
{

    No_Codigo *no_aux = (No_Codigo *)malloc(sizeof(No_Codigo));
    no_aux = le_no_codigo(arq, pos);

    if (nivel == final)
    {
        printf("%d ", no_aux->info);
    }

    nivel++;

    if (nivel <= final)
    {
        if (no_aux->esquerda != -1 && no_aux->direita != -1)
        { //2 filhos
            printa_nivel(arq, no_aux->esquerda, nivel, final);
            printa_nivel(arq, no_aux->direita, nivel, final);
        }
        else if (no_aux->esquerda != -1)
        { //1 filho a esquerda
            printa_nivel(arq, no_aux->esquerda, nivel, final);
        }
        else if (no_aux->direita != -1)
        { //1 filho a direita
            printa_nivel(arq, no_aux->direita, nivel, final);
        }
    }
}

void printa_arvore_por_nivel()
{
    FILE *arquivo_codigo;
    arquivo_codigo = fopen("bdcodigos.bin", "rb+");

    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab = le_cabecalho_codigos(arquivo_codigo);

    if (cab->pos_raiz == -1) //nao tem nenhum codigo cadastrado
    {
        printf("Nao existem livros cadastrados!\n");
    }
    else
    {
        for (int i = 0; i < 999; i++)
        {
            printa_nivel(arquivo_codigo, cab->pos_raiz, 1, i);
        }
    }

    fclose(arquivo_codigo);
}

int existe_codigo(FILE *arq, int codigo, int pos)
{
    if (pos == -1)
        return 0;
    No_Codigo *no = (No_Codigo *)malloc(sizeof(No_Codigo));
    no = le_no_codigo(arq, pos);

    if (codigo > no->info && no->direita != -1)
    { //navega para direita
        return existe_codigo(arq, codigo, no->direita);
    }

    else if (codigo < no->info && no->esquerda != -1)
    { //navega para esquerda
        return existe_codigo(arq, codigo, no->esquerda);
    }
    else if (codigo == no->info)
    {
        return 1;
    }

    return 0;
}
