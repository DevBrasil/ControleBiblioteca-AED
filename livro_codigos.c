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

int adiciona_codigo_no_bd_codigos(FILE *arq, int pos, Codigo info)
{
    No_Codigo *aux = (No_Codigo *)malloc(sizeof(No_Codigo));

    if (pos == -1)
    { //Adicionar no
        aux->direita = -1;
        aux->esquerda = -1;
        aux->info = info;
        aux->pos_livro = -1;

        Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
        cab = le_cabecalho_codigos(arq);

        if (cab->pos_livre != -1)
        { //adicionar no na posicao livre
            No_Codigo *livre = (No_Codigo *)malloc(sizeof(No_Codigo));
            livre = le_no_codigo(arq, cab->pos_livre);

            //escrevendo a posicao do livro
            pos = cab->pos_livre;

            //escrevendo o livro na posicao livre
            escreve_no_codigo(arq, aux, cab->pos_livre);

            //arrumar lista de livros livres
            cab->pos_livre = livre->info;

            free(livre);
        }
        else
        {
            //escrevendo a posicao do livro
            pos = cab->pos_topo;

            escreve_no_codigo(arq, aux, cab->pos_topo);
            cab->pos_topo++;
        }

        escreve_cabecalho_codigo(arq, cab);
        //printf("raiz %d , topo %d , livre %d \n", cab->pos_raiz, cab->pos_topo, cab->pos_livre);
        free(cab);
    }
    else
    {
        aux = le_no_codigo(arq, pos);

        if (info > aux->info)
        { //adicionando codigo maior que o codigo atual;
            aux->direita = adiciona_codigo_no_bd_codigos(arq, aux->direita, info);
            escreve_no_codigo(arq, aux, pos);
        }
        else
        { //adicionando codigo menor que o codigo atual;
            aux->esquerda = adiciona_codigo_no_bd_codigos(arq, aux->esquerda, info);
            escreve_no_codigo(arq, aux, pos);
        }
    }

    free(aux);

    return pos;
}

