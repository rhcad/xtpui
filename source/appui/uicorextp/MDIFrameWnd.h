#pragma once

#include "MainFrm.h"

class CMainMDIFrame : public CXTPMDIFrameWnd, CMainFrame
{
public:
    DECLARE_DYNCREATE(CMainMDIFrame)
    CMainMDIFrame();
    virtual ~CMainMDIFrame();

// Attributes
public:

// Operations
public:
    BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
		CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMainMDIFrame)
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
    virtual BOOL OnCmdMsg(UINT id, int nCode, void* pExtra, AFX_CMDHANDLERINFO*);
    //}}AFX_VIRTUAL

// Implementation
private:
    CXTPTabClientWnd    m_wndClient;

// Generated message map functions
protected:
    //{{AFX_MSG(CMainMDIFrame)
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
