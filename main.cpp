#include <stdio.h>// Essa bibliotca � usada quando queremos adicionar outras fun��es predefinidas da biblioteca de fun��es do pr�-processado
#include <stdlib.h> //Essa bibliotca possui fun��es envolvendo aloca��o de mem�ria, controle de processos, convers�es e outras...
#include <conio.h>// Essa  biblioteca(arquivo cabe�alho) de C usado principalmente por compiladores MS-DOS para fornecer input / output console

#include <SDL2/SDL.h>// Essa � uma bibliotca para trabalhar com a tela gr�fica
#include <SDL2/SDL_image.h> // Essa biblioteca � usada para carregar as imagens do arquivo

#define WIDTH 640 // Define o comprimento do xadrez
#define HEIGHT 480 // Define a altura do xadrez
#define TOTAL_PECAS 12  // Define a quantidade de pecas

// Fonte do exemplo: https://www.youtube.com/watch?v=3JLFZWxTruM

SDL_Window  * window;// Vari�vel � um ponteiro que ser� usada para desenhar a tela do xadrez
SDL_Renderer * render;// Vari�vel � um ponteiro que ser� usada para renderizar a tela do xadrez
SDL_Event evento;// Vari�vel para usar nos eventos da tela

// Cria uma matriz com 2 dimens�es 8 por 8 posi��es
char xadrez[8][8] = {
    {'T', 'C', 'B', 'Q', 'K', 'B', 'C', 'T'},// Cria as pe�as PRETAS na parte de cima representada por LETRAS mai�scula
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'t', 'c', 'b', 'q', 'k', 'b', 'c', 't'}};// Cria as pe�as BRANCAS na parte de baixo representada por LETRAS min�scula

SDL_Texture * imagens[TOTAL_PECAS]; // Carrega as imagens no vetor com 12 posi��es

// Esse vetor de duas posi��es quarda os nomes das imagens que est� no disco/arquivo
char nomesImagens[][50] = {
    "BB.PNG", "BP.PNG", "CB.PNG", "CP.PNG", "KB.PNG", "KP.PNG",
    "QB.PNG", "QP.PNG", "PB.PNG", "PP.PNG", "TB.PNG", "TP.PNG"
};
// Esse vetor � para representar qual pe�a representa a sua imagem, Tipo quem representa a imagem do cavalo? 'c' e 'C' etc..
char pecas[] = {'b', 'B', 'c', 'C', 'k', 'K', 'q', 'Q', 'p', 'P', 't', 'T'};
int linhaOrigem = -1; // Quarda o valor da linha de origem quando foi clicada pela primeira vez, iniciando com -1
int colunaOrigem = -1; // Quarda o valor da coluna de origem quando foi clicada pela primeira vez, iniciando com -1

// Fun��o que inicializa a tela gr�fica
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

// Fun��o para carregar as imagens do arquivo para o xadrez
int init_imagens() {
    if(IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG) {// Inicia verificando o tipo das imagens
        int i;
        //printf(" Carregando imagens .... \n");
        for(i = 0; i < TOTAL_PECAS; i++){
            char nomeImg[50] = "imagens/"; // Pega o caminho da imagem
            strcat(nomeImg, nomesImagens[i]);// Concatena o caminho da imagem com o nome da imagem na posi��o i do la�o de repeti��o
            SDL_Surface * surImage = IMG_Load(nomeImg);// Carrega a imagem que est� na variavel: nomeImg
            imagens[i] = SDL_CreateTextureFromSurface(render, surImage);// Aqui � onde carrega propriamente dita no array imagens
        }
    }
}
// Essa fun��o recebe um caractere e retorna uma imagem
SDL_Texture * imagemFromPeca(char peca) {
    int i;
    for (i = 0; i < TOTAL_PECAS; i++) {
        if(pecas[i] == peca) {// Verifica a imagem correspondente ao caractere
            return imagens[i]; // Retorna a imagem que corresponde ao caractere
        }
    }
}

