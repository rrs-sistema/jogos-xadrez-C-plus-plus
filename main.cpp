#include <stdio.h>// Essa bibliotca é usada quando queremos adicionar outras funções predefinidas da biblioteca de funções do pré-processado
#include <stdlib.h> //Essa bibliotca possui funções envolvendo alocação de memória, controle de processos, conversões e outras...
#include <conio.h>// Essa  biblioteca(arquivo cabeçalho) de C usado principalmente por compiladores MS-DOS para fornecer input / output console

#include <SDL2/SDL.h>// Essa é uma bibliotca para trabalhar com a tela gráfica
#include <SDL2/SDL_image.h> // Essa biblioteca é usada para carregar as imagens do arquivo

#define WIDTH 640 // Define o comprimento do xadrez
#define HEIGHT 480 // Define a altura do xadrez
#define TOTAL_PECAS 12  // Define a quantidade de pecas

// Fonte do exemplo: https://www.youtube.com/watch?v=3JLFZWxTruM

SDL_Window  * window;// Variável é um ponteiro que será usada para desenhar a tela do xadrez
SDL_Renderer * render;// Variável é um ponteiro que será usada para renderizar a tela do xadrez
SDL_Event evento;// Variável para usar nos eventos da tela

// Cria uma matriz com 2 dimensões 8 por 8 posições
char xadrez[8][8] = {
    {'T', 'C', 'B', 'Q', 'K', 'B', 'C', 'T'},// Cria as peças PRETAS na parte de cima representada por LETRAS maiúscula
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'t', 'c', 'b', 'q', 'k', 'b', 'c', 't'}};// Cria as peças BRANCAS na parte de baixo representada por LETRAS minúscula

SDL_Texture * imagens[TOTAL_PECAS]; // Carrega as imagens no vetor com 12 posições

// Esse vetor de duas posições quarda os nomes das imagens que está no disco/arquivo
char nomesImagens[][50] = {
    "BB.PNG", "BP.PNG", "CB.PNG", "CP.PNG", "KB.PNG", "KP.PNG",
    "QB.PNG", "QP.PNG", "PB.PNG", "PP.PNG", "TB.PNG", "TP.PNG"
};
// Esse vetor é para representar qual peça representa a sua imagem, Tipo quem representa a imagem do cavalo? 'c' e 'C' etc..
char pecas[] = {'b', 'B', 'c', 'C', 'k', 'K', 'q', 'Q', 'p', 'P', 't', 'T'};
int linhaOrigem = -1; // Quarda o valor da linha de origem quando foi clicada pela primeira vez, iniciando com -1
int colunaOrigem = -1; // Quarda o valor da coluna de origem quando foi clicada pela primeira vez, iniciando com -1

