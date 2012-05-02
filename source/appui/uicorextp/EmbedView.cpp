#include "stdafx.h"
#include "EmbedView.h"
#include "EmbedWnd.h"
#include "PaneManager.h"
#include "ViewItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CEmbedView, CView)

BEGIN_MESSAGE_MAP(CEmbedView, CView)
    //{{AFX_MSG_MAP(CEmbedView)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_WM_SIZE()
    ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
    ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CEmbedView::CEmbedView() : m_wnd(NULL), m_paneLayout(NULL)
{
}

CEmbedView::~CEmbedView()
{
    SafeDelete(m_paneLayout);
    SafeDelete(m_wnd);
}

BOOL CEmbedView::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style &= ~WS_BORDER;
    cs.dwExStyle &= ~(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
    cs.lpszName = _T("EmbedView");
    return CView::PreCreateWindow(cs);
}

void CEmbedView::OnInitialUpdate()
{
    CView::OnInitialUpdate();
}

static inline bool IsHideClient(const std::wstring& clsid)
{
	return StrCmpNW(clsid.c_str(), L"-", 1) == 0;
}

int CEmbedView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

    ViewItem* item = FindViewItem(GetDocument()->GetDocTemplate());
    m_wnd = new CEmbedWnd();

    if (item)
    {
        m_wnd->SetCaption(item->caption.c_str());
    }

    VERIFY(m_wnd->CreateWnd(this, 1));

    if (item)
    {
        if (!m_wnd->CreateSubWnd(x3::w2a(item->clsid).c_str(), 2)
            && !IsHideClient(item->clsid))
        {
            X3LOG_WARNING2(L"@uicorextp:IDS_CREATEVIEW_FAIL", item->clsid);
        }

        if (!item->layout.empty())
        {
            m_paneManager.InstallDockingPanes(this);
            m_paneManager.HideClient(IsHideClient(item->clsid));

            m_paneLayout = new CPaneManager(this, &m_paneManager, item->layout);
            m_paneLayout->CreateDockingPanes();
            if (m_paneLayout->GetViewCount() > 0)
            {
                m_wnd->ModifyStyleEx(0, WS_EX_STATICEDGE);
            }
        }
    }
	
	return 0;
}

void CEmbedView::OnDestroy()
{
    SafeCall(m_paneLayout, SaveDockingPanes());
    SafeCall(m_paneLayout, DestroyPanes());
    SafeCall(m_wnd, DestroyWindow());

    CView::OnDestroy();
}

LRESULT CEmbedView::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	return m_paneLayout && m_paneLayout->OnDockingPaneNotify(wParam, lParam);
}

BOOL CEmbedView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    if (m_wnd && m_wnd->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
        return TRUE;

    if (m_paneLayout && m_paneLayout->DoCmdMsg(nID, nCode, pExtra, pHandlerInfo))
        return TRUE;

    return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CEmbedView::OnSetFocus(CWnd* pOldWnd)
{
    CView::OnSetFocus(pOldWnd);

    if (m_wnd)
    {
        m_wnd->SetFocus();
    }
}

void CEmbedView::OnKillFocus(CWnd* pNewWnd)
{
    CView::OnKillFocus(pNewWnd);
}

void CEmbedView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    if (cx > 0 && cy > 0)
    {
        CRect rcClient(0, 0, cx, cy);
        RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, 0, 0, &rcClient);
        RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &rcClient, &rcClient);

        if (m_wnd)
        {
            m_wnd->MoveWindow(&rcClient);
        }
    }
}
