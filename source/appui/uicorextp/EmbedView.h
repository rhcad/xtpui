#pragma once

class CEmbedWnd;
class CPaneManager;

class CEmbedView : public CView
{
protected: // create from serialization only
    CEmbedView();
    DECLARE_DYNCREATE(CEmbedView)

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEmbedView)
    public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO*);
    protected:
    virtual void OnInitialUpdate(); // called first time after construct
    virtual void OnDraw(CDC*) {};
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CEmbedView();

protected:
    CEmbedWnd*              m_wnd;
    CXTPDockingPaneManager  m_paneManager;
    CPaneManager*           m_paneLayout;

private:
    void CreateDockingPanes();

// Generated message map functions
protected:
    //{{AFX_MSG(CEmbedView)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
