//funçoes de personagens (.h)
#include "Inventario_functions.h"

typedef struct{
    int id;
    char nome[50];
    char classe[50];
    Inventario invent;
} Personagens;

typedef struct p{
    Personagens dado;
    struct p *prox;
    struct p *ant;
} NoPersonagens;

typedef struct{
    NoPersonagens *inicio;
    NoPersonagens *fim;
    int tam;
} ListaPersonagens;


void opcoesPersonagens();

//ARQUIVOS
Personagens cadastraPersonagens();
int lePersonagens(FILE *f, Personagens *p);
int salvaPersonagens(ListaPersonagens *l, char *filename);

//LISTAS
void criaPersonagens(ListaPersonagens *l);
int ListaVaziaPersonagens(ListaPersonagens l);

int inserePersonagens(ListaPersonagens *l, Personagens dado);
int removePersonagens(ListaPersonagens *l, int id);
void exibePersonagens(ListaPersonagens);
//int TamanhoPersonagens(ListaPersonagens l);
void itemAcao(ListaPersonagens *l,char *filename, int id);

void opcoesPersonagens()
{
    printf("\n------------------ MENU ----------------------\n");
    printf("    Opcao 1: Registrar novo personagem;\n");
    printf("    Opcao 2: Mostrar lista de personagens;\n");
    printf("    Opcao 3: Remover um Personagem da Lista;\n");
    printf("    Opcao 4: Modificar o inventario de um personagem;\n");
    printf("    Opcao 5: Fechar Programa.\n");
    printf("----------------------------------------------\n");
}

//ARQUIVOS
Personagens cadastraPersonagens()
{
    Personagens p;

    printf("\n-Digite as informacoes do seu Personagem: ");
    printf("\nId: ");
    scanf("%d", &p.id);
    getchar();
    printf("Nome: ");
    scanf("%s", p.nome);
    printf("Classe: ");
    scanf("%s", p.classe);

    cria(&p.invent);

    return p;
}

int lePersonagens(FILE *g, Personagens *p){
    return fscanf(g, "%d %s %s", &p->id, p->nome, p->classe);
}

int salvaPersonagens(ListaPersonagens *l, char *filename) {
    if (l == NULL) {
        printf("Erro: Lista não inicializada.\n");
        return 0;
    }
    if (l->inicio == NULL) {
        printf("Erro: Lista de personagens vazia.\n");
        return 0;
    }

    FILE *g = fopen(filename, "w");
    if (g == NULL) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    NoPersonagens *aux = l->inicio;
    int sucessoInventario = 0; // Variável para rastrear se algum inventário foi salvo

    while (aux != NULL) {
        // Verifique se os dados do personagem são válidos
        if (aux->dado.nome == NULL || aux->dado.classe == NULL) {
            printf("Erro: Dados inválidos no personagem!\n");
            aux = aux->prox;
            continue;
        }

        printf("\nSalvando: %s, Classe: %s\n", aux->dado.nome, aux->dado.classe);

        int n = fprintf(g, "%d %s %s\n", aux->dado.id, aux->dado.nome, aux->dado.classe);
        if (n < 0) {
            printf("Erro ao escrever no arquivo.\n");
        }

        // Salve o inventário do personagem
        if (!salvaInventario(&(aux->dado.invent), "inventario.txt")) {
            printf("Erro ao salvar inventario de %s.\n", aux->dado.nome);
        }
        else {
            sucessoInventario = 1; // Marca que pelo menos um inventário foi salvo
        }

        aux = aux->prox;
    }

    fclose(g);

    // Exibe mensagem de sucesso geral apenas se algum inventário foi salvo
    if (sucessoInventario) {
        printf("Inventario(s) salvo(s) com sucesso em 'inventario.txt'.\n");
    }

    return 1;
}


//LISTAS
void criaPersonagens(ListaPersonagens *l)
{
    l->inicio = NULL;
    l->fim = NULL;
    l->tam = 0;
}

int ListaVaziaPersonagens(ListaPersonagens l){
    if(l.inicio == NULL)
        return 1;

    return 0;
}

int inserePersonagens(ListaPersonagens *l, Personagens dado) {
    // Verifica se o ID já existe na lista
    NoPersonagens *temp = l->inicio;
    while (temp != NULL) {
        if (temp->dado.id == dado.id) {
            printf("Erro: O ID %d de personagem ja existe na lista!\n", dado.id);
            return 0; // ID duplicado
        }
        temp = temp->prox;
    }

    // Aloca memória para o novo nó
    NoPersonagens *aux = (NoPersonagens *)malloc(sizeof(NoPersonagens));
    if (aux == NULL) {
        printf("ERRO NA ALOCAÇÃO DE MEMÓRIA!!\n");
        return 0;
    }

    // Preenche o novo nó
    aux->dado = dado;
    aux->prox = NULL;
    aux->ant = NULL;

    // Caso 1: Lista vazia, insere como o único elemento
    if (l->inicio == NULL) {
        l->inicio = aux;
        l->fim = aux;
    } else {
        NoPersonagens *current = l->inicio;
        NoPersonagens *previous = NULL;

        // Caso 2: Inserção no início ou no meio da lista
        while (current != NULL && current->dado.id < dado.id) {
            previous = current;
            current = current->prox;
        }

        // Se o anterior for NULL, significa que o novo personagem tem o menor ID e vai para o início
        if (previous == NULL) {
            aux->prox = l->inicio;
            l->inicio->ant = aux;
            l->inicio = aux;
        } else {
            // Inserção no meio ou final
            previous->prox = aux;
            aux->ant = previous;

            if (current != NULL) {
                current->ant = aux;
                aux->prox = current;
            } else {
                // Caso inserido no final
                l->fim = aux;
            }
        }
    }

    // Atualiza o tamanho da lista
    l->tam++;
    return 1;
}


