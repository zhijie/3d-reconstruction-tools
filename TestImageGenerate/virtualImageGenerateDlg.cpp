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

// virtualImageGenerateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "virtualImageGenerate.h"
#include "virtualImageGenerateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CvirtualImageGenerateDlg dialog




CvirtualImageGenerateDlg::CvirtualImageGenerateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CvirtualImageGenerateDlg::IDD, pParent),
	orgPoint(10,10),glWidth(300),glHeight(300)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CvirtualImageGenerateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, sliderEyePos_X);
	DDX_Control(pDX, IDC_SLIDER2, sliderEyePos_Y);
	DDX_Control(pDX, IDC_SLIDER3, sliderEyePos_Z);
	DDX_Control(pDX, IDC_STATIC_Z, label_Z);
	DDX_Control(pDX, IDC_STATIC_Y, label_Y);
	DDX_Control(pDX, IDC_STATIC_X, label_X);
}

BEGIN_MESSAGE_MAP(CvirtualImageGenerateDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CvirtualImageGenerateDlg::OnBnClickedSaveImage)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CvirtualImageGenerateDlg::OnChangEyePos_X)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CvirtualImageGenerateDlg::OnChangeEyePos_Y)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CvirtualImageGenerateDlg::OnChangeEyePos_Z)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RADIO1, &CvirtualImageGenerateDlg::OnBnClickedRadio_Combination)
	ON_BN_CLICKED(IDC_RADIO2, &CvirtualImageGenerateDlg::OnBnClickedRadio_Cube)
	ON_BN_CLICKED(IDC_RADIO3, &CvirtualImageGenerateDlg::OnBnClickedRadio_Chessboard)
	ON_BN_CLICKED(IDC_RADIO4, &CvirtualImageGenerateDlg::OnChangeBackroundToWhite)
	ON_BN_CLICKED(IDC_RADIO5, &CvirtualImageGenerateDlg::OnChangeBackgroundToBlack)
END_MESSAGE_MAP()


// CvirtualImageGenerateDlg message handlers

BOOL CvirtualImageGenerateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	

	//setup gl
	CRect rect(orgPoint,CPoint(orgPoint.x+glWidth,orgPoint.y+glHeight));
	openGL.Create(rect,this);

	//initial slider controls
	sliderEyePos_X.SetRange(-50,50,true);
	sliderEyePos_Y.SetRange(-50,50,true);
	sliderEyePos_Z.SetRange(-50,50,true);
	sliderEyePos_X.SetPos((int)openGL.eyePos.x*10);
	sliderEyePos_Y.SetPos((int)openGL.eyePos.y*10);
	sliderEyePos_Z.SetPos((int)openGL.eyePos.z*10);
	CString txt;
	txt.Format("%3.1f",openGL.eyePos.x);
	label_X.SetWindowText(txt.GetBuffer());
	txt.Format("%3.1f",openGL.eyePos.y);
	label_Y.SetWindowText(txt.GetBuffer());
	txt.Format("%3.1f",openGL.eyePos.z);
	label_Z.SetWindowText(txt.GetBuffer());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CvirtualImageGenerateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CvirtualImageGenerateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CvirtualImageGenerateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CvirtualImageGenerateDlg::OnChangEyePos_X(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	openGL.eyePos.x=(float)sliderEyePos_X.GetPos()/10;
	CString t;
	t.Format("%3.1f",openGL.eyePos.x);
	label_X.SetWindowText(t.GetBuffer());
	*pResult = 0;
}

void CvirtualImageGenerateDlg::OnChangeEyePos_Y(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	openGL.eyePos.y=(float)sliderEyePos_Y.GetPos()/10;
	CString t;
	t.Format("%3.1f",openGL.eyePos.y);
	label_Y.SetWindowText(t.GetBuffer());
	*pResult = 0;
}

