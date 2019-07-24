#ifndef LIVROS_DADOS   
#define LIVROS_DADOS

struct Dados_Livro;
struct Cabecalho_livros_dados;
struct No_livro;


void adiciona_livro(Dados_Livro);
void teste();
void imprimi_lista(FILE *arq);
void procura_no(FILE *arq, int codigo);
void retira(FILE *arq, int codigo);

#endif 
