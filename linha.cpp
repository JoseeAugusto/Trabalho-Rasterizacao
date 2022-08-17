//*********************************************************************
//  Codigo exemplo para aula pratica de Computacao Grafica 
//  Assunto: Rasterizacao e Transformacoes Geometricas
//
//  Autor: Prof. Laurindo de Sousa Britto Neto
//*********************************************************************

// Bibliotecas utilizadas pelo OpenGL
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "funcoes.h"

// Biblioteca com funcoes matematicas
#include <cstdio>
#include <cstdlib>

// Variaveis Globais
bool click1 = false, click2 = false;

double x_1,y_1,x_2,y_2;

int width = 512, height = 512; //Largura e altura da janela


// Declaracoes forward das funcoes utilizadas
void init(void);
void reshape(int w, int h);
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);

// Funcao que implementa o Algoritmo Imediato para rasterizacao de retas
//void retaImediata(double x1,double y1,double x2,double y2);

//void bresenham(double x1, double y1, double x2, double y2);

//void reducaoAoPrimeiroOctante(double x1, double y1, double x2, double y2);

//void desenhaQuadrilatero(double x1, double y1, double x2, double y2);

//void desenhaTriangulo(double x1, double y1, double x2, double y2);
// Funcao que percorre a lista de pontos desenhando-os na tela
void drawPontos();

// Funcao Principal do C
int main(int argc, char** argv){
    glutInit(&argc, argv); // Passagens de parametro C para o glut
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); // Selecao do Modo do Display e do Sistema de cor utilizado
    glutInitWindowSize (width, height);  // Tamanho da janela do OpenGL
    glutInitWindowPosition (100, 100); //Posicao inicial da janela do OpenGL
    glutCreateWindow ("Rasterizacao"); // Da nome para uma janela OpenGL
    init(); // Chama funcao init();
    glutReshapeFunc(reshape); //funcao callback para redesenhar a tela
    glutKeyboardFunc(keyboard); //funcao callback do teclado
    glutMouseFunc(mouse); //funcao callback do mouse
    glutDisplayFunc(display); //funcao callback de desenho
    glutMainLoop(); // executa o loop do OpenGL
    return 0; // retorna 0 para o tipo inteiro da funcao main();
}

// Funcao com alguns comandos para a inicializacao do OpenGL;
void init(void){
    glClearColor(0.0, 0.0, 0.0, 0.0); //Limpa a tela com a cor branca;
}

void reshape(int w, int h)
{
	// Reinicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Definindo o Viewport para o tamanho da janela
	glViewport(0, 0, w, h);
	
	width = w;
	height = h;
    glOrtho (0, w, 0, h, -1 ,1);  

   // muda para o modo GL_MODELVIEW (não pretendemos alterar a projecção
   // quando estivermos a desenhar na tela)
	glMatrixMode(GL_MODELVIEW);
    click1 = true; //Para redesenhar os pixels selecionados
    click2 = true;
}

// Funcao usada na funcao callback para utilizacao das teclas normais do teclado
void keyboard(unsigned char key, int x, int y){
    switch (key) { // key - variavel que possui valor ASCII da tecla precionada
        case 27: // codigo ASCII da tecla ESC
            exit(0); // comando pra finalizacao do programa
        break;
    }
}

//Funcao usada na funcao callback para a utilizacao do mouse
void mouse(int button, int state, int x, int y)
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
            if(click1){
                click2 = true;
                x_2 = x;
                y_2 = height - y;
                printf("x2y2(%.0f,%.0f)\n",x_2,y_2);
                glutPostRedisplay();
            }else{
                click1 = true;
                x_1 = x;
                y_1 = height - y;
                printf("x1y1(%.0f,%.0f)\n",x_1,y_1);
            }
         }
         break;
/*
      case GLUT_MIDDLE_BUTTON:
         if (state == GLUT_DOWN) {
            glutPostRedisplay();
         }
         break;
      case GLUT_RIGHT_BUTTON:
         if (state == GLUT_DOWN) {
            glutPostRedisplay();
         }
         break;
*/
      default:
         break;
   }
}

// Funcao usada na funcao callback para desenhar na tela
void display(void){
    glClear(GL_COLOR_BUFFER_BIT); //Limpa o buffer de cores e reinicia a matriz
    glLoadIdentity();
    
    glColor3f (1.0, 1.0, 1.0); // Seleciona a cor default como preto
    

    if(click1 && click2){
        //retaImediata(x_1,y_1,x_2,y_2);
        //bresenham(x_1,y_1,x_2,y_2);
        reducaoAoPrimeiroOctante(x_1,y_1,x_2,y_2);
        //desenhaQuadrilatero(x_1,y_1,x_2,y_2);
        //desenhaTriangulo(x_1,y_1,x_2,y_2);
        drawPontos();
        //click1 = false;
        click2 = false;
    }

    glutSwapBuffers(); // manda o OpenGl renderizar as primitivas

}

//Funcao que desenha os pontos contidos em uma lista de pontos
void drawPontos(){
    ponto * pnt;
    pnt = pontos;
    glBegin(GL_POINTS); // Seleciona a primitiva GL_POINTS para desenhar
        while(pnt != NULL){
            glVertex2i(pnt->x,pnt->y);
            pnt = pnt->prox;
        }
    glEnd();  // indica o fim do desenho
}


