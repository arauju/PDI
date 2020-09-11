#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "CImg.h"
using namespace std;

using namespace cimg_library;
void limiarizacao(CImg<unsigned char> *image,CImg<unsigned char> *imgSaida);
int defineTamZoom(CImg<unsigned char> *image,int cor);
void fazRuidoSalPimenta(CImg<unsigned char> *image,int porcentagem);
void fazRuidoAleatorio(CImg<unsigned char> *image,int intervalo);
int* returnPixelSemRuido(CImg<unsigned char> *image);
void media(CImg<unsigned char> *img, CImg<unsigned char> *saida);
int achaMoeda(CImg<unsigned char> *image);
void preencherBranco(CImg<unsigned char> *image,int linha, int coluna,int tam);
float calcularValor(int tam);

const int zoomPadrao = 900;
const int umC = 7580; 
const int dezC = 10388;
const int cincoC = 12425;
const int cinquentaC = 13701;
const int vinteCinco = 16120;
const int umReal = 18773;
