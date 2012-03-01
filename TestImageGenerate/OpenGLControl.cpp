/**********************************************************************
    
  OpenGLControl
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

// OpenGLControl.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "OpenGLControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// COpenGLControl

COpenGLControl::COpenGLControl():stuff(COMPOUND),backgroundColor(WHITE_BACK)
{
	dc = NULL;
}

COpenGLControl::~COpenGLControl()
{
	if (dc)
	{
		delete dc;
	}
}


BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	//{{AFX_MSG_MAP(COpenGLControl)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten COpenGLControl 


void COpenGLControl::InitGL()
{
	//glClearColor(1,(double)0x99/255,0,1.0);
	//glClearColor(1,1,1,1);
	glClearColor(1,1,1,1);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

}

void COpenGLControl::Render()
{
	if (backgroundColor==WHITE_BACK){
		glClearColor(1,1,1,1);
	}else if (backgroundColor==BLACK_BACK){
		glClearColor(0,0,0,1);
	}
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();

	gluLookAt(eyePos.x,eyePos.y,eyePos.z,0,0,0,0,1,0);

	drawStuff.Draw(stuff);

	glFlush();

	SwapBuffers(dc->m_hDC);
}



void COpenGLControl::Create(CRect rect, CWnd *parent)
{
	CString className = AfxRegisterWndClass(
		CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
		NULL,
		(HBRUSH)GetStockObject(BLACK_BRUSH),
		NULL);

	CreateEx(
		0,
		className,
		"OpenGL",
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		rect,
		parent,
		0);

}

void COpenGLControl::OnPaint() 
{
	/** OpenGL section **/

	openGLDevice.makeCurrent();

	Render();
}

void COpenGLControl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cy == 0)								
	{
		cy = 1;						
	}
	glViewport(0,0,cx,cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,(GLfloat)cx/(GLfloat)cy,0.1,3000.0);
	glMatrixMode(GL_MODELVIEW);
}


int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	dc = new CClientDC(this);

	openGLDevice.create(dc->m_hDC);
	InitGL();

	return 0;
}

BOOL COpenGLControl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
