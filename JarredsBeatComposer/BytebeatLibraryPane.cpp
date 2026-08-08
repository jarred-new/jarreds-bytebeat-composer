#include "stdafx.h"
#include "BytebeatLibraryPane.h"
#include "MainFrm.h"

#include <fstream>
#include <string>

IMPLEMENT_DYNAMIC(CBytebeatLibraryPane, CDockablePane)

BEGIN_MESSAGE_MAP(
	CBytebeatLibraryPane,
	CDockablePane)

	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_NOTIFY(
	NM_DBLCLK,
	1001,
	OnNMDblclkTree)

END_MESSAGE_MAP()



CBytebeatLibraryPane::CBytebeatLibraryPane()
{
}



CBytebeatLibraryPane::~CBytebeatLibraryPane()
{
}



int CBytebeatLibraryPane::OnCreate(
	LPCREATESTRUCT lpCreateStruct)
{

	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Get the client area rectangle
	CRect rect;
	GetClientRect(&rect);

	if (rect.IsRectEmpty())
	{
		rect.SetRect(0, 0, 200, 300);
	}

	if (!m_tree.Create(
		WS_VISIBLE |
		WS_CHILD |
		WS_BORDER |
		TVS_HASLINES |
		TVS_LINESATROOT |
		TVS_HASBUTTONS |
		TVS_SHOWSELALWAYS,

		rect,
		this,
		1001))
	{
		TRACE0("Failed to create tree control\n");
		return -1;
	}

	/*m_tree.InsertItem(
		_T("Bytebeat Formula Library"));*/

	return 0;
}

void CBytebeatLibraryPane::OnUpdateCmdUI(
	CFrameWnd* pTarget,
	BOOL bDisableIfNoHndler)
{
	CDockablePane::OnUpdateCmdUI(
		pTarget,
		bDisableIfNoHndler);
}

void CBytebeatLibraryPane::OnSize(
	UINT nType,
	int cx,
	int cy)
{

	CDockablePane::OnSize(
		nType,
		cx,
		cy);


	if (m_tree.GetSafeHwnd())
	{
		m_tree.MoveWindow(
			0,
			0,
			cx,
			cy);
	}
}

void CBytebeatLibraryPane::LoadDatabase(
	CString filename)
{

	m_tree.DeleteAllItems();
	m_entries.clear();


	HTREEITEM root =
		m_tree.InsertItem(
		_T("Bytebeat Formula Library"));

	CT2A fileNameA(filename);

	std::ifstream file(
		fileNameA);

	if (!file)
		return;

	std::string line;
	CString currentName;
	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		// Comment = title

		if (line.find("//") == 0)
		{

			currentName =
				CString(
				line.substr(2).c_str());

			currentName.Trim();
		}
		else
		{
			BYTEBEAT_ENTRY e;
			e.name =
				currentName;

			e.formula =
				CString(
				line.c_str());

			e.formula.Trim();

			m_entries.push_back(e);

			HTREEITEM item =
				m_tree.InsertItem(
				e.name,
				root);

			m_tree.SetItemData(
				item,
				(DWORD_PTR)
				(m_entries.size() - 1));

			currentName.Empty();
		}
	}

	m_tree.Expand(
		root,
		TVE_EXPAND);

}

void CBytebeatLibraryPane::OnNMDblclkTree(
	NMHDR* pNMHDR,
	LRESULT* pResult)
{

	HTREEITEM item =
		m_tree.GetSelectedItem();


	if (item)
	{
		DWORD_PTR data =
			m_tree.GetItemData(item);

		if (data < m_entries.size())
		{
			CString formula =
				m_entries[data].formula;

			// Create a new tab with the selected formula
			CMainFrame* pMainFrame = 
				(CMainFrame*)AfxGetMainWnd();

			if (pMainFrame)
			{
				pMainFrame->CreateNewDocumentWithFormula(formula);
			}
		}

	}
	*pResult = 0;
}

CString CBytebeatLibraryPane::GetSelectedFormula()
{
	HTREEITEM item =
		m_tree.GetSelectedItem();

	if (item)
	{
		DWORD_PTR data =
			m_tree.GetItemData(item);

		if (data < m_entries.size())
			return m_entries[data].formula;

	}

	return _T("");
}