// Fun��o que pinta a tela com as imagens do xadrez
int pintarTela() {
    int w = WIDTH / 8; // Define a largura(pixel) de cada quadrado do xadrez
    int h = HEIGHT / 8; // Define a altura(pixel) de cada quadrado do xadrez
    int lin, col;// cria as v�riaveis do tipo inteiro para linhas(lin) coluna(col)

    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);// Limpando a tela pintando de preto
    SDL_RenderClear(render);// limpa o render

    for (lin = 0; lin < 8; lin++)// Inicia o la�o da linha do xadrez
    {
          for (col = 0; col < 8; col++)// Inicia o la�o da coluna do xadrez
          {
              if((col + lin) % 2 == 0){
                    /*
                    Para pintar a tela, COR sim COR n�o, usamos a soma da coluna com a linha divide
                    por dois e pegamos o resto, depois verifica se o resto � zero(0)
                    */
                SDL_SetRenderDrawColor(render, 240,255,255, 255);
              } else {
                SDL_SetRenderDrawColor(render, 176,196,222, 255);
              }

              int x = col * w;// X indica onde come�a cada casinha do quadrado, ou seja o X come�a do zero(0), depois oitenta(80) etc... at� chegar 640px
              int y = lin * h;// Y indica onde termina cada casinha do quadrado, funciona do mesmo jeito que o Y s� que usando a linha
              SDL_Rect rDest = {x, y, w, h};// Cria o ret�ngulo de destino, definido a altura e largura, informando onde come�a, onde termina, qual a largura e altura
              SDL_RenderFillRect(render, &rDest);// Renderiza o ret�ngulo
              // Mostrando as pe�as no tabuleiro
              char p = xadrez[lin][col];//
              if(p != ' ') {// Verifica � uma pe�a ou um espa�o vazio
                int wOri, hOri; // Variavel para pegar a largura total da imagem e altura total da imagem
                SDL_Texture * img = imagemFromPeca( p );//
                SDL_QueryTexture(img, NULL, NULL, &wOri, &hOri);//
                SDL_Rect rOrig = {0, 0, wOri, hOri}; // Criar o ret�ngulo de origem
                SDL_RenderCopy(render, img, &rOrig, &rDest); //
              }

              // Aqui marca a celula selecionada
              if(linhaOrigem == lin && colunaOrigem == col) {//Verifica se a linha e coluna de origem � igual e linha e coluna que est� sendo pintada
                SDL_SetRenderDrawColor(render, 0, 0, 255, 255);// Pinha a celula de verde
                SDL_RenderDrawRect(render, &rDest);// Renderiza a celula pintada de verde
              }
          }
    }

    SDL_RenderPresent(render);// Apresenta a tela com grafico
}
    /*
        Fun��o usada para mover as pe�as, essa fun��o pega por paramentros a linha de origem,
        coluna de origem, linha de destino e coluna de destino esse fun��o vai retornar um(1) se tiver tudo certo,
        caso contr�rio rtorna zero(0)
    */
