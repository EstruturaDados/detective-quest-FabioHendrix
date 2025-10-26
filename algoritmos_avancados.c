// Inclusão das bibliotecas necessárias pro programa funcionar
#include <stdio.h>
#include <stdlib.h> // Para malloc() e free() (memória)
#include <string.h> // Para strcmp() (comparar strings)

// CONSTANTES E ESTRUTURAS DE DADOS
#define TAMANHO_HASH 10 // Define o tamanho da Tabela Hash (vetor principal)

// Struct da Sala (Árvore do Mapa)
struct Sala {
    char nome[50];
    char pista[100];
    struct Sala* esquerda;
    struct Sala* direita;
};

// Struct do Nó da Pista (Árvore das Pistas - BST)
struct NoPista {
    char pista[100];
    struct NoPista* esquerda;
    struct NoPista* direita;
};

// Struct do item da Tabela Hash (para guardar Pista -> Suspeito)
struct ItemHash {
    char pista[100];
    char suspeito[50];
    struct ItemHash* proximo; // Ponteiro para o próximo item, caso ocorra uma 'colisão' (lista encadeada)
};

// VARIÁVEIS GLOBAIS
// Tabela Hash principal
struct ItemHash* tabelaHash[TAMANHO_HASH];

// Funções 
// Funções da Mansão e Pistas
struct Sala* criarSala(const char* nome, const char* pista);
struct NoPista* inserirPista(struct NoPista* raiz, const char* pista);
void explorarSalas(struct Sala* salaInicial, struct NoPista** raizPistas);
void exibirPistas(struct NoPista* raiz);

// Novas Funções da Tabela Hash e Julgamento
unsigned int funcaoHash(const char* chave);
void inserirNaHash(const char* pista, const char* suspeito);
void preencherTabelaHash();
const char* encontrarSuspeito(const char* pista);
int contarPistasRecursivo(struct NoPista* raizPistas, const char* suspeitoAcusado);
void verificarSuspeitoFinal(struct NoPista* raizPistas);

// Funções de Limpeza de Memória
void liberarMapa(struct Sala* salaAtual);
void liberarPistas(struct NoPista* raiz);
void liberarHash();

// FUNÇÃO PRINCIPAL
int main() {
    // Inicializa a Tabela Hash
    // Garante que todos os 'baldes' (índices) da tabela comecem vazios (NULL)
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
    preencherTabelaHash(); // Preenche a Tabela Hash com as pistas e suspeitos do jogo

    // Monta o Mapa (Árvore da Mansão)
    struct Sala* raizMansao = criarSala("Hall de Entrada", "Um convite amassado no chao.");
    raizMansao->esquerda = criarSala("Sala de Estar", "Uma lareira com cinzas recentes.");
    raizMansao->direita = criarSala("Biblioteca", "Um livro sobre venenos fora do lugar.");
    raizMansao->esquerda->esquerda = criarSala("Cozinha", "Faca de chef faltando no cepo.");
    raizMansao->direita->direita = criarSala("Escritorio", "Um testamento rasgado na lixeira.");

    // A Árvore de Pistas (BST) começa vazia (vai ser preenchida pelo jogador)
    struct NoPista* raizPistas = NULL;

    // Inicia a exploração (loop do jogador)
    printf("Detective Quest: O Julgamento Final\n");
    explorarSalas(raizMansao, &raizPistas);

    // Inicia a fase de julgamento (depois que o jogador sai da exploração)
    verificarSuspeitoFinal(raizPistas);
    
    // Libera a memória de TODAS as três estruturas de dados
    liberarMapa(raizMansao);
    liberarPistas(raizPistas);
    liberarHash();
    printf("\nO caso está encerrado.\n");

    return 0;
}


// IMPLEMENTAÇÃO DAS FUNÇÕES DA TABELA HASH

// Calcula o índice (a 'gaveta') para uma pista (chave)
unsigned int funcaoHash(const char* chave) {
    unsigned int valorHash = 0;
    // Algoritmo simples para misturar os caracteres da string
    for (int i = 0; chave[i] != '\0'; i++) {
        valorHash = 31 * valorHash + chave[i];
    }
    // Usa o módulo (%) para garantir que o índice caiba no tamanho da tabela (0-9)
    return valorHash % TAMANHO_HASH;
}

