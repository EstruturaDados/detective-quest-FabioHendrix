// Inclusão das bibliotecas necessárias pro programa funcionar
#include <stdio.h>
#include <stdlib.h> // Para malloc() e free() (alocar e liberar memória)
#include <string.h> // Para strcpy() (copiar nomes)

// ESTRUTURA DE DADOS

// Struct da Sala
struct Sala {
    char nome[50];
    struct Sala* esquerda; // Ponteiro para a sala da esquerda (NULL se não houver)
    struct Sala* direita; // Ponteiro para a sala da direita (NULL se não houver)
};

// Funções 
struct Sala* criarSala(const char* nome);
void explorarSalas(struct Sala* salaInicial);
void liberarMapa(struct Sala* salaAtual); // Função para limpar a memória (recursiva)

// FUNÇÃO PRINCIPAL
int main() {
    // Montagem do Mapa da Mansão 
    struct Sala* raiz = criarSala("Hall de Entrada");
    struct Sala* salaDeEstar = criarSala("Sala de Estar");
    struct Sala* biblioteca = criarSala("Biblioteca");
    struct Sala* cozinha = criarSala("Cozinha");
    struct Sala* salaDeJantar = criarSala("Sala de Jantar");
    struct Sala* jardimDeInverno = criarSala("Jardim de Inverno");
    struct Sala* escritorio = criarSala("Escritorio");

    // Conectando os cômodos (ligando os ponteiros).
    raiz->esquerda = salaDeEstar;
    raiz->direita = biblioteca;

    // Da Sala de Estar, (esquerda) para Cozinha, (direita) para Sala de Jantar.
    salaDeEstar->esquerda = cozinha;
    salaDeEstar->direita = salaDeJantar;

    // Da Biblioteca, (esquerda) para Jardim, (direita) para Escritório.
    biblioteca->esquerda = jardimDeInverno;
    biblioteca->direita = escritorio;

    // Início da Exploração
    printf("Bem-vindo ao Detective Quest!\n");
    printf("Você está no Hall de Entrada. Comece a explorar a mansão.\n");

    // Chama a função que deixa o jogador andar pelo mapa (árvore).
    explorarSalas(raiz);

    // Limpeza da Memória
    // Depois que o jogador sai, a memória de todas as salas precisa ser liberada.
    // A função recursiva limpa tudo a partir da raiz.
    liberarMapa(raiz);
    printf("\nVocê saiu da mansão. O caso continua...\n");

    return 0;
}

// Função para criar (alocar memória) uma nova sala
// Recebe o nome e devolve um ponteiro para a sala pronta
struct Sala* criarSala(const char* nome) {
    // Pede memória do tamanho de uma struct Sala.
    struct Sala* novaSala = (struct Sala*) malloc(sizeof(struct Sala));
    
    // Copia o nome para dentro da struct.
    strcpy(novaSala->nome, nome);
    
    // Garante que as "portas" (ponteiros) começam fechadas (NULL).
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    // Retorna o endereço de onde está a sala nova.
    return novaSala;
}

// Função principal de navegação (loop do jogo)
// Recebe o ponteiro da sala onde o jogador começa
void explorarSalas(struct Sala* salaInicial) {
    struct Sala* salaAtual = salaInicial; // O ponteiro 'salaAtual' guarda onde o jogador está.
    char escolha;

    // Loop principal: continua até o jogador digitar 's' ou chegar num beco sem saída.
    while (1) {
        printf("\n----------------------------------\n");
        printf("Você está em: %s\n", salaAtual->nome);

        // Checa se é um "nó-folha" (sem caminhos para esquerda OU direita).
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Este cômodo não tem mais saídas. Fim da exploração neste caminho.\n");
            break; // Quebra o loop 'while (1)'.
        }

        // Mostra as opções de caminho (só se o caminho não for NULL).
        printf("Para onde você quer ir?\n");
        if (salaAtual->esquerda != NULL) {
            printf(" (e) Esquerda -> %s\n", salaAtual->esquerda->nome);
        }
        if (salaAtual->direita != NULL) {
            printf(" (d) Direita  -> %s\n", salaAtual->direita->nome);
        }
        printf(" (s) Sair da mansão\n");
        printf("Sua escolha: ");
        
        // O 'espaço' antes do %c é importante para ignorar o 'Enter' de loops passados.
        scanf(" %c", &escolha); 

        if (escolha == 'e') {
            if (salaAtual->esquerda != NULL) {
                // O jogador "anda": o ponteiro 'salaAtual' agora aponta para a sala da esquerda.
                salaAtual = salaAtual->esquerda; 
            } else {
                printf("Não há uma porta à esquerda aqui.\n");
            }
        } else if (escolha == 'd') {
            if (salaAtual->direita != NULL) {
                // O jogador "anda": o ponteiro 'salaAtual' agora aponta para a sala da direita.
                salaAtual = salaAtual->direita; 
            } else {
                printf("Não há uma porta à direita aqui.\n");
            }
        } else if (escolha == 's') {
            printf("Você decidiu parar a exploração por enquanto.\n");
            break; // Sai do loop 'while (1)'.
        } else {
            printf("Comando desconhecido, detetive. Tente novamente.\n");
        }
    }
}

// Função recursiva para liberar a memória da árvore (em Pós-Ordem)
// Pós-Ordem: (Esquerda, Direita, Raiz)
void liberarMapa(struct Sala* salaAtual) {
    // Condição de parada (base da recursão): se o nó for NULL, não faz nada.
    if (salaAtual == NULL) {
        return;
    }
    // Libera tudo que está à esquerda.
    liberarMapa(salaAtual->esquerda);
    // Libera tudo que está à direita.
    liberarMapa(salaAtual->direita);
    // Libera o nó atual (depois que os filhos já foram liberados).
    free(salaAtual);
}
