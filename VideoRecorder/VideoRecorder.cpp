/**********************************************************************
    
  VideoRecorder
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
using namespace std;

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#pragma comment(lib,"cv210d.lib")
#pragma comment(lib,"cxcore210d.lib")
#pragma comment(lib,"highgui210d.lib")

int main(int argc,char** argv)
{
	char* videoName[2]={"../data/videos/video0.avi","../data/videos/video1.avi"};
	if (argc==3){//user defined filename
		videoName[0]=argv[1];
		videoName[1]=argv[2];
	}

	CvCapture* pCap[2];
	IplImage* pImg[2]={NULL,NULL};
	CvVideoWriter* pVideoWriter[2];


	for (int i=0;i<2;i++){
		if( !(pCap[i] = cvCreateCameraCapture(i))){
			fprintf(stderr, "Can not open camera .\n");
			return -1;
		}
		pImg[i]=cvQueryFrame(pCap[i]);
		//writer
		double f=cvGetCaptureProperty(pCap[i],CV_CAP_PROP_FOURCC);
		char* fourcc=(char*)&f;
		//double fps=cvGetCaptureProperty(pCap[i],CV_CAP_PROP_FPS);
		double fps=15;
		CvSize size=cvSize((int)cvGetCaptureProperty(pCap[i],CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(pCap[i],CV_CAP_PROP_FRAME_HEIGHT));
		pVideoWriter[i]=cvCreateVideoWriter(videoName[i],CV_FOURCC(fourcc[0],fourcc[1],fourcc[2],fourcc[3]),fps,size);
		if (pVideoWriter[i]==NULL){
			fprintf(stderr,"can not create video writer .\n");
			return -1;
		}		
	}
	
	cvNamedWindow("camera1");
	cvNamedWindow("camera2");

	cout<<"press Enter to start ,Esc to quit"<<endl;
	bool startWrite=false;
	while ((pImg[0]=cvQueryFrame(pCap[0]))&&(pImg[1]=cvQueryFrame(pCap[1]))){
		if (startWrite==true){
			cvWriteFrame(pVideoWriter[0],pImg[0]);
			cvWriteFrame(pVideoWriter[1],pImg[1]);			
		}

		cvShowImage("camera1",pImg[0]);
		cvShowImage("camera2",pImg[1]);
		int key=cvWaitKey(3);
		if (key==27){// esc to quit
			cout<<"recording stopped"<<endl;
			break;
		}else if (key==13){//enter to quit
			cout<<"recording started"<<endl;
			startWrite=true;
		}
	}

	cvDestroyAllWindows();

	cvReleaseCapture(&pCap[0]);
	cvReleaseCapture(&pCap[1]);
	cvReleaseVideoWriter(&pVideoWriter[0]);
	cvReleaseVideoWriter(&pVideoWriter[1]);
	return 0;
}

