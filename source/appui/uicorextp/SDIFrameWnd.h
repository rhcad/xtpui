#pragma once

#include "MainFrm.h"

class CMainSDIFrame : public CXTPFrameWnd, CMainFrame
{
public:
    DECLARE_DYNCREATE(CMainSDIFrame)
    CMainSDIFrame();
    virtual ~CMainSDIFrame();

// Attributes
public:

// Operations
public:
    BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
		CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMainSDIFrame)
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCmdMsg(UINT id, int nCode, void* pExtra, AFX_CMDHANDLERINFO*);
    //}}AFX_VIRTUAL

// Implementation
private:

// Generated message map functions
protected:
    //{{AFX_MSG(CMainSDIFrame)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnClose();
    afx_msg void OnDestroy();
    //}}AFX_MSG
    afx_msg void OnUpdateRibbonTab(CCmdUI* pCmdUI);
    afx_msg void OnCustomize();
    afx_msg void OnCustomizeQuickAccess();
    afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
};