int moverPeca(int linOri, int colOri, int linDes, int colDes) {
	int mover = 0;// Vari�vel que indica se o usu�rio pode mover ou n�o a pe�a
	int peca; // Vari�vel que indica a pe�a
	// calcula o deslocamento da linha vertical
	int deslocamentoVertical = abs(linDes - linOri); // cacula o deslocamento vertical e pegar o valor absoluto, para isso usamos a fun��o "abs" retorna o valor absoluto
	// calcula o deslocamento da linha horizontal
	int deslocamentoHorizontal = abs(colDes - colOri); // cacula o deslocamento horizontal e pegar o valor absoluto, para isso usamos a fun��o "abs" retorna o valor absoluto
    // Esse primeiro if, verifica se o deslocamento das linha e colunas est�o dentro do tabuleiro
    // se a linha e coluna de origem tiver entre 0 e 8 est� dentro do tabuleiro
    // a mesma regra se aplica a linha e coluna de destino
    // essa verifi��o: (deslocamentoVertical + deslocamentoHorizontal > 0) � para analisar se o usu�rio mexeu na mesma casa, ou seja: saiu dela pra ela mesmo
	if ( 	(linOri >= 0 && linOri < 8 && colOri >= 0 && colOri < 8) &&
	  		(linDes >= 0 && linDes < 8 && colDes >= 0 && colDes < 8) &&
            (deslocamentoVertical + deslocamentoHorizontal > 0)) {
	  	peca = xadrez[linOri][colOri];// pega a pe�a do xadrez na linha de origem e coluna de origem

	  	/*
            Testando deslocamento da TORRE representada pela LETRA "T" ou "t"
            Se a pe�a que foi mexida � a TORRE e o deslocamento da vertical for igual(0) ou o
            deslocamento da horizontal for igual(0), isso indica que o usu�rio pode mover
	  	*/
	  	if((peca == 'T' || peca == 't') && (deslocamentoVertical == 0 || deslocamentoHorizontal == 0)) {
	  		mover = 1;
		}
	  	/*
            Testando o deslocamento do BISPO representada pela LETRA "B" ou "b"
            Se a pe�a que foi mexida � o BISPO e o deslocamento da vertical for igua ao
            deslocamento horizontal, indica que o usu�rio pode mover
	  	*/
	  	if((peca == 'B' || peca == 'b') && (deslocamentoVertical == deslocamentoHorizontal)) {
	  		mover = 1;
		}
	  	/*
            Testando o deslocamento do CAVALO representada pela LETRA "C" ou "c"
            Se a pe�a que foi mexida � o CAVALO e o deslocamento da vertical for igual a um(1) e
            deslocamento horizontal for igual a dois(2) OU deslocamento da vertical for igual a dois(2) e
            deslocamento horizontal for igual a um(1)
            indica que o usu�rio pode mover
	  	*/
	  	if((peca == 'C' || peca == 'c') &&
		  ( (deslocamentoVertical == 1 && deslocamentoHorizontal == 2) ||
		  (deslocamentoVertical == 2 && deslocamentoHorizontal == 1) ) ) {
	  		mover = 1;
		}
	  	/*
            Testando o deslocamento do RAINHA representada pela LETRA "Q" ou "q"
            Se a pe�a que foi mexida � o RAINHA e o deslocamento da vertical � igual ao deslocamento horizontal ou
            deslocamento da vertical � igual zero(0) ou o deslocamento horizontal � igual zero(0)
            indica que o usu�rio pode mover
	  	*/
	  	if((peca == 'Q' || peca == 'q') &&
		  ((deslocamentoVertical == deslocamentoHorizontal) || (deslocamentoVertical == 0 || deslocamentoHorizontal == 0) )) {
	  		mover = 1;
		}
	  	/*
            Testando o deslocamento do REI representada pela LETRA "K" ou "k"
            Se a pe�a que foi mexida � o REI e o deslocamento da vertical � maior ou igual que zero(0) e menor ou igual a um(1) e o
            deslocamento da horizontal � maior ou igual a zero(0) e o menor igual a um(1)
            indica que o usu�rio pode mover
	  	*/
	  	if((peca == 'K' || peca == 'k') &&
		  ((deslocamentoVertical >= 0 && deslocamentoVertical <= 1) && (deslocamentoHorizontal >= 0 && deslocamentoHorizontal <= 1) )) {
	  		mover = 1;
		}
	  	/*
            Testando o deslocamento do PE�O(PRETO) representada pela LETRA "P"
            Se a pe�a que foi mexida � o PE�O e o resulta da linha de origem menos(-) a linha de origem � igual a um(1) e o
            deslocamento da horizontal � igual a zero(0)
            indica que o usu�rio pode mover
	  	*/
	  	if((peca == 'P') && ( (linDes - linOri) == 1 ) && deslocamentoHorizontal == 0 ) {
	  		mover = 1;
		}

	  	/*
            Testando o deslocamento do PE�O(BRANCO) representada pela LETRA "p"
            Se a pe�a que foi mexida � o PE�O e o resulta da linha de destino menos(-) a linha de destino � igual a um(1) e o
            deslocamento da horizontal � igual a zero(0)
            indica que o usu�rio pode mover
	  	*/
	  	if((peca == 'p') && ( (linOri - linDes) == 1 ) && deslocamentoHorizontal == 0 ) {
	  		mover = 1;
		}

	    if(mover == 1) {// Verifica se o usu�rio pode mover
	    	xadrez[linDes][colDes] = xadrez[linOri][colOri]; //Move a pe�a da origem para o destino
	    	xadrez[linOri][colOri] = ' '; // deixa a origem vazia
	    	return 1; // Movimento realizado com sucesso
		} else {
		  	return 9; // Retorna o valor 9 para informar que a pe�a n�o pode ser movida desse jeito
		}
	  }
	  else
	  {
	    return 0;// Retorna zero(0) de coordenadas inv�lidas
	  }
}
// Essa fun��o vai capturar o evento do mouse
int capturaEventos() {
    int e = SDL_PollEvent(&evento);// Pega qual evento
    if(evento.type == SDL_QUIT) {// Verifica se o evento chamado foi para sair
        exit(0);// Finaliza o jogo de xadrez
    } else if(evento.type == SDL_MOUSEMOTION) {// Verifica se o tipo do evento � com o mouse e pega a posi��o do mouse
        //printf(" X : %d,  Y : %d \n", evento.motion.x, evento.motion.y);// Um teste para ver a posi��o do mouse quando � movido
    } else if(evento.type == SDL_MOUSEBUTTONDOWN) { // Verifica se o usu�rio clicou com o mouse
        //printf(" X : %d,  Y : %d \n", evento.motion.x, evento.motion.y);// Um teste para ver a posi��o clique do mouse quando precionado
        int w  = WIDTH / 8;// Pega a largura da celula clicada
        int h  = HEIGHT / 8;// Pega a altura da celula clicada
        int coluna = evento.motion.x / w;// Descobre a coluna que foi selecionada pelo usu�rio
        int linha = evento.motion.y / h;// Descobre a linha que foi selecionada pelo usu�rio
        if(linhaOrigem == -1) {// se a linha de origem for igual a -1, linha de origem vai receber o valor onde deve ficar nova pe�a movida
            linhaOrigem = linha; // Linha de origem recebe a linha selecionada
            colunaOrigem = coluna; // coluna de origem recebe a coluna selecionada
        } else {
            // Caso contr�rio essa linha j� havia sido marcada e vamos mover a pe�a para esse lugar
            moverPeca(linhaOrigem, colunaOrigem, linha, coluna);// Chama a fun��o mover pe�a passando a linha de origem, coluna de origem, a linha atual e coluna atual
            linhaOrigem = -1;// Reinicia com o valor negativo para a linha de origem como se tivesse no inicio do jogo
            colunaOrigem = -1;// Reinicia com o valor negativo para a coluna de origem como se tivesse no inicio do jogo
        }

        printf(" Linha : %d,  Coluna : %d \n", linha, coluna);// Mostra qual foi a linha e coluna que foi clicada
    }
}
// Fun��o principal onde inicia o programa
int main(int argc, char **args)
{
  init_graficos();// Chama a fun��o para iniciar o gr�fico do xadrez
  init_imagens();// Chama a fun��o para pintar a tela com as imagens do xadrez

  while (1)// Fica rodando e chamando as fun��es do xadrez
  {
    pintarTela(); // Chama a fun��o para pintar a tela
    capturaEventos();// Chama a fun��o que captura os eventos do mouse
    SDL_Delay( 15 );// Aplica um pequeno delay(espera)
  }

  getch();// Espera o usu�rio interagir com o xadrez
  return 0;
}



