/**********************************************************************
    
  ChessboarCornerFinder
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

#include <string>
#include <iostream>
using namespace std;

int main(int argc,char** argv)
{
	if (argc!=4&&argc!=5){
		cout<<"usage:\tchessboardCornerFinder fi nw nh [fs]"
			<<"\n\tfi\tfilename of the image"
			<<"\n\tnw\tnumber of chessboard corners in width"
			<<"\n\tnh\tnumber of chessboard corners in height"
			<<"\n\tfs\tname of the file to save the result "<<endl;
		return 0;
	}
	string imgname=argv[1];
	int nw=atoi(argv[2]);
	int nh=atoi(argv[3]);
	
	IplImage* img=cvLoadImage(imgname.c_str(),CV_LOAD_IMAGE_GRAYSCALE);
	if (!img){
		cout<<"failed to load image"<<endl;
		return -1;
	}
	CvMat* corners=cvCreateMat(nw,nh,CV_32FC2);
	int cornerCount;
	int ret=cvFindChessboardCorners(img,cvSize(nw,nh),(CvPoint2D32f*)corners->data.fl,&cornerCount,CV_CALIB_CB_ADAPTIVE_THRESH |CV_CALIB_CB_NORMALIZE_IMAGE);
	cvFindCornerSubPix(img,(CvPoint2D32f*)corners->data.fl,cornerCount,cvSize(11,11), 
		cvSize(-1,-1),cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));
	IplImage* imgcolor=cvCreateImage(cvGetSize(img),8,3);
	cvCvtColor(img,imgcolor,CV_GRAY2RGB);
	cvDrawChessboardCorners(imgcolor,cvSize(nw,nh),(CvPoint2D32f *)corners->data.fl,cornerCount,ret);
	
	imgname+=".corners.jpg";
	if (!cvSaveImage(imgname.c_str(),imgcolor)){
		cout<<"failed to save image:"<<imgname<<endl;
	}

	if (!ret){//not find all corners
		cout<<"not all corners were found"<<endl;
	}
	
	string foutname;//name of the file to save the result
	if (argc==5){
		foutname=argv[4];
	}else{
		int dotpos=imgname.find_last_of('.');
		if (dotpos==imgname.npos){//not possible
		}else{
			foutname=imgname.substr(0,dotpos);
		}
		foutname+="-corners.yml";
	}

	//save as yml 
	CvFileStorage* fs=cvOpenFileStorage(foutname.c_str(),NULL,CV_STORAGE_WRITE);
	cvWrite(fs,"corners",corners);
	cvReleaseFileStorage(&fs);

	cout<<"success to save data in "<<foutname.c_str()<<endl;

	cvReleaseMat(&corners);
	cvReleaseImage(&img);

	return 0;
}