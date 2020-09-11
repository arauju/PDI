#include "CImg.h"
#include <cmath>
#include <iostream>
#include <stdlib.h>
//mediana, media, max, min e sobel com 2 mascaras
using namespace std;
using namespace cimg_library;


int filtroMedia[3][3]={{1,1,1},{1,1,1},{1,1,1}};

int filtroMediana[3][3]={{1,1,1},{1,1,1},{1,1,1}};

int filtroSobelY[3][3]={{1,0,-1},
						{2,0,-2},
						{1,0,-1}};

int filtroSobelX[3][3]={{1,2,1},
						{0,0,0},
						{-1,-2,-1}};

float peso;
float Zmin = 255;
float Zmax = 0;

void toBW(CImg<unsigned char> &img);
void media(CImg<unsigned char> &img, CImg<unsigned char> &saida);

float acharMediana(float vetor[9]);

void mediana(CImg<unsigned char> &img, CImg<unsigned char> &saida);

void convolucaoX(CImg<unsigned char> &img, CImg<unsigned char> &saida);

void convolucaoY(CImg<unsigned char> &img, CImg<unsigned char> &saida);

void getMagnitude(CImg<unsigned char> &img, CImg<unsigned char> &saida);

//	Sobel magido do Madeira
// void getMagnitude(CImg<unsigned char> &img, CImg<unsigned char> &saida){
// 	CImgList<unsigned char> sobel = img.get_gradient("xy",2);
// 	saida = (sobel(0).sqr() + sobel(1).sqr()).sqrt().normalize(0,255);
// }

void RGBtoXYZ();


