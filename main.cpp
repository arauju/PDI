#include "main.h"

int main(int argc, char *argv[]){
char *imgEntrada;
int ruido = 0; //ruido 1, sal e pimenta e ruido 2, aleatorio.
int porcentagem = 0; //caso do sal e pimenta
int intervalo = 0; //caso do aleatorio
float valorTotal = 0;


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

//Coloca os Ruidos nas imagens
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

//Ajuda a limpar o ruido sal e pimenta
if(ruido == 1){
	image.blur_median(10, 0);
	if((strcmp (argv[argc-1], "debug") == 0)){
		cout << "Imagem Media Colorida salva!" << endl;
		image.get_normalize(0,255).save_png("saida_MediaCL.png");
	}
}

//sabendo o tamanho da img e a posição do quadrado do zoom irei fazer a contagem dos pixels para determinar o valor size tanto da erosão e da dilatação a partir dele e tornar uma coisa mais dinamica
int tamZoom = defineTamZoom(&image, 0);
if((strcmp (argv[argc-1], "debug") == 0)){
		cout << "Tamanho do zoom inicial: " << tamZoom << endl;
}

//limiarização
limiarizacao(&image,&imgSaida);
if((strcmp (argv[argc-1], "debug") == 0)){
	cout << "Imagem Limiarização salva!" << endl;
	imgSaida.get_normalize(0,255).save_png("saida_limiarização.png");
}

if((strcmp (argv[argc-1], "debug") == 0)){
	cout << "Tamanho do zoom inicial:" << tamZoom << endl;
}

const int constMagica = 0.0001;
int dilateValue = 5;
int erodeValue = 5;

if(ruido == 2){
	//erosão
		erodeValue = erodeValue + tamZoom/600;
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
		if(tamZoom >500)
			imgSaida.erode(6);//imgSaida.erode(9-(porcentagem/5));//9
		else
			imgSaida.erode(9);//imgSaida.erode(9-(porcentagem/5));//9
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
//salvar img	

if((strcmp (argv[argc-1], "debug") == 0)){
	cout << "Saida Final " << endl;
	imgSaida.get_normalize(0,255).save_png("saida.png");
}

if(ruido == 2){
	tamZoom = defineTamZoom(&imgSaida, 1);
	if((strcmp (argv[argc-1], "debug") == 0)){
		cout << "Tamanho do zoom final ruido 2: " << tamZoom << endl;
	}
}

int a = 0;
	while(a == 0){
		int valor = 0;	
		valor = achaMoeda(&imgSaida);
		valor = (valor*zoomPadrao)/tamZoom;
		valor += 800;
		if(valor == 800){
			break;
		}
		if((strcmp (argv[argc-1], "debug") == 0)){
			if(valor > 4000)
				cout << "Moeda de tamanho: " << valor << endl;
		}
		valorTotal += calcularValor(valor);
	}
cout << "Valor Final: " << valorTotal << endl;

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

int achaMoeda(CImg<unsigned char> *image){

int linhaInicial, colunaInicial;
	for(int row = 0; row < image->height(); row++) {
		for(int col = 0; col < image->width(); col++) {
			if(image->atXY(col,row) == 1){
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
tam = tam+1;
preencherBranco(image, linhaInicial, colunaInicial, tam);

image->get_normalize(0,255).save_png("a.png");
tam = pow (tam/2 , 2);
tam = tam * 3.14159265359;

return tam;
}
int a = 0;
void preencherBranco(CImg<unsigned char> *image,int linha, int coluna,int tam){
	for(int row = linha; row > linha - tam; row--) {
		for(int col = coluna- tam/2; col < coluna + (tam/2+30) ; col++) {
			if(image->atXY(col,row) == 1){
				image->atXY(col,row) = 0;
			}
		}
	}

}

float calcularValor(int tam){
	float valor = 0;
	if(tam >= 5000 && tam < 8980){
		valor = 0.01;	
	}else if(tam >= 8980 && tam < 11406){
		valor = 0.10;	
	}else if(tam >= 11406 && tam < 13063){
		valor = 0.05;	
	}else if(tam >= 13063 && tam < 14910){
		valor = 0.50;	
	}else if(tam >= 14910 && tam < 17446){
		valor = 0.25;	
	}else if(tam > 17446){
		valor = 1.00;	
	}
return valor;
}









