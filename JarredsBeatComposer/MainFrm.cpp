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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "JarredsBeatComposer.h"
#include "JarredsBeatComposerView.h"
#include "WavExporter.h"
#include "FrequencyDlg.h"
#include "WavExporterDlg.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CWavExporterDlg;

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_BUTTON_PLAY, &CMainFrame::OnButtonPlay)
	ON_COMMAND(ID_BUTTON_STOP, &CMainFrame::OnButtonStop)
	ON_COMMAND(ID_VIEW_OUTPUT_LOG, &CMainFrame::OnViewOutputLog)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT_LOG, &CMainFrame::OnUpdateViewOutputLog)
	ON_COMMAND(ID_FILE_EXPORT, &CMainFrame::OnFileExport)
	ON_COMMAND(ID_BUTTON_SETFREQ, &CMainFrame::OnButtonSetfreq)
	ON_COMMAND(ID_COMBOTYPE, &CMainFrame::OnCombotype)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
	freq = 8000;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_VS2005; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = TRUE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	mdiTabParams.m_nTabBorderSize = 5;
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	CString databaseDir = GetAppDir() + L"formulas.txt";
	m_wndLibrary.LoadDatabase(databaseDir);

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	m_wndLibrary.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	DockPane(&m_wndLibrary);

	

	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	nSel = 0;
	pCombo = nullptr;
	pRibbon = &m_wndRibbonBar;
	if (pRibbon)
	{
		CMFCRibbonBaseElement* pElem = pRibbon->FindByID(ID_COMBOTYPE);
		pCombo = DYNAMIC_DOWNCAST(CMFCRibbonComboBox, pElem);
	}
	if (!pCombo)
		return 0;

	// Populate items once (safe to call repeatedly)
	if (pCombo->GetCount() == 0)
	{
		pCombo->AddItem(_T("Bytebeat"));
		pCombo->AddItem(_T("Signed Bytebeat"));
		pCombo->SelectItem(0); // default
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;
	// Create output window
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}

	// Create library window
	CString strLibraryWnd;
	bNameValid = strLibraryWnd.LoadString(IDS_LIBRARY_WND);
	ASSERT(bNameValid);
	if (!m_wndLibrary.Create(strLibraryWnd, this, CRect(0, 0, 200, 300), TRUE, ID_VIEW_LIBRARYWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Library window\n");
		return FALSE; // failed to create
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

void CMainFrame::CreateNewDocumentWithFormula(const CString& formula)
{
	// Get the first document template
	POSITION pos = theApp.GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate = theApp.GetNextDocTemplate(pos);

	if (pTemplate)
	{
		// Create a new document
		CDocument* pDoc = pTemplate->CreateNewDocument();
		if (pDoc)
		{
			// Create a new frame for this document
			CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDoc, NULL);
			if (pFrame)
			{
				// Initialize the document
				pTemplate->InitialUpdateFrame(pFrame, pDoc, TRUE);

				// Get the view from the document
				POSITION viewPos = pDoc->GetFirstViewPosition();
				if (viewPos)
				{
					CView* pView = pDoc->GetNextView(viewPos);
					CJarredsBeatComposerView* pEditView = dynamic_cast<CJarredsBeatComposerView*>(pView);
					if (pEditView)
					{
						// Set the formula text in the edit control
						CEdit& editCtrl = pEditView->GetEditCtrl();
						editCtrl.SetWindowTextW(formula);
						
						// Mark document as modified
						if (pDoc)
							pDoc->SetModifiedFlag(TRUE);
					}
				}
			}
		}
	}
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}


void CMainFrame::OnButtonPlay()
{
	auto* editTextView = (CJarredsBeatComposerView*)MDIGetActive()->GetActiveView();

	CString f = editTextView->GetEditTextFormula();

	std::wstring wFormula(f);
	std::string formula(wFormula.begin(), wFormula.end());

	m_engine.SetFormula(formula);

	CString playLog;
	playLog.Format(L"Playing: %s", f);

	if (nSel == 0)
	{
		m_wndOutput.AddLog(playLog);
		if (!m_player.Start(&m_engine, freq)) {
			if (m_engine.HasParseError()) {
				CString msg1;
				msg1.Format(L"Parser Error: %s",
					m_engine.GetParseError());

				m_wndOutput.AddLog(L"Stopped, error has been detected");
				m_wndOutput.AddLog(msg1);
			}

			if (m_engine.HasRuntimeError()) {
				CString msg2;
				msg2.Format(L"Runtime Error: %d",
					m_engine.GetRuntimeError());

				m_wndOutput.AddLog(L"Stopped, error has been detected");
				m_wndOutput.AddLog(msg2);
			}
		}
	}
	else if (nSel == 1) {
		m_wndOutput.AddLog(playLog);
		if (!m_player.StartSigned(&m_engine, freq)) {
			if (m_engine.HasParseError()) {
				CString msg1;
				msg1.Format(L"Parser Error: %s",
					m_engine.GetParseError());
				m_wndOutput.AddLog(L"Stopped, error has been detected");
				m_wndOutput.AddLog(msg1);
			}
			if (m_engine.HasRuntimeError()) {
				CString msg2;
				msg2.Format(L"Runtime Error: %d",
					m_engine.GetRuntimeError());
				m_wndOutput.AddLog(L"Stopped, error has been detected");
				m_wndOutput.AddLog(msg2);
			}
		}
	}
}


void CMainFrame::OnButtonStop()
{
	m_wndOutput.AddLog(L"Stopped");
	m_player.Stop();
}


void CMainFrame::OnViewOutputLog()
{
	if (m_wndOutput.IsVisible())
	{
		m_wndOutput.ShowPane(FALSE, FALSE, FALSE); // Hide
	}
	else
	{
		m_wndOutput.ShowPane(TRUE, FALSE, TRUE);  // Show & activate
	}
}


void CMainFrame::OnUpdateViewOutputLog(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndOutput.IsVisible());
}


