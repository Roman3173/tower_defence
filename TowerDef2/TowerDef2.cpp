// Tower Defence.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "iostream"
#include <Windows.h>
#include <conio.h>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include  <GL/glut.h>
#include  <math.h>
#include <GL/glaux.h>
#pragma comment (lib, "glaux.lib")
using namespace std ;

AUX_RGBImageRec *MyImage,*Towerr,*Monster,*Road,*Castle,*Bullet;

void TextOut(int x,int y,char *string); 
void Fire(); 
void WayOfCreeps();
void Bullett();
void MapPickUp();

char str[10] ;

int E  , n , n1 = 0 , l = 0 , EE = 20 , Bu ; // переменные для определения количества башен , мобов и т.д. n и n1 - это задержки стрельбы и появления мобов

int way[20] , step[51] , R[500] ;   // переменные нужные для довоги и определения пути монстров 

int PriceOfTower = 50;
int Money = 50 ; 

int N = 1000,M = 400;		// количество клеток 
int Scale = 1;	// сторона клетки 
int w = Scale*N;	// ширина
int h = Scale*M;	// высота

int Pass = 0; // сколько мобов прошло 


struct 
{ 
	double x ;
	double y ;
	int hp ;
	double k ;
	double b ;
	bool Si ;
	int numb ; 
}   s[500], m[500] , bu[500000] ;

//вспомогательная функция, создаёт прозрачность (альфа-канал)
void SetAlpha(AUX_RGBImageRec *image) 
{
 int w=image->sizeX; 
 int h=image->sizeY;
 unsigned char *m_pbits = new unsigned char [4* w * h]; //3 для RGB +1 для альфа-канала.

for (int i=0; i<w*h; i++) 
{ m_pbits[4 * i + 0] =image->data[3 * i + 0]; 
  m_pbits[4 * i + 1] =image->data[3 * i + 1]; 
  m_pbits[4 * i + 2] =image->data[3 * i + 2];
if ((image->data[3*i+0]==0) && (image->data[3*i+1]==0)
	  && (image->data[3*i+2]==0) )  m_pbits[4*i+3] = 0;   else m_pbits[4*i+3]=255;    }

image->data = m_pbits;
}		
// читаем дорогу из файла
void MapPickUp()
{
	FILE* in   ;
	char a; int b; 

	a = 0 ;
	in = fopen ( "map.txt" , " r " ) ;

	for ( int i = 0 ; i < 19 ; i ++ ) 
	{
		fread ( &a , sizeof(char) , 1 , in);
		//switch(a){
		//case 'U': 
		//	break;
		//}
		b = int ( a )%48;
		way[i] = b; 
	}
	fclose(in) ;
}	// читаем дорогу из файла
// задаем дорогу ( рисуем ) 
void doroga2()
{
	int X = 0 , Y = 0 ; 


	glColor3f(0.0,0.0,2.5);

	glRasterPos2d(X,Y);
    glDrawPixels(Road->sizeX, Road->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Road->data);      

	for ( int i = 0 ; i <= 19 ; i ++ ) 
	{
		switch (way[i])
		{
			case 0 :
			{
				//glRectf(X,Y, X+50, Y+30);
				glRasterPos2d(X,Y);
				glDrawPixels(Road->sizeX, Road->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Road->data);
				glRasterPos2d(X+20,Y);
				glDrawPixels(Road->sizeX, Road->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Road->data);
				X = X + 50 ;
				break ;
			}
			case 1 :
			{
				//glRectf(X,Y, X+30, Y+50);
				glRasterPos2d(X,Y);
				glDrawPixels(Road->sizeX, Road->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Road->data);
				glRasterPos2d(X,Y+20);
				glDrawPixels(Road->sizeX, Road->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Road->data);
				Y = Y + 50;
				break ;
			}
			case 2:
			{
				//glRectf(X,Y, X-50, Y+30);
				glRasterPos2d(X,Y);
				glDrawPixels(Road->sizeX, Road->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Road->data);
				glRasterPos2d(X-20,Y);
				glDrawPixels(Road->sizeX, Road->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Road->data);
				X = X - 50 ;
				break ;
			}
			case 3:
			{
				//glRectf(X,Y, X+30, Y-50);
				glRasterPos2d(X,Y);
				glDrawPixels(Road->sizeX, Road->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Road->data);
				glRasterPos2d(X,Y-20);
				glDrawPixels(Road->sizeX, Road->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Road->data);
				Y = Y - 50 ;
				break ;
			}
		}

		if ( i == 19 ) 
		{
			glEnable(GL_ALPHA_TEST);																 
		    glAlphaFunc(GL_NOTEQUAL,0);  
			glRasterPos2d(X-20,Y-10);
			glDrawPixels(Castle->sizeX, Castle->sizeY, GL_RGBA, GL_UNSIGNED_BYTE, Castle->data); 
			glDisable(GL_ALPHA_TEST); 
		}
	}

}	  
 // задаем врагов ( рисуем ) 