// Insere a ligação Pista -> Suspeito na Tabela Hash.
// Usa 'Encadeamento Separado' para tratar colisões.
void inserirNaHash(const char* pista, const char* suspeito) {
    // Calcula onde a pista deve ficar
    unsigned int indice = funcaoHash(pista);
    // Aloca memória para o novo item
    struct ItemHash* novoItem = (struct ItemHash*) malloc(sizeof(struct ItemHash));
    // Preenche os dados
    strcpy(novoItem->pista, pista);
    strcpy(novoItem->suspeito, suspeito);
    
    // (Tratamento de Colisão) O novo item aponta para quem ja estava na gaveta
    novoItem->proximo = tabelaHash[indice]; 
    // A 'gaveta' (índice) agora aponta para o novo item (ele vira o primeiro da lista)
    tabelaHash[indice] = novoItem;
}

// Busca na Tabela Hash qual suspeito está ligado a uma pista.
const char* encontrarSuspeito(const char* pista) {
    // Calcula o índice onde a pista DEVERIA estar.
    unsigned int indice = funcaoHash(pista);
    // Pega o primeiro item dessa 'gaveta'.
    struct ItemHash* atual = tabelaHash[indice];
    
    // Percorre a lista encadeada (se houver colisão).
    while (atual != NULL) {
        // Se achou a pista exata, retorna o suspeito.
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    // Se não achou na lista, a pista não tem suspeito.
    return "Ninguem";
}

// Função que 'cadastra' as pistas e seus culpados no início do jogo.
void preencherTabelaHash() {
    inserirNaHash("Um convite amassado no chao.", "Mordomo");
    inserirNaHash("Uma lareira com cinzas recentes.", "Governanta");
    inserirNaHash("Um livro sobre venenos fora do lugar.", "Herdeira");
    inserirNaHash("Faca de chef faltando no cepo.", "Cozinheiro");
    inserirNaHash("Um testamento rasgado na lixeira.", "Herdeira"); // A Herdeira tem 2 pistas
}


// IMPLEMENTAÇÃO DAS FUNÇÕES DE JULGAMENTO

// Função que roda a lógica final de acusação.
void verificarSuspeitoFinal(struct NoPista* raizPistas) {
    printf("\n\n--- FASE DE JULGAMENTO ---\n");
    printf("Com base nas pistas que você coletou, é hora de fazer uma acusação.\n");
    
    printf("\nPistas em seu diário:\n");
    if(raizPistas == NULL) {
        printf("Você não coletou nenhuma pista. Impossível fazer uma acusação.\n");
        return;
    }
    // Mostra as pistas coletadas (em ordem alfabética, vindo da BST)
    exibirPistas(raizPistas);

    char acusado[50];
    printf("\nSuspeitos: Mordomo, Governanta, Herdeira, Cozinheiro\n");
    printf("Digite o nome do suspeito que você acusa: ");
    scanf("%49s", acusado);

    // Chama a função recursiva para contar quantas pistas (da BST) batem com o acusado.
    int contagem = contarPistasRecursivo(raizPistas, acusado);

    printf("\n--- O VEREDITO ---\n");
    printf("Você acusou %s. Após analisar as evidências...\n", acusado);
    printf("... %d pista(s) que você encontrou apontam para este suspeito.\n", contagem);

    // Lógica de vitória: 2 ou mais pistas.
    if (contagem >= 2) {
        printf("Evidências suficientes! %s confessou o crime! Caso resolvido!\n", acusado);
    } else {
        printf("Evidências insuficientes! O verdadeiro culpado escapou nas sombras...\n");
    }
}

// Função recursiva que analisa a arvore de pistas (BST)
// e conta quantas delas apontam para o acusado.
int contarPistasRecursivo(struct NoPista* raizPistas, const char* suspeitoAcusado) {
    // Caso Base (Parada): Se o nó é nulo, retorna 0.
    if (raizPistas == NULL) {
        return 0;
    }
    
    // Pega a pista do nó atual e procura na Tabela Hash quem é o suspeito (ex: "Cozinheiro").
    const char* suspeitoDaPista = encontrarSuspeito(raizPistas->pista);
    int contagemAtual = 0;
    
    // Compara: O suspeito da pista ("Cozinheiro") é igual ao que o jogador acusou?
    if (strcmp(suspeitoDaPista, suspeitoAcusado) == 0) {
        contagemAtual = 1; // Se sim, conta 1 para ESTE nó.
    }
    
    // Retorna o resultado deste nó (1 ou 0) somado com o resultado da busca na esquerda E na direita.
    return contagemAtual + 
           contarPistasRecursivo(raizPistas->esquerda, suspeitoAcusado) + 
           contarPistasRecursivo(raizPistas->direita, suspeitoAcusado);
}


// FUNÇÕES DOS NÍVEIS ANTERIORES

// Função para criar (alocar memória) uma nova sala
struct Sala* criarSala(const char* nome, const char* pista) {
    struct Sala* novaSala = (struct Sala*) malloc(sizeof(struct Sala));
    strcpy(novaSala->nome, nome);
    if (pista != NULL) strcpy(novaSala->pista, pista);
    else strcpy(novaSala->pista, ""); // Garante string vazia se a pista for NULL
    novaSala->esquerda = novaSala->direita = NULL;
    return novaSala;
}

// Função recursiva para inserir uma pista na Árvore de Busca (BST)
struct NoPista* inserirPista(struct NoPista* raiz, const char* pista) {
    // Caso Base (Parada): Achou um lugar vazio (NULL), cria o nó.
    if (raiz == NULL) {
        struct NoPista* novoNo = (struct NoPista*) malloc(sizeof(struct NoPista));
        strcpy(novoNo->pista, pista);
        novoNo->esquerda = novoNo->direita = NULL;
        return novoNo;
    }
    // Passo Recursivo: Compara alfabeticamente
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista); // Vai para a esquerda
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista); // Vai para a direita
    }
    return raiz; // Retorna a raiz (modificada ou não)
}

