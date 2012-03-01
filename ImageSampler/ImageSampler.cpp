/**********************************************************************
    
  ImageSampler
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

#include <iostream>
#include <fstream>
using namespace std;

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#pragma comment(lib,"cv210d.lib")
#pragma comment(lib,"cxcore210d.lib")
#pragma comment(lib,"highgui210d.lib")

int main(int argc,char** argv)
{
	CvCapture* pCap[2];
	IplImage* pImg[2];

	if( !(pCap[0] = cvCaptureFromCAM(0))){
		fprintf(stderr, "Can not open camera 1.\n");
		return -1;
	}
	if (!(pCap[1] = cvCaptureFromCAM(1))){
		fprintf(stderr,"Can not open camera 2.\n");
		return -2;
	}

	pImg[0]=cvQueryFrame(pCap[0]);
	pImg[1]=cvQueryFrame(pCap[1]);
	if (pImg[0]==NULL){
		fprintf(stderr,"can not query frame from camera 1.\n");
		return -3;
	}
	if (pImg[1]==NULL){
		fprintf(stderr,"can not query frame from camera 2.\n");
		return -4;
	}

	cvNamedWindow("camera1");
	cvNamedWindow("camera2");

	int count=0;
	char nameBuf[50];
	cout<<"press Enter to save a pair of images"<<endl;

	while ((pImg[0]=cvQueryFrame(pCap[0]))&&(pImg[1]=cvQueryFrame(pCap[1]))){
		cvShowImage("camera1",pImg[0]);
		cvShowImage("camera2",pImg[1]);
		int key=cvWaitKey(3);
		if (key==27){// esc to quit
			break;
		}else if (key==13){//save image
			sprintf_s(nameBuf,50,"../data/images/image0_%03d.jpg",count);
			if (!cvSaveImage(nameBuf,pImg[0])){
				cout<<"can not save image :"<<nameBuf<<endl;
			}else{
				cout<<"saved :"<<nameBuf<<endl;
			}
			sprintf_s(nameBuf,50,"../data/images/image1_%03d.jpg",count);
			if (!cvSaveImage(nameBuf,pImg[1])){
				cout<<"can not save image :"<<nameBuf<<endl;
			}else{
				cout<<"saved :"<<nameBuf<<endl;
			}
			count++;
		}
	}
	
	cvReleaseCapture(&pCap[0]);
	cvReleaseCapture(&pCap[1]);

	return 0;
}