void vrag()
{

	if ( E >= EE)
		E -- ;

	glEnable(GL_ALPHA_TEST);																 
    glAlphaFunc(GL_NOTEQUAL,0);  

	for (int i = 0 ; i <= E ;  i++ ) 
	{
		if ( s[i].hp >= 1  )
		{
					
			glRasterPos2d(s[i].x,s[i].y);
			glDrawPixels(Monster->sizeX, Monster->sizeY, GL_RGBA, GL_UNSIGNED_BYTE, Monster->data); 	

		};
	}
	n ++ ;

	if ( n >= 40 )
	{
		E ++ ;
		n = 0 ; 
	}

	glDisable(GL_ALPHA_TEST); 
}   // задаем врагов
// задаем башни ( рисуем )
void Tower()
{

	glEnable(GL_ALPHA_TEST);																 
    glAlphaFunc(GL_NOTEQUAL,0);  

	for (int i = 0 ; i <= l ;  i++ ) 
	{
		glRasterPos2d(m[i].x,m[i].y);
		glDrawPixels(Towerr->sizeX, Towerr->sizeY, GL_RGBA, GL_UNSIGNED_BYTE, Towerr->data);  
	}
	glDisable(GL_ALPHA_TEST); 

}	// задаем башню 
// выводим все на экран
void display()
{


	int y , z , v ;
	glClear(GL_COLOR_BUFFER_BIT);

	glRasterPos2d(0,0);
    glDrawPixels(MyImage->sizeX, MyImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, MyImage->data);              


	doroga2(); 
	vrag() ;
	Tower() ;
	Bullett();

	glColor3f( 0.0,0.0,0.0);

	itoa(Money,str,10);
	TextOut(w-130,h-50,str);
	TextOut(w-190,h-50,"Gold: ");

	TextOut(w-220,h-100,"Tower Price: ");
	
	itoa(50,str,10);
	TextOut(w-80,h-100,str);

	TextOut(w-220,h-150,"( To Buy Tower ");
	TextOut(w-200,h-170,"press Num3 )");

	TextOut(w-220,h-200,"( To move tower ");
	TextOut(w-200,h-220,"use Up,Down... )");
	
	TextOut(w-220,h-250,"Monster passed:");
	itoa(Pass,str,10);
	TextOut(w-50,h-250,str);


	glFlush();
}	  
// покупаем башню 
void NewTower()
{
	l++ ;
	m[l].x = 100 ;
	m[l].y = 50 ;
}   
 // задаем действия для клавиатуры 
void KeyboardEvent(int key, int a, int b)
{   
	switch(key)
	{
	case 105 :	 if ( Money >= PriceOfTower )
				 {
					NewTower() ;
					Money = Money - PriceOfTower ; 
				 }
				break;
	case 101 : 	 m[l].y =   m[l].y + 5  ; break;
	case 102 :   m[l].x =   m[l].x + 5  ; break;
	case 100 :   m[l].x =	m[l].x - 5  ; break;
	case 103 :   m[l].y =	m[l].y - 5  ; break;
	}
}	 
//  определение движение врагов ( идут по дороге взятой из файла ) 
void WayOfCreeps()
{

	for (int i = 0 ; i <= E ;  i++ ) 
	{
		if ( R[i] == 20 )
		{
			s[i].hp = -1 ;
			 Pass ++ ;  
			 R[i] = 0 ;
		}
		if (s[i].hp > 0)
		{
			if ( step[i] == 51 )
			{
				R[i] ++ ;
				step[i] = 0 ;
			}
			

			switch (way[R[i]])
			{
			case 0:
				{
					s[i].x ++;
					step[i] ++ ;
					break ;
				}
			case 1:
				{
					s[i].y ++;
					step[i] ++ ;
					break ;
				}
			case 2:
				{
					s[i].x --;
					step[i] ++ ;
					break ;
				}
			case 3:
				{
					s[i].y --;
					step[i] ++ ;
					break ;
				}
			}
		}
	}

}		
// функция вывода текста 
void TextOut(int x,int y,char *string)
{
	char *c ;
	glRasterPos2f(x,y) ; 

	for ( c=string ; *c != '\0' ;c++ )
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	};
}	 
 // задаем выстрелы башни 
