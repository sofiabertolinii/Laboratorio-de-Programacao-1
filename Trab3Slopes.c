#include "janela.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAMANHO 10

// definindo as cores que vão ser utilizadas (tem que ser no mínimo 10)
cor_t branco = {1.0, 1.0, 1.0, 1.0};  
cor_t preto = {0.0, 0.0, 0.0, 1.0};  
cor_t vermelho = {1.0, 0.0, 0.0, 1.0};  
cor_t verde = {0.0, 1.0, 0.0, 1.0};  
cor_t azul = {0.0, 0.0, 1.0, 1.0}; 
cor_t amarelo = {1.0, 1.0, 0.0, 1.0};  
cor_t ciano = {0.0, 1.0, 1.0, 1.0};  
cor_t magenta = {1.0, 0.0, 1.0, 1.0}; 
cor_t laranja = {1.0, 0.5, 0.0, 1.0};  
cor_t cinza = {0.5, 0.5, 0.5, 1.0};  
cor_t transparente = {0, 0, 0, 0};

// estipular quais cores serão utilizadas nas fases, para saber qual o valor maximo possivel para cada fase
// a primeira fase só deve ter quatro cores (isso influencia no valor máximo)
// fase 1 vai da cor 0 até a cor 3, cada peça removida vale 1 ponto, passagem da primeira pra segunda vale 50 pontos
// fase 2 vai da cor 0 até a cor 4, cada peça removida vale 2 pontos, passagem da segunda pra terceira vale 100 pontos
// fase 3 vai da cor 0 até a cor 5, cada peça removida vale 3 pontos, passagem da terceira pra quarta vale 150 pontos
// fase 4 vai da cor 0 até a cor 6, cada peça removida vale 4 pontos, passagem da quarta pra quinta vale 200 pontos
// fase 5 vai da cor 0 até a cor 7, cada peça removida vale 5 pontos, passagem da quinta pra sexta vale 250 pontos

// NOTAS
// O NÚMERO 9 VAI REPRESENTAR UM ESPAÇO EM BRANCO (na matriz de jogo)!!!!!!!!!!!!!!!!!!!!!!!!
// a função de retorna linha e retorna coluna deve ser chamada a todo momento para que a posição esteja sempre atualizada

// DESCRIÇÃO DAS FUNÇÕES (em ordem):
// retorna_numero_aleatorio retorna um número entre 0 e o número digitado - 1
// inicializa_tabuleiro: coloca numeros aleatorios (no intervalo de cada fase na matriz) e posiciona o cursor
// retorna_posicao_cursor: retorna a linha e a coluna da posição co cursor na matriz do cursor por ponteiros
// verifica_espaço_vazio retorna 1 se aquela casa contém o número 9 (que representa um espaço vazio)
// verifica_PeloMenos_umaCasaOcupada_naColuna: retorna 1 se pelo menos uma casinha da coluna está ocupada
// verifica_PeloMenos_umaCasaOcupada_naLinha: retorna 1 se pelo menos uma casa da linha está ocupada
// verifica_coluna_completa: verifica se todas as casinhas daquela coluna tem a mesma cor (está completa e deve ser limpa)
// move_linha_pra_esquerda: função que move a linha pra esquerda
// move_linha_pra_direita: função que move a linha pra direita 
// roda_coluna_pra_direita: função que roda a última casa da coluna pra primeira casa da coluna à sua direita
// roda_coluna_pra esquerda: função que roda a última casa da coluna pra primeira casa da coluna _a sua esquerda
// elimina_linha: faz o jogador perder 2 pontos, elimina uma linha inteira
// coloca_peca_PrimeiraLinha: coloca uma peça em um lugar aleatório da primeira linha se ela estiver inteira vazia
// limpa_coluna_completa: itera sobre todas as colunas e se elas estiverem com todas as cores iguais são limpas
// faz_casinhas_cairem: deixa os espaços vazios no topo e faz as casinhas cairem para a base 
// pega_nome: função que coleta o apelido do usuário
// atualiza_cor: usa uma variável do estado do jogo pra organizar na saída do tabuleiro na tela
// desenha_pontuacao: imprime a pontuação na tela
// desenha_matriz: imprime o tabuleiro na tela
// desenha_cursor_linha: quando tá no modo linha, imprime as linhas em cima e embaixo
// desenha_cursor_coluna: quando tá no modo coluna, imprime as linhas dos lados
// desenha_jogo: reune as funções de desenho
// desenha_tempo: imprime o tempo na tela
// tela_entre_fases: a tela que aparece antes de uma fase se iniciar
// tela_de_instruções: a tela que aparece assim que o jogo abre com as instruções
// pega_tecla: usada no jogo principal, chama a função correspondente pra cada tecla
// reconhece_vitoria: reconhece quando o jogador venceu
// pega_enter: reconhece a tecla enter (é usada nas duas funções abaixo)
// perdeu_continua_jogando e ganhou_continua_jogando: telas que aparecem após ganhar ou perder
// roda_jogo: junta todas as funções
// ler_recordes: funçãoo que lê o arquivo de recordes (se ele existir) e aloca toda a memória necessária
// salvar_recordes: função que passa os recordes da memória pro arquivo
// inserir_recorde: função que insere um novo recorde na memória
// sistema de pontuação
// 01. a pontuação por peça e a pontuação mínima são definidas é definidas na inicializa_tabuleiro()
// 02. se o jogador decidir eliminar uma linha ele perde dois pontos
// 03. para que o jogador passe de fase, eledeve atingir a pontuação mínima dentro do tempo máximo

