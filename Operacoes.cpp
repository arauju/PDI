#include <cmath>
#include <iostream>
#include <stdlib.h>
#include "Operacoes.h"
#include "CImg.h"
//mediana, media, max, min e sobel com 2 mascaras
using namespace std;
using namespace cimg_library;

CImg<unsigned char> imgEntrada("balloons_noisy.png"),
		imgSaidaMedia(imgEntrada.width(),imgEntrada.height(),imgEntrada.depth(),1),
		imgSobelX(imgEntrada.width(),imgEntrada.height(),imgEntrada.depth(),1),
		imgSobelY(imgEntrada.width(),imgEntrada.height(),imgEntrada.depth(),1),
		aux(imgEntrada.width(),imgEntrada.height(),imgEntrada.depth(),1),
		
		R(imgEntrada.width(),imgEntrada.height(),imgEntrada.depth(),1), //Imagem com a dimensão R
		G(imgEntrada.width(),imgEntrada.height(),imgEntrada.depth(),1), //Imagem com a dimensão G
		B(imgEntrada.width(),imgEntrada.height(),imgEntrada.depth(),1); //Imagem com a dimensão B;
CImg<unsigned char> imgSaidaMediana(imgEntrada.width(),imgEntrada.height(),imgEntrada.depth(),3),
					C1(imgEntrada.width(),imgEntrada.height(),imgEntrada.depth(),1),
 					C2(imgEntrada.width(),imgEntrada.height(),imgEntrada.depth(),1),
 					C3(imgEntrada.width(),imgEntrada.height(),imgEntrada.depth(),1);		



void toBW(CImg<unsigned char> &img) {
    if(img.spectrum() == 1)
        return;
    else {
        float bw;
        for(int row = 0; row < img.height(); row++) {
		    for(int col = 0; col < img.width(); col++) {
		        bw = 0.299f * img.atXYZC(col, row, 0, 0) + 0.587f * img.atXYZC(col, row, 0, 1) + 0.114f * img.atXYZC(col, row, 0, 2);
		        img.atXY(col, row) = (unsigned char) (bw);
		    }
		}
    }
}

void media(CImg<unsigned char> &img, CImg<unsigned char> &saida) {
	for(int row = 0; row < imgEntrada.height(); row++) {
		for(int col = 0; col < imgEntrada.width(); col++) {
			float acum = 0; peso=0;
			for(int i = -1; i <= 1; i++) {
				for(int j = -1; j <= 1; j++) {
					unsigned char k = img.atXY(col+j,row+i);
					peso+= filtroMedia[1+i][1+j];
					acum += k * filtroMedia[1+i][1+j];
					// printf("F: %f\n", filtroMedia[1+i][1+j]);
				}
			}
			acum/=peso;

			saida.atXY(col, row) = (unsigned char) (acum);
		}
	}
}

float acharMediana(float vetor[9]) {
	float mediana, temp;
	for (int i = 0; i < 9; ++i)
	{
		for (int j = i; j < 9; ++j)
		{
			if (vetor[j] < vetor[i])
			{
				temp = vetor[i];
				vetor[i] = vetor[j];
				vetor[j] = temp;
			}	
		}
	}

	return vetor[4];	//Mediana
	// return vetor[0];	//Minima
	// return vetor[8];	//Maxima
}

void mediana(CImg<unsigned char> &img, CImg<unsigned char> &saida) {
	float vetor[9] = {0};
	float mediana;
	int a=0;
	for(int row = 0; row < imgEntrada.height(); row++) {
		for(int col = 0; col < imgEntrada.width(); col++) {
			float acum = 0; peso=0;
			a = 0;
			for(int i = -1; i <= 1; i++) {
				for(int j = -1; j <= 1; j++) {
					unsigned char k = img.atXY(col+j,row+i);
					// peso+= filtroMedia[1+i][1+j];
					vetor[a++] = k;
				}
			}
			mediana = acharMediana(vetor);
			// acum/=peso;
			saida.atXY(col, row) = mediana;
		}
	}
}

void convolucaoX(CImg<unsigned char> &img, CImg<unsigned char> &saida) {
	for(int row = 0; row < imgEntrada.height(); row++) {
		for(int col = 0; col < imgEntrada.width(); col++) {
			float acum = 0; peso=0;
			for(int i = -1; i <= 1; i++) {
				for(int j = -1; j <= 1; j++) {
					unsigned char k = img.atXY(col+j,row+i);
					// peso+=kernel[1+i][1+j];
					acum += k * filtroSobelX[1+i][1+j];
				}
			}
			// acum/=peso;
			saida.atXY(col, row) = (unsigned char) (acum);
		}
	}
}

