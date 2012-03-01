/**********************************************************************
    
  ChessboardImageGenerator
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

#include <cstdlib>
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

	if (argc<4||argc>5){
		cout<<"usage: chessboardGenerator h w l [f]"
			<<"\n\th\tlattice number in height"
			<<"\n\tw\tlattice number in width"
			<<"\n\tl\tlength of lattice in pixel"
			<<"\n\tf\tfilename with proper extension for saving the generated image"<<endl;
			
		exit(0);
	}

	int h=atoi(argv[1]);
	int w=atoi(argv[2]);
	int len=atoi(argv[3]);

	int height=h*len;
	int width=w*len;
	IplImage* img=cvCreateImage(cvSize(width,height),8,1);
	for (int i=0;i<h;i++){
		for (int j=0;j<w;j++){
			for (int lah=0;lah<len;lah++){
				for (int law=0;law<len;law++){
					unsigned char*p=(unsigned char*)img->imageData+(i*len+lah)*img->widthStep+j*len+law;
					if ((i+j)%2){
						*p=255;
					}else{
						*p=0;
					}
				}
			}
		}
	}

	string str;
	if (argc==5){
		str=argv[4];		
	}else{
		str="chessboard";
		for (int i=1;i<4;i++){
			str+="-";
			str+=argv[i];
		}
		str+=".jpg";
	}
	
	if(cvSaveImage(str.c_str(),img)){
		cout<<"succeed to save image:"<<str<<endl;
	}else{
		cout<<"failed to save image:"<<str<<endl;
	}

	cvReleaseImage(&img);

	return 0;
}