// estrutura de dados do jogo
typedef struct {
    int MatrizJogo[10][10]; // tabuleiro (matriz) onde as informações do jogo serão armazenadas
    int MatrizCursor[10][10]; // tabuleiro (matriz) onde a posição do cursor será armazenada
    int etapa; // em qual fase o jogo está (vai até a fase 6)
    int numCores; // número de cores de cada etapa
    char apelido[100]; //nome que o usuario digitou (pra guardar o recorde)
    int linha_cursor; // atualiza a posição do cursor por ponteiros
    int coluna_cursor; // atualiza a posição do cursor por ponteiros
    int qtd_pontos_usuario; // quantos pontos o usuario fez
    int pontos_por_peca; // quantos pontos retirar uma peça vale em uma etapa
    int qtd_minima_pontos; // qual é a quantidade minima de pontos em uma etapa pra passar dela
    cor_t cor; // cor que vai ser impressa de acordo com o numero da matriz TIRAR
    int modo_linha_coluna; // define se o usuario esta no modo linha (0) ou no modo coluna (1)
    double tempo_por_etapa; // quanto tempo tem por etapa
    double tempo_passado; // quanto tempo se passou até o momento
    bool ganhou; // indica que o jogador ganhou a fase 5
    bool sair; // indica que o jogador apertou esc
    int pts_final; // quantidade pontos final que vai ser salvo nos recordes
    int etapa_final; // etapa final que vai ser salvo nos recordes
} EstadoDoJogo;

// criei uma struct separada pra armazenar o recorde pra ficar mais organizado no código na hora de gravar a pontuação no arquivo
typedef struct {
    int pontuacao;
    char apelido [30];
    int etapa;
} Recorde;


int retorna_numero_aleatorio(int numero) // retorna um numero aleatório entre 0 e numero - 1
{
    int x = rand() % numero;
    return x;
}

void inicializa_tabuleiro(EstadoDoJogo *jogo) // recebe o ponteiro para que as mudanças sejam feitas no estado do jogo
{
    // valor da etapa 1
    jogo->numCores = 4;
    jogo->pontos_por_peca = 1;
    jogo->qtd_minima_pontos = 50;
    jogo->tempo_por_etapa = 120;

    // da etapa 2 até a 5
    for (int i = 2; i <= jogo->etapa; i++) {
        jogo->numCores += 1;
        jogo->pontos_por_peca += 1;
        jogo->qtd_minima_pontos += 50;
        jogo->tempo_por_etapa += 20;
    }

    for(int i = 0; i < TAMANHO; i++){
        for(int j = 0; j < TAMANHO; j++) {
            jogo->MatrizJogo[i][j] = retorna_numero_aleatorio(jogo->numCores); // aqui passa a quantidade de cores que é utilizada nessa etapa
            jogo->MatrizCursor[i][j] = 0; // a matriz de cursor tem 0 nas casas vazias e 1 na casa onde o cursor está
        }
    }
    jogo->MatrizCursor[0][4] = 1; // esse 1 armazena a posição onde o cursor está
    jogo->modo_linha_coluna = 0; // (para indicar que esta no modo linha, mas pode ser trocado pressionando Enter)
    jogo->qtd_pontos_usuario = 0; // a qtd de pontos q o usuario fez recebe zero a cada etapa pra nao dar bug e ele passar direto
    // resetando estados gerais da partida
    jogo->ganhou = false;
    jogo->sair = false;
}


// essa função te que ser chamada antes de qualquer movimento para que sempre esteja atualizada e não dê erro
void retorna_posicao_cursor (EstadoDoJogo *jogo)
{
    for (int l = 0; l < TAMANHO; l++) {
        for (int c = 0; c < TAMANHO; c++) {
            if (jogo->MatrizCursor[l][c] == 1) {
                jogo->linha_cursor = l; //retorna a ppsição da linha e da coluna por ponteiro pra variável que fica 
                jogo->coluna_cursor = c; // dentro da estrutura de estado do jogo
            }
        }
    }
} 

// fazer uma função que verifica se tem um espaço em branco
int verifica_espaco_vazio(EstadoDoJogo *jogo, int linha, int coluna)
{
    if (jogo->MatrizJogo[linha][coluna] == 9) {
        return 1; // retorna 1 se o espaço contém o número 9 (o número 9 simboliza o espaço vazio na matriz de jogo)
    }
    return 0; // retorna 0 se o espaço não está vazio
}

// fazer uma função que verifica se a coluna tem pelo menos uma casa ocupada
int verifica_PeloMenos_umaCasaOcupada_naColuna(EstadoDoJogo *jogo, int coluna)
{
    for(int l = 0; l < TAMANHO; l++) {
        if (jogo->MatrizJogo[l][coluna] != 9) {
            return 1; // pelo menos uma casa está ocupada
        }
    }
    return 0; // se chegou até aqui é pq nenhuma casa está ocupada
}

// função que verifica se a linha não está inteira vazia, ou seja, se ela tem pelo menos uma casa ocupada
int verifica_PeloMenos_umaCasaOcupada_naLinha(EstadoDoJogo *jogo, int linha)
{
    for(int c = 0; c < TAMANHO; c++) {
        if(jogo->MatrizJogo[linha][c] != 9) { // se pelo menos uma casa for diferente de 9
            return 1; 
        }
    }
    return 0;
}

// fazer uma função que verifica se uma coluna inteira está completa de cores iguais
// o retorno dessa função já é o que vai ser somado na pontuação do usuário
int verifica_coluna_completa(EstadoDoJogo *jogo, int coluna)
{
    if (jogo->MatrizJogo[0][coluna] == 9) { // se o primeiro espaço for 9 significa que a coluna não ta completa
        return 0; 
    }
    for(int l = 0; l < TAMANHO - 1; l++) {
        if (jogo->MatrizJogo[l + 1][coluna] != jogo->MatrizJogo[l][coluna]) {
            return 0; // se for diferente ja retorna 0 de cara 
        }
    }
    jogo->qtd_pontos_usuario += jogo->pontos_por_peca * 10; // se chegou ate aqui, eh pq tudo é igual
    // na etapa em que o usuario está (essa pontuação é definida na inicializa_tabuleiro()). multiplica por 10 pq sao 10 peças por coluna
    return 1;
}


void move_linha_pra_esquerda(EstadoDoJogo *jogo)
{
    int aux = jogo->MatrizJogo[jogo->linha_cursor][0]; // salva o primeiro valor
    for(int c = 1; c < TAMANHO; c++) {
        // passa o valor da casa da frente para a casa de trás
        jogo->MatrizJogo[jogo->linha_cursor][c - 1] = jogo->MatrizJogo[jogo->linha_cursor][c]; 
    }
    jogo->MatrizJogo[jogo->linha_cursor][9] = aux; // passa o primeiro valor pro último
}