void convolucaoY(CImg<unsigned char> &img, CImg<unsigned char> &saida) {
	for(int row = 0; row < imgEntrada.height(); row++) {
		for(int col = 0; col < imgEntrada.width(); col++) {
			float acum = 0; peso=0;
			for(int i = -1; i <= 1; i++) {
				for(int j = -1; j <= 1; j++) {
					unsigned char k = img.atXY(col+j,row+i);
					// peso+=kernel[1+i][1+j];
					acum += k * filtroSobelY[1+i][1+j];
				}
			}
			// acum/=peso;
			saida.atXY(col, row) = (unsigned char) (acum);
		}
	}
}

void getMagnitude(CImg<unsigned char> &img, CImg<unsigned char> &saida){
 	float pY,pX;
 	for(int row = 0; row < imgEntrada.height(); row++) {
 		for(int col = 0; col < imgEntrada.width(); col++) {
 			pY = saida.atXY(col, row) * saida.atXY(col, row);
 			pX = img.atXY(col,row) * img.atXY(col,row);
 			saida.atXY(col, row) = sqrt(pY + pX);
 		}
 	}
 }

//	Sobel magido do Madeira
// void getMagnitude(CImg<unsigned char> &img, CImg<unsigned char> &saida){
// 	CImgList<unsigned char> sobel = img.get_gradient("xy",2);
// 	saida = (sobel(0).sqr() + sobel(1).sqr()).sqrt().normalize(0,255);
// }

void RGBtoXYZ() {
	float r, g, b, x, y, z;
	for(int i=0; i < imgEntrada.width(); i++) {
		for(int j=0; j < imgEntrada.height(); j++) {
			//Escalando R,G e B de [0,255] para [0,1]
			r = R.atXY(i,j)/255.0f;
			g = G.atXY(i,j)/255.0f;
			b = B.atXY(i,j)/255.0f;
			//Calculando X, Y, e Z
			x = (r*0.409 + g*0.310 + b*0.200);
			y = (r*0.177 + g*0.813 + b*0.010);
			z = (r*0.000 + g*0.010 + b*0.990);
			//Jogando para as imagens X, Y, e Z, reescalando para [0,255] de volta
			C1.atXY(i,j) = x*255;
			C2.atXY(i,j) = y*255;
			C3.atXY(i,j) = z*255;
			//Criando a imagem de 3 canais
			imgSaidaMediana.atXYZC(i,j,0,1) = C1.atXY(i,j);
			imgSaidaMediana.atXYZC(i,j,0,2) = C2.atXY(i,j);
			imgSaidaMediana.atXYZC(i,j,0,3) = C3.atXY(i,j);
		}
	}
}


int main(void) {

	R = imgEntrada.get_channel(0);
	G = imgEntrada.get_channel(1);	
	B = imgEntrada.get_channel(2);

	// mediana(R, R);
	// R = aux;
	// mediana(G, G);
	// G = aux;
	// mediana(B, B);
	// B = aux;

	// RGBtoXYZ();
	// toBW(imgEntrada);
	media(imgEntrada, imgSaidaMedia);
	mediana(imgEntrada, imgSaidaMediana);
	convolucaoX(imgEntrada, imgSobelX);
	convolucaoY(imgEntrada, imgSobelY);
	getMagnitude(imgSobelX,imgSobelY);


	printf("Maxima: %f\n", Zmax);
	printf("Minima: %f\n", Zmin);

	CImgDisplay displays[4];
	displays[0].display(imgEntrada);
	displays[1].display(imgSaidaMedia);
	displays[2].display(imgSaidaMediana); 
	displays[3].display(imgSobelY); 
	displays[0].set_title("Imagem Entrada");
	displays[1].set_title("Imagem Media");
	displays[2].set_title("Imagem Mediana");
	displays[3].set_title("Imagem Sobel");

	// imgSaidaMediana.save_png("1.png");

	bool isAnyDisplayClosed = false;
	while (!isAnyDisplayClosed) {
		for(int i = 0; i < 4; i++) {
			if(displays[i].is_closed()) {
				isAnyDisplayClosed = true;
				break;
			}
		}
		for(int i = 0; i < 4; i++) {
			if(displays[i].is_closed()) {
				displays[i].wait();
			}
		}
    
	}
	return 0;
}
