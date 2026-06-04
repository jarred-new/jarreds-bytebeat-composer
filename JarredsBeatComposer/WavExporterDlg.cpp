// WavExporterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JarredsBeatComposer.h"
#include "WavExporterDlg.h"
#include "afxdialogex.h"
#include "JarredsBeatComposerView.h"

class CJarredsBeatComposerView;

// CWavExporterDlg dialog

IMPLEMENT_DYNAMIC(CWavExporterDlg, CDialogEx)

CWavExporterDlg::CWavExporterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWavExporterDlg::IDD, pParent)
	, m_secondsExport(10)
	, isCEngine(true)
{

}

CWavExporterDlg::~CWavExporterDlg()
{
}

void CWavExporterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SECONDS, m_secondsExport);
	DDV_MinMaxInt(pDX, m_secondsExport, 1, 999999);
	DDX_Control(pDX, IDC_EDIT_FORMULA, m_formulaOutput);
}


BEGIN_MESSAGE_MAP(CWavExporterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_BEATC, &CWavExporterDlg::OnBnClickedRadioBeatc)
	ON_BN_CLICKED(IDC_RADIO_CARMEN, &CWavExporterDlg::OnBnClickedRadioCarmen)
END_MESSAGE_MAP()


// CWavExporterDlg message handlers


void CWavExporterDlg::OnBnClickedRadioBeatc()
{
	isCEngine = true;
	UpdateData(TRUE);
}


void CWavExporterDlg::OnBnClickedRadioCarmen()
{
	isCEngine = false;
	UpdateData(TRUE);
}


BOOL CWavExporterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 1. Get the main frame of the MDI application
	CFrameWnd* pMainWnd = static_cast<CFrameWnd*>(AfxGetMainWnd());
	if (pMainWnd != nullptr)
	{
		// 2. Get the active MDI child frame
		CFrameWnd* pActiveChild = pMainWnd->GetActiveFrame();
		if (pActiveChild != nullptr)
		{
			// 3. Get the active view from the child frame
			CView* pView = pActiveChild->GetActiveView();

			// 4. Verify the view is the correct CEditView (or your custom derivative)
			if (pView != nullptr && pView->IsKindOf(RUNTIME_CLASS(CEditView)))
			{
				CEditView* pEditView = static_cast<CEditView*>(pView);

				// 5. Get the underlying CEdit control and fetch the text
				CEdit& editCtrl = pEditView->GetEditCtrl();
				CString strText;
				editCtrl.GetWindowText(strText);

				// 6. Set the formula text
				m_formulaOutput.SetWindowTextW(strText);
				m_formulaOutput.UpdateData(TRUE);
				// Now you have the text in strText, do something with it!
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
