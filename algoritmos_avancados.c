// Inclusão das bibliotecas necessárias pro programa funcionar
#include <stdio.h>
#include <stdlib.h> // Para malloc() e free() (memória)
#include <string.h> // Para strcpy() e strcmp() (strings)

// ESTRUTURAS DE DADOS

// Struct da Sala (Árvore do Mapa)
struct Sala {
    char nome[50];
    char pista[100]; // Campo para guardar a pista do cômodo
    struct Sala* esquerda;
    struct Sala* direita;
};

// Struct do Nó da Pista (Árvore das Pistas - BST)
struct NoPista {
    char pista[100];
    struct NoPista* esquerda;
    struct NoPista* direita;
};

// Funções (Protótipos) 
struct Sala* criarSala(const char* nome, const char* pista);
struct NoPista* inserirPista(struct NoPista* raiz, const char* pista);
void explorarSalasComPistas(struct Sala* salaInicial, struct NoPista** raizPistas);
void exibirPistas(struct NoPista* raiz);
void liberarMapa(struct Sala* salaAtual);
void liberarPistas(struct NoPista* raiz);

// FUNÇÃO PRINCIPAL
int main() {
    // Montagem do Mapa (Árvore da Mansão)
    // Criando a 'raiz' (Hall) e ligando os 'filhos' (outras salas)
    struct Sala* raizMansao = criarSala("Hall de Entrada", "Um convite amassado no chão.");
    raizMansao->esquerda = criarSala("Sala de Estar", "Uma lareira com cinzas recentes.");
    raizMansao->direita = criarSala("Biblioteca", "Um livro sobre venenos fora do lugar.");
    raizMansao->esquerda->esquerda = criarSala("Cozinha", "Faca de chef faltando no cepo.");
    raizMansao->esquerda->direita = criarSala("Sala de Jantar", NULL); // Sala sem pista
    raizMansao->direita->esquerda = criarSala("Jardim de Inverno", "Vaso de planta quebrado.");
    raizMansao->direita->direita = criarSala("Escritorio", "Um testamento rasgado na lixeira.");

    // A Árvore de Pistas (BST) começa vazia (NULL)
    struct NoPista* raizPistas = NULL;

    // Início da Exploração
    printf("Detective Quest: O Mistério das Pistas Ordenadas\n");
    
    // Chama a exploração.
    // Passa &raizPistas (ponteiro para ponteiro, ou **) para que a função 'explorar' possa mudar a 'raizPistas' original na main.
    explorarSalasComPistas(raizMansao, &raizPistas);

    // Exibição das Pistas Coletadas
    printf("\n\n--- DIÁRIO DO DETETIVE ---\n");
    printf("Pistas coletadas em ordem alfabética:\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        // A função 'exibirPistas' vai mostrar em ordem alfabética
        exibirPistas(raizPistas);
    }
    
    // Limpeza da Memória
    // Libera a memória das DUAS árvores
    liberarMapa(raizMansao);
    liberarPistas(raizPistas);
    printf("\nVocê se retira para analisar as pistas. O caso continua...\n");

    return 0;
}

// Função para criar (alocar memória) uma nova sala
// Recebe o nome e a pista (pista pode ser NULL)
struct Sala* criarSala(const char* nome, const char* pista) {
    struct Sala* novaSala = (struct Sala*) malloc(sizeof(struct Sala));
    strcpy(novaSala->nome, nome);
    if (pista != NULL) {
        strcpy(novaSala->pista, pista);
    } else {
        strcpy(novaSala->pista, ""); // Se a pista for NULL, coloca uma string vazia ""
    }
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função recursiva para inserir uma pista na Árvore de Busca (BST). A BST vai manter as pistas em ordem alfabética
struct NoPista* inserirPista(struct NoPista* raiz, const char* pista) {
    // Caso Base (Parada): Se a 'raiz' (ou sub-raiz) for NULL, achamos o lugar certo, Então Cria o nó.
    if (raiz == NULL) {
        struct NoPista* novoNo = (struct NoPista*) malloc(sizeof(struct NoPista));
        strcpy(novoNo->pista, pista);
        novoNo->esquerda = novoNo->direita = NULL;
        return novoNo;
    }

    // Passo Recursivo: Compara a pista nova com a pista do nó atual
    // (strcmp < 0 = vem antes; > 0 = vem depois)
    if (strcmp(pista, raiz->pista) < 0) {
        // Se a pista nova vem antes (alfabeticamente), vai para a esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        // Se a pista nova vem depois (alfabeticamente), vai para a direita
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se for igual (== 0), não faz nada (evita duplicatas)

    return raiz;
}

// Função de navegação (loop do jogo)
// Recebe a raiz do mapa e o endereço da raiz das pistas 
void explorarSalasComPistas(struct Sala* salaInicial, struct NoPista** raizPistas) {
    struct Sala* salaAtual = salaInicial;
    char escolha;

    while (1) {
        printf("\n----------------------------------\n");
        printf("Você está em: %s\n", salaAtual->nome);

        // Lógica de coleta de pista
        // Se a sala atual tem uma pista (strlen > 0)
        if (strlen(salaAtual->pista) > 0) {
            printf(">> PISTA ENCONTRADA: \"%s\"\n", salaAtual->pista);
            // Insere a pista na BST. *raizPistas é o valor do ponteiro (a raiz atual)
            *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
            // Apaga a pista da sala (coloca "") para não coletar de novo
            strcpy(salaAtual->pista, ""); 
        }

        printf("\nPara onde você quer ir?\n");
        if (salaAtual->esquerda != NULL) printf(" (e) Esquerda -> %s\n", salaAtual->esquerda->nome);
        if (salaAtual->direita != NULL) printf(" (d) Direita  -> %s\n", salaAtual->direita->nome);
        printf(" (s) Sair para analisar pistas\n");
        printf("Sua escolha: ");
        
        scanf(" %c", &escolha); // Espaço antes do %c para limpar o buffer

        if (escolha == 'e' && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda; // Navega para a esquerda
        } else if (escolha == 'd' && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita; // Navega para a direita
        } else if (escolha == 's') {
            break; // Sai do loop
        } else {
            printf("Caminho inválido ou comando desconhecido.\n");
        }
    }
}

// Exibe as pistas em ordem alfabética
// Usa o percurso "Em-Ordem" (In-Order Traversal): Esquerda, Raiz, Direita.
void exibirPistas(struct NoPista* raiz) {
    // Caso Base (Parada): Se o nó for NULL, só retorna.
    if (raiz == NULL) {
        return;
    }
    // Chama recursivo para a ESQUERDA (vai até o fim da esquerda)
    exibirPistas(raiz->esquerda);
    // Imprime o nó ATUAL (o do meio)
    printf(" - %s\n", raiz->pista);
    // Chama recursivo para a DIREITA (vai para a direita)
    exibirPistas(raiz->direita);
}

// Funções para Liberar Memória
// Função recursiva para liberar a memória da arvore do mapa (Pós-Ordem)
void liberarMapa(struct Sala* salaAtual) {
    if (salaAtual == NULL) return;
    liberarMapa(salaAtual->esquerda);
    liberarMapa(salaAtual->direita);
    free(salaAtual);
}

// Função recursiva para liberar a memória da arvore de pistas (Pós-Ordem)
void liberarPistas(struct NoPista* raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}
