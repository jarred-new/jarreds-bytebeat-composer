#pragma once
#include <afxcmn.h>
#include "resource.h"

// CFrequencyDlg dialog

class CFrequencyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFrequencyDlg)

public:
	CFrequencyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFrequencyDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_FREQ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_freq;
	afx_msg void OnEnChangeEdit1();
};