void move_linha_pra_direita(EstadoDoJogo *jogo)
{
    int aux = jogo->MatrizJogo[jogo->linha_cursor][9]; // salva o último valor
    for(int c = 9; c > 0; c--) {
        // passa o valor da casa de trás para a casa da frente
        jogo->MatrizJogo[jogo->linha_cursor][c] = jogo->MatrizJogo[jogo->linha_cursor][c - 1]; 
    }
    jogo->MatrizJogo[jogo->linha_cursor][0] = aux; // passa o último valor pro primeiro
}

// se tiver um espaço em branco será representado pelo número 9
void roda_coluna_pra_direita(EstadoDoJogo *jogo)
{
    // para rodar a coluna para direita, significa que a última peça da coluna vai para a primeira peça da coluna da direita
    // requisitos: a primeira peça da coluna a direita deve estar vazia + a coluna principal deve ter pelo menos uma casa ocupada
    // para rodar a coluna para direita, não pode ser a coluna 9
    if (jogo->coluna_cursor == 9) {
        return;
    }
    if (verifica_PeloMenos_umaCasaOcupada_naColuna(jogo, jogo->coluna_cursor)) { // se tem pelo menos uma casa ocupada
        if (verifica_espaco_vazio(jogo, 0, jogo->coluna_cursor + 1)) { // se a primeira casa da coluna da direita está vazia
            // a primeira casa da coluna da direita recebe a última casa da coluna principal
            jogo->MatrizJogo[0][jogo->coluna_cursor + 1] = jogo->MatrizJogo[9][jogo->coluna_cursor];
            jogo->MatrizJogo[9][jogo->coluna_cursor] = 9; // a última casa da coluna principal recebe 9 simboliza o espaço vazio
        }
    }
}

void roda_coluna_pra_esquerda(EstadoDoJogo *jogo)
{
    // para rodar a coluna para esquerda, significa que a última peça da coluna vai para a primeira peça da coluna da esquerda
    // requisitos: a primeira peça da coluna a direita deve estar vazia + a coluna principal deve ter pelo menos uma casa ocupada
    // para rodar a coluna para a esquerda não pode ser a coluna 0
    if (jogo->coluna_cursor == 0) {
        return;
    }
    if (verifica_PeloMenos_umaCasaOcupada_naColuna(jogo, jogo->coluna_cursor)) { // se tem pelo menos uma casa ocupada
        if (verifica_espaco_vazio(jogo, 0, jogo->coluna_cursor - 1)) { // se a primeira casa da coluna da direita está vazia
            // a primeira casa da coluna da esquerda recebe a última casa da coluna principal
            jogo->MatrizJogo[0][jogo->coluna_cursor - 1] = jogo->MatrizJogo[9][jogo->coluna_cursor];
            jogo->MatrizJogo[9][jogo->coluna_cursor] = 9; // a última casa da coluna principal recebe 9 simboliza o espaço vazio
        }
    }
}


void move_cursor(EstadoDoJogo *jogo, int movimento)
{
    int coluna;
    int linha;

    if (movimento == 1) { // se esta no modo linha e vai passar pra linha de cima
        if (jogo->linha_cursor == 0) { // não pode ir pra cima se ja estiver na coluna 0, evita erro
            return;
        }
        coluna = 0;
        linha = -1;
    } else if (movimento == 2) { // se esta no modo linha e vai passar pra linha de baixo
        if (jogo->linha_cursor == 9) { // não pode ir pra baixo se ja esta na linha 9
            return;
        }
        coluna = 0;
        linha = 1;
    } else if (movimento == 3) { // se esta no modo coluna e vai andar pra esquerda
        if (jogo->coluna_cursor == 0) {
            return;
        }
        coluna = -1;
        linha = 0;
    } else if (movimento == 4) { // se esta no modo coluna e vai andar pra direita
        if (jogo->coluna_cursor == 9) {
            return;
        }
        coluna = 1;
        linha = 0;
    }

    jogo->MatrizCursor[jogo->linha_cursor][jogo->coluna_cursor] = 0; // a casinha fica vazia

    jogo->linha_cursor+= linha;
    jogo->coluna_cursor+= coluna;

    jogo->MatrizCursor[jogo->linha_cursor][jogo->coluna_cursor] = 1;

}

// o movimento que faz o usuario perder 2 pontos - ele pode eliminar uma linha inteira se quiser
// a linha não pode estar inteira vazia
void elimina_linha (EstadoDoJogo *jogo)
{
    if (verifica_PeloMenos_umaCasaOcupada_naLinha(jogo, jogo->linha_cursor)) { // se a linha tem pelo menos uma casa ocupada
        for(int c = 0; c < 10; c++) {
            jogo->MatrizJogo[jogo->linha_cursor][c] = 9; // recebe 9 pq o 9 simboliza o espaço em branco
        }
        jogo->qtd_pontos_usuario -= 2; // perde dois pontos por eliminar uma linha
    }
}

void limpa_coluna_completa (EstadoDoJogo *jogo)
{
    for(int c = 0; c < TAMANHO; c++) {
        if (verifica_coluna_completa(jogo, c)) { // se ela tá completa de cores iguais
            for(int l = 0; l < 10; l++) { // todos os espaços daquela coluna recebem 9
                jogo->MatrizJogo[l][c] = 9; // o 9 simboliza o espaço vazio
            }
        }
    }
}

void faz_casinhas_cairem(EstadoDoJogo *jogo)
{
    int mudou;
    do {
        mudou = 0; // a variável mudou recebe zero a cada nova iteração
        for (int c = 0; c < TAMANHO; c++) {
            for (int l = 8; l >= 0; l--) { // de baixo para cima até a segunda linha
                if (verifica_espaco_vazio(jogo, l + 1, c) && !verifica_espaco_vazio(jogo, l, c)) { 
                    // se a de baixo está vazia e a de cima tem peça
                    jogo->MatrizJogo[l + 1][c] = jogo->MatrizJogo[l][c]; // a de baixo recebe a de cima
                    jogo->MatrizJogo[l][c] = 9; // a de cima recebe o 9
                    mudou = 1; // a variável mudou recebe 1
                }
            }
        }
    } while (mudou);
}

