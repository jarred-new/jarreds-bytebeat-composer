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

// JarredsBeatComposerView.h : interface of the CJarredsBeatComposerView class
//

#pragma once

class CJarredsBeatComposerDoc;

class CJarredsBeatComposerView : public CEditView
{
protected: // create from serialization only
	CJarredsBeatComposerView();
	DECLARE_DYNCREATE(CJarredsBeatComposerView)

	// Flag to suppress handling of EN_CHANGE notifications while setting text programmatically
	bool m_settingTextProgrammatically;

	// Attributes
public:
	CJarredsBeatComposerDoc* GetDocument() const;

	// Operations
public:

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

	// Implementation
public:
	virtual ~CJarredsBeatComposerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	CString GetEditTextFormula();
	virtual void OnInitialUpdate();
	afx_msg void OnPaint();
	afx_msg void OnEnChange();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

#ifndef _DEBUG  // debug version in JarredsBeatComposerView.cpp
inline CJarredsBeatComposerDoc* CJarredsBeatComposerView::GetDocument() const
   { return reinterpret_cast<CJarredsBeatComposerDoc*>(m_pDocument); }
#endif

