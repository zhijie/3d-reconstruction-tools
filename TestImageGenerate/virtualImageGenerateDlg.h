/**********************************************************************
    
  virtualImageGenerateDlg
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

// virtualImageGenerateDlg.h : header file
//

#pragma once
#include "OpenGLControl.h"
#include "afxcmn.h"
#include "afxwin.h"

#pragma comment(lib,"gdiplus.lib")

// CvirtualImageGenerateDlg dialog
class CvirtualImageGenerateDlg : public CDialog
{
// Construction
public:
	CvirtualImageGenerateDlg(CWnd* pParent = NULL);	// standard constructor
	COpenGLControl openGL;

	CPoint orgPoint;
	int glWidth;
	int glHeight;

	// Dialog Data
	enum { IDD = IDD_VIRTUALIMAGEGENERATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support



// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSaveImage();
	CSliderCtrl sliderEyePos_X;
	CSliderCtrl sliderEyePos_Y;
	CSliderCtrl sliderEyePos_Z;
	afx_msg void OnChangEyePos_X(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeEyePos_Y(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeEyePos_Z(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();

	void errhandler(char* message);
	PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp);
	void CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi, 
		HBITMAP hBMP, HDC hDC) ;
	CStatic label_Z;
	CStatic label_Y;
	CStatic label_X;
	afx_msg void OnBnClickedRadio_Combination();
	afx_msg void OnBnClickedRadio_Cube();
	afx_msg void OnBnClickedRadio_Chessboard();
	afx_msg void OnChangeBackroundToWhite();
	afx_msg void OnChangeBackgroundToBlack();
};
