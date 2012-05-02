/*! \file EmbedWnd.h
 *  \brief Define the proxy window class for IWndFactory.
 *  \author Zhang Yungui, X3 C++ PluginFramework
 *  \date   2011.5.26
 */
#ifndef X3_VIEW_MFC_EMBEDWND_H_
#define X3_VIEW_MFC_EMBEDWND_H_

#include "IWndFactory.h"

//! The proxy window class for IWndFactory.
/*! This window has a child window which IWndFactory is implemented.
    Use this to enable child window class to be implemented in
    another module and avoid problem in frame window.
    \ingroup _GROUP_VIEW_
*/
class CEmbedWnd : public CWnd
{
    DECLARE_DYNCREATE(CEmbedWnd)
public:
    CEmbedWnd(const char* clsidWnd = "");
    virtual ~CEmbedWnd();

// Attributes
public:
    //! Returns the child window's handle.
    HWND GetChildWnd() const;

    //! Returns the child window which IWndFactory is implemented.
    AnyObject GetWndObj() const;

    //! Returns the child window's class id.
    std::string GetClsid() const;

    //! Returns this window caption.
    CString GetCaption() const;

    //! Set window caption when embed in tab control.
    void SetCaption(LPCTSTR text);

// Operations
public:
    //! Create this embed window before call CreateSubWnd.
    bool CreateWnd(CWnd* pParentWnd, UINT id = AFX_IDW_PANE_FIRST, 
        DWORD dwStyle = WS_CHILD | WS_VISIBLE, DWORD dwExStyle = 0);

    //! Create the child window which IWndFactory is implemented.
    bool CreateSubWnd(const char* clsidWnd, int id = AFX_IDW_PANE_FIRST);

    //! Call IWndFactory::Refresh.
    void Refresh();

    //! Set sub menu index (see TrackPopupMenu).
    void SetContextMenu(int menuIndex);

    //! Set menu id to refresh (see IWndFactory::Refresh).
    static void SetViewRefreshID(UINT id);

// ClassWizard generated virtual function overrides
public:
    //{{AFX_VIRTUAL(CEmbedWnd)
    public:
    virtual BOOL DestroyWindow();
    virtual BOOL OnCmdMsg(UINT id, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    //}}AFX_VIRTUAL

    virtual bool TrackPopupMenu(int menuIndex, const CPoint& point)
    {
        menuIndex; point; return false;
    }

// Generated message map functions
protected:
    //{{AFX_MSG(CEmbedWnd)
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    Object<IWndFactory>  m_wnd;
    CString         m_caption;
    std::string     m_clsid;
    int             m_menuIndex;
    long            m_locker;
};

#endif // X3_VIEW_MFC_EMBEDWND_H_
