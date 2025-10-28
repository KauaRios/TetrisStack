#include <stdio.h>
#include <stdlib.h> // Para rand(), srand()
#include <string.h> // Para sprintf(), strcpy()
#include <time.h>   // Para time()

// --- 1. CONSTANTES DOS REQUISITOS ---

// Requisito: "Inicializar a fila... com 5 elementos"
#define TAMANHO_FILA 5 

// Requisito: "Inicializar uma pilha... com capacidade limitada (por exemplo, 3)."
#define CAPACIDADE_PILHA 3

// --- 2. ESTRUTURA DO ELEMENTO (PEÇA) ---
typedef struct {
    int id;
    char nome[20]; // Ex: "Peca_I"
} Elemento;


// --- 3. IMPLEMENTAÇÃO DA FILA CIRCULAR ---
typedef struct {
    Elemento itens[TAMANHO_FILA]; // Usa a constante correta
    int inicio;
    int fim;
    int total;
} Fila;

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int FilaCheia(Fila *f) {
    return f->total == TAMANHO_FILA; // Usa a constante correta
}

int FilaVazia(Fila *f) {
    return f->total == 0;
}

/**
 * Insere um elemento no fim da fila (circular).
 */
void Inserir(Fila *f, Elemento e) {
    if (FilaCheia(f)) {
        return; // Não faz nada se estiver cheia
    }
    f->itens[f->fim] = e;
    f->fim = (f->fim + 1) % TAMANHO_FILA; // Lógica circular
    f->total++;
}

/**
 * Retira um elemento do início da fila (circular).
 */
Elemento Retirar(Fila *f) {
    Elemento vazio = {-1, "Vazio"}; // Peça "fantasma" para erro
    if (FilaVazia(f)) {
        return vazio;
    }
    Elemento e = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % TAMANHO_FILA; // Lógica circular
    f->total--;
    return e;
}

// --- 4. IMPLEMENTAÇÃO DA PILHA (RESERVA) ---
typedef struct {
    Elemento itens[CAPACIDADE_PILHA]; // Usa a constante correta
    int topo;
} Pilha;

void inicializarPilha(Pilha *p) {
    p->topo = -1; // Pilha vazia
}

int PilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int PilhaCheia(Pilha *p) {
    // O topo é um índice (0, 1, 2), então está cheia se for == 2
    return p->topo == CAPACIDADE_PILHA - 1; // Usa a constante correta
}

/**
 * Insere um elemento no topo da pilha.
 */
void push(Pilha *p, Elemento e) {
    if (PilhaCheia(p)) {
        return; // Não faz nada se estiver cheia
    }
    p->topo++; // Incrementa o topo
    p->itens[p->topo] = e; // Insere o elemento
}

/**
 * Retira um elemento do topo da pilha.
 */
Elemento pop(Pilha *p) {
    Elemento vazio = {-1, "Vazio"};
    if (PilhaVazia(p)) {
        return vazio;
    }
    Elemento e = p->itens[p->topo];
    p->topo--; // Apenas decrementa o topo
    return e;
}

// --- 5. FUNÇÕES AUXILIARES ---

/**
 * Requisito: Gera peças automaticamente.
 * Esta função controla o ID sequencial e o tipo aleatório.
 */
Elemento gerarPecaAutomatica() {
    // 'static' faz a variável manter seu valor entre chamadas da função
    static int proximo_id = 1; 3
    
    char tipos[] = {'I', 'O', 'T', 'L'}; // Tipos de peça
    Elemento e;

    e.id = proximo_id++; // Usa o ID atual e DEPOIS incrementa para a próxima vez
    
    char tipoEscolhido = tipos[rand() % 4]; // Sorteia um índice de 0 a 3
    sprintf(e.nome, "Peca_%c", tipoEscolhido); // Formata o nome

    return e; // Retorna a peça criada
}

/**
 * Limpa o buffer de entrada (stdin) para evitar bugs do scanf.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- 6. FUNÇÕES DE EXIBIÇÃO ---

/**
 * Requisito: Exibe o estado atual da fila.
 */
void exibirFila(Fila *f) {
    printf("--- Fila de Pecas (Proxima -> Fim) ---\n");
    if (FilaVazia(f)) {
        printf("[Fila Vazia]\n");
    } else {
        // Itera na ordem correta (do início para o fim)
        for (int i = 0; i < f->total; i++) {
            int indice = (f->inicio + i) % TAMANHO_FILA;
            printf("[%s (ID:%d)] ", f->itens[indice].nome, f->itens[indice].id);
        }
        printf("\n");
    }
    printf("----------------------------------------\n");
}

/**
 * Requisito: Exibe o estado atual da pilha.
 */