// função que coloca uma peça aleatória se a primeira linha estiver vazia
void coloca_peca_PrimeiraLinha (EstadoDoJogo *jogo)
{ // se a primeira linha não tiver pelo menos uma casa preenchida
    if (!(verifica_PeloMenos_umaCasaOcupada_naLinha(jogo, 0))) {
        while(!(verifica_PeloMenos_umaCasaOcupada_naLinha(jogo, 0))) {
            // uma posição aleatória da primeira linha recebe um número de cor aleatório do número de cores definidp no estado do jogo
            jogo->MatrizJogo[0][retorna_numero_aleatorio(10)] = retorna_numero_aleatorio(jogo->numCores);
            faz_casinhas_cairem(jogo);
        }
    }
}

void pega_nome(EstadoDoJogo *jogo) {

  int i = 0;
  jogo->apelido[0] = '\0'; // inicializa a string ja com o final

  j_seleciona_fonte(NULL, 40);

  while (1) { // while até o usuário digitar enter
    if (j_tem_tecla()) { // se o usuario apertou uma tecla
      tecla_t tec = j_tecla(); // variavel tec recebe a tecla

      if (tec == T_ENTER) { // se a tecla pressionada foi o enter
        break;  // encerra

      } else if ((tec == T_BS || tec == T_BACKSPACE) && i > 0) { // se a tecla pressionada foi o backspace
        i--; // apaga o ultimo caractere
        jogo->apelido[i] = '\0';

      } else if (tec >= 32 && tec <= 126 && i < 99) { // se a tecla ta dentro das letras
        jogo->apelido[i++] = (char)tec; //nome recebe a letra
        jogo->apelido[i] = '\0'; //coloca o final novamente
      }
    }

    char linha[100] = "______________________________";
    retangulo_t fundo = {{0, 0}, {1500, 1000}};
    j_retangulo(fundo, 0, amarelo, amarelo); // colocando um fundo
    ponto_t frase = {400, 450};
    ponto_t posicao_nome = {490, 530};
    ponto_t linha_embaixo = {450, 550};
    j_texto(frase, preto, "Digite seu nome e pressione ENTER:");
    j_texto(posicao_nome, laranja, jogo->apelido);
    j_texto(linha_embaixo, preto, linha);
    j_mostra();  // atualiza a tela com as letras novas
  }
}

// função pra atualizar a cor por ponteiros de acordo com o número que aparece no quadradinho da matriz
void atualiza_cor (EstadoDoJogo *jogo, int l, int c) 
{
    if (jogo->MatrizJogo[l][c] == 0) {
        jogo->cor = vermelho;
    } else if (jogo->MatrizJogo[l][c] == 1) {
        jogo->cor = verde;
    } else if (jogo->MatrizJogo[l][c] == 2) {
        jogo->cor = azul;
    } else if (jogo->MatrizJogo[l][c] == 3) {
        jogo->cor = amarelo; // primeira fase vem ate aqui
    } else if (jogo->MatrizJogo[l][c] == 4) {
        jogo->cor = ciano; // segunda fase vem ate aqui
    } else if (jogo->MatrizJogo[l][c] == 5) {
        jogo->cor = magenta; //terceira fase vem ate aqui
    } else if (jogo->MatrizJogo[l][c] == 6) {
        jogo->cor = laranja; // quarta fase vem ate aqui
    } else if (jogo->MatrizJogo[l][c] == 7) {
        jogo->cor = cinza; // quinta fase vem até aqui
    } else if (jogo ->MatrizJogo[l][c] == 9) {
        jogo->cor = preto; // casas vazias
    }
}

void desenha_pontuacao (EstadoDoJogo *jogo)
{
    ponto_t posicao_pontuacao = {70, 50};
    j_texto(posicao_pontuacao, branco, "Pontuação Atual:");
    char texto_pontos[100];
    sprintf(texto_pontos, "%d", jogo->qtd_pontos_usuario);
    ponto_t posicao_pontuacao2 = {300, 50};
    j_texto(posicao_pontuacao2, branco, texto_pontos);
}

// usei defines globais pra poder calcular a posicao das linhas, retangulos, etc
#define LARGURA_RETANGULO 120.0f // o comprimento do retangulo no eixo x
#define ALTURA_RETANGULO 60.0f // o comprimeiro do retangulo no eixo y
#define ESPACAMENTO_LADO 15.0f // a distancia entre um retangulo e outro no eixo x
#define ESPACAMENTO_ALTURA 25.0f // a distancia entre um retangulo e outro no eixo y
#define MARGEM_LADOS 80.0f // a distancia da borda da janela no eixo x
#define MARGEM_ALTURA 100.0f // a distancia da borda da janela no ixo y

void desenha_matriz (EstadoDoJogo *jogo)
{

    for (int l = 0; l < TAMANHO; l++) {
        for (int c = 0; c < TAMANHO; c++) {
            float x = MARGEM_LADOS + c * (LARGURA_RETANGULO + ESPACAMENTO_LADO); // posicao x do inicio do retangulo
            float y = MARGEM_ALTURA + l * (ALTURA_RETANGULO + ESPACAMENTO_ALTURA); // posicao y do inicio do retangulo

            atualiza_cor(jogo, l, c);
            retangulo_t r = {{x, y}, {LARGURA_RETANGULO, ALTURA_RETANGULO}}; // desenhando o retangulo
            j_retangulo(r, 2, jogo->cor, jogo->cor);
        }
    }

}