void CvirtualImageGenerateDlg::OnChangeEyePos_Z(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	openGL.eyePos.z=(float)sliderEyePos_Z.GetPos()/10;
	CString t;
	t.Format("%3.1f",openGL.eyePos.z);
	label_Z.SetWindowText(t.GetBuffer());
	*pResult = 0;
}
///////////////////////////////////////////////////
//save bmp into a file
//void CvirtualImageGenerateDlg::errhandler(char* message)
//{
//	MessageBox(message);
//	exit(-1);
//}
//PBITMAPINFO CvirtualImageGenerateDlg::CreateBitmapInfoStruct(HBITMAP hBmp)
//{ 
//	BITMAP bmp; 
//	PBITMAPINFO pbmi; 
//	WORD    cClrBits; 
//
//	// Retrieve the bitmap color format, width, and height. 
//	if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) 
//		errhandler("GetObject"); 
//
//	// Convert the color format to a count of bits. 
//	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
//	if (cClrBits == 1) 
//		cClrBits = 1; 
//	else if (cClrBits <= 4) 
//		cClrBits = 4; 
//	else if (cClrBits <= 8) 
//		cClrBits = 8; 
//	else if (cClrBits <= 16) 
//		cClrBits = 16; 
//	else if (cClrBits <= 24) 
//		cClrBits = 24; 
//	else cClrBits = 32; 
//
//	// Allocate memory for the BITMAPINFO structure. (This structure 
//	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
//	// data structures.) 
//
//	if (cClrBits != 24) 
//		pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
//		sizeof(BITMAPINFOHEADER) + 
//		sizeof(RGBQUAD) * (1<< cClrBits)); 
//
//	// There is no RGBQUAD array for the 24-bit-per-pixel format. 
//
//	else 
//		pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
//		sizeof(BITMAPINFOHEADER)); 
//
//	// Initialize the fields in the BITMAPINFO structure. 
//
//	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
//	pbmi->bmiHeader.biWidth = bmp.bmWidth; 
//	pbmi->bmiHeader.biHeight = bmp.bmHeight; 
//	pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
//	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
//	if (cClrBits < 24) 
//		pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 
//
//	// If the bitmap is not compressed, set the BI_RGB flag. 
//	pbmi->bmiHeader.biCompression = BI_RGB; 
//
//	// Compute the number of bytes in the array of color 
//	// indices and store the result in biSizeImage. 
//	// For Windows NT, the width must be DWORD aligned unless 
//	// the bitmap is RLE compressed. This example shows this. 
//	// For Windows 95/98/Me, the width must be WORD aligned unless the 
//	// bitmap is RLE compressed.
//	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
//		* pbmi->bmiHeader.biHeight; 
//	// Set biClrImportant to 0, indicating that all of the 
//	// device colors are important. 
//	pbmi->bmiHeader.biClrImportant = 0; 
//	return pbmi; 
//} 
//void CvirtualImageGenerateDlg::CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi, 
//				   HBITMAP hBMP, HDC hDC) 
//{ 
//	HANDLE hf;                 // file handle 
//	BITMAPFILEHEADER hdr;       // bitmap file-header 
//	PBITMAPINFOHEADER pbih;     // bitmap info-header 
//	LPBYTE lpBits;              // memory pointer 
//	DWORD dwTotal;              // total count of bytes 
//	DWORD cb;                   // incremental count of bytes 
//	BYTE *hp;                   // byte pointer 
//	DWORD dwTmp; 
//
//	pbih = (PBITMAPINFOHEADER) pbi; 
//	lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
//
//	if (!lpBits) 
//		errhandler("GlobalAlloc"); 
//
//	// Retrieve the color table (RGBQUAD array) and the bits 
//	// (array of palette indices) from the DIB. 
//	if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, 
//		DIB_RGB_COLORS)) 
//	{
//		errhandler("GetDIBits"); 
//	}
//
//	// Create the .BMP file. 
//	hf = CreateFile(pszFile, 
//		GENERIC_READ | GENERIC_WRITE, 
//		(DWORD) 0, 
//		NULL, 
//		CREATE_ALWAYS, 
//		FILE_ATTRIBUTE_NORMAL, 
//		(HANDLE) NULL); 
//	if (hf == INVALID_HANDLE_VALUE) 
//		errhandler("CreateFile"); 
//	hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
//	// Compute the size of the entire file. 
//	hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + 
//		pbih->biSize + pbih->biClrUsed 
//		* sizeof(RGBQUAD) + pbih->biSizeImage); 
//	hdr.bfReserved1 = 0; 
//	hdr.bfReserved2 = 0; 
//
//	// Compute the offset to the array of color indices. 
//	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + 
//		pbih->biSize + pbih->biClrUsed 
//		* sizeof (RGBQUAD); 
//
//	// Copy the BITMAPFILEHEADER into the .BMP file. 
//	if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), 
//		(LPDWORD) &dwTmp,  NULL)) 
//	{
//		errhandler("WriteFile"); 
//	}
//
//	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
//	if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) 
//		+ pbih->biClrUsed * sizeof (RGBQUAD), 
//		(LPDWORD) &dwTmp, ( NULL)) )
//		errhandler("WriteFile"); 
//
//	// Copy the array of color indices into the .BMP file. 
//	dwTotal = cb = pbih->biSizeImage; 
//	hp = lpBits; 
//	if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) 
//		errhandler("WriteFile"); 
//
//	// Close the .BMP file. 
//	if (!CloseHandle(hf)) 
//		errhandler("CloseHandle"); 
//
//	// Free memory. 
//	GlobalFree((HGLOBAL)lpBits);
//}
//
void CvirtualImageGenerateDlg::OnBnClickedSaveImage()
{
	//save gl to an image
	CFileDialog fileDlg(false,NULL,NULL,4|2,"Bitmap(*.bmp)|*.bmp||");
	if (fileDlg.DoModal()==IDOK){
		CString filename=fileDlg.GetPathName();
		CString ext=fileDlg.GetFileExt();
		if (ext!="bmp"){
			filename.Append(".bmp");
		}
		////use gdi+
		//Image img(filename.GetString());
		//Graphics graphics();
		
		//CDC* dc=GetDC();
		//CDC* memDC;
		//memDC->CreateCompatibleDC(dc);
		//CBitmap bmp;
		//bmp.CreateCompatibleBitmap(dc,glWidth,glHeight);
		//memDC->SelectObject(&bmp);

		int datasize=glWidth*glHeight*3;
		char* data=new char[datasize];
		glReadPixels(0,0,glWidth,glHeight,GL_RGB,GL_UNSIGNED_BYTE,data);
		//change storage mode from bgr to rgb
		for (int i=0;i<datasize;i+=3){			
			data[i]^=data[i+2]^=data[i]^=data[i+2];
		}


		BITMAPFILEHEADER bitmapFileHeader; 
		BITMAPINFOHEADER bitmapInfoHeader; 

		bitmapFileHeader.bfType = 0x4D42; //"BM" 
		bitmapFileHeader.bfSize = datasize; 
		bitmapFileHeader.bfReserved1 = 0; 
		bitmapFileHeader.bfReserved2 = 0; 
		bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 

		bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER); 
		bitmapInfoHeader.biWidth = glWidth; 
		bitmapInfoHeader.biHeight = glHeight; 
		bitmapInfoHeader.biPlanes = 1; 
		bitmapInfoHeader.biBitCount = 24; 
		bitmapInfoHeader.biCompression = BI_RGB; 
		bitmapInfoHeader.biSizeImage = 0; 
		bitmapInfoHeader.biXPelsPerMeter = 0; 
		bitmapInfoHeader.biYPelsPerMeter = 0; 
		bitmapInfoHeader.biClrUsed = 0; 
		bitmapInfoHeader.biClrImportant = 0; 
		CFile file;
		file.Open(filename.GetBuffer(),CFile::modeCreate|CFile::modeWrite);
		if (file){
			file.Write(&bitmapFileHeader,sizeof(bitmapFileHeader));
			file.Write(&bitmapInfoHeader,sizeof(bitmapInfoHeader));
			file.Write(data,datasize);
			file.Close();
		}else{
			MessageBox("filed to open file to write bmp ");
			//exit(0);
		}

		delete[] data;
	}
}

void CvirtualImageGenerateDlg::OnClose()
{
	
	

	CDialog::OnClose();
}

void CvirtualImageGenerateDlg::OnBnClickedRadio_Combination()
{
	openGL.stuff=COMPOUND;
}

void CvirtualImageGenerateDlg::OnBnClickedRadio_Cube()
{
	openGL.stuff=CUBE;
}

void CvirtualImageGenerateDlg::OnBnClickedRadio_Chessboard()
{
	openGL.stuff=CHESSBOARD;
}

void CvirtualImageGenerateDlg::OnChangeBackroundToWhite()
{
	openGL.backgroundColor=WHITE_BACK;
}

void CvirtualImageGenerateDlg::OnChangeBackgroundToBlack()
{
	openGL.backgroundColor=BLACK_BACK;
}