void CMainFrame::OnFileExport()
{
	auto* editTextView = (CJarredsBeatComposerView*)MDIGetActive()->GetActiveView();

	CString f = editTextView->GetEditTextFormula();

	std::wstring wFormula(f);
	std::string formula(wFormula.begin(), wFormula.end());

	m_engine.SetFormula(formula);

	CWavExporterDlg exporterDlg;

	if (exporterDlg.DoModal() == IDOK) {
		CFileDialog sfd(FALSE, L"wav", NULL,
			OFN_OVERWRITEPROMPT,
			L"WAV Files (*.wav)|*.wav");

		if (sfd.DoModal() == IDOK) {
			CString fullPath = sfd.GetFolderPath()
				+ L"\\"
				+ sfd.GetFileName();
			m_wndOutput.AddLog(L"Exporting: " + fullPath);

			int seconds = exporterDlg.m_secondsExport;
			bool isCEngine = exporterDlg.isCEngine;

			CWavExporter::Export(fullPath,
				m_engine, 
				freq, 
				seconds, 
				isCEngine,
				nSel);

			m_wndOutput.AddLog(L"Export Done!");
		}
	}
}


void CMainFrame::OnButtonSetfreq()
{
	CFrequencyDlg freqDlg;

	freqDlg.SetFrequency(freq);
	if (freqDlg.DoModal() == IDOK) {
		this->freq = freqDlg.m_freq;
		UpdateData(TRUE);
	}
}

void CMainFrame::OnCombotype()
{
	// Read current selection
	nSel = pCombo->GetCurSel();
	

	// Handle selection change:
	// - update application state here (call a method on your document/controller)
	// - or send a message/command to other parts of the app
	// Example: call a hypothetical handler function (implement it in your class)
	// OnWaveTypeChanged(nSel, sSel);
	
	//CString sSel;
	//sSel.Format(L"%d", nSel);
	//AfxMessageBox(sSel);
}