void exibirPilha(Pilha *p) {
    printf("--- Pilha de Reserva (Topo -> Base) ---\n");
    if (PilhaVazia(p)) {
        printf("[Pilha Vazia]\n");
    } else {
        // Itera do topo para a base (LIFO)
        for (int i = p->topo; i >= 0; i--) {
            printf("[%s (ID:%d)]\n", p->itens[i].id, p->itens[i].nome);
        }
    }
    printf("----------------------------------------\n");
}

// --- 7. FUNÇÃO PRINCIPAL (MAIN) ---
int main() {
    // Inicializa o gerador aleatório (só precisa fazer isso uma vez)
    srand(time(NULL)); 

    // Requisito: Declara UMA fila e UMA pilha
    Fila fila;
    Pilha pilhaReserva;

    // Inicializa as estruturas
    inicializarFila(&fila);
    inicializarPilha(&pilhaReserva);

    // Requisito: "Inicializar a fila... com 5 elementos"
    for (int i = 0; i < TAMANHO_FILA; i++) {
        Elemento e = gerarPecaAutomatica(); // Chama a função que gera a peça
        Inserir(&fila, e);
    }

    int opcao;
    do {
        // Requisito: "Exibir o estado atual... após cada ação"
        // (Exibimos antes de pedir a próxima ação)
        exibirFila(&fila);
        exibirPilha(&pilhaReserva);

        printf("\n--- Opcoes de Acao ---\n");
        printf("1. Jogar peca (Remove da fila)\n");
        printf("2. Reservar peca (Fila -> Pilha)\n");
        printf("3. Usar peca reservada (Remove da Pilha)\n");
        printf("4. Atualizar/Exibir Estado\n"); // A OPÇÃO QUE FALTAVA
        printf("0. Sair\n"); // Opção de saída
        printf("Opcao: ");
        
        scanf("%d", &opcao);
        limparBuffer(); // Limpa o buffer após o scanf

        Elemento novaPeca; // Peça que será gerada

        switch (opcao) {
            case 1: // Jogar peça
                printf("\n[Acao: Jogar Peca]\n");
                Elemento jogada = Retirar(&fila);
                
                if (jogada.id == -1) {
                    printf("ERRO: Fila Vazia. Nao e possivel jogar.\n");
                } else {
                    printf("Peca Jogada: %s (ID: %d)\n", jogada.nome, jogada.id);
                    
                    // Requisito: "A cada ação, uma nova peça é gerada"
                    novaPeca = gerarPecaAutomatica();
                    Inserir(&fila, novaPeca);
                    printf("Nova peca (%s, ID: %d) entrou no fim da fila.\n", novaPeca.nome, novaPeca.id);
                }
                break;

            case 2: // Reservar peça
                printf("\n[Acao: Reservar Peca]\n");
                if (PilhaCheia(&pilhaReserva)) {
                    printf("ERRO: Pilha de reserva esta Cheia! Nao pode reservar.\n");
                } else {
                    Elemento reserva = Retirar(&fila);
                    if (reserva.id == -1) {
                        printf("ERRO: Fila Vazia. Nao e possivel reservar.\n");
                    } else {
                        push(&pilhaReserva, reserva); // Coloca na pilha
                        printf("Peca Reservada: %s (ID: %d)\n", reserva.nome, reserva.id);

                        // Requisito: "A cada ação, uma nova peça é gerada"
                        novaPeca = gerarPecaAutomatica();
                        Inserir(&fila, novaPeca);
                        printf("Nova peca (%s, ID: %d) entrou no fim da fila.\n", novaPeca.nome, novaPeca.id);
                    }
                }
                break;

            case 3: // Usar peça reservada
                printf("\n[Acao: Usar Peca Reservada]\n");
                Elemento usada = pop(&pilhaReserva);
                
                if (usada.id == -1) {
                    printf("ERRO: Pilha Vazia. Nao ha pecas para usar.\n");
                } else {
                    printf("Peca Usada da Reserva: %s (ID: %d)\n", usada.nome, usada.id);

                    // Requisito: "A cada ação, uma nova peça é gerada"
                    novaPeca = gerarPecaAutomatica();
                    Inserir(&fila, novaPeca);
                    printf("Nova peca (%s, ID: %d) entrou no fim da fila.\n", novaPeca.nome, novaPeca.id);
                }
                break;

            case 4: // Exibir
                // Apenas informa, pois o loop já exibe automaticamente no início
                printf("\n[Acao: Atualizando exibicao...]\n");
                break;
            
            case 0: // Sair
                printf("Saindo do jogo...\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
        
        if(opcao != 0) {
             printf("\n========================================\n");
        }

    } while (opcao != 0); // Loop continua enquanto a opção não for 0

    return 0; // Fim do programa
}