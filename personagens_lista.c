//codigo principal (.c)
#include "personagens_functions.h"

int main()
{
    ListaPersonagens l;
    Personagens p;
    int n, id;

    FILE *g = fopen("personagens.txt", "r");
    if(g == NULL){
        printf("Erro ao abrir a lista de personagens(arquivo)!!\n");
        exit(1);
    }

    /*FILE *f = fopen(filename, "a");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo de inventario.\n");
        return 0;
    }*/

    criaPersonagens(&l);

    while(lePersonagens(g, &p) > 0){
        inserePersonagens(&l, p);}

    fclose(g);

    printf("PERSONAGENS");

    do{
        opcoesPersonagens();

        printf("\n>>O que deseja fazer? (use somente os numeros das opcoes): ");
        scanf("%d", &n);

        switch(n){
        case 1: //cadastra personagem
            p = cadastraPersonagens();
            inserePersonagens(&l, p);
            break;

        case 2: //mostra personagem
            exibePersonagens(l);
            break;

        case 3: //retira personagem
            printf("\nDigite o ID do Personagem a ser retirado: ");
            scanf("%d", &id);
            removePersonagens(&l, id);
            break;

        case 4: //modificar inventario
            printf("\nDigite o ID do Personagem escolhido: ");
            scanf("%d", &id);
            itemAcao(&l, "personagens.txt", id);
            break;

        case 5: //sair
            salvaPersonagens(&l, "personagens.txt");
            salvaInventario(&p.invent, "inventario.txt");
            exit(1);
            break;
        }
    }
    while(1);
    fclose(g); //LISTA PERSONAGENS
    //fclose(f); //INVENTARIO

    return 0;
}