void insere_codigo(int info)
{
    FILE *arq = fopen("bdcodigos.bin", "rb+");

    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab = le_cabecalho_codigos(arq);

    //apontando raiz
    int raiz;
    raiz = adiciona_codigo_no_bd_codigos(arq, cab->pos_raiz, info);

    cab = le_cabecalho_codigos(arq);
    cab->pos_raiz = raiz;

    escreve_cabecalho_codigo(arq, cab);

    fclose(arq);
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

int maximo_codigo_pos_livro(FILE *arq, int pos)
{
    No_Codigo *no_aux = (No_Codigo *)malloc(sizeof(No_Codigo));
    no_aux = le_no_codigo(arq, pos);

    if (no_aux->direita != -1)
    {
        return maximo_codigo(arq, no_aux->direita);
    }
    else
        return no_aux->pos_livro;
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
int minimo_codigo_pos_livro(FILE *arq, int pos)
{
    No_Codigo *no_aux = (No_Codigo *)malloc(sizeof(No_Codigo));
    no_aux = le_no_codigo(arq, pos);

    if (no_aux->esquerda != -1)
    {
        return minimo_codigo(arq, no_aux->esquerda);
    }
    else
        return no_aux->pos_livro;
}



void imprimi_lista_codigo()
{

    FILE *arq = fopen("bdcodigos.bin", "rb+");

    No_Codigo x;
    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab = le_cabecalho_codigos(arq);

   // printf("Raiz %d  Topo %d Livre %d\n", cab->pos_raiz, cab->pos_topo, cab->pos_livre);
    int i = 0;
    while (fread(&x, sizeof(No_Codigo), 1, arq))
    {

        printf("\n[%d] - Codigo:  %d , esquerda: %d , direita: %d , pos_livro: %d \n", i, x.info, x.esquerda, x.direita, x.pos_livro);
        i++;
    }

    printf("\n");

    fclose(arq);
}

void imprimir_arvore_binaria_na_notacao(FILE *arq, int pos)
{
    if (pos != -1)
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
}

int qtdLivros(FILE *arq, int pos)
{
    if (pos == -1)
    {
        return 0;
    }
    else
    {
        No_Codigo *aux = (No_Codigo *)malloc(sizeof(No_Codigo));
        aux = le_no_codigo(arq, pos);
        return qtdLivros(arq, aux->esquerda) + qtdLivros(arq, aux->direita) + 1;
    }
}

void imprime_tudo_notacao()
{
    FILE *arq = fopen("bdcodigos.bin", "rb+");
    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab = le_cabecalho_codigos(arq);
    imprimir_arvore_binaria_na_notacao(arq, cab->pos_raiz);
    printf("\n\n");
    fclose(arq);
}

int excluir_codigo(FILE *arq, int pos, Codigo codigo)
{

    No_Codigo *aux = (No_Codigo *)malloc(sizeof(No_Codigo));
    aux = le_no_codigo(arq, pos);

    if (codigo > aux->info)
    { //navega para direita
        aux->direita = excluir_codigo(arq, aux->direita, codigo);
        escreve_no_codigo(arq, aux, pos);
    }

    else if (codigo < aux->info)
    { //navega para esquerda
        aux->esquerda = excluir_codigo(arq, aux->esquerda, codigo);
        escreve_no_codigo(arq, aux, pos);
    }
    else
    { // encontrou

        if (aux->esquerda == -1 && aux->direita == -1)
        { //no folha
            Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
            cab = le_cabecalho_codigos(arq);

            //limpar livro;
            aux->esquerda = -1;
            aux->pos_livro = -2;
            aux->direita = -1;

            //Encadear lista de posicoes livres;
            aux->info = cab->pos_livre;
            cab->pos_livre = pos;

            //escrever informacoes no arquivo;
            escreve_cabecalho_codigo(arq, cab);
            escreve_no_codigo(arq, aux, pos);

            //retornar uma posicao nula;
            return -1;
        }
        if (aux->esquerda == -1)
        { //somente filho a direita
            aux->info = minimo_codigo(arq, aux->direita);
            aux->pos_livro = minimo_codigo_pos_livro(arq,aux->direita);
            aux->direita = excluir_codigo(arq, aux->direita, aux->info);
            escreve_no_codigo(arq, aux, pos);
        }
        else
        { // dois filhos ou 1 a esquerda
            aux->info = maximo_codigo(arq, aux->esquerda);
            aux->pos_livro = maximo_codigo_pos_livro(arq,aux->esquerda);
            aux->esquerda = excluir_codigo(arq, aux->esquerda, aux->info);
            escreve_no_codigo(arq, aux, pos);
        }
    }

    return pos;
}

void iniciar_exclusao_Codigo(int info)
{
    FILE *arq = fopen("bdcodigos.bin", "rb+");

    Cabecalho_Codigo *cab = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab = le_cabecalho_codigos(arq);

    int raiz;
    raiz = excluir_codigo(arq, cab->pos_raiz, info);

    cab = le_cabecalho_codigos(arq);
    cab->pos_raiz = raiz;

    escreve_cabecalho_codigo(arq, cab);
    free(cab);
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

    if (cab->pos_raiz != -1)
    {
        int qtd = qtdLivros(arquivo_codigo, cab->pos_raiz);
        for (int i = 0; i < qtd; i++)
        {
            printa_nivel(arquivo_codigo, cab->pos_raiz, 1, i);
            printf("\n");
        }
    }
    else
    {
        printf("Nao existem livros cadastrados\n");
    }

    fclose(arquivo_codigo);
}

int existe_codigo(FILE *arq, int codigo, int pos)
{
    if (pos == -1)
    {
        return 0;
    }

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

int mudar(FILE *arq, int pos, int i, Dados_Livro v[])
{
    if (pos == -1)
    {
        return 0;
    }
    else
    {
        No_Codigo *aux = (No_Codigo *)malloc(sizeof(No_Codigo));
        aux = le_no_codigo(arq, pos);

        FILE *x = fopen("bd.bin", "rb+");
        No_livro *a = le_no_livro(x, aux->pos_livro);
        fclose(x);

        i++;
        v[i] = a->livro;
        if(aux->esquerda != -1){
            i = mudar(arq, aux->esquerda, i, v);
        }
        
        if(aux->direita != -1){
            i = mudar(arq, aux->direita, i, v);
        }

    }

   
    return i;
}

int particiona(Dados_Livro V[], int inicio, int final)
{
    int esq, dir;
    esq = inicio;
    dir = final;

    Dados_Livro pivo = V[inicio];
    Dados_Livro aux;


    while (esq < dir)
    {
        while (strcmp(V[esq].titulo, pivo.titulo) <= 0)
            esq++;
        while (strcmp(V[dir].titulo, pivo.titulo) > 0)
            dir--;
        if (esq < dir)
        {
            aux = V[esq];
            V[esq] = V[dir];
            V[dir] = aux;
        }
    }

    V[inicio] = V[dir];
    V[dir] = pivo;

    return dir;
}

void QuickSort(Dados_Livro V[], int inicio, int fim)
{
    int pivo;
    if (fim > inicio)
    {
        pivo = particiona(V, inicio, fim);
        QuickSort(V, inicio, pivo - 1);
        QuickSort(V, pivo + 1, fim);
    }
}

void gerarListagemporTitulo()
{

    //Abertura de Arquivo e Contabilização do Vetor
    FILE *x = fopen("bdcodigos.bin", "rb+");
    Cabecalho_Codigo *cab2 = (Cabecalho_Codigo *)malloc(sizeof(Cabecalho_Codigo));
    cab2 = le_cabecalho_codigos(x);

    if (cab2->pos_raiz != -1)
    { //Tamanho do vetor definido pela função qtdLivros
        int n = qtdLivros(x, cab2->pos_raiz);

        //Alocando vetor dinâmicamente conforme o tanto de livros cadastrados no sistema
        Dados_Livro *v = (Dados_Livro *)malloc(n * sizeof(Dados_Livro));

        mudar(x, cab2->pos_raiz, -1, v);

        //Remover os comentários dessa sessão para imprimir no console os livros do sistema antes da ordenação
        //
        //
        //   for (int p = 0; p < n; p++)
        //   {
        //       printf("[%d] = %s - Codigo: %d\n", p, v[p].titulo, v[p].codigo);
        //   }
        //-----------------------------------------------------------------------------------------------------

        //Efetuando Quicksort no vetor
        QuickSort(v, 0, n-1);

        FILE *h = fopen("Catalago de Livros.txt", "w");

        //Escrevendo no arquivo os livros já ordenados
        for (int i = 0; i < n; i++)
        {
            fprintf(h, "Código: %d Possui: %d Unidade(s). Livro: %s escrito por: %s\n", v[i].codigo, v[i].exemplares, v[i].titulo, v[i].autor);
        }

        fclose(h);

        //Imprimindo no console os livros já ordenados
        for (int asdf = 0; asdf < n; asdf++)
        {
            printf("[%d] = %s - Codigo: %d\n", asdf, v[asdf].titulo, v[asdf].codigo);
        }
    }
    else
    {
        printf("Nenhum livro cadastrado\n");
    }

    fclose(x);
}
