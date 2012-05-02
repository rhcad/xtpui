#include "stdafx.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
    ON_WM_CREATE()
END_MESSAGE_MAP()

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
		| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MAXIMIZE;

    return CMDIChildWnd::PreCreateWindow(cs);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    CXTPPaintManager* pPaintManager = ((CXTPMDIFrameWnd*)GetParentFrame()
        )->GetCommandBars()->GetPaintManager();

    if (pPaintManager->GetCurrentTheme() > xtpThemeWhidbey)
    {
        EnableOffice2007Frame(this, pPaintManager);
    }

    CMenu* pMenu = GetSystemMenu(FALSE);

	pMenu->RemoveMenu(SC_RESTORE, MF_BYCOMMAND);
	pMenu->RemoveMenu(SC_MINIMIZE, MF_BYCOMMAND);
	pMenu->RemoveMenu(SC_MAXIMIZE, MF_BYCOMMAND);
    pMenu->RemoveMenu(SC_CLOSE, MF_BYCOMMAND);
	
	return 0;
}
