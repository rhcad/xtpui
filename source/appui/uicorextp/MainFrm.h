#pragma once

class CPaneManager;

class CMainFrame
{
public:
    CMainFrame();
    ~CMainFrame();

public:
    UINT GetFrameID() const { return m_id; }
    std::wstring GetLocalizationString(const std::wstring& name) const;
    UINT FindID(const std::wstring& name) const;
    BOOL HasRibbonBar() const;

protected:
    BOOL PreCreateWindow(CREATESTRUCT& cs);
    BOOL BeforeLoadFrame(CFrameWnd* pMainWnd);
    BOOL AfterLoadFrame();
    BOOL OnCmdMsg(UINT id, int nCode, void* pExtra, AFX_CMDHANDLERINFO*);
    BOOL InitRibbonBars(CXTPCommandBars* pCommandBars);
    void LoadCommandBars();
    void SaveCommandBars();

    afx_msg void OnClose();
    afx_msg void OnDestroy();
    afx_msg void OnUpdateRibbonTab(CCmdUI* pCmdUI);
    afx_msg void OnCustomize();
    afx_msg void OnCustomizeQuickAccess();
    int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
    LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);

private:
    void SetRibbonFont();
    BOOL InitRibbonTheme();
    BOOL CreateStatusBar();
    BOOL CreateRibbonBar();
    void CreateDockingPanes();

    UINT GetNodeID(const ConfigSection& node, LPCWSTR name);
    void ShowCustomizeDialog(int nSelectedPage);
    void ExchangeLayout(CXTPPropExchange* px);

protected:
    CXTPStatusBar       m_statusBar;
    UINT                m_id;
    std::wstring        m_appid;
    std::wstring        m_appname;
    bool                m_enableCustomize;
    CXTPDockingPaneManager  m_paneManager;
    CPaneManager*       m_paneLayout;

private:
    CFrameWnd*          m_mainwnd;
    CXTPCommandBars*    m_cmdbars;
    ConfigSection       m_frameNode;
    std::map<std::wstring, UINT>    m_idnames;
    std::wstring        m_barsfile;
};
