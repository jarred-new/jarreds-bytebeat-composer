#pragma once
#include "afxwin.h"


// CWavExporterDlg dialog

class CWavExporterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWavExporterDlg)

public:
	CWavExporterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWavExporterDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_WAVEXPORTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_secondsExport;
	CEdit m_formulaOutput;
	afx_msg void OnBnClickedRadioBeatc();
	afx_msg void OnBnClickedRadioCarmen();
	bool isCEngine;
	virtual BOOL OnInitDialog();
};
