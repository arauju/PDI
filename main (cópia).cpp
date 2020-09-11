#include "main.h"

int main(int argc, char *argv[]){
char *imgEntrada;
int ruido = 0; //ruido 1, sal e pimenta e ruido 2, aleatorio.
int porcentagem = 0; //caso do sal e pimenta
int intervalo = 0; //caso do aleatorio
if(argc > 1){
	imgEntrada = argv[1];
	if(argc > 3){
		ruido = (int) *argv[2] - 48;
		if(ruido == 1){
			porcentagem = atoi(argv[3]);
			if(porcentagem < 0 && porcentagem > 100){
				cout << "Falha ao digitar Porcentagem." << endl;
				exit(2);		
			}
		}if(ruido == 2)
			intervalo = atoi(argv[3]);
		if(ruido < 1 || ruido > 2){ 
			cout << "Falha ao digitar ruído." << endl;
			exit(1);		
		}
	
	}
}
CImg<unsigned char> image(imgEntrada),
imgSaida(image.width(),image.height(),image.depth(),1,0);

if(ruido == 1){ 
	fazRuidoSalPimenta(&image,porcentagem);
	if((strcmp (argv[argc-1], "debug") == 0)){
		cout << "Imagem Ruido sal e pimenta salva! com porcentagem: " << porcentagem << endl;
		image.get_normalize(0,255).save_png("saida_RuidoSalPim.png");
	}
}else if(ruido == 2){ 
	fazRuidoAleatorio(&image,intervalo);
	if((strcmp (argv[argc-1], "debug") == 0)){
		cout << "Imagem Ruido Aleatorio salva! com intervalo: " << intervalo << endl;
		image.get_normalize(0,255).save_png("saida_RuidoAleatorio.png");
	}
}


//media pra ajudar na limpeza da imagem
if(ruido == 1){
	image.blur_median(10, 0);
	if((strcmp (argv[argc-1], "debug") == 0)){
		cout << "Imagem Media Preto e branco salva!" << endl;
		image.get_normalize(0,255).save_png("saida_MediaPB.png");
	}
}
//sabendo o tamanho da img e a posição do quadrado do zoom irei fazer a contagem dos pixels para determinar o valor size tanto da erosão e da dilatação a partir dele e tornar uma coisa mais dinamica
int tamZoom = defineTamZoom(&image, 0);
if(ruido == 2){
	tamZoom = 2 * tamZoom;
}
if((strcmp (argv[argc-1], "debug") == 0)){
	cout << "Tamanho do zoom inicial:" << tamZoom << endl;
}
//limiarização
limiarizacao(&image,&imgSaida);
if((strcmp (argv[argc-1], "debug") == 0)){
	cout << "Imagem Limiarização salva!" << endl;
	imgSaida.get_normalize(0,255).save_png("saida_limiarização.png");
}

const int constMagica = 0.0001;

int dilateValue = 3 + tamZoom/180;
int erodeValue = 3 + tamZoom/180;

if(ruido == 2){
	//erosão
		erodeValue = erodeValue;
		imgSaida.erode(erodeValue+(tamZoom*constMagica));
	if((strcmp (argv[argc-1], "debug") == 0)){
		cout << "Imagem erosada " << erodeValue << " salva!" << endl;
		imgSaida.get_normalize(0,255).save_png("saida_erosada.png");
	}
		

	//dilatação
		imgSaida.dilate(dilateValue+(tamZoom*constMagica));
	if((strcmp (argv[argc-1], "debug") == 0)){
		cout << "Imagem dilatada "<< dilateValue << " salva!" << endl;
		imgSaida.get_normalize(0,255).save_png("saida_dilatada.png");
	}
}else{
	if(ruido == 1){
		imgSaida.erode(tamZoom/115);
		dilateValue = dilateValue + tamZoom/115;
	}
	//dilatação
		imgSaida.dilate(dilateValue+(tamZoom*constMagica));
	if((strcmp (argv[argc-1], "debug") == 0)){
		cout << "Imagem dilatada "<< dilateValue << " salva!" << endl;
		imgSaida.get_normalize(0,255).save_png("saida_dilatada.png");
	}

	//erosão
		imgSaida.erode(erodeValue+(tamZoom*constMagica));
	if((strcmp (argv[argc-1], "debug") == 0)){
		cout << "Imagem erosada " << erodeValue << " salva!" << endl;
		imgSaida.get_normalize(0,255).save_png("saida_erosada.png");
	}
}


tamZoom = defineTamZoom(&imgSaida, 1);
if((strcmp (argv[argc-1], "debug") == 0)){
	cout << "Tamanho do zoom final:" << tamZoom << endl;
}
achaMoeda(&imgSaida);
//salvar img	
	imgSaida.get_normalize(0,255).save_png("saida.png");
}