int removePersonagens(ListaPersonagens *l, int id){// retira ponto da lista pelo Id
    NoPersonagens *aux;

    if (l->inicio == NULL)
    {
        printf("LISTA VAZIA!!!\n");
        return 0;
    }

    // 1- Lista unitária
    if ((id == l->inicio->dado.id) && (l->inicio == l->fim))
    {
        aux = l->inicio;
        l->inicio = NULL;
        l->fim = NULL;
        free(aux);
        return 1;
    }

    // 2- Retira no inicio (primeiro item)
    if (id == l->inicio->dado.id)
    {
        aux = l->inicio;
        l->inicio = aux->prox;
        l->inicio->ant = NULL;
        free(aux);
        return 1;
    }

    // 3- Retira no fim
    if (id == l->fim->dado.id)
    {
        aux = l->fim;
        l->fim = aux->ant;
        l->fim->prox = NULL;
        free(aux);  // Corrigido: libera o nó correto (aux)
        return 1;
    }

    // 4- Retira no meio
    aux = l->inicio;
    while (aux != NULL && aux->dado.id != id)
    {
        aux = aux->prox;
    }

    // Verificação se o elemento foi encontrado
    if (aux == NULL) {
    printf("DADO NAO ENCONTRADO!!\n");
    return 0;
    }

    // Ajuste dos ponteiros de ant e prox
    if (aux->ant != NULL)
    {
        aux->ant->prox = aux->prox;
    }

    if (aux->prox != NULL)
    {
        aux->prox->ant = aux->ant;
    }

    free(aux);  // Libera o nó removido
    return 1;
}

void exibePersonagens(ListaPersonagens l){
    NoPersonagens *aux;
    /*if(aux == NULL){
        printf("Erro ao alocar memoria (EXIBE_PERSONAGENS)!!\n");
        exit(1);
    }*/

    if(l.inicio == NULL){
        printf("LISTA VAZIA!!\n");}

    else{
        printf("\nExibindo Personagens:\n\n");
        aux = l.inicio;
        while (aux != NULL){
            printf("Id: %d\n", aux->dado.id);
            printf("Nome: %s\n", aux->dado.nome);
            printf("Classe: %s\n", aux->dado.classe);
            printf("Inventario: %d itens\n\n", contarItensInventario(&(aux->dado.invent)));
            aux = aux->prox;
        }
    }
}


void itemAcao(ListaPersonagens *l, char *filename, int id  ) {
    FILE *f = fopen("inventario.txt", "r");
    if (f == NULL) {
        printf("Inventário não encontrado. Criando um novo...\n");
        f = fopen("inventario.txt", "w");
        if (f == NULL) {
            printf("Erro ao criar o inventário!!\n");
            exit(1);
        }
    }
    fclose(f);

    Personagens *p = NULL;
    NoPersonagens *temp = l->inicio;  // Começa no início da lista

    // Percorre a lista para encontrar o personagem com o ID correto
    while (temp != NULL) {
        if (temp->dado.id == id) {
            p = &(temp->dado);  // Personagem encontrado, atribui ao ponteiro p
            break;  // Sai do loop se o personagem for encontrado
        }
        temp = temp->prox;  // Avança para o próximo personagem na lista
    }

    // Se o personagem não for encontrado
    if (p == NULL) {
        printf("ID invalido!!!\n");
        return;  // Encerra a função caso o ID não seja encontrado
    }

    printf("\n\tPersonagem escolhido: %s\n", p->nome);

    int continuar = 1;
    int n;
    Item i;
    int idRetira;

    do {
        opcoesItem();

        printf("\n>>O que deseja fazer? (use somente os numeros das opcoes): ");
        if (scanf("%d", &n) != 1) {
            printf("Entrada invalida! Tente novamente.\n");
            while (getchar() != '\n'); // Limpa o buffer
            continue;
        }

        switch (n) {
        case 1:
            i = cadastraItem();
            insereItem(&p->invent, i);
            break;

        case 2:
            mostra(&p->invent);
            break;

        case 3:
            printf("\nDigite o ID do Item a ser retirado: ");
            if (scanf("%d", &idRetira) != 1) {
                printf("Entrada invalida! Tente novamente.\n");
                while (getchar() != '\n'); // Limpa o buffer
                continue;
            }
            retiraItem(&p->invent, idRetira);
            break;

        case 4:
            salvaPersonagens(l, filename);
            salvaInventario(&p->invent, "inventario.txt");
            continuar = 0; // Encerra o laço
            break;

        default:
            printf("Opçao invalida! Tente novamente.\n");
            break;
        }
    } while (continuar);
}