void desenha_cursor_linha (EstadoDoJogo *jogo)
{
    retorna_posicao_cursor(jogo); // atualiza os ponteiros antes da movimentação (evita erro)

    float altura_base = MARGEM_ALTURA + jogo->linha_cursor * (ALTURA_RETANGULO + ESPACAMENTO_ALTURA); // calcula o y do cursor
    float largura_inicio = MARGEM_LADOS; // coloca a margem pra não ficar colado no início da janela
    float largura_fim = MARGEM_LADOS + 10 * (LARGURA_RETANGULO + ESPACAMENTO_LADO) - ESPACAMENTO_LADO; // calcula o x final da linha

    // LINHA DE CIMA
    ponto_t cima1 = {largura_inicio, altura_base - ESPACAMENTO_ALTURA / 2}; //marca o inicio da linha de cima
    ponto_t cima2 = {largura_fim,    altura_base - ESPACAMENTO_ALTURA / 2}; // marca o fim da linha de cima
    ponto_t baixo1 = {largura_inicio, altura_base + ALTURA_RETANGULO + ESPACAMENTO_ALTURA / 2}; // marca o inicio da linha de baixo
    ponto_t baixo2 = {largura_fim,    altura_base + ALTURA_RETANGULO + ESPACAMENTO_ALTURA / 2}; // marca o fim da linha de baixo
    j_linha(cima1, cima2, 6, branco);
    j_linha(baixo1, baixo2, 6, branco);

    // posição do canto de cima esquerdo do quadradinho do cursor
    float x = MARGEM_LADOS + jogo->coluna_cursor * (LARGURA_RETANGULO + ESPACAMENTO_LADO); // posição horizontal do inicio do quadradinho
    float y = MARGEM_ALTURA + jogo->linha_cursor * (ALTURA_RETANGULO + ESPACAMENTO_ALTURA); // posição vertical do inicio do quadradinho

    // LINHAS DO LADO (EM VOLTA DO QUADRADINHO DO CURSOR)
    ponto_t esquerda1 = {x - ESPACAMENTO_LADO / 2, y}; // ponto de cima da linha esquerda
    ponto_t esquerda2 = {x - ESPACAMENTO_LADO / 2, y + ALTURA_RETANGULO}; // ponto de baixo da linha esquerda 
    ponto_t direita1 = {x + LARGURA_RETANGULO + ESPACAMENTO_LADO / 2, y}; // ponto de cima da linha direita
    ponto_t direita2 = {x + LARGURA_RETANGULO + ESPACAMENTO_LADO / 2, y + ALTURA_RETANGULO}; // ponto de baixo da linha direita
    j_linha(esquerda1, esquerda2, 6, branco);
    j_linha(direita1, direita2, 6, branco);
}



void desenha_cursor_coluna (EstadoDoJogo *jogo)
{
    retorna_posicao_cursor(jogo);

    float largura_base = MARGEM_LADOS + jogo->coluna_cursor * (LARGURA_RETANGULO + ESPACAMENTO_LADO); // calcula o x da coluna do cursor
    float altura_inicio = MARGEM_ALTURA; // inicio da linha no eixo y
    float altura_fim = MARGEM_ALTURA + 10 * (ALTURA_RETANGULO + ESPACAMENTO_ALTURA) - ESPACAMENTO_ALTURA; // fim da linha no eixo y

    // LINHAS ESQUERDA E DIREITA
    ponto_t inicio_esquerda = {largura_base - ESPACAMENTO_LADO / 2, altura_inicio}; // posição do inicio da linha esquerda
    ponto_t fim_esquerda = {largura_base - ESPACAMENTO_LADO / 2, altura_fim}; // posição do fim da linha esquerda
    ponto_t inicio_direita = {largura_base + LARGURA_RETANGULO + ESPACAMENTO_LADO / 2, altura_inicio}; // posição do inicio da linha direita
    ponto_t fim_direita = {largura_base + LARGURA_RETANGULO + ESPACAMENTO_LADO / 2, altura_fim}; // posição do inicio da linha direita
    j_linha(inicio_esquerda, fim_esquerda, 6, branco); 
    j_linha(inicio_direita, fim_direita, 6, branco);

    // posição do canto de cima esquerdo do quadradinho do cursor
    float x = MARGEM_LADOS + jogo->coluna_cursor * (LARGURA_RETANGULO + ESPACAMENTO_LADO);
    float y = MARGEM_ALTURA + jogo->linha_cursor * (ALTURA_RETANGULO + ESPACAMENTO_ALTURA);

    // LINHAS EM CIMA E EMBAIXO (DO QUADRADO DO CURSOR)
    ponto_t inicio_cima = {x, y - ESPACAMENTO_ALTURA / 2}; // ponto de inicio da linha de cima
    ponto_t cima_fim = {x + LARGURA_RETANGULO, y - ESPACAMENTO_ALTURA / 2}; // fim da linha de cima
    ponto_t inicio_baixo = {x, y + ALTURA_RETANGULO + ESPACAMENTO_ALTURA / 2}; // ponto de inicio da linha de baixo
    ponto_t baixo_fim = {x + LARGURA_RETANGULO, y + ALTURA_RETANGULO + ESPACAMENTO_ALTURA / 2}; // fim da linha de baixo
    j_linha(inicio_cima, cima_fim, 6, branco);
    j_linha(inicio_baixo, baixo_fim, 6, branco);
}

// eu separei as funções de desenho em funções mais curtas e as reuni nessa função geral pra ficar mais organizado e com funções menos
// compridas
void desenha_jogo (EstadoDoJogo *jogo)
{
    desenha_pontuacao(jogo);
    if (jogo->modo_linha_coluna == 1) {
        desenha_cursor_coluna(jogo);
    } else if (jogo->modo_linha_coluna == 0) {
        desenha_cursor_linha(jogo);
    }
    desenha_matriz(jogo);
}

void desenha_tempo(int tempo_restante) // função separada pra evitar erro devido ao momento de inicialização de contagem 
// da variavel do tempo
{
    cor_t cor_tempo = branco;
    char tempo_str[50];
    sprintf(tempo_str, "Tempo restante: %d s", tempo_restante); // converte em uma string
    ponto_t pos_tempo = {70, 20};
    if (tempo_restante <= 20) {
        cor_tempo = vermelho; // faz o tempo ficar vermelho se falta menos de 20 segundos
    }
    j_texto(pos_tempo, cor_tempo, tempo_str);
}

