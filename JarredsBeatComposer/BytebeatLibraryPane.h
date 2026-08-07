#pragma once

#include <afxcontrolbars.h>
#include <vector>


struct BYTEBEAT_ENTRY
{
	CString name;
	CString formula;
};


class CBytebeatLibraryPane : public CDockablePane
{
	DECLARE_DYNAMIC(CBytebeatLibraryPane)

public:

	CBytebeatLibraryPane();
	virtual ~CBytebeatLibraryPane();


	void LoadDatabase(CString filename);


protected:

	CTreeCtrl m_tree;

	std::vector<BYTEBEAT_ENTRY> m_entries;


	virtual void OnUpdateCmdUI(
		CFrameWnd* pTarget,
		BOOL bDisableIfNoHndler);


	afx_msg int OnCreate(
		LPCREATESTRUCT lpCreateStruct);


	afx_msg void OnSize(
		UINT nType,
		int cx,
		int cy);


	afx_msg void OnNMDblclkTree(
		NMHDR* pNMHDR,
		LRESULT* pResult);


	DECLARE_MESSAGE_MAP()


public:

	CString GetSelectedFormula();

};