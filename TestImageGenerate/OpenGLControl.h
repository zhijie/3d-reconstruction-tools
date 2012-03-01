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

#if !defined(AFX_OPENGLCONTROL_H__52A6B63B_01A2_449D_8691_1FF59EECAB71__INCLUDED_)
#define AFX_OPENGLCONTROL_H__52A6B63B_01A2_449D_8691_1FF59EECAB71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenGLControl.h : Header-Datei
//
#include "stdafx.h"
#include "OpenGLDevice.h"
#include "DrawStuff.h"
#include <GL\gl.h>
#include <GL\glu.h>

/////////////////////////////////////////////////////////////////////////////
// Fenster COpenGLControl 

class GLPoint3f{
public:
	float x;
	float y;
	float z;
	GLPoint3f():x(3),y(4),z(5){}
};
class COpenGLControl : public CWnd
{
// Konstruktion
public:
	COpenGLControl();
	
// Attribute
public:
	Sample stuff;
	BackgroundColor backgroundColor;
// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(COpenGLControl)
	public:
	//}}AFX_VIRTUAL

// Implementierung
public:
	CClientDC* dc;
	void Create(CRect rect,CWnd* parent);
	virtual ~COpenGLControl();

	GLPoint3f eyePos;
	DrawStuff drawStuff;

	OpenGLDevice openGLDevice;
	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	void InitGL();
	void Render();
	


	//{{AFX_MSG(COpenGLControl)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_OPENGLCONTROL_H__52A6B63B_01A2_449D_8691_1FF59EECAB71__INCLUDED_