void Fire()
{
	int y , z , v ;
	int i ;

	for ( int q = 0 ; q <= l ; q++ )
	{

		for ( i = 0 ; s[i].hp == 0 ; i ++ ) ;


		for ( int K = i ; K <= E ; K++ )
		{
			y = m[q].x-s[K].x ;
			z = m[q].y-s[K].y ;
			v = sqrt(long double(y*y+z*z)) ;


			if ( v <= 200 ) 
			{
				if ( s[K].hp >= 1 )
				{
					glColor3f(1.0,1.0,0.0);

					glBegin(GL_LINES);
					glVertex2f(m[q].x+36 , m[q].y+37  ); glVertex2f((s[K].x+10) , (s[K].y+10));
					glVertex2f(m[q].x+35 , m[q].y+36  ); glVertex2f((s[K].x+10) , (s[K].y+10));
					glVertex2f(m[q].x+35 , m[q].y+38  ); glVertex2f((s[K].x+10) , (s[K].y+10));
					glVertex2f(m[q].x+37 , m[q].y+38  ); glVertex2f((s[K].x+10) , (s[K].y+10));
					glVertex2f(m[q].x+37 , m[q].y+36  ); glVertex2f((s[K].x+10) , (s[K].y+10));
					glVertex2f(m[q].x+34 , m[q].y+35  ); glVertex2f((s[K].x+10) , (s[K].y+10));
					glVertex2f(m[q].x+34 , m[q].y+39  ); glVertex2f((s[K].x+10) , (s[K].y+10));
					glVertex2f(m[q].x+38 , m[q].y+39  ); glVertex2f((s[K].x+10) , (s[K].y+10));
					glVertex2f(m[q].x+38 , m[q].y+35  ); glVertex2f((s[K].x+10) , (s[K].y+10));


					s[K].hp = s[K].hp - m[0].hp ;

					if ( s[K].hp == 0)
						Money = Money + 10 ;

					glEnd();
					glFlush();
					break ;
				}
			}
		}
	}
}   

void Fire2()
{
	int y , z , v ;
	int i ;

	for ( int q = 0 ; q <= l ; q++ )
	{

		for ( i = 0 ; s[i].hp == 0 ; i ++ ) ;


		for ( int K = i ; K <= E ; K++ )
		{
			y = m[q].x-s[K].x ;
			z = m[q].y-s[K].y ;
			v = sqrt(long double(y*y+z*z)) ;


			if ( v <= 200 ) 
			{
				if ( s[K].hp >= 1 )
				{
					if ( n1 == 5)
					{
						n1 = 0 ;
					Bu ++ ;
					bu[Bu].x = m[q].x ;
					bu[Bu].y = m[q].y ;
					bu[Bu].numb = K ;
					
					if (((s[K].x - m[q].x) != 0 )&&( (s[K].y - m[q].y) != 0 ))
					{
						if ( s[K].x < m[q].x )
							bu[Bu].Si = false ;
						else 
							bu[Bu].Si = true ;

						bu[Bu].k = (s[K].y - m[q].y)/(s[K].x - m[q].x);
						bu[Bu].b = bu[Bu].y - bu[Bu].x * bu[Bu].k ;
					}

					s[K].hp = s[K].hp - m[0].hp ;

					if ( s[K].hp <= 0)
						Money = Money + 10 ;
					}
					n1++ ; 
						
					break ;
					
				}
			}
		}
	}

}

void Bullett()
{
	
	glEnable(GL_ALPHA_TEST);																 
    glAlphaFunc(GL_NOTEQUAL,0);  

	for ( int i = 0 ; i < Bu ; i ++ )
	{
		if ( bu[i].Si == true )
			bu[i].x = bu[i].x + 5 ;
		if ( bu[i].Si == false )
			bu[i].x = bu[i].x - 5 ;

		bu[i].y = bu[i].k * bu[i].x + bu[i].b;

		glRasterPos2d(bu[i].x,bu[i].y);
		glDrawPixels(Bullet->sizeX, Bullet->sizeY, GL_RGBA, GL_UNSIGNED_BYTE, Bullet->data);  
	}

	glDisable(GL_ALPHA_TEST); 
}
// рисуем следующий шаг 
void timer (int = 0)
{
	display();
	WayOfCreeps();
	Fire2();

	glutTimerFunc(10,timer,0) ;
}		 

int main(int argc, char **argv)	
{
	for (int i = 0 ; i <= EE ;  i++ ) // определяем начальное положение врагов и задаем их жизни 
	{
		s[i].x = 0 ;
		s[i].y = 3 ;
		s[i].hp = 300 ;
	}

	m[l].x = 280;			// определяем начальное положение башни и ее урон ( hp ) 
	m[l].y = 160;
	m[l].hp = 10;

	MapPickUp() ;

	for ( int i = 0 ; i <= 50 ; i++ )	
		step[i] = 0 ;

	for ( int i = 0 ; i <= 499 ; i++ )
		R[i] = 0 ;

	MyImage = auxDIBImageLoad("image2.bmp");
	Towerr = auxDIBImageLoad("ArrowTower1.bmp");
	SetAlpha(Towerr);
	Monster  = auxDIBImageLoad("Monster1.bmp");
	SetAlpha(Monster);
	Road  = auxDIBImageLoad("doroga.bmp");
	Castle  = auxDIBImageLoad("Castle.bmp");
	SetAlpha(Castle);
	Bullet  = auxDIBImageLoad("Bullet.bmp");
	SetAlpha(Bullet);

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB );
	glutInitWindowSize (w,h);
	glutCreateWindow ("Игровое поле"); 
	glClearColor(0.0,1.0,1.6,1.0);  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,0,h);

	glutDisplayFunc (display);
	glutSpecialFunc(KeyboardEvent);
	glutTimerFunc(10 , timer,0);

	glutMainLoop();

	return 0;
}