// tela que vai aparecer entre as fases, tem duração de 10 segundos e fla a pontuação necessária naquela etapa
void tela_entre_fases(EstadoDoJogo *jogo)
{
    for(int tempo = 10; tempo >= 0; tempo--) {

        retangulo_t fundo = {{0, 0}, {1500, 1000}};
        j_retangulo(fundo, 0, branco, branco); // colocando um fundo
        j_seleciona_fonte(NULL, 100); // fonte maior

        char etapa_str[50], pontuacao_str[50], tempo_str[50];

        sprintf(etapa_str, "Fase %d", jogo->etapa);
        ponto_t pos_etapa = {550, 250}; // posição do texto da etapa
        j_texto(pos_etapa, magenta, etapa_str);

        sprintf(pontuacao_str, "Pontuação mínima: %d", jogo->qtd_minima_pontos);
        ponto_t pos_pontuacao = {200, 400}; // posição da pontuação
        j_texto(pos_pontuacao, magenta, pontuacao_str);

        sprintf(tempo_str, "Seu jogo começa em:  %d", tempo);
        ponto_t pos_tempo = {100, 550}; // posição do tempo
        j_texto(pos_tempo, verde, tempo_str);

        j_cochila(1);
        if (tempo <= 3) { // se faltar 3 segundos ou menos o usuario recebe boa sorte
            ponto_t pos_boasorte = {400, 800};
            j_texto(pos_boasorte, azul, "BOA SORTE!");
        }
        j_mostra(); // atualiza a tela a cada segundo
    }
    j_seleciona_fonte(NULL, 26); // volta ao tamanho padrão da fonte
}

// tela de instruções - só aparece uma vez no jogo (no inicio) e passa pro jogo quando o usuário aperta enter
void tela_de_instrucoes() {

    while (1) {
        j_seleciona_fonte(NULL, 70);
        retangulo_t fundo = {{0, 0}, {1500, 1000}};
        j_retangulo(fundo, 0, branco, branco); // colocando um fundo
        retangulo_t marcador = {{450, 20}, {540, 115}};
        j_retangulo(marcador, 0, preto, preto);

        ponto_t t1 = {500, 100};
        j_texto(t1, branco, "INSTRUÇÕES");
        j_seleciona_fonte(NULL, 50);
        ponto_t t2 = {300, 300};
        j_texto(t2, preto, "Monte colunas de cores iguais");
        ponto_t t3 = {120, 370};
        j_texto(t3, preto, "Use as setinhas do seu teclado para se movimentar");
        ponto_t t4 = {50, 440};
        j_texto(t4, preto, "Use as teclas [A] e [D] para mover as peças das colunas");
        ponto_t t5 = {90, 510};
        j_texto(t5, preto, "Use a tecla Enter para mudar de linha para coluna!");
        ponto_t t6 = {280, 690};
        j_texto(t6, preto, "Aperte [Enter] para começar o jogo");
        j_mostra();
        if (j_tem_tecla()) // o jogo coeça quando o usuário apertar enter
        {
            tecla_t tecla = j_tecla();
            if (tecla == T_ENTER) {
                break;
            }
        }
    }
}

// função que reconhece a entrada da tecla e chama a função correspondente
void pega_tecla (EstadoDoJogo *jogo)
{
    if(j_tem_tecla()) {
        tecla_t tecla = j_tecla();
        if (tecla == T_ENTER) { // MUDA DO MODO LINHA PRA COLUNA
            if (jogo->modo_linha_coluna == 1) {
                jogo->modo_linha_coluna = 0;
            } else if (jogo->modo_linha_coluna == 0) {
                jogo->modo_linha_coluna = 1;
            }
        }

        if (tecla == T_ESC) { // tecla esc pra sair
            jogo->sair = true;
            return;

        } else if (tecla == T_BAIXO && jogo->modo_linha_coluna == 0) { // PASSAR PRA LINHA DE BAIXO
            // se ele apertou a setinha pra baixo e ta no modo linha
            move_cursor(jogo, 2);

        } else if (tecla == T_CIMA && jogo->modo_linha_coluna == 0) { // PASSAR PRA LINHA DE CIMA
            // se ele apertou a setinha pra cima e ta no modo linha
            move_cursor(jogo, 1);

        } else if (tecla == T_DIREITA && jogo->modo_linha_coluna == 1) { // PASSAR PRA COLUNA PRA DIREITA
            // se ele apertou a setinha da direita e ta no modo coluna
            move_cursor(jogo, 4);

        } else if (tecla == T_ESQUERDA && jogo->modo_linha_coluna == 1) { 
            // se ele apertou a setinha da esquerda e ta no modo linha //PASSAR PRA COLUNA DA ESQUERDA
            move_cursor(jogo, 3);

        } else if (tecla == T_DIREITA && jogo->modo_linha_coluna == 0) { // RODAR  LINHA PRA DIREITA
            // se ele apertou a setinha da direita e ta no modo linha
            move_linha_pra_direita(jogo);

        } else if (tecla == T_ESQUERDA && jogo->modo_linha_coluna == 0) { // RODAR A LINHA PRA ESQUERDA
            // se ele apertou a setinha da esquerda e ta no modo linha
            move_linha_pra_esquerda(jogo);

        } else if (tecla == T_BACKSPACE) { // ELIMINAR UMA LINHA (-2 PONTOS)
            // se ele apertou a backspace, esse é o movimento que faz ele perder dois pontos
            elimina_linha(jogo);

        } else if (tecla == T_LETRAD && jogo->modo_linha_coluna == 1) { // IMPORTANTE!!!!!!! EU DECLAREI ESSAS TECLAS NO ARQUIVO JANELA.H
            // movimento de rodar a coluna pra direita (ultima casinha da coluna vai pro primeiro espaço vazio da outra)
            // se ele apertou D e ta no modo coluna 
            roda_coluna_pra_direita(jogo);

        } else if (tecla == T_LETRAA && jogo->modo_linha_coluna == 1) { // IMPORTANTE!!!!!!!!! EU DECLAREI ESSAS TECLAS NO ARQUIVO JANELA.H
            // movimento de rodar a coluna pra esquerda (ultima casinha da coluna vai pro primeiro espaço vazio da outra)
            // se ele apertou A e ta no modo coluna
            roda_coluna_pra_esquerda(jogo);
        }
    }
}

