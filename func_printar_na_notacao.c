void imprimir_arvore_binaria_na_notacao(Tree *t)
{
    printf(",[");
    if (!treeIsEmpty(t))
    {

        printf("%d", t->num);
        imprimir_arvore_binaria_na_notacao(t->sae);
        imprimir_arvore_binaria_na_notacao(t->sad);
        printf("]");
    }
    else
    {
        printf("]");
    }
}
