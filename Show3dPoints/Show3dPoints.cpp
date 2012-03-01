/**********************************************************************
    
  Show3dPoints
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

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#pragma comment(lib,"cv210d.lib")
#pragma comment(lib,"cxcore210d.lib")
#pragma comment(lib,"highgui210d.lib")

#include <gl/glut.h>

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Point3d 
{
public:
	float x;
	float y;
	float z;
	Point3d():x(0),y(0),z(0){}
	Point3d(float* p){x=p[0];y=p[1];z=p[2];}
};
///////////////////////////////////////////////////
//cv
void reconstruct3Dpoint(float M1[3][4],float M2[3][4],float p1[2],float p2[2],float p3d[3]);
void getPerpectiveProjectionMat(float M1[3][4],float M2[3][4]);
void get3dPoints(vector<Point3d>&points,char* f1,char*f2);

void drawLocusOnImage();
///////////////////////////////////////////////////
GLsizei winWidth=500,winHeight=500;
vector<Point3d> pointVec;
//perspective parameters
Point3d eyePos;
static int radius=1000;
static float angleHorizontal=0;
static float angleVertical=0;
static float transHorizontal=0;
static float transVertical=0;

static unsigned int timerInterval=50;
bool isTimerOn=false;

static const float PI=3.1415926;
static bool isLButtonDown=false;
static bool isMButtonDown=false;
static CvPoint oldPoint;
//gl
void init(void);
void display(void);
void winReshapeFcn(GLint width,GLint height);
void timerFunc(int id);//change eye position
void mouseFunc(int button,int action,int xMouse,int yMouse);//change eye position
void keyFunc(GLubyte key,int xMouse,int yMouse);
void timerSwitch(int option);
void writeCharacter(Point3d p,char* str);
void mouseMotionFunc(int xMouse,int yMouse);
//utilities
void updateEyePosition(void);

void main(int argc,char** argv)
{
	cout<<"s\tshow locus on images and save them"<<endl
		<<"+\tzoom in"<<endl
		<<"-\tzoom out"<<endl
		<<"ESC\tquit"<<endl;

	char* datafile[2];
	if (argc==3){
		datafile[0]=argv[1];
		datafile[1]=argv[2];
	}else{
		datafile[0]="../data/videos/video_cv_glove_0.avi.yml";
		datafile[1]="../data/videos/video_cv_glove_1.avi.yml";
	}

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(winWidth,winHeight);
	glutCreateWindow("Locus");

	//points
	get3dPoints(pointVec,datafile[0],datafile[1]);

	init();
	//menu
	glutCreateMenu(timerFunc);
	glutAddMenuEntry("Turn On Timer",1);
	glutAddMenuEntry("Turn Off Timer",2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(display);
	glutReshapeFunc(winReshapeFcn);
	//glutIdleFunc(timerFunc);
	glutKeyboardFunc(keyFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
	glutTimerFunc(timerInterval,&timerFunc,0);

	glutMainLoop();

}
///////////////////////////////////////////////////
//cv

void reconstruct3Dpoint(float M1[3][4],float M2[3][4],float p1[2],float p2[2],float p3d[3])
{
	float coeff1[4][3];
	float coeff2[4];

	coeff1[0][0]=p1[0]*M1[2][0]-M1[0][0];
	coeff1[0][1]=p1[0]*M1[2][1]-M1[0][1];
	coeff1[0][2]=p1[0]*M1[2][2]-M1[0][2];
	coeff1[1][0]=p1[1]*M1[2][0]-M1[1][0];
	coeff1[1][1]=p1[1]*M1[2][1]-M1[1][1];
	coeff1[1][2]=p1[1]*M1[2][2]-M1[1][2];
	coeff1[2][0]=p2[0]*M2[2][0]-M2[0][0];
	coeff1[2][1]=p2[0]*M2[2][1]-M2[0][1];
	coeff1[2][2]=p2[0]*M2[2][2]-M2[0][2];
	coeff1[3][0]=p2[1]*M2[2][0]-M2[1][0];
	coeff1[3][1]=p2[1]*M2[2][1]-M2[1][1];
	coeff1[3][2]=p2[1]*M2[2][2]-M2[1][2];

	coeff2[0]=M1[0][3]-p1[0]*M1[2][3];
	coeff2[1]=M1[1][3]-p1[1]*M1[2][3];
	coeff2[2]=M2[0][3]-p2[0]*M2[2][3];
	coeff2[3]=M2[1][3]-p2[1]*M2[2][3];

	CvMat mat1=cvMat(4,3,CV_32FC1,coeff1);
	CvMat mat2=cvMat(4,1,CV_32FC1,coeff2);
	CvMat matResult=cvMat(3,1,CV_32FC1,p3d);
	cvSolve(&mat1,&mat2,&matResult,CV_SVD);
}
void getPerpectiveProjectionMat(float M1[3][4],float M2[3][4])
{
	char* names[2]={"../data/images/coorfile0.ifl-parameters.yml","../data/images/coorfile1.ifl-parameters.yml"};
	float (*ret[2])[4]={M1,M2};

	for (int i=0;i<2;i++){
		CvFileStorage* fs=cvOpenFileStorage(names[i],0,CV_STORAGE_READ);
		if(!fs){
			cout<<"failed to load file storage"<<endl;
			exit(0);
		}

		CvMat* r,* t,*intri;
		CvFileNode* node=cvGetFileNodeByName(fs,0,"rotate0");
		r=(CvMat*)cvRead(fs,node);
		node=cvGetFileNodeByName(fs,0,"translate0");
		t=(CvMat*)cvRead(fs,node);
		node=cvGetFileNodeByName(fs,0,"intrinsic");
		intri=(CvMat*)cvRead(fs,node);

		CvMat * rr=cvCreateMat(3,3, CV_32FC1);

		cvRodrigues2(r,rr);

		for (int h=0;h<3;h++){
			ret[i][h][3]=t->data.fl[h];
			for (int w=0;w<3;w++){
				ret[i][h][w]=*(rr->data.fl+h*3+w);
			}
		}
		CvMat m=cvMat(3,4,CV_32FC1,ret[i]);
		cvMatMul(intri,&m,&m);

		cvReleaseMat(&r);
		cvReleaseMat(&rr);
		cvReleaseMat(&t);

		cvReleaseFileStorage(&fs);
	}
}
void get3dPoints(vector<Point3d>&points,char* yml1,char* yml2)
{
	float M1[3][4];
	float M2[3][4];
	getPerpectiveProjectionMat(M1,M2);
	//read two 2d points sequences
	CvMat* mat[2];
	char* fsname[2]={yml1,yml2};

	CvFileStorage* fs[2];
	for (int i=0;i<2;i++){
		fs[i]=cvOpenFileStorage(fsname[i],NULL,CV_STORAGE_READ);
		CvFileNode* node=cvGetFileNodeByName(fs[i],0,"fingertip");
		mat[i]=(CvMat*)cvRead(fs[i],node);
	}

	points.clear();
	//reconstruct
	for (int i=0;i<mat[0]->rows;i++){
		float p[3]={0};
		float p1[2]={mat[0]->data.i[i<<1],mat[0]->data.i[(i<<1)+1]};
		float p2[2]={mat[1]->data.i[i<<1],mat[1]->data.i[(i<<1)+1]};
		if (p1[0]>0&&p1[1]>0&&p2[0]>0&&p2[1]>0){
			reconstruct3Dpoint(M1,M2,p1,p2,p);
			Point3d p3d(p);
			points.push_back(p3d);
		}
	}
}

void drawLocusOnImage()
{
	char* names[2]={"../data/images/coorfile0.ifl-parameters.yml","../data/images/coorfile1.ifl-parameters.yml"};
	char* imgName[2]={"../data/images/image0_000.jpg","../data/images/image1_000.jpg"};

	CvMat* objectPoints;
	//3d data
	objectPoints=cvCreateMat(pointVec.size(),3,CV_32FC1);
	vector<Point3d>::iterator ite;
	float* p=objectPoints->data.fl;
	for (ite=pointVec.begin();ite!=pointVec.end();ite++){
		*p++=ite->x;
		*p++=ite->y;
		*p++=ite->z;
	}

	for (int i=0;i<2;i++){
		CvMat* imagePoints;
		CvMat* r,* t,*intri,*distortion;
		CvFileStorage* fs=cvOpenFileStorage(names[i],0,CV_STORAGE_READ);
		
		CvFileNode* node=cvGetFileNodeByName(fs,0,"rotate0");
		r=(CvMat*)cvRead(fs,node);
		node=cvGetFileNodeByName(fs,0,"translate0");
		t=(CvMat*)cvRead(fs,node);
		node=cvGetFileNodeByName(fs,0,"intrinsic");
		intri=(CvMat*)cvRead(fs,node);
		node=cvGetFileNodeByName(fs,0,"distortion");
		distortion=(CvMat*)cvRead(fs,node);
		cvReleaseFileStorage(&fs);

		imagePoints=cvCreateMat(pointVec.size(),2,CV_32FC1);	
	
		//reproject coordinates from 3d to window
		cvProjectPoints2(objectPoints,r,t,intri,distortion,imagePoints);
		
		//show and save images
		IplImage* img;
		char buf[500]={0};
		img=cvLoadImage(imgName[i]);
		if (!img){
			cout<<"failed to load image"<<endl;
			exit(0);
		}
		float* pdata=imagePoints->data.fl;
		for (int pc=1;pc<imagePoints->rows;pc++){
			int x=int(*pdata++);
			int y=int(*pdata++);
			CvPoint p1=cvPoint(x,y);
			x=int(*pdata++);
			y=int(*pdata++);
			CvPoint p2=cvPoint(x,y);
			pdata-=2;
			//
			cvLine(img,p1,p2,CV_RGB(255,0,0),2);
		}
		sprintf(buf,"%s.locus.jpg",imgName[i]);
		cvShowImage(buf,img);
		cvSaveImage(buf,img);
		
		//release memory
		cvReleaseMat(&imagePoints);
		cvReleaseMat(&r);
		cvReleaseMat(&t);
		cvReleaseMat(&distortion);
		cvReleaseImage(&img);		
	}
	cvReleaseMat(&objectPoints);	
}
///////////////////////////////////////////////////
//gl

void init(void)
{
	glClearColor(1.0,1.0,1.0,1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); 	
	
	updateEyePosition();
	
	gluLookAt(eyePos.x,eyePos.y,eyePos.z,0.0,0.0,0.0,0.0,1.0,0.0);
	

	//glPushMatrix();
	//glTranslatef(transHorizontal,transVertical,0);
	
	//draw coordinate axis
	glColor3ub(255,0,0);//x
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(300,0,0);
	glEnd();
	glColor3ub(0,255,0);//y
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,300,0);
	glEnd();
	glColor3ub(0,0,255);//z
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(0,0,300);
	glEnd();

	float blue=0;
	float red=255;
	float step=255/pointVec.size();

	//	
	vector<Point3d>::iterator ite;
	for (ite=pointVec.begin();ite!=pointVec.end();ite++){		
		//char
		char buf[4]={0};
		writeCharacter(*ite,_itoa(ite-pointVec.begin(),buf,10));
		//draw little sphere around the points
		glColor3ub(GLubyte(red),0,GLubyte(blue));
		red-=step;
		blue+=step;
		glPushMatrix();
		glTranslatef(ite->x,ite->y,ite->z);
		glutSolidSphere(3.0,5,5);
		glPopMatrix();
	}
	//line points up
	glColor3ub(0,0,0);
	glBegin(GL_LINE_STRIP);
	for (ite=pointVec.begin();ite!=pointVec.end();ite++){
		glVertex3f(ite->x,ite->y,ite->z);
	}
	glEnd();
	
	//glPopMatrix();
	glFlush();
}

void winReshapeFcn(GLint width,GLint height)
{
	winWidth=width;
	winHeight=height;

	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,(GLfloat)width/(GLfloat)height,0.1,2000.0);
	glMatrixMode(GL_MODELVIEW);
}

void timerFunc(int id)
{
	if (isTimerOn){
		angleHorizontal+=5;
		if (angleHorizontal>360){
			angleHorizontal-=360;
		}
		//repaint
		glutPostRedisplay();
	}
	
}
void writeCharacter(Point3d p,char* str)
{
	//write
	glColor3ub(0,0,0);//black
	int len=strlen(str);
	glRasterPos3d(p.x,p.y,p.z);
	//glRasterPos2d(winX,winY);
	for (int i=0;i<len;i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,str[i]);
	}
}
void timerSwitch(int option)
{
	switch (option){
		case 1:
			isTimerOn=true;
			break;
		case 2:
			isTimerOn=false;
			break;
		default:
			break;
	}
}
void keyFunc(GLubyte key,int xMouse,int yMouse)
{
	//change radius
	if (key=='-'){//zoom out
		radius+=50;
		glutPostRedisplay();
	}else if (key=='+'){//zoom in
		radius-=50;
		glutPostRedisplay();
	}else if (key=='s'){
		//show locus in images
		drawLocusOnImage();
	}else if (key==27){//esc
		exit(0);
	}	
}
void mouseFunc(int button,int action,int xMouse,int yMouse)
{
	if (button==GLUT_LEFT_BUTTON){
		if (action==GLUT_DOWN){
			oldPoint.x=xMouse;
			oldPoint.y=yMouse;
			isLButtonDown=true;
		}else if (action==GLUT_UP){
			isLButtonDown=false;
		}
	}/*else if (button==GLUT_MIDDLE_BUTTON){
		if (action==GLUT_DOWN){
			oldPoint.x=xMouse;
			oldPoint.y=yMouse;
			isMButtonDown=true;
		}else if (action==GLUT_UP){
			isMButtonDown=false;
		}
	}*/
	
}
void mouseMotionFunc(int xMouse,int yMouse)
{
	if (isLButtonDown){//rotate perspective
		//change eyePos
		angleVertical+=(yMouse-oldPoint.y)*180/winHeight;
		angleHorizontal-=(xMouse-oldPoint.x)*180/winWidth;
		if (angleVertical>360){
			angleVertical-=360;
		}else if (angleVertical<-360){
			angleVertical+=360;
		}
		if (angleHorizontal>360){
			angleHorizontal-=360;
		}else if (angleHorizontal<-360){
			angleHorizontal+=360;
		}
		//update oldPoint
		oldPoint.x=xMouse;
		oldPoint.y=yMouse;
		glutPostRedisplay();
	}//else if (isMButtonDown){//transform
	//	transHorizontal=oldPoint.x-xMouse;
	//	transVertical=oldPoint.y-yMouse;
	//	//update oldPoint
	//	oldPoint.x=xMouse;
	//	oldPoint.y=yMouse;
	//	glutPostRedisplay();
	//}
}

//update position of eye,according to angleHorizontal and angleVertical
void updateEyePosition()
{
	float radVertical=angleVertical*PI/180;
	float radHorizontal=angleHorizontal*PI/180;
	eyePos.y=radius*sin(radVertical);
	float t=radius*cos(radVertical);
	eyePos.x=t*sin(radHorizontal);
	eyePos.z=t*cos(radHorizontal);
}