// função que reconhece vitória
// para as fases 1 até 4: passa pra próxima fase e reincia o tabuleiro
// para a fase 5: o bool ganhou da estrutura de dados recebe true pq é a última fase
void reconhece_vitoria (EstadoDoJogo *jogo, int tempo_restante, double *tempo_inicio)
{
    // reconhece a vitória se a quantidade de pontos já atingiu o mínimo da etapa e o tempo ainda ta no prazo
    if (jogo->qtd_pontos_usuario >= jogo->qtd_minima_pontos && tempo_restante > 0) {
        
        jogo->pts_final = jogo->qtd_pontos_usuario; // passa a pontuação final
        jogo->etapa_final = jogo->etapa; // passa a etapa final pra salvar nos recordes

        if (jogo->etapa == 5) { // se estiver na etapa 5 não tem mais fase, é só a tela de vitória
            jogo->ganhou = true;
            return;
        }

        jogo->etapa++; // a etapa é incrementada
        inicializa_tabuleiro(jogo); // o tabuleiro é inicializado novamente
        tela_entre_fases(jogo); // aparece a tela de início das fases novamente
        *tempo_inicio = j_relogio(); // o tempo é reiniciado
    }
}


int perdeu_continua_jogando () // usuário perdeu e vai escolher se quer continuar jogando
{
    while (1) {

        j_seleciona_fonte(NULL, 180);
        ponto_t t1 = {210, 400};
        j_texto(t1, branco, "GAME OVER");
        j_seleciona_fonte(NULL, 40);
        ponto_t t2 = {165, 650};
        j_texto(t2, branco, "Aperte [Enter] para jogar novamente e [Esc] para sair");
        j_mostra();
         j_seleciona_fonte(NULL, 26);

        if (j_tem_tecla()) {
            tecla_t tecla = j_tecla();
            if (tecla == T_ENTER) {
                return 1;
            } else if (tecla == T_ESC) {
                return 0;
            }
        }
    }
}

 int ganhou_continua_jogando () // usuário ganhou e vai escolher se quer continuar jogando
 {     
    while (1) {         
        retangulo_t fundo = {{0, 0}, {1500, 1000}};        
        j_retangulo(fundo, 0, ciano, ciano); // colocando um fundo
        j_seleciona_fonte(NULL, 180);
        ponto_t t1 = {100, 400};
        j_texto(t1, azul, "VOCÊ VENCEU!");
        j_seleciona_fonte(NULL, 40);
        ponto_t t2 = {165, 650};
        j_texto(t2, azul, "Aperte [Enter] para jogar novamente e [Esc] para sair");
        j_mostra();
        j_seleciona_fonte(NULL, 26);

        if (j_tem_tecla()) {
            tecla_t tecla = j_tecla();
            if (tecla == T_ENTER) {
                return 1;
            } else if (tecla == T_ESC) {
                return 0;
            }
        }
    }
}


void roda_jogo(EstadoDoJogo *jogo)
{
    inicializa_tabuleiro(jogo); // recebe como parâmetro o ponteiro da variável de estado jogo como ponteiro
    pega_nome(jogo); // pega o apelido do usuario
    tela_de_instrucoes(); // mostra a tela de instruções
    tela_entre_fases(jogo); // mostra a tela entre fases
    double tempo_inicio = j_relogio(); // marca o inicio da partida assim que sai da tela de entrada

    while (1) 
    {
        double tempo_atual = j_relogio(); // conta o tempo atual a cada loop
        jogo->tempo_passado = tempo_atual - tempo_inicio; // variavel de estado do jogo recebe o tempo passado
        int tempo_restante = (int)jogo->tempo_por_etapa - (int)jogo->tempo_passado; // tempo restante calculado
        desenha_jogo(jogo); // mostra o jogo
        desenha_tempo(tempo_restante); // mostra o tempo
        j_mostra(); // imprime tudo
        pega_tecla(jogo); // pega a tecla
        faz_casinhas_cairem(jogo); // efeito gravidade
        coloca_peca_PrimeiraLinha(jogo); // nunca deixa a primeira linha vazia
        limpa_coluna_completa(jogo); // limpa e soma a pontuação quando a coluna ta completa
        desenha_jogo(jogo); // imprime o jogo
        desenha_tempo(tempo_restante); // imprime o tempo
        j_mostra(); // imprime na tela
        reconhece_vitoria(jogo, tempo_restante, &tempo_inicio); // se ganhou, passa pra proxima fase

        // se o usuário perdeu, ganhou (fase 5) ou quer sair, acaba
        if (tempo_restante <= 0 || jogo->ganhou == true || jogo->sair == true) {
            jogo->pts_final = jogo->qtd_pontos_usuario;
            jogo->etapa_final = jogo->etapa;
            break;
        }
    }
}

// eu fiz o sistema de recorde como estava escrito no github
// função 01: faz a leitura do arquivo de recordes e aloca a memória

// const char recebe o nome do arquivo a ser lido
// ponteiro de ponteiro recorde **vetor recebe o endereço do vetor da alocação dinamica pra poder alocar o espaço do arquivo
// ponteiro de quantidade recebe a quantidade de recordes existentes
int ler_recordes (const char *nome_arquivo, Recorde **vetor, int *qtd) {

    FILE *arquivo = fopen(nome_arquivo, "r"); // abre o arquivo no modo leitura
    if (arquivo == NULL) { // se nao deu pra abrir (ele ainda nao existe)
        *vetor = NULL; // o vetor da alocação fica null
        *qtd = 0; // a quantidade de recordes gravados é 0
        return 1; // sem erro, mas nenhum recorde lido
    }

    int capacidade = 4; // começa com espaço para 4
    int quantidade = 0; // quantidade lida até agr é zero

    // aloca espaço pra 4 vetores primeiro
    Recorde *primeiro_vetor = malloc(capacidade * sizeof(Recorde));

    if (primeiro_vetor == NULL) { // se nao deu pra alocar
        fclose(arquivo);
        return 0; // erro de alocação
    }

    while (1) {

        Recorde r; // criei uma variavel pra armazenar a leitura de cada recorde (os que ja estao no arquivo)
        // faz a leitura e passa pra variavel criada
        int r_ok = fscanf (arquivo, "%19s %d %d", r.apelido, &r.pontuacao, &r.etapa);
        if (r_ok != 3) {
            break; // chegou ao fim ou erro na leitura
        }

        // realocando se precisar de mais memória
        if (quantidade >= capacidade) { // se a quantidade for maior que a capacidade do espaço alocado
            capacidade *= 2; // a capacidade dobra
            // cria um novo ponteiro e realoca o primeiro vetor 
            Recorde *novo = realloc (primeiro_vetor, capacidade * sizeof(Recorde));

            if (novo == NULL) { // se não deu pra realocar a memória
                free(primeiro_vetor); // libera o primeiro vetor
                fclose(arquivo);
                return 0; // erro de realocação
            }
            primeiro_vetor = novo; // passa o vetor novo pro antigo
        }

        primeiro_vetor[quantidade] = r; // copia o recorde lido pro vetor
        quantidade++; // aumenta o contador de elementos
    }

    fclose(arquivo);
    *vetor = primeiro_vetor; // o endereço do vetor original recebe o primeiro vetor criado
    *qtd = quantidade; // o endereço da quantidade original recebe a quantidade nova (dos que foram lidos)
    return 1; // se chegou até aqui é pq não deu erro
}