// Função que inicializa a tela gráfica
int init_graficos() {
	if(SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
		//printf("\n Inicializou o SDL ... ");
		window = SDL_CreateWindow("X A D R E Z - UNICESUMAR - Acsa & Rivaldo", 50, 50, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
		if(window != NULL) {
			render = SDL_CreateRenderer(window, -1, 0);
			if(render != NULL) {

			} else {
				printf("\n Erro ao criar o render ... \n");
			}
		} else {
			printf("\n Erro ao criar a janela ... \n");
		}
	} else {
		printf("\n Erro ao iniciar o SDL ... \n");
	}
}

// Função para carregar as imagens do arquivo para o xadrez
int init_imagens() {
    if(IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG) {// Inicia verificando o tipo das imagens
        int i;
        //printf(" Carregando imagens .... \n");
        for(i = 0; i < TOTAL_PECAS; i++){
            char nomeImg[50] = "imagens/"; // Pega o caminho da imagem
            strcat(nomeImg, nomesImagens[i]);// Concatena o caminho da imagem com o nome da imagem na posição i do laço de repetição
            SDL_Surface * surImage = IMG_Load(nomeImg);// Carrega a imagem que está na variavel: nomeImg
            imagens[i] = SDL_CreateTextureFromSurface(render, surImage);// Aqui é onde carrega propriamente dita no array imagens
        }
    }
}
// Essa função recebe um caractere e retorna uma imagem
SDL_Texture * imagemFromPeca(char peca) {
    int i;
    for (i = 0; i < TOTAL_PECAS; i++) {
        if(pecas[i] == peca) {// Verifica a imagem correspondente ao caractere
            return imagens[i]; // Retorna a imagem que corresponde ao caractere
        }
    }
}

// Função que pinta a tela com as imagens do xadrez
int pintarTela() {
    int w = WIDTH / 8; // Define a largura(pixel) de cada quadrado do xadrez
    int h = HEIGHT / 8; // Define a altura(pixel) de cada quadrado do xadrez
    int lin, col;// cria as váriaveis do tipo inteiro para linhas(lin) coluna(col)

    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);// Limpando a tela pintando de preto
    SDL_RenderClear(render);// limpa o render

    for (lin = 0; lin < 8; lin++)// Inicia o laço da linha do xadrez
    {
          for (col = 0; col < 8; col++)// Inicia o laço da coluna do xadrez
          {
              if((col + lin) % 2 == 0){
                    /*
                    Para pintar a tela, COR sim COR não, usamos a soma da coluna com a linha divide
                    por dois e pegamos o resto, depois verifica se o resto é zero(0)
                    */
                SDL_SetRenderDrawColor(render, 240,255,255, 255);
              } else {
                SDL_SetRenderDrawColor(render, 176,196,222, 255);
              }

              int x = col * w;// X indica onde começa cada casinha do quadrado, ou seja o X começa do zero(0), depois oitenta(80) etc... até chegar 640px
              int y = lin * h;// Y indica onde termina cada casinha do quadrado, funciona do mesmo jeito que o Y só que usando a linha
              SDL_Rect rDest = {x, y, w, h};// Cria o retângulo de destino, definido a altura e largura, informando onde começa, onde termina, qual a largura e altura
              SDL_RenderFillRect(render, &rDest);// Renderiza o retângulo
              // Mostrando as peças no tabuleiro
              char p = xadrez[lin][col];//
              if(p != ' ') {// Verifica é uma peça ou um espaço vazio
                int wOri, hOri; // Variavel para pegar a largura total da imagem e altura total da imagem
                SDL_Texture * img = imagemFromPeca( p );//
                SDL_QueryTexture(img, NULL, NULL, &wOri, &hOri);//
                SDL_Rect rOrig = {0, 0, wOri, hOri}; // Criar o retângulo de origem
                SDL_RenderCopy(render, img, &rOrig, &rDest); //
              }

              // Aqui marca a celula selecionada
              if(linhaOrigem == lin && colunaOrigem == col) {//Verifica se a linha e coluna de origem é igual e linha e coluna que está sendo pintada
                SDL_SetRenderDrawColor(render, 0, 0, 255, 255);// Pinha a celula de verde
                SDL_RenderDrawRect(render, &rDest);// Renderiza a celula pintada de verde
              }
          }
    }

    SDL_RenderPresent(render);// Apresenta a tela com grafico
}
    /*
        Função usada para mover as peças, essa função pega por paramentros a linha de origem,
        coluna de origem, linha de destino e coluna de destino esse função vai retornar um(1) se tiver tudo certo,
        caso contrário rtorna zero(0)
    */
