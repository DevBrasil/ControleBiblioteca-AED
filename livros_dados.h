#ifndef LIVROS_DADOS
#define LIVROS_DADOS

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
} Cabecalho_livros_dados;

// estrutura de no para lista encadeada
typedef struct
{
    Dados_Livro livro;
    int prox;
} No_livro;

//Escreve no arquivo o cabe ̧calho contendo as informacoes da lista
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: cabe ̧calho escrito no arquivo
void escreve_cabecalho_livro(FILE *arq, Cabecalho_livros_dados *cab);

//Cria uma lista nova em arquivo
//Pre-condicao: arquivo aberto para leitura/escrita
//Pos-condicao: arquivo  ́e inicializado com uma lista vazia
void cria_lista_vazia(FILE *arq);

//L^e o cabe ̧calho do arquivo contendo as informacoes da lista
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: retorna o ponteiro para o cabecalho lido
Cabecalho_livros_dados *le_cabecalho_livro(FILE *arq);

//l^e um n ́o em uma determinada posicao do arquivo
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
// pos deve ser uma posicao valida da lista
//Pos-condicao: ponteiro para no lido  ́e retornado
No_livro *le_no_livro(FILE *arq, int pos);

//Insere os dados do livro no arquivo
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de dados
//Pos-condicao: dados inseridos no arquivo.
//Retorno: Posicao do livro do arquivo binario.
int insere_livro(FILE *arq, Dados_Livro livro);

//Mostra dados de um livro enviando sua posicao
//Pre-condicao: nenhuma
//Pos-condicao: nenhuma
void imprime_livro(int pos);

//Escreve um no em uma determinada posicao do arquivo
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
// pos deve ser uma posicao valida do arquivo
//Pos-condicao: no escrito no arquivo
void escreve_livro_no(FILE *arq, No_livro *x, int pos);

//Retira um no da lista em uma posicao
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista , pos valido
//Pos-condicao: no retirado da lista
void retira(FILE *arq, int pos);

#endif
