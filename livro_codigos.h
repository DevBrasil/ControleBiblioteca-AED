#ifndef LIVRO_CODIGO
#define LIVRO_CODIGO
#include <stdio.h>

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

//Escreve um no em uma determinada posi ̧c~ao do arquivo
//Precondicao: arquivo deve estar aberto e ser um arquivo de arvore
//pos deve ser uma posicao valida do arquivo
//Pos-condicao: no escrito no arquivo
void escreve_no_codigo(FILE *arq, No_Codigo *x, int pos);

//Escreve um cabecalho no arquivo
//Precondicao: arquivo deve estar aberto e ser um arquivo de arvore
//cab nao pode ser nulo
//Pos-condicao: cabecalho escrito no arquivo
void escreve_cabecalho_codigo(FILE *arq, Cabecalho_Codigo *cab);

//inicia um arquivo de codigo vazio
//Precondicao: arquivo deve estar aberto e ser um arquivo de arvore
//Pos-condicao: arquivo de codigo sem nada na arvore criado
void cria_arvore_vazia_codigo(FILE *arq);

//le um no em uma determinada posicao do arquivo
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//pos deve ser uma posicao v ́alida da lista
//Pos-condicao: ponteiro para no lido  ́e retornado
No_Codigo *le_no_codigo(FILE *arq, int pos);

//Le o cabecalho do arquivo contendo as informacoes da lista
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: retorna o ponteiro para o cabe ̧calho lido
Cabecalho_Codigo *le_cabecalho_codigos(FILE *arq);

//Percorre a arvore no arquivo e devolve a posicao em que se econtra o codigo
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: retorna a posicao do codigo no arquivo
int acha_posicao_do_codigo(FILE *arq, int codigo, int pos);

//Insere no codigo dentro da arvore a localizacao do livro no arquivo de dados
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: insere a posicao do livro no arquivo de codigos
void adiciona_posicao_do_livro_no_codigo(FILE *arq, int posicao_livro, int codigo);

//Percorre a arvore no arquivo e devolve a posicao em que se econtra o livro de tal codigo
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//o codigo deve existir
//Pos-condicao: retorna a posicao do livro no arquivo de dados
int posicao_do_livro(FILE *arq, int pos, int codigo);

//Insere um novo codigo na pos
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//pos deve ser uma posicao valida
//Pos-condicao: codigo inserido dentro da arvore de codigos na posicao
int adiciona_codigo_no_bd_codigos(FILE *arq, int pos, Codigo info);

//Insere um novo codigo dentro da arvore de codigos
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: codigo inserido dentro da arvore de codigos
void insere_codigo(int info);

//Procura pelo maior codigo da arvore apartir daquele no
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: retorna o maior codigo da arvore apartir daquele no
int maximo_codigo(FILE *arq, int pos);

//Procura pelo menor codigo da arvore apartir daquele no
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: retorna o menor codigo da arvore apartir daquele no
int minimo_codigo(FILE *arq, int pos);

//imprime a arvore inteira em forma de lista
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: imprime a arvore inteira em forma de lista
void imprimi_lista_codigo(FILE *arq);

//imprime a arvore inteira de forma binaria
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: imprime a arvore inteira e forma binaria
void imprimir_arvore_binaria_na_notacao(FILE *arq, int pos);

//imprime a arvore inteira de forma binaria
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: imprime a arvore inteira e forma binaria
void imprime_tudo_notacao();

//Excluir um codigo dentro da arvore em uma pos exata
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//pos deve ser uma posicao valida
//Pos-condicao: codigo excluido da arvore de codigos
int excluir_codigo(FILE *arq, int pos, Codigo codigo);

//Excluir um codigo dentro da arvore em uma pos exata
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//pos deve ser uma posicao valida
//Pos-condicao: codigo excluido da arvore de codigos
void iniciar_exclusao_Codigo(int info);

//Printa o codigo de uma determinada posicao na arvore
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: codigo mostrada na tela
void printa_no(FILE *arq, int pos);

//Printa os codigo de um nivel da arvore
//Pre-condicao: arquivo deve estar aberto e ser um arquivo de lista
//Pos-condicao: codigos de tal nivel mostrados na tela
void printa_nivel(FILE *arq, int pos, int nivel, int final);

//Percorre a árvore e devolve o total de livros presente na estrutura
//Pre-condição: arquivo deve estar aberto e pos deve ser diferente de -1
//Pos-condição: retorna a quantidade de livros
int qtdLivros(FILE *arq, int pos, int qtd);

//Printa a arvore de codigos por niveis
//Pre-condicao: nenhuma
//Pos-condicao: arvore printada no console por niveis
void printa_arvore_por_nivel();

//Verifica se um codigo existe no arquivo de codigo
//Precondicao: arquivo deve estar aberto e ser um arquivo de arvore
//Pos-condicao: retorna se o codigo existe na arvore
int existe_codigo(FILE *arq, int codigo, int pos);



//Gera um arquivo com toda as informações dos livros separados por ordem alfabética
//Pre-condição: arquivo deve estar aberto
//Pos-condição: nenhuma
void gerarListagemporTitulo();

#endif
