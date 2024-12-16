#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int quantidade;
    char nome[50];
    char funcaoItem[50];
} Item;

typedef struct no {
    Item dado;
    struct no *prox;
    struct no *ant;
} No;

typedef struct {
    No *inicio;
    No *fim;
    int tam;
} Inventario;

void cria(Inventario *invent);
int insereItem(Inventario *invent, Item dado);
void mostra(Inventario *invent);
int retiraItem(Inventario *invent, int id);
int contarItensInventario(Inventario *invent);
int salvaInventario(Inventario *invent, char *filename);
Item cadastraItem();
void opcoesItem();

void cria(Inventario *invent) {
    invent->inicio = NULL;
    invent->fim = NULL;
    invent->tam = 0;
}

int insereItem(Inventario *invent, Item dado) {
    No *aux = (No *)malloc(sizeof(No));
    if (aux == NULL) {
        printf("Erro na alocação de memória\n");
        return 0;
    }

    aux->dado = dado;
    aux->prox = NULL;
    aux->ant = NULL;

    if (invent->inicio == NULL) {
        invent->inicio = aux;
        invent->fim = aux;
    } else {
        invent->fim->prox = aux;
        aux->ant = invent->fim;
        invent->fim = aux;
    }

    invent->tam++;
    return 1;
}

void mostra(Inventario *invent) {
    if (invent->inicio == NULL) {
        printf(" INVENTARIO VAZIO \n");
        return;
    }

    No *aux = invent->inicio;
    while (aux != NULL) {
        printf("ID: %d\n", aux->dado.id);
        printf("Nome: %s\n", aux->dado.nome);
        printf("Funcao: %s\n", aux->dado.funcaoItem);
        printf("Quantidade: %d\n", aux->dado.quantidade);
        aux = aux->prox;
    }
}

int retiraItem(Inventario *invent, int id) {
    if (invent->inicio == NULL) {
        printf(" INVENTARIO VAZIO \n");
        return 0;
    }

    No *aux = invent->inicio;
    while (aux != NULL && aux->dado.id != id) {
        aux = aux->prox;
    }

    if (aux == NULL) {
        printf("Item não encontrado.\n");
        return 0;
    }

    if (aux->ant != NULL) {
        aux->ant->prox = aux->prox;
    }
    if (aux->prox != NULL) {
        aux->prox->ant = aux->ant;
    }

    if (aux == invent->inicio) {
        invent->inicio = aux->prox;
    }
    if (aux == invent->fim) {
        invent->fim = aux->ant;
    }

    free(aux);
    invent->tam--;
    return 1;
}

int contarItensInventario(Inventario *invent) {
    int tamItens = 0;
    No *aux = invent->inicio;

    while (aux != NULL) {
        tamItens++;  // Incrementa o contador a cada item encontrado
        aux = aux->prox;  // Avança para o próximo item
    }

    return tamItens;  // Retorna a quantidade de itens
}

int salvaInventario(Inventario *invent, char *filename) {
    // Verifica se o inventário está vazio antes de abrir o arquivo
    if (invent->inicio == NULL) {
        printf("Inventario vazio, nada para salvar.\n");
        return 0; // Sem dados para salvar
    }

    FILE *f = fopen(filename, "a");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo de inventario.\n");
        return 0;
    }

    // Itera sobre os itens do inventario e escreve no arquivo
    No *aux = invent->inicio;
    while (aux != NULL) {
        fprintf(f, "%d %s %s %d\n", aux->dado.id, aux->dado.nome, aux->dado.funcaoItem, aux->dado.quantidade);
        aux = aux->prox;
    }

    fclose(f); // Fecha o arquivo ao final
    return 1; // Sucesso
}

Item cadastraItem() {
    Item i;

    printf("\n>>ID: ");
    scanf("%d", &i.id);
    getchar();
    printf(">>Nome do item: ");
    scanf("%s", i.nome);
    printf(">>Funcao do item: ");
    scanf("%s", i.funcaoItem);
    printf(">>Quantidade: ");
    scanf("%d", &i.quantidade);

    return i;
}

void opcoesItem() {
    printf("\n------------------ MENU DE INVENTARIO ----------------------\n");
    printf("                Opcao 1: Adicionar item;\n");
    printf("                Opcao 2: Mostrar inventario;\n");
    printf("                Opcao 3: Retirar item;\n");
    printf("                Opcao 4: Fechar inventário.\n");
    printf("------------------------------------------------------------\n");
}
