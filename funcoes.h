#include <cmath>

// Estrututa de dados para o armazenamento dinamico dos pontos
// selecionados pelos algoritmos de rasterizacao
struct ponto{
    int x;
    int y;
    ponto * prox;
};

// Lista encadeada de pontos
// indica o primeiro elemento da lista
ponto * pontos = NULL;

// Funcao para armazenar um ponto na lista
// Armazena como uma Pilha (empilha)
ponto * pushPonto(int x, int y){
	ponto * pnt;
	pnt = new ponto;
	pnt->x = x;
	pnt->y = y;
	pnt->prox = pontos;
	pontos = pnt;
	return pnt;
}

// Funcao para desarmazenar um ponto na lista
// Desarmazena como uma Pilha (desempilha)
ponto * popPonto(){
	ponto * pnt;
	pnt = NULL;
	if(pontos != NULL){
		pnt = pontos->prox;
		delete pontos;
		pontos = pnt;
	}
	return pnt;
}

void retaImediata(double x1,double y1,double x2,double y2){
    double m, b, yd, xd;
    double xmin, xmax,ymin,ymax;
    //Armazenando os extremos para desenho
    pontos = pushPonto((int)x1,(int)y1);
    pontos = pushPonto((int)x2,(int)y2);

    if(x2-x1 != 0){ //Evita a divisão por zero
        m = (y2-y1)/(x2-x1);
        b = y1 - (m*x1);

        if(m>=-1 && m <= 1){ // Verifica se o declive da reta tem tg de -1 a 1, se verdadeira calcula incrementando x
            xmin = (x1 < x2)? x1 : x2;
            xmax = (x1 > x2)? x1 : x2;

            for(int x = (int)xmin+1; x < xmax; x++){
                yd = (m*x)+b;
                yd = floor(0.5+yd);
                pontos = pushPonto(x,(int)yd);
            }
        }else{ // Se menor que -1 ou maior que 1, calcula incrementado os valores de y
            ymin = (y1 < y2)? y1 : y2;
            ymax = (y1 > y2)? y1 : y2;

            for(int y = (int)ymin + 1; y < ymax; y++){
                xd = (y - b)/m;
                xd = floor(0.5+xd);
                pontos = pushPonto((int)xd,y);
            }
        }

    }else{ // se x2-x1 == 0, reta perpendicular ao eixo x
        ymin = (y1 < y2)? y1 : y2;
        ymax = (y1 > y2)? y1 : y2;
        for(int y = (int)ymin + 1; y < ymax; y++){
            pontos = pushPonto((int)x1,y);
        }
    }
}

void bresenham(double x1, double y1, double x2, double y2, bool simetrico, bool declive){
	double dx = x2 - x1, dy = y2 - y1;
	double d = (2*dy)-dx, incE = 2*dy, incNE = 2*(dy-dx);
	if(declive && simetrico){
		pontos = pushPonto((int)y1, (int)x1*(-1));
		pontos = pushPonto((int)y2, (int)x2*(-1));
	}
	else if(declive){
		pontos = pushPonto((int)y1, (int)x1);
		pontos = pushPonto((int)y2, (int)x2);
	}
	else if(simetrico){
		pontos = pushPonto((int)x1, (int)y1*(-1));
		pontos = pushPonto((int)x2, (int)y2*(-1));
	}
	int y = (int)y1;
	for(int x = (int)x1+1; x < x2; x++){
		if(d <= 0){
			d = d + incE;
			if(declive && simetrico){
				pontos = pushPonto(y, x*(-1));
			}
			else if(declive){
				pontos = pushPonto(y, x);
			}
			else if(simetrico){
				pontos = pushPonto(x, y*(-1));
			}
			else{
				pontos = pushPonto(x, y);
			}
		}
		else{
			d = d + incNE;
			y += 1;
			if(declive && simetrico){
				pontos = pushPonto(y, x*(-1));
			}
			else if(declive){
				pontos = pushPonto(y, x);
			}
			else if(simetrico){
				pontos = pushPonto(x, y*(-1));
			}
			else{
				pontos = pushPonto(x, y);
			}
		}
	}
}

void reducaoAoPrimeiroOctante(double x1, double y1, double x2, double y2){
	double dx = x2 - x1, dy = y2 - y1, aux = 0;
	bool simetrico = false, declive = false;
	
	if(dx*dy < 0){
		y1 *= -1;
		y2 *= -1;
		dx = x2 - x1;
		dy = y2 - y1;
		simetrico = true;
	}
	
	if(fabs(dx) < fabs(dy)){
		aux = x1;
		x1 = y1;
		y1 = aux;
		
		aux = x2;
		x2 = y2;
		y2 = aux;
		
		dx = x2 - x1;
		dy = y2 - y1;
		//simetrico = true;
		declive = true;
	}
	
	if(x1 > x2){
		aux = x1;
		x1 = x2;
		x2 = aux;
		
		aux = y1;
		y1 = y2;
		y2 = aux;
		
		dx = x2 - x1;
		dy = y2 - y1;	
	}
	
	bresenham(x1, y1, x2, y2, simetrico, declive);
		
}

void desenhaQuadrilatero(double x1, double y1, double x2, double y2){
	reducaoAoPrimeiroOctante(x1, y1, x2, y1);
	reducaoAoPrimeiroOctante(x2, y1, x2, y2);
	reducaoAoPrimeiroOctante(x2, y2, x1, y2);
	reducaoAoPrimeiroOctante(x1, y2, x1, y1);
}