// função 02 função de escrita que passa o conteúdo da alocação dinâmica (na memória) pro arquivo

// const char recebe o nome do arquivo
// passa o ponteiro de onde está alocado a memória
// passa a quantidade de recordes existentes até o momento
int salvar_recordes (const char *nome_arquivo, Recorde *vetor, int qtd) {

    FILE *arquivo = fopen(nome_arquivo, "w"); // abre o arquivo no modo escrita
    if (arquivo == NULL) {
        return 0; // erro ao abrir o arquivo para escrita
    }

    // itera sobre toda a quantidade de recordes armazenadas dinamicamente na memoria e grava no arquivo de novo
    for (int i = 0; i < qtd; i++) {
        // grava o apelido, a pontuação e a etapa no arquivo
        fprintf(arquivo, "%s %d %d\n", vetor[i].apelido, vetor[i].pontuacao, vetor[i].etapa);
    }

    fclose(arquivo); // fecha o arquivo
    return 1; // sucesso
}

// função 03 que insere a nova pontuação, altera o registro em memória
int inserir_recorde(Recorde **vetor, int *qtd, int *capacidade, Recorde novo) {

    // verifica se precisa de realocar
    if (*qtd >= *capacidade) { // se a quantidade for maior que a capacidade
        int nova_capacidade = (*capacidade) * 2; // cria uma nova capacidade (dobrando a capacidade original igual na outra função)

        // realoca na variavel criada o vetor original
        Recorde *primeiro_vetor = realloc(*vetor, nova_capacidade * sizeof(Recorde)); 
        if (primeiro_vetor == NULL) { // se não deu pra realocar retorna zero
            return 0; // erro de realocação
        }
        *vetor = primeiro_vetor; // o vetor original recebe o vetor realocado
        *capacidade = nova_capacidade; // a capacidade recebe a nova capacidade
    }

    int posicao_pontuacao = 0; // procurando a posição pra colocar o novo recorde
    // enquanto ele for menor que a quantidade de recordes que ja estão lá (pra não sobrepor) e enquanto a pontuação
    // que está naquele índice for maior que a pontuação que vai ser inserida (pra ficar em ordem decrescente)
    while (posicao_pontuacao < *qtd && (*vetor)[posicao_pontuacao].pontuacao >= novo.pontuacao) {
        posicao_pontuacao++; // uma posição a mais
    }

    // move pra direita pra não sobreescrever os dados que já estavam lá no caso da pontuação ser maior
    for (int i = *qtd; i > posicao_pontuacao; i--) {
        (*vetor)[i] = (*vetor)[i - 1];
    }

    // usa o ponteiro pra inserir o novo recorde
    (*vetor)[posicao_pontuacao] = novo;

    // usa ponteiro pra colocar mais um na quantidade
    (*qtd)++;

    return 1; // se chegou até aqui é pq deu certo
}

int main()
{
    tamanho_t tamanho_janela = {1500, 1000};
    j_inicializa(tamanho_janela, "Jogo das Cores");
    srand(time(NULL)); 
    j_seleciona_fonte(NULL, 26);

    // variaveis pra controle dos recordes
    Recorde *recordes = NULL;
    int qtd_recordes = 0;
    int capacidade_recordes = 4;
    const char *nome_arquivo = "recordes.txt";

    // le os recordes alocando a memoria e verifica pra ter certeza que deu certo
    // coloquei essa verificação pq antes estava fechando quando o arquivo não existia
    // se o ponteiro for null aloca o estpaço mesmo assim
    int leu = ler_recordes(nome_arquivo, &recordes, &qtd_recordes);

    if (recordes == NULL) {
    recordes = malloc(capacidade_recordes * sizeof(Recorde));
        if (recordes == NULL) {
            printf("Erro ao alocar memória para recordes.\n");
            return 1;
        }
    }

    // se a leitura deu certo e ja tem algum recorde salvo ajusta a capacidade
    if (leu && qtd_recordes > 0) {
        capacidade_recordes = qtd_recordes;
    } else if (!leu) {
        printf("Erro ao ler o arquivo de recordes.\n");
    }

    // loop principal do jogo
    while (1)
    {
        EstadoDoJogo jogo;
        jogo.etapa = 1; 
        roda_jogo(&jogo); 

        // se o jogador apertou esc o recorde não é salvo pq ele não terminou a partida
        if (jogo.sair){
            break;
        }

        // depois da partida, grava o recorde
        Recorde novo;
        novo.pontuacao = jogo.pts_final;
        novo.etapa = jogo.etapa_final;
        strncpy(novo.apelido, jogo.apelido, sizeof(novo.apelido));

        // insere o novo recorde na memória e guarda no arquivo
        if (!inserir_recorde(&recordes, &qtd_recordes, &capacidade_recordes, novo)) {
            printf("Erro ao inserir novo recorde.\n");

        } else if (!salvar_recordes(nome_arquivo, recordes, qtd_recordes)) {
            printf("Erro ao salvar recordes.\n");
        }

        if (jogo.ganhou) { // se ele ganhou

            if (!ganhou_continua_jogando()) { // se ele ganhou e não quer continuar jogando, sai do loop
                break;
            } 

        } else { // se ele perdeu

            if (!perdeu_continua_jogando()) { // se ele perdeu e não quer continuar jogando, sais do loop
                break;
            }
        }

    free(recordes); // libera a memória alocada
    j_finaliza(); // fecha a janela gráfica
    return 0;
    }
}


