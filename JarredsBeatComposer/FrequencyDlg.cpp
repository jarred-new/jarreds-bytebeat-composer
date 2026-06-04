// FrequencyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JarredsBeatComposer.h"
#include "FrequencyDlg.h"
#include "afxdialogex.h"


// CFrequencyDlg dialog

IMPLEMENT_DYNAMIC(CFrequencyDlg, CDialogEx)

CFrequencyDlg::CFrequencyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFrequencyDlg::IDD, pParent)
	, m_freq(0)
{

}

CFrequencyDlg::~CFrequencyDlg()
{
}

void CFrequencyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_freq);
	DDV_MinMaxInt(pDX, m_freq, 1000, 999999);
}


BEGIN_MESSAGE_MAP(CFrequencyDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CFrequencyDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CFrequencyDlg message handlers


void CFrequencyDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	//UpdateData(TRUE);
}
