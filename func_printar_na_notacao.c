void imprimir_arvore_binaria_na_notacao(Tree *t)
{
    printf("[");
    if (!treeIsEmpty(t))
    {

        printf("%d", t->num);
        printf(",");
        imprimir_arvore_binaria_na_notacao(t->sae);
        printf(",");
        imprimir_arvore_binaria_na_notacao(t->sad);
        printf("]");
    }
    else
    {
        printf("]");
    }
}
