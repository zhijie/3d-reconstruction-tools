/**********************************************************************
    
  DrawStuff
  Release Date: 2010/12/31
  Copyright (C) 2010	Zhijie Lee
                        email: onezeros.lee@gmail.com 
                        web: http://blog.csdn.net/onezeros

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
**********************************************************************/

#include "StdAfx.h"
#include "DrawStuff.h"
enum Aspect{FRONT,BACK,LEFT,RIGHT,UP,DOWN};
enum Color{BLUE,GREEN,RED,ORANGE,WHITE,YELLOW};
const GLubyte colorTable[6][3]={
	{0,0,255},{0,255,0},{255,0,0},{255,102,0},{255,255,255},{255,255,0}
};

DrawStuff::DrawStuff(void)
{
}

DrawStuff::~DrawStuff(void)
{
}

void DrawStuff::Cube()
{
	//draw cube
	GLint vertices[8][3]={
		{-1,-1,-1},{1,-1,-1},{-1,-1,1},{1,-1,1},
		{-1,1,-1},{1,1,-1},{-1,1,1},{1,1,1}
	};

	//in order:	front ,back ,left  ,right,top,bottom
	GLubyte vertIndex[6][4]={
		6,2,3,7,
		5,1,0,4,		
		4,0,2,6,
		7,3,1,5,		
		7,5,4,6,
		2,0,1,3
	};
	for (int i=0;i<6;i++){
		glColor3ubv(colorTable[i]);
		glBegin(GL_QUADS);
		for (int j=0;j<4;j++){
			glVertex3iv(vertices[vertIndex[i][j]]);
		}		
		glEnd();
	}
}
void DrawStuff::Composition()
{
	//a little complicated object
	GLint vertices[8][3]={
		{-1,-1,-1},{1,-1,-1},{-1,-1,1},{1,-1,1},
		{-1,1,-1},{1,1,-1},{-1,1,1},{1,1,1}
	};

	//in order:	front ,back ,left  ,right,top,bottom
	GLubyte vertIndex[6][4]={
		6,2,3,7,
		5,1,0,4,		
		4,0,2,6,
		7,3,1,5,		
		7,5,4,6,
		2,0,1,3
	};
	glPushMatrix();
	glScalef(0.5,0.5,0.5);
	float translateMat[3][3]={{0,0,0},{0,0,2},{0,2,-2}};
	for (int t=0;t<3;t++){
		glTranslatef(translateMat[t][0],translateMat[t][1],translateMat[t][2]);
		for (int i=0;i<6;i++){
			glColor3ubv(colorTable[i]);
			glBegin(GL_QUADS);
			for (int j=0;j<4;j++){
				glVertex3iv(vertices[vertIndex[i][j]]);
			}		
			glEnd();
		}
	}
	glPopMatrix();
}
void DrawStuff::Chessboard()
{
	//chess board
	float latticeLen=0.4f;
	for (int z=-3;z<3;z++){
		for (int x=-3;x<3;x++){
			if ((x+z)%2){
				glColor3ub(0,0,0);
			}else{
				glColor3ub(255,255,255);
			}
			glBegin(GL_QUADS);
			glVertex3f(x*latticeLen,0,z*latticeLen);
			glVertex3f((x+1)*latticeLen,0,z*latticeLen);
			glVertex3f((x+1)*latticeLen,0,(z+1)*latticeLen);
			glVertex3f(x*latticeLen,0,(z+1)*latticeLen);
			glEnd();
		}
	}
}
void DrawStuff::Draw(int object)
{
	switch (object){
		case COMPOUND:
			Composition();
			break;
		case CUBE:
			Cube();
			break;
		case CHESSBOARD:
			Chessboard();
			break;
		default:
			break;
	}
	
	
}