// Função de navegação (loop do jogo)
void explorarSalas(struct Sala* salaInicial, struct NoPista** raizPistas) {
    struct Sala* salaAtual = salaInicial;
    char escolha;
    while (1) {
        printf("\n----------------------------------\nVocê está em: %s\n", salaAtual->nome);
        // Se a sala tem uma pista (strlen > 0)
        if (strlen(salaAtual->pista) > 0) {
            printf(">> PISTA ENCONTRADA: \"%s\"\n", salaAtual->pista);
            *raizPistas = inserirPista(*raizPistas, salaAtual->pista); // Insere na BST
            strcpy(salaAtual->pista, ""); // Apaga a pista da sala
        }
        printf("\n(e) Esquerda | (d) Direita | (s) Sair para Julgamento\nEscolha: ");
        scanf(" %c", &escolha); // Espaço antes de %c
        if (escolha == 'e' && salaAtual->esquerda != NULL) salaAtual = salaAtual->esquerda;
        else if (escolha == 'd' && salaAtual->direita != NULL) salaAtual = salaAtual->direita;
        else if (escolha == 's') break; // Sai do loop de exploração
        else printf("Caminho inválido.\n");
    }
}

// Exibe as pistas em ordem alfabética (Percurso Em-Ordem: Esq, Raiz, Dir)
void exibirPistas(struct NoPista* raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esquerda);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

// Libera a memória da arvore do mapa (Pós-Ordem)
void liberarMapa(struct Sala* salaAtual) {
    if (salaAtual == NULL) return;
    liberarMapa(salaAtual->esquerda);
    liberarMapa(salaAtual->direita);
    free(salaAtual);
}

// Libera a memória da arvore de pistas (Pós-Ordem)
void liberarPistas(struct NoPista* raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// Função para limpar a memória da Tabela Hash (todas as listas encadeadas)
void liberarHash() {
    // Passa por cada  indice do vetor principal
    for (int i = 0; i < TAMANHO_HASH; i++) {
        struct ItemHash* atual = tabelaHash[i];
        // Percorre a lista encadeada (se houver) naquele indice
        while (atual != NULL) {
            struct ItemHash* temp = atual; // Guarda o nó atual
            atual = atual->proximo; // Avança o 'atual'
            free(temp); // Libera o nó que ficou para trás
        }
    }
}
