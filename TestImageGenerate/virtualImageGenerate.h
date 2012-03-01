/**********************************************************************
    
  virtualImageGenerate
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

// virtualImageGenerate.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CvirtualImageGenerateApp:
// See virtualImageGenerate.cpp for the implementation of this class
//

class CvirtualImageGenerateApp : public CWinApp
{
public:
	CvirtualImageGenerateApp();
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CvirtualImageGenerateApp theApp;