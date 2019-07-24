#ifndef LIVRO_CODIGO   
#define LIVRO_CODIGO



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
void escreve_no_codigo(FILE *arq, No_Codigo *x, int pos);

void escreve_cabecalho_codigo(FILE *arq, Cabecalho_Codigo *cab);

void cria_arvore_vazia_codigo(FILE *arq);

//l^e um n ́o em uma determinada posi ̧c~ao do arquivo
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
// pos deve ser uma posi ̧c~ao v ́alida da lista
//P ́os-condi ̧c~ao: ponteiro para n ́o lido  ́e retornado
No_Codigo *le_no_codigo(FILE *arq, int pos);

//L^e o cabe ̧calho do arquivo contendo as informa ̧c~oes da lista
//Pr ́e-condi ̧c~ao: arquivo deve estar aberto e ser um arquivo de lista
//P ́os-condi ̧c~ao: retorna o ponteiro para o cabe ̧calho lido
Cabecalho_Codigo *le_cabecalho_codigos(FILE *arq);
int acha_posicao_do_codigo(FILE *arq, int codigo, int pos);

void adiciona_posicao_do_livro(FILE *arq, int posicao_livro, int codigo);


int adiciona_codigo_no_bd_codigos(FILE *arq, Codigo info, int pos);

int insere_codigo(FILE *arq, Codigo info);

int maximo_codigo(FILE *arq, int pos);

int minimo_codigo(FILE *arq, int pos);
void imprimi_lista(FILE *arq);
void imprimir_arvore_binaria_na_notacao(FILE *arq, int pos);

int excluir_codigo(FILE *arq, int pos, Codigo codigo);

void teste_codigos();
#endif 
