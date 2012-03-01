/**********************************************************************
    
  CalibrateFromPoints
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

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc,char** argv)
{
	if (argc!=4){
		cout<<"usage: CalibFromPoints f.ifl l w h"
			<<"\n\tf.ifl\tlist of files which store filename of corner coordinates"
			<<"\n\t\tlike this"
			<<"\n\t\t\timg0.jpg"
			<<"\n\t\t\timg1.jpg"
			<<"\n\t\t\timg2.jpg"
			<<"\n\tl\tlength of chessboard lattice"
			<<"\n\tw\twidth of the images"
			<<"\n\th\theight of the images"
			<<endl;
		return 0;
	}
	//read files
	vector<string> filenameVec;
	fstream fin(argv[1],ios::in);
	if(!fin.is_open()){
		cout<<"can not open file:"<<argv<<endl;
		return 0;
	}
	while(!fin.eof()){
		char nameBuf[MAX_PATH];
		fin>>nameBuf;
		filenameVec.push_back(nameBuf);
	}
	fin.close();

	int cornerCount;
	int cornerHeightNum;
	int cornerWidthNum;
	//get amount of corners
	CvFileStorage* fs=cvOpenFileStorage(filenameVec[0].c_str(),0,CV_STORAGE_READ);
	CvFileNode* node=cvGetFileNodeByName(fs,0,"corners");
	CvMat* corner=(CvMat*)cvRead(fs,node);
	cornerHeightNum=corner->height;
	cornerWidthNum=corner->width;
	cornerCount=corner->cols*corner->rows;
	cvReleaseFileStorage(&fs);

	//calibration
	CvSize imgSize=cvSize(atoi(argv[3]),atoi(argv[4]));
	int imgCount=filenameVec.size();
	double latticeLen=atof(argv[2]);
	CvMat* countMat=cvCreateMat(imgCount,1,CV_32SC1);
	CvMat* points2d=cvCreateMat(imgCount*cornerCount,2,CV_32FC1);
	CvMat* points3d=cvCreateMat(imgCount*cornerCount,3,CV_32FC1);
	CvMat* intrinsicMat=cvCreateMat(3,3,CV_32FC1);
	CvMat* distortionMat=cvCreateMat(1,4,CV_32FC1);
	CvMat* rotateMat=cvCreateMat(imgCount,3,CV_32FC1);
	CvMat* transMat=cvCreateMat(imgCount,3,CV_32FC1);
	//set counteMat
	for (int i=0;i<imgCount;i++){
		countMat->data.i[i]=cornerCount;
	}
	//set points2d from files
	for (unsigned int i=0;i<filenameVec.size();i++){
		CvFileStorage* fs=cvOpenFileStorage(filenameVec[0].c_str(),0,CV_STORAGE_READ);
		CvFileNode* node=cvGetFileNodeByName(fs,0,"corners");
		CvMat* corner=(CvMat*)cvRead(fs,node);
		memcpy(points2d->data.fl+i*cornerCount*2,corner->data.fl,sizeof(float)*cornerCount*2);
		cvReleaseFileStorage(&fs);
	}
	//set points3d
	int l=cornerCount*3;
	for (int m=0;m<imgCount;m++){
		CvPoint3D32f* pt=(CvPoint3D32f*)points3d->data.fl+m*cornerCount;
		for (int i=0;i<cornerHeightNum;i++){
			for (int j=0;j<cornerWidthNum;j++){
				*pt++=cvPoint3D32f((5-i)*latticeLen,(5-j)*latticeLen,0);
			}
		}
	}
	//calibration
	cvCalibrateCamera2(points3d,points2d,countMat,imgSize,intrinsicMat,distortionMat,rotateMat,transMat,0);
	//save camera parameters
	string fsname=argv[1];
	fsname+="-parameters.yml";
	CvFileStorage* fstorage = cvOpenFileStorage(fsname.c_str(), NULL, CV_STORAGE_WRITE);
	char buf[20];
	cvWrite(fstorage, "intrinsic",intrinsicMat);
	cvWrite(fstorage, "distortion",distortionMat);
	for (int i=0;i<imgCount;i++){
		CvMat r=cvMat(1,3,CV_32FC1,rotateMat->data.fl+i*3);	
		CvMat t=cvMat(1,3,CV_32FC1,transMat->data.fl+i*3);
		sprintf(buf,"rotate%d",i);
		cvWrite(fstorage, buf,&r);
		sprintf(buf,"translate%d",i);
		cvWrite(fstorage, buf,&t);
	}	
	cvReleaseFileStorage(&fstorage);

	cvReleaseMat(&countMat);
	cvReleaseMat(&points2d);
	cvReleaseMat(&points3d);
	cvReleaseMat(&intrinsicMat);
	cvReleaseMat(&distortionMat);
	cvReleaseMat(&rotateMat);
	cvReleaseMat(&transMat);

	cout<<"Done.parameters saved in"<<fsname<<endl;

	return 0;
}
