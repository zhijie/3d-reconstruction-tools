/**********************************************************************
    
  OpenGLDevice
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


// OpenGLDevice.h: Schnittstelle für die Klasse OpenGLDevice.
//
// Copyright by Andr?Stein
// E-Mail: stonemaster@steinsoft.net, andre_stein@web.de
// http://www.steinsoft.net
//////////////////////////////////////////////////////////////////////

#ifndef OPENGL_DEVICE_H
#define OPENGL_DEVICE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class OpenGLDevice
{
public:
	OpenGLDevice(HDC& deviceContext,int stencil = 0);
	OpenGLDevice(HWND& window,int stencil = 0);
	OpenGLDevice();

	bool create(HDC& deviceContext,int  stencil = 0);
	bool create(HWND& window,int stencil = 0);

	void destroy();
	void makeCurrent(bool disableOther = true);

	
	virtual ~OpenGLDevice();

	HDC deviceContext;
protected:
	bool setDCPixelFormat(HDC& deviceContext,int stencil);
	
	HGLRC renderContext;
	
};

#endif // ifndef OPENGL_DEVICE_H
