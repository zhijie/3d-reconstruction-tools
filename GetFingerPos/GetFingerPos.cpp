/**********************************************************************
    
  GetFingerPos
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
#include <string>
#include <queue>
using namespace std;

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#pragma comment(lib,"cv210d.lib")
#pragma comment(lib,"cxcore210d.lib")
#pragma comment(lib,"highgui210d.lib")

class CvTarget{
public:
	int erea;//size
	int top;//position
	int bottom;
	int left;
	int right;
	CvTarget():erea(0),top(0),bottom(0),left(0),right(0){}
	int width(){return right-left;}
	int height(){return bottom-top;}
};
void cvFindTarget(const IplImage* img_,const int erea_threshold,CvTarget &targets)
{
	assert(img_->nChannels==1);

	IplImage* img=cvCreateImage(cvSize(img_->width,img_->height),IPL_DEPTH_8U,1);
	cvCopy(img_,img);

	CvTarget tar;
	for (int h=0;h<img->height;h++){
		for (int w=0;w<img->width;w++){
			if (*(unsigned char*)(img->imageData+h*img->widthStep+w)==255){
				CvTarget target;
				target.top=h;
				target.bottom=h;
				target.left=w;
				target.right=w;
				queue<CvPoint> points;
				points.push(cvPoint(w,h));
				*(img->imageData+h*img->widthStep+w)=0;
				//find target with breadth iteration
				while(!points.empty()){
					target.erea++;
					CvPoint p=points.front();
					points.pop();					

					if (p.x>0&&*(unsigned char*)(img->imageData+p.y*img->widthStep+p.x-1)==255){//left
						*(img->imageData+p.y*img->widthStep+p.x-1)=0;					
						points.push(cvPoint(p.x-1,p.y));
						if (target.left>p.x-1){
							target.left=p.x-1;
						}
					}
					if (p.y+1<img->height&&*(unsigned char*)(img->imageData+(p.y+1)*img->widthStep+p.x)==255){//bottom
						*(img->imageData+(p.y+1)*img->widthStep+p.x)=0;					
						points.push(cvPoint(p.x,p.y+1));
						if (target.bottom<p.y+1){
							target.bottom=p.y+1;
						}
					}
					if (p.x+1<img->width&&*(unsigned char*)(img->imageData+p.y*img->widthStep+p.x+1)==255){//right
						*(img->imageData+p.y*img->widthStep+p.x+1)=0;					
						points.push(cvPoint(p.x+1,p.y));
						if (target.right<p.x+1){
							target.right=p.x+1;
						}
					}
					if (p.y>0&&*(unsigned char*)(img->imageData+(p.y-1)*img->widthStep+p.x)==255){//top
						*(img->imageData+(p.y-1)*img->widthStep+p.x)=0;					
						points.push(cvPoint(p.x,p.y-1));
					}
				}
				if (target.erea>erea_threshold){
					if (target.erea>tar.erea){
						tar=target;
					}
				}				
			}
		}
	}
	targets=tar;
	cvReleaseImage(&img);
}


int main(int argc,char** argv)
{
	char* videoFile[2];
	static int threshold_Cb=105;
	if (argc==3){//GetFingerPos 0.avi 1.avi
		videoFile[0]=argv[1];
		videoFile[1]=argv[2];
	}else if(argc==4){
		videoFile[0]=argv[1];
		videoFile[1]=argv[2];
		threshold_Cb=atoi(argv[3]);
	}	else{
		videoFile[0]="../data/videos/video_cv_glove_0.avi";
		videoFile[1]="../data/videos/video_cv_glove_1.avi";
	}

	char* winName[2]={"camera1","camera2"};

	CvCapture* pCap[2];
	IplImage* pImg[2];
	
	IplImage* imgCb[2];
	double frameCount=0;//counts of frames in the two videos must be the same
	CvMat* pointsMat[2];

	for (int i=0;i<2;i++){
		if( !(pCap[i] = cvCaptureFromAVI(videoFile[i]))){
			fprintf(stderr, "Can not open avi.\n");
			return -1;
		}
		pImg[i]=cvQueryFrame(pCap[i]);
		if (pImg[i]==NULL){
			fprintf(stderr,"can not query frame from avi.\n");
			return -1;
		}
		imgCb[i]=cvCreateImage(cvGetSize(pImg[i]),8,1);
		
		frameCount=cvGetCaptureProperty(pCap[i],CV_CAP_PROP_FRAME_COUNT);
		frameCount--;///////////////////////////////////////////////////
		pointsMat[i]=cvCreateMat(int(frameCount),1,CV_32SC2);
		cvNamedWindow(winName[i]);
	}
	
	int frameCounter=0;
	bool run=true;
	while (frameCounter<frameCount){
		cvShowImage(winName[0],pImg[0]);
		cvShowImage(winName[1],pImg[1]);

		if (run&&(pImg[0]=cvQueryFrame(pCap[0]))&&(pImg[1]=cvQueryFrame(pCap[1]))){
			for (int i=0;i<2;i++){			
				cvCvtColor(pImg[i],pImg[i],CV_BGR2YCrCb);
				cvSplit(pImg[i],NULL,NULL,imgCb[i],NULL);
				cvThreshold(imgCb[i],imgCb[i],threshold_Cb,255,CV_THRESH_BINARY_INV);

				cvErode(imgCb[i],imgCb[i]);
				cvDilate(imgCb[i],imgCb[i]);
				
				//find rough position of hand

				CvTarget target;
				cvFindTarget(imgCb[i],4000,target);

				
				//keep position of fingertip
				CvPoint point=cvPoint(-1,-1);
				if (target.width()>0){//find
					//find fingertip from binary image,				
					int left=-1;
					int right=-1;
					for (int w=target.left;w<=target.right;w++){
						unsigned char*p=(unsigned char*)imgCb[i]->imageData+target.bottom*imgCb[i]->widthStep+w;
						if (*p==255){
							if (left<0){
								left=w;
							}else {
								right=w;
							}
						}
					}
					point.y=target.bottom;
					if (right>0){
						point.x=(right+left)>>1;
					}else{
						point.y=left;
					}
				}
				pointsMat[i]->data.i[frameCounter<<1]=point.x;
				pointsMat[i]->data.i[(frameCounter<<1)+1]=point.y;
			}
			frameCounter++;
		}
		
		cvShowImage("binary1",imgCb[0]);
		cvShowImage("binary2",imgCb[1]);
	
		int key=cvWaitKey(3);
		if (key==' '){
			run=!run;
		}else if (key==27){
			break;
		}
	}

	for (int i=0;i<2;i++){
		//file storage
		string str=videoFile[i];
		str+=".yml";
		CvFileStorage* fs=cvOpenFileStorage(str.c_str(),NULL,CV_STORAGE_WRITE);
		cvWrite(fs,"fingertip",pointsMat[i]);
		cvReleaseFileStorage(&fs);
		
		cvReleaseMat(&pointsMat[i]);
		cvReleaseImage(&imgCb[i]);
		cvReleaseCapture(&pCap[i]);	
	}

	return 0;
}