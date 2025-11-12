#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// Estrutura da peça do Tetris
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca pecas[MAX_FILA];
    int inicio;
    int fim;
    int tamanho;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca pecas[MAX_PILHA];
    int topo;
} Pilha;

// Protótipos das funções
void inicializarFila(Fila *fila);
int filaVazia(Fila *fila);
int filaCheia(Fila *fila);
void enfileirar(Fila *fila, Peca p);
Peca desenfileirar(Fila *fila);
void exibirFila(Fila *fila);

void inicializarPilha(Pilha *pilha);
int pilhaVazia(Pilha *pilha);
int pilhaCheia(Pilha *pilha);
void empilhar(Pilha *pilha, Peca p);
Peca desempilhar(Pilha *pilha);
void exibirPilha(Pilha *pilha);

Peca gerarPeca();

void trocarPecaAtual(Fila *fila, Pilha *pilha);
void trocarMultiplas(Fila *fila, Pilha *pilha);

// Variável global de controle de ID
int contador_id = 0;

// Função principal
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    do {
        printf("\n=== Estado Atual ===\n");
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras peças da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\n>> Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca());
                } else {
                    printf("\nFila vazia!\n");
                }
                break;

            case 2: // Reservar
                if (pilhaCheia(&pilha)) {
                    printf("\nPilha cheia! Não é possível reservar.\n");
                } else if (filaVazia(&fila)) {
                    printf("\nFila vazia!\n");
                } else {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("\n>> Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                    enfileirar(&fila, gerarPeca());
                }
                break;

            case 3: // Usar peça reservada
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("\n>> Peça usada da reserva: [%c %d]\n", usada.nome, usada.id);
                    enfileirar(&fila, gerarPeca());
                } else {
                    printf("\nPilha vazia! Nenhuma peça reservada.\n");
                }
                break;

            case 4: // Trocar frente da fila com topo da pilha
                trocarPecaAtual(&fila, &pilha);
                break;

            case 5: // Troca múltipla
                trocarMultiplas(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}

// === Implementações ===

// ----- Fila -----
void inicializarFila(Fila *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->tamanho = 0;
}

int filaVazia(Fila *fila) {
    return fila->tamanho == 0;
}

int filaCheia(Fila *fila) {
    return fila->tamanho == MAX_FILA;
}

void enfileirar(Fila *fila, Peca p) {
    if (filaCheia(fila)) return;
    fila->pecas[fila->fim] = p;
    fila->fim = (fila->fim + 1) % MAX_FILA;
    fila->tamanho++;
}

Peca desenfileirar(Fila *fila) {
    Peca p;
    if (filaVazia(fila)) {
        p.nome = '?';
        p.id = -1;
        return p;
    }
    p = fila->pecas[fila->inicio];
    fila->inicio = (fila->inicio + 1) % MAX_FILA;
    fila->tamanho--;
    return p;
}

void exibirFila(Fila *fila) {
    printf("Fila de peças: ");
    if (filaVazia(fila)) {
        printf("(vazia)\n");
        return;
    }
    int i = fila->inicio;
    for (int c = 0; c < fila->tamanho; c++) {
        printf("[%c %d] ", fila->pecas[i].nome, fila->pecas[i].id);
        i = (i + 1) % MAX_FILA;
    }
    printf("\n");
}

// ----- Pilha -----
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

int pilhaCheia(Pilha *pilha) {
    return pilha->topo == MAX_PILHA - 1;
}

void empilhar(Pilha *pilha, Peca p) {
    if (pilhaCheia(pilha)) return;
    pilha->pecas[++pilha->topo] = p;
}

Peca desempilhar(Pilha *pilha) {
    Peca p;
    if (pilhaVazia(pilha)) {
        p.nome = '?';
        p.id = -1;
        return p;
    }
    p = pilha->pecas[pilha->topo--];
    return p;
}

void exibirPilha(Pilha *pilha) {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(pilha)) {
        printf("(vazia)\n");
        return;
    }
    for (int i = pilha->topo; i >= 0; i--) {
        printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
    }
    printf("\n");
}

// ----- Peça -----
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = contador_id++;
    return nova;
}

// ----- Trocas -----
void trocarPecaAtual(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\nFila vazia! Não há peça para trocar.\n");
        return;
    }
    if (pilhaVazia(pilha)) {
        printf("\nPilha vazia! Não há peça para trocar.\n");
        return;
    }

    // Índice da frente da fila
    int frente = fila->inicio;
    // Troca direta
    Peca temp = fila->pecas[frente];
    fila->pecas[frente] = pilha->pecas[pilha->topo];
    pilha->pecas[pilha->topo] = temp;

    printf("\n>> Trocada a peça da frente da fila [%c %d] com o topo da pilha [%c %d]\n",
           pilha->pecas[pilha->topo].nome, pilha->pecas[pilha->topo].id,
           fila->pecas[frente].nome, fila->pecas[frente].id);
}

void trocarMultiplas(Fila *fila, Pilha *pilha) {
    if (fila->tamanho < 3) {
        printf("\nFila não possui 3 peças para troca múltipla.\n");
        return;
    }
    if (pilha->topo + 1 < 3) {
        printf("\nPilha não possui 3 peças para troca múltipla.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idxFila = (fila->inicio + i) % MAX_FILA;
        int idxPilha = pilha->topo - i;
        Peca temp = fila->pecas[idxFila];
        fila->pecas[idxFila] = pilha->pecas[idxPilha];
        pilha->pecas[idxPilha] = temp;
    }

    printf("\n>> Trocadas as 3 primeiras peças da fila com as 3 da pilha!\n");
}