void limiarizacao(CImg<unsigned char> *image,CImg<unsigned char> *imgSaida){
int* refRGB;
//valor de cada cor de acordo com sua camada(fundo) //refazer pra nao pegar ruido
refRGB = returnPixelSemRuido(image);
	for(int col = 0; col < image->width(); col++) {
		for(int row = 0; row < image->height(); row++) {
			if(image->atXYZC(col, row,0,0) == refRGB[0] && image->atXYZC(col, row,0,1)== refRGB[1] && image->atXYZC(col, row,0,2)== refRGB[2]){
				imgSaida->atXY(col, row) = 0;
			}else imgSaida->atXY(col, row) = 1;
		}
	}

}

int defineTamZoom(CImg<unsigned char> *image,int cor){
int tamZoom=0;

	for(int col = 450; col < 529; col++) {
		for(int row = 0; row < 70; row++) {
			if(image->atXY(col, row) == cor){
				tamZoom++;
			}
		}
	}
return tamZoom;
}

void fazRuidoSalPimenta(CImg<unsigned char> *image,int porcentagem){
srand(time(NULL));
int pretoOuBranco ;

	for(int col = 0; col < image->width(); col++) {
		for(int row = 0; row < image->height(); row++) {
			if(rand() % 100 +1 <= porcentagem){
				pretoOuBranco =rand()%2;
				if(pretoOuBranco == 1){
					image->atXYZC(col, row,0,0) = 0;
					image->atXYZC(col, row,0,1) = 0;
					image->atXYZC(col, row,0,2) = 0;
				}else{
					image->atXYZC(col, row,0,0) = 255;
					image->atXYZC(col, row,0,1) = 255;
					image->atXYZC(col, row,0,2) = 255;			
				}
			}
		}
	}
}


void fazRuidoAleatorio(CImg<unsigned char> *image,int intervalo){
srand(time(NULL));
int corSorteada ;
int menosOuMais ;
	for(int col = 0; col < image->width(); col++) {
		for(int row = 0; row < image->height(); row++) {
			corSorteada = rand() % intervalo;
			menosOuMais =rand()%4;
			if(menosOuMais == 1){
				image->atXYZC(col, row,0,0) += corSorteada;
				image->atXYZC(col, row,0,1) += corSorteada;
				image->atXYZC(col, row,0,2) += corSorteada;
			}else if(menosOuMais == 0){
				image->atXYZC(col, row,0,0) -= corSorteada;
				image->atXYZC(col, row,0,1) -= corSorteada;
				image->atXYZC(col, row,0,2) -= corSorteada;			
			}
		}
	}
}

int* returnPixelSemRuido(CImg<unsigned char> *image){
	int pixel = 5;
	int* refRGB;
	refRGB = (int *) malloc(3*sizeof(int));
	refRGB[0] = image->atXYZC(pixel,pixel,0,0);
	refRGB[1] = image->atXYZC(pixel,pixel,0,1);
	refRGB[2] = image->atXYZC(pixel,pixel,0,2);
	
	while(true){
		for(int i = -1; i <= 1; i++) {
			for(int j = -1; j <= 1; j++) {
				if(refRGB[0] == image->atXYZC(pixel+j,pixel+i,0,0) && refRGB[1] == image->atXYZC(pixel+j,pixel+i,0,1) && refRGB[2] == image->atXYZC(pixel+j,pixel+i,0,2)){
					return refRGB;	
				}else pixel++;
			}
		}
	}
}

void achaMoeda(CImg<unsigned char> *image){

int linhaInicial, colunaInicial;
	for(int row = 0; row < image->height(); row++) {
		for(int col = 0; col < image->width(); col++) {
			if(image->atXY(col,row) == 1){
					cout << "ENCONTROU: " << col<<" " << row << endl;
					linhaInicial = row;
					colunaInicial = col;
					break;
			}
		}
	}
int tam=0;
	for(int row = linhaInicial; row > 0 ; row--) {
		if(image->atXY(colunaInicial,row) == 1){
				tam++;
		}else break;
	}
cout << "Tam: " << tam+1 << endl;

	
}

void preencherBranco(CImg<unsigned char> *image,int linha, int coluna, int tamanho){

}
















