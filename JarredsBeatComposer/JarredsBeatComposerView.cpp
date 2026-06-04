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

// JarredsBeatComposerView.cpp : implementation of the CJarredsBeatComposerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "JarredsBeatComposer.h"
#endif

#include "JarredsBeatComposerDoc.h"
#include "JarredsBeatComposerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJarredsBeatComposerView

IMPLEMENT_DYNCREATE(CJarredsBeatComposerView, CEditView)

BEGIN_MESSAGE_MAP(CJarredsBeatComposerView, CEditView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(EN_CHANGE, &CJarredsBeatComposerView::OnEnChange)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

// CJarredsBeatComposerView construction/destruction

CJarredsBeatComposerView::CJarredsBeatComposerView()
{
	// TODO: add construction code here

}

CJarredsBeatComposerView::~CJarredsBeatComposerView()
{
}

BOOL CJarredsBeatComposerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

void CJarredsBeatComposerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CJarredsBeatComposerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CJarredsBeatComposerView diagnostics

#ifdef _DEBUG
void CJarredsBeatComposerView::AssertValid() const
{
	CEditView::AssertValid();
}

void CJarredsBeatComposerView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CJarredsBeatComposerDoc* CJarredsBeatComposerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJarredsBeatComposerDoc)));
	return (CJarredsBeatComposerDoc*)m_pDocument;
}
#endif //_DEBUG

CString CJarredsBeatComposerView::GetEditTextFormula() {
	CString s;
	GetEditCtrl().GetWindowTextW(s);
	return s;
}

// CJarredsBeatComposerView message handlers


void CJarredsBeatComposerView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	GetEditCtrl().SetMargins(50, 0);
}


void CJarredsBeatComposerView::OnPaint()
{
	// 1. Let the edit control draw the text first
	// This prevents the text from turning white or disappearing
	Default();

	// 2. Now paint the line numbers on top of the margin area
	CClientDC dc(this);
	CRect rectClient;
	GetClientRect(&rectClient);

	// Exclude the actual text area from being painted over to prevent flickering
	CEdit& edit = GetEditCtrl();
	CRect rectFormat;
	edit.GetRect(&rectFormat);
	dc.ExcludeClipRect(&rectFormat);

	// 3. Draw the margin background (Matches default window color)
	CRect rectMargin(0, 0, rectFormat.left, rectClient.Height());
	dc.FillSolidRect(&rectMargin, RGB(209, 209, 209));

	// 4. Draw the line numbers
	CFont* pOldFont = dc.SelectObject(GetFont());
	dc.SetTextColor(RGB(23, 212, 136)); // Gray text for numbers
	dc.SetBkMode(TRANSPARENT);

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	int nLineHeight = tm.tmHeight + tm.tmExternalLeading;

	int nFirstVisible = edit.GetFirstVisibleLine();
	int nTotalLines = edit.GetLineCount();
	int nVisibleCount = rectClient.Height() / nLineHeight;
	int nLastVisible = min(nFirstVisible + nVisibleCount, nTotalLines - 1);

	for (int i = nFirstVisible; i <= nLastVisible; i++)
	{
		CString strNum;
		strNum.Format(_T("%d"), i + 1);

		// Position numbers perfectly inside the margin space
		CRect rectNum(0, rectFormat.top + ((i - nFirstVisible) * nLineHeight), rectFormat.left - 5, rectFormat.top + ((i - nFirstVisible + 1) * nLineHeight));

		dc.DrawText(strNum, &rectNum, DT_RIGHT | DT_SINGLELINE | DT_VCENTER);
	}

	dc.SelectObject(pOldFont);
}


void CJarredsBeatComposerView::OnEnChange()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEditView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	Invalidate();
	UpdateWindow();
}


void CJarredsBeatComposerView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CEditView::OnVScroll(nSBCode, nPos, pScrollBar);
	Invalidate();
	UpdateWindow();
}
