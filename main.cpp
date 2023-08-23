#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WIDTH 640
#define HEIGHT 480
#define TOTAL_PECAS 12

// Fonte: https://www.youtube.com/watch?v=3JLFZWxTruM

SDL_Window  * window;
SDL_Renderer * render;
SDL_Event evento;

char xadrez[8][8] = {
    {'T', 'C', 'B', 'Q', 'K', 'B', 'C', 'T'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'t', 'c', 'b', 'q', 'k', 'b', 'c', 't'}};

SDL_Texture * imagens[TOTAL_PECAS];
char nomesImagens[][50] = {
    "BB.PNG", "BP.PNG", "CB.PNG", "CP.PNG", "KB.PNG", "KP.PNG",
    "QB.PNG", "QP.PNG", "PB.PNG", "PP.PNG", "TB.PNG", "TP.PNG"
};

char pecas[] = {'b', 'B', 'c', 'C', 'k', 'K', 'q', 'Q', 'p', 'P', 't', 'T'};
int linhaOrigem = -1;
int colunaOrigem = -1;

int init_graficos() {
	if(SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
		printf("\n Inicializou o SDL ... ");
		window = SDL_CreateWindow("X A D R E Z - RIVA && ACSA", 50, 50, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
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

int init_imagens() {
    if(IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG) {
        int i;
        printf(" Carregando imagens .... \n");
        for(i = 0; i < TOTAL_PECAS; i++){
            char nomeImg[50] = "imagens/";
            strcat(nomeImg, nomesImagens[i]);
            printf(" Imagem :%s \n", nomeImg);
            SDL_Surface * surImage = IMG_Load(nomeImg);
            imagens[i] = SDL_CreateTextureFromSurface(render, surImage);
        }
    }
}

SDL_Texture * imagemFromPeca(char peca) {
    int i;
    for (i = 0; i < TOTAL_PECAS; i++) {
        if(pecas[i] == peca) {
            return imagens[i];
        }
    }
}

int pintarTela() {
    int w = WIDTH / 8;
    int h = HEIGHT / 8;
    int lin, col;

    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);// Limpando a tela
    SDL_RenderClear(render);// Pintando a tela de preto

    for (lin = 0; lin < 8; lin++)
    {
          for (col = 0; col < 8; col++)
          {
               /*
              if((col + lin) % 2 == 0){
                SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
              } else {
                SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
              }
             */
/*
              if((col + lin) % 2 == 0){
                SDL_SetRenderDrawColor(render, 164, 147, 196, 255);
              } else {
                SDL_SetRenderDrawColor(render, 196, 147, 147, 255);
              }
              */
              if((col + lin) % 2 == 0){
                SDL_SetRenderDrawColor(render, 240,255,255, 255);
              } else {
                SDL_SetRenderDrawColor(render, 176,196,222, 255);
              }

              int x = col * w;
              int y = lin * h;
              SDL_Rect r = {x, y, w, h};
              SDL_RenderFillRect(render, &r);
              char p = xadrez[lin][col];
              if(p != ' ') {
                int wOri, hOri;
                SDL_Texture * img = imagemFromPeca( p );
                SDL_QueryTexture(img, NULL, NULL, &wOri, &hOri);
                SDL_Rect rOrig = {0, 0, wOri, hOri};
                SDL_RenderCopy(render, img, &rOrig, &r);
              }

              if(linhaOrigem == lin && colunaOrigem == col) {
                SDL_SetRenderDrawColor(render, 0, 0, 255, 255);
                SDL_RenderDrawRect(render, &r);
              }
          }
    }

    SDL_RenderPresent(render);
}

int pintarTelaTexto()
{
  system("CLS");
  int lin, col, r;
  for (lin = 0; lin < 8; lin++)
  {
    for (r = 0; r < 3; r++)
    {
      for (col = 0; col < 8; col++)
      {
        char peca = xadrez[lin][col];
        char letra = ((lin + col) % 2 == 0) ? '\xB2' : ' ';
        letra = (r == 1 && peca != ' ') ? peca : letra;
        if ((lin + col) % 2 == 0)
        { // se a divisao do valor for par pinta a casa
          printf("\xB2\xB2%c\xB2\xB2", letra);
        }
        else
        {
          printf("  %c  ", letra);
        }
      }
      printf("\n");
    }
  }
}

int moverPeca(int linOri, int colOri, int linDes, int colDes) {
	int mover = 0;
	int peca;
	// calcula o deslocamento da linha vertical
	int deslocamentoVertical = abs(linDes - linOri); // obs: a função abs retorna o valor absoluto
	// calcula o deslocamento da linha horizontal
	int deslocamentoHorizontal = abs(colDes - colOri);

	if ( 	(linOri >= 0 && linOri < 8 && colOri >= 0 && colOri < 8) &&
	  		(linDes >= 0 && linDes < 8 && colDes >= 0 && colDes < 8) &&
            (deslocamentoVertical + deslocamentoHorizontal > 0)) {
	  	peca = xadrez[linOri][colOri];

	  	// Testando deslocamento da torre
	  	if((peca == 'T' || peca == 't') && (deslocamentoVertical == 0 || deslocamentoHorizontal == 0)) {
	  		mover = 1;
		}
		// Testando o deslocamento do bispo
	  	if((peca == 'B' || peca == 'b') && (deslocamentoVertical == deslocamentoHorizontal)) {
	  		mover = 1;
		}

		// Testando o deslocamento do cavalo
	  	if((peca == 'C' || peca == 'c') &&
		  ( (deslocamentoVertical == 1 && deslocamentoHorizontal == 2) ||
		  (deslocamentoVertical == 2 && deslocamentoHorizontal == 1) ) ) {
	  		mover = 1;
		}

	  	// Testando deslocamento da reainha
	  	if((peca == 'Q' || peca == 'q') &&
		  ((deslocamentoVertical == deslocamentoHorizontal) || (deslocamentoVertical == 0 || deslocamentoHorizontal == 0) )) {
	  		mover = 1;
		}

	  	// Testando deslocamento da rei
	  	if((peca == 'K' || peca == 'k') &&
		  ((deslocamentoVertical >= 0 && deslocamentoVertical <= 1) && (deslocamentoHorizontal >= 0 && deslocamentoHorizontal <= 1) )) {
	  		mover = 1;
		}

	  	// Testando deslocamento do peao preto"Maiusculo"
	  	if((peca == 'P') && ( (linDes - linOri) == 1 ) && deslocamentoHorizontal == 0 ) {
	  		mover = 1;
		}

	  	// Testando deslocamento do peao branco"Minusculo"
	  	if((peca == 'p') && ( (linOri - linDes) == 1 ) && deslocamentoHorizontal == 0 ) {
	  		mover = 1;
		}

	    if(mover == 1) {
	    	xadrez[linDes][colDes] = xadrez[linOri][colOri];
	    	xadrez[linOri][colOri] = ' ';
	    	return 1; // Movimento realizado com sucesso
		} else {
		  	return 9; // Peca nao pode ser movida desse jeito
		}

	  }
	  else
	  {
	    return 0;
	  }
}

int capturaEventos() {
    int e = SDL_PollEvent(&evento);
    if(evento.type == SDL_QUIT) {
        exit(0);
    } else if(evento.type == SDL_MOUSEMOTION) {
        //printf(" X : %d,  Y : %d \n", evento.motion.x, evento.motion.y);
    } else if(evento.type == SDL_MOUSEBUTTONDOWN) {
        //printf(" X : %d,  Y : %d \n", evento.motion.x, evento.motion.y);
        int w  = WIDTH / 8;
        int h  = HEIGHT / 8;
        int coluna = evento.motion.x / w;
        int linha = evento.motion.y / h;
        if(linhaOrigem == -1) {
            linhaOrigem = linha;
            colunaOrigem = coluna;
        } else {
            moverPeca(linhaOrigem, colunaOrigem, linha, coluna);
            linhaOrigem = -1;
            colunaOrigem = -1;
        }

        printf(" Linha : %d,  Coluna : %d \n", linha, coluna);
    }
}

int main(int argc, char **args)
{
  init_graficos();
  init_imagens();

  int linOri, colOri, linDes, colDes;
  while (1)
  {
    pintarTela();
    capturaEventos();
    SDL_Delay( 15 );
  }

  getch();
  return 0;
}



