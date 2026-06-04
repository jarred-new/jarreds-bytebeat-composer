// SplashScreen.cpp : implementation file
//

#include "stdafx.h"
#include "JarredsBeatComposer.h"
#include "SplashScreen.h"
#include "afxdialogex.h"


// CSplashScreen dialog

IMPLEMENT_DYNAMIC(CSplashScreen, CDialogEx)

CSplashScreen::CSplashScreen(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSplashScreen::IDD, pParent)
{

}

CSplashScreen::~CSplashScreen()
{
}

void CSplashScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressBar);
}


BEGIN_MESSAGE_MAP(CSplashScreen, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CSplashScreen::OnNMCustomdrawProgress1)
END_MESSAGE_MAP()


// CSplashScreen message handlers


void CSplashScreen::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDB_SPLASHSCREEN), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	if (hBmp) {
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);

		HBITMAP hOldBmp = (HBITMAP)memDC.SelectObject(hBmp);

		CRect rect;
		GetClientRect(&rect);

		BITMAP bmp;
		::GetObject(hBmp, sizeof(bmp), &bmp);

		dc.SetStretchBltMode(HALFTONE);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(),
			&memDC, 0, 0, bmp.bmWidth, bmp.bmHeight,
			SRCCOPY);

		memDC.SelectObject(hOldBmp);
		::DeleteObject(hBmp);
	}
}


void CSplashScreen::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_timer) {
		KillTimer(m_timer);
		this->DestroyWindow();
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CSplashScreen::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	m_progressBar.SetMarquee(TRUE, 10);

	m_timer = SetTimer(TIMER_SPLASH, 5000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSplashScreen::OnNMCustomdrawProgress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
