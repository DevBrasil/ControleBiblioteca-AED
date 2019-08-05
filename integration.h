#ifndef INTEGRATION
#define INTEGRATION

//Remove espaços desnecessarios nas frases
//Pre-condicao: Deve ser uma string
//Pos-condicao: nenhum
//Retorno: Frase sem espacos desnecessarios
char *remover_espacos_frase(char s[]);

//Insere um livro no arquivo de codigos e no de dados
//Pre-condicao: livro nao pode ser null e nao pode existir nos arquivo
//Pos-condicao: livro adicionado nos arquivos
void adiciona_livro(Dados_Livro livro);

//Insere um livro no arquivo de codigos e no de dados usando entradas por teclado
//Pre-condicao: nenhuma
//Pos-condicao: livro adicionado nos arquivos
void adiciona_livro_entradas();

//Excluir um livro no arquivo de codigos e no de dados usando entradas por teclado
//Pre-condicao: nenhuma
//Pos-condicao: livro adicionado nos arquivos
void exclui_livro();

//Imprime na telas a lista de livros ordenados por codigo
//Pre-condicao: nenhuma
//Pos-condicao: livro adicionado nos arquivos
void imprime_em_ordem_de_codigo(FILE *arq, int pos);

//Imprime na telas a lista de livros ordenados por codigo
//Pre-condicao: nenhuma
//Pos-condicao: livro adicionado nos arquivos
void livros_ordenados_por_codigo();

//Atualiza o numero de exemplares de um livro
//Pre-condicao: nenhuma
//Pos-condicao: numero de exemplares de um livro atualizado
void atualizar_exmplares();

//Procura por um livro usando seu codigo
//Pre-condicao: nenhuma
//Pos-condicao: Mostra na tela as informacoes do livro
void buscar_dados_do_livro();

//INSERIR DADOS NO SISTEMA VIA ARQUIVO TXT
//ENTRADA: NENHUMA
//RETORNO: NENHUM
//PRE-CONDICAO: NENHUMA
//POS-CONDICAO: NEHUMA
void inserir_via_arquivo_txt();

#endif