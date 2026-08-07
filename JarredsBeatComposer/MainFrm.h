// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "OutputWnd.h"
#include "BytebeatLibraryPane.h"

#include "BytebeatEngine.h"
#include "BytebeatPlayer.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	COutputWnd        m_wndOutput;
	CBytebeatLibraryPane m_wndLibrary;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

protected:
	CBytebeatEngine m_engine;
	CBytebeatPlayer m_player;
public:
	void CreateNewDocumentWithFormula(const CString& formula);
	afx_msg void OnButtonPlay();
	afx_msg void OnButtonStop();
	afx_msg void OnViewOutputLog();
	afx_msg void OnUpdateViewOutputLog(CCmdUI *pCmdUI);
	afx_msg void OnFileExport();
	afx_msg void OnButtonSetfreq();

protected:
	CString GetAppDir()
	{
		wchar_t szPath[MAX_PATH];

		// Get full path of current executable
		GetModuleFileNameW(
			NULL,
			szPath,
			MAX_PATH);


		CString path(szPath);


		// Find last backslash
		int lastSlash = path.ReverseFind(_T('\\'));


		if (lastSlash != -1)
		{
			return path.Left(lastSlash + 1);
		}


		return path;
	}

protected:
	int freq;
	CMFCRibbonComboBox* pCombo;
	CMFCRibbonBar* pRibbon;
	int nSel;
public:
	afx_msg void OnCombotype();
};


