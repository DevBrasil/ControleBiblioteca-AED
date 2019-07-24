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
 typedef struct {} Cabecalho_livros_dados;

// estrutura de no para lista encadeada
typedef struct
{
    Dados_Livro livro;
    int prox;
} No_livro;

//Escreve no arquivo o cabe ̧calho contendo as informa ̧c~oes da lista
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
//P ́os-condi ̧c~ao: cabe ̧calho escrito no arquivo
void escreve_cabecalho_livro(FILE *arq, Cabecalho_livros_dados *cab);

//Cria uma lista nova em arquivo
//Pr ́e-condi ̧c~ao: arquivo aberto para leitura/escrita
//P ́os-condi ̧c~ao: arquivo  ́e inicializado com uma lista vazia
void cria_lista_vazia(FILE *arq);

//L^e o cabe ̧calho do arquivo contendo as informa ̧c~oes da lista
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
//P ́os-condi ̧c~ao: retorna o ponteiro para o cabe ̧calho lido
Cabecalho_livros_dados *le_cabecalho_livro(FILE *arq);

//l^e um n ́o em uma determinada posi ̧c~ao do arquivo
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
// pos deve ser uma posi ̧c~ao v ́alida da lista
//P ́os-condi ̧c~ao: ponteiro para n ́o lido  ́e retornado
No_livro *le_no_livro(FILE *arq, int pos);

//Escreve um n ́o em uma determinada posi ̧c~ao do arquivo
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
// pos deve ser uma posi ̧c~ao v ́alida do arquivo
//P ́os-condi ̧c~ao: n ́o escrito no arquivo
void escreve_livro_no(FILE *arq, No_livro *x, int pos);
void adiciona_livro();

//Retira um n ́o da lista
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
//P ́os-condi ̧c~ao: n ́o retirado da lista caso perten ̧ca a ela
void retira(FILE *arq, int codigo);

void procura_no(FILE *arq, int codigo);

void imprimi_lista_livro(FILE *arq);

void teste();

#endif 
