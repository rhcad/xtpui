/*! \file EmbedWnd.cpp
 *  \brief Implement the proxy window class of CEmbedWnd.
 *  \author Zhang Yungui, X3 C++ PluginFramework
 *  \date   2011.5.26
 */
#include "stdafx.h"
#include "EmbedWnd.h"
#include "WndFactory.h"
#include <utilfunc/lockcount.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static UINT s_nRefreshID = 0;

IMPLEMENT_DYNCREATE(CEmbedWnd, CWnd)

BEGIN_MESSAGE_MAP(CEmbedWnd, CWnd)
    //{{AFX_MSG_MAP(CEmbedWnd)
    ON_WM_SETFOCUS()
    ON_WM_SIZE()
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CEmbedWnd::CEmbedWnd(const char* clsidWnd)
    : m_clsid(clsidWnd), m_menuIndex(-1), m_locker(0)
{
}

CEmbedWnd::~CEmbedWnd()
{
    SafeCall(m_wnd, DestroyWnd());
}

void CEmbedWnd::SetViewRefreshID(UINT id)
{
    s_nRefreshID = id;
}

void CEmbedWnd::SetContextMenu(int menuIndex)
{
    m_menuIndex = menuIndex;
}

HWND CEmbedWnd::GetChildWnd() const
{
    return this && m_wnd ? m_wnd->GetWindow() : NULL;
}

AnyObject CEmbedWnd::GetWndObj() const
{
    return AnyObject(m_wnd);
}

CString CEmbedWnd::GetCaption() const
{
    return m_caption;
}

void CEmbedWnd::SetCaption(LPCTSTR text)
{
    m_caption = text;
}

std::string CEmbedWnd::GetClsid() const
{
    return m_clsid;
}

BOOL CEmbedWnd::DestroyWindow()
{
    SafeCall(m_wnd, DestroyWnd());
    return CWnd::DestroyWindow();
}

BOOL CEmbedWnd::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CWnd::PreCreateWindow(cs))
        return FALSE;

    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

    return TRUE;
}

bool CEmbedWnd::CreateWnd(CWnd* pParentWnd, UINT id, DWORD dwStyle, DWORD dwExStyle)
{
    return !!CWnd::CreateEx(dwExStyle, NULL,
        m_caption.IsEmpty() ? _T("EmbedWnd") : m_caption,
        dwStyle, CRect(0, 0, 0, 0), pParentWnd, id);
}

bool CEmbedWnd::CreateSubWnd(const char* clsidWnd, int id)
{
    ASSERT(!m_wnd && GetSafeHwnd());

    m_clsid = clsidWnd;
    if (clsidWnd && *clsidWnd && m_wnd.create(clsidWnd))
    {
        if (m_wnd->CreateWnd(GetSafeHwnd(), id))
        {
            // for embed dialog
            if (::GetDlgCtrlID(m_wnd->GetWindow()) != AFX_IDW_PANE_FIRST)
            {
                ModifyStyleEx(WS_EX_STATICEDGE | WS_EX_CLIENTEDGE, 0);
            }

            return true;
        }
    }

    return false;
}

void CEmbedWnd::OnPaint()
{
    CPaintDC dc(this);

    if (GetChildWnd() == NULL)
    {
        CRect rect;
        GetClientRect(&rect);

        dc.SetBkMode(TRANSPARENT);
        dc.SetTextColor(RGB(255, 0, 0));

        if (!m_caption.IsEmpty())
        {
            dc.DrawText(m_caption, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            rect.top += dc.GetTextExtent(m_caption).cy * 2;
        }
        ::DrawTextA(dc.GetSafeHdc(),
            m_clsid.empty() ? "?" : m_clsid.c_str(),
            -1, &rect,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
}

void CEmbedWnd::Refresh()
{
    SafeCall(m_wnd, Refresh());
}

BOOL CEmbedWnd::OnCmdMsg(UINT id, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    LockCount locker (&m_locker);
    if (m_locker > 1)
    {
        return FALSE;
    }

    if (m_wnd && ::IsWindowVisible(m_wnd->GetWindow()))
    {
        if (DoCmdMsgHelper(m_wnd.p(), id, nCode, pExtra, pHandlerInfo))
        {
            return TRUE;
        }

        if (id == s_nRefreshID && CN_COMMAND == nCode)
        {
            if (NULL == pHandlerInfo)
            {
                m_wnd->Refresh();
            }

            return TRUE;
        }
    }

    return CWnd::OnCmdMsg(id, nCode, pExtra, pHandlerInfo);
}

void CEmbedWnd::OnSetFocus(CWnd* pOldWnd)
{
    CWnd::OnSetFocus(pOldWnd);

    if (m_wnd)
    {
        ::SetFocus(m_wnd->GetWindow());
    }
}

void CEmbedWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    if (m_wnd && cx > 0 && cy > 0)
    {
        ::MoveWindow(m_wnd->GetWindow(), 0, 0, cx, cy, TRUE);
    }
}

BOOL CEmbedWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    AFX_NOTIFY notify;
    notify.pResult = pResult;
    notify.pNMHDR = (NMHDR*)lParam;

    int nCode = MAKELONG(notify.pNMHDR->code, WM_REFLECT_BASE+WM_NOTIFY);

    if (m_wnd && DoCmdMsgHelper(m_wnd.p(), 0, nCode, &notify, NULL))
    {
        return TRUE;
    }

    return CWnd::OnNotify(wParam, lParam, pResult);
}

BOOL CEmbedWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    if (message == WM_CONTEXTMENU && m_menuIndex >= 0)
    {
        if (TrackPopupMenu(m_menuIndex, CPoint(lParam)))
        {
            return TRUE;
        }
    }

    return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}