int moverPeca(int linOri, int colOri, int linDes, int colDes) {
	int mover = 0;// Variável que indica se o usuário pode mover ou não a peça
	int peca; // Variável que indica a peça
	// calcula o deslocamento da linha vertical
	int deslocamentoVertical = abs(linDes - linOri); // cacula o deslocamento vertical e pegar o valor absoluto, para isso usamos a função "abs" retorna o valor absoluto
	// calcula o deslocamento da linha horizontal
	int deslocamentoHorizontal = abs(colDes - colOri); // cacula o deslocamento horizontal e pegar o valor absoluto, para isso usamos a função "abs" retorna o valor absoluto
    // Esse primeiro if, verifica se o deslocamento das linha e colunas estão dentro do tabuleiro
    // se a linha e coluna de origem tiver entre 0 e 8 está dentro do tabuleiro
    // a mesma regra se aplica a linha e coluna de destino
    // essa verifição: (deslocamentoVertical + deslocamentoHorizontal > 0) é para analisar se o usuário mexeu na mesma casa, ou seja: saiu dela pra ela mesmo
	if ( 	(linOri >= 0 && linOri < 8 && colOri >= 0 && colOri < 8) &&
	  		(linDes >= 0 && linDes < 8 && colDes >= 0 && colDes < 8) &&
            (deslocamentoVertical + deslocamentoHorizontal > 0)) {
	  	peca = xadrez[linOri][colOri];// pega a peça do xadrez na linha de origem e coluna de origem

	  	/*
            Testando deslocamento da TORRE representada pela LETRA "T" ou "t"
            Se a peça que foi mexida é a TORRE e o deslocamento da vertical for igual(0) ou o
            deslocamento da horizontal for igual(0), isso indica que o usuário pode mover
	  	*/
	  	if((peca == 'T' || peca == 't') && (deslocamentoVertical == 0 || deslocamentoHorizontal == 0)) {
	  		mover = 1;
		}
	  	/*
            Testando o deslocamento do BISPO representada pela LETRA "B" ou "b"
            Se a peça que foi mexida é o BISPO e o deslocamento da vertical for igua ao
            deslocamento horizontal, indica que o usuário pode mover
	  	*/
	  	if((peca == 'B' || peca == 'b') && (deslocamentoVertical == deslocamentoHorizontal)) {
	  		mover = 1;
		}
	  	/*
            Testando o deslocamento do CAVALO representada pela LETRA "C" ou "c"
            Se a peça que foi mexida é o CAVALO e o deslocamento da vertical for igual a um(1) e
            deslocamento horizontal for igual a dois(2) OU deslocamento da vertical for igual a dois(2) e
            deslocamento horizontal for igual a um(1)
            indica que o usuário pode mover
	  	*/
	  	if((peca == 'C' || peca == 'c') &&
		  ( (deslocamentoVertical == 1 && deslocamentoHorizontal == 2) ||
		  (deslocamentoVertical == 2 && deslocamentoHorizontal == 1) ) ) {
	  		mover = 1;
		}
	  	/*
            Testando o deslocamento do RAINHA representada pela LETRA "Q" ou "q"
            Se a peça que foi mexida é o RAINHA e o deslocamento da vertical é igual ao deslocamento horizontal ou
            deslocamento da vertical é igual zero(0) ou o deslocamento horizontal é igual zero(0)
            indica que o usuário pode mover
	  	*/
	  	if((peca == 'Q' || peca == 'q') &&
		  ((deslocamentoVertical == deslocamentoHorizontal) || (deslocamentoVertical == 0 || deslocamentoHorizontal == 0) )) {
	  		mover = 1;
		}
	  	/*
            Testando o deslocamento do REI representada pela LETRA "K" ou "k"
            Se a peça que foi mexida é o REI e o deslocamento da vertical é maior ou igual que zero(0) e menor ou igual a um(1) e o
            deslocamento da horizontal é maior ou igual a zero(0) e o menor igual a um(1)
            indica que o usuário pode mover
	  	*/
	  	if((peca == 'K' || peca == 'k') &&
		  ((deslocamentoVertical >= 0 && deslocamentoVertical <= 1) && (deslocamentoHorizontal >= 0 && deslocamentoHorizontal <= 1) )) {
	  		mover = 1;
		}
	  	/*
            Testando o deslocamento do PEÃO(PRETO) representada pela LETRA "P"
            Se a peça que foi mexida é o PEÃO e o resulta da linha de origem menos(-) a linha de origem é igual a um(1) e o
            deslocamento da horizontal é igual a zero(0)
            indica que o usuário pode mover
	  	*/
	  	if((peca == 'P') && ( (linDes - linOri) == 1 ) && deslocamentoHorizontal == 0 ) {
	  		mover = 1;
		}

	  	/*
            Testando o deslocamento do PEÃO(BRANCO) representada pela LETRA "p"
            Se a peça que foi mexida é o PEÃO e o resulta da linha de destino menos(-) a linha de destino é igual a um(1) e o
            deslocamento da horizontal é igual a zero(0)
            indica que o usuário pode mover
	  	*/
	  	if((peca == 'p') && ( (linOri - linDes) == 1 ) && deslocamentoHorizontal == 0 ) {
	  		mover = 1;
		}

	    if(mover == 1) {// Verifica se o usuário pode mover
	    	xadrez[linDes][colDes] = xadrez[linOri][colOri]; //Move a peça da origem para o destino
	    	xadrez[linOri][colOri] = ' '; // deixa a origem vazia
	    	return 1; // Movimento realizado com sucesso
		} else {
		  	return 9; // Retorna o valor 9 para informar que a peça não pode ser movida desse jeito
		}
	  }
	  else
	  {
	    return 0;// Retorna zero(0) de coordenadas inválidas
	  }
}
// Essa função vai capturar o evento do mouse
int capturaEventos() {
    int e = SDL_PollEvent(&evento);// Pega qual evento
    if(evento.type == SDL_QUIT) {// Verifica se o evento chamado foi para sair
        exit(0);// Finaliza o jogo de xadrez
    } else if(evento.type == SDL_MOUSEMOTION) {// Verifica se o tipo do evento é com o mouse e pega a posição do mouse
        //printf(" X : %d,  Y : %d \n", evento.motion.x, evento.motion.y);// Um teste para ver a posição do mouse quando é movido
    } else if(evento.type == SDL_MOUSEBUTTONDOWN) { // Verifica se o usuário clicou com o mouse
        //printf(" X : %d,  Y : %d \n", evento.motion.x, evento.motion.y);// Um teste para ver a posição clique do mouse quando precionado
        int w  = WIDTH / 8;// Pega a largura da celula clicada
        int h  = HEIGHT / 8;// Pega a altura da celula clicada
        int coluna = evento.motion.x / w;// Descobre a coluna que foi selecionada pelo usuário
        int linha = evento.motion.y / h;// Descobre a linha que foi selecionada pelo usuário
        if(linhaOrigem == -1) {// se a linha de origem for igual a -1, linha de origem vai receber o valor onde deve ficar nova peça movida
            linhaOrigem = linha; // Linha de origem recebe a linha selecionada
            colunaOrigem = coluna; // coluna de origem recebe a coluna selecionada
        } else {
            // Caso contrário essa linha já havia sido marcada e vamos mover a peça para esse lugar
            moverPeca(linhaOrigem, colunaOrigem, linha, coluna);// Chama a função mover peça passando a linha de origem, coluna de origem, a linha atual e coluna atual
            linhaOrigem = -1;// Reinicia com o valor negativo para a linha de origem como se tivesse no inicio do jogo
            colunaOrigem = -1;// Reinicia com o valor negativo para a coluna de origem como se tivesse no inicio do jogo
        }

        printf(" Linha : %d,  Coluna : %d \n", linha, coluna);// Mostra qual foi a linha e coluna que foi clicada
    }
}
// Função principal onde inicia o programa
int main(int argc, char **args)
{
  init_graficos();// Chama a função para iniciar o gráfico do xadrez
  init_imagens();// Chama a função para pintar a tela com as imagens do xadrez

  while (1)// Fica rodando e chamando as funções do xadrez
  {
    pintarTela(); // Chama a função para pintar a tela
    capturaEventos();// Chama a função que captura os eventos do mouse
    SDL_Delay( 15 );// Aplica um pequeno delay(espera)
  }

  getch();// Espera o usuário interagir com o xadrez
  return 0;
}



