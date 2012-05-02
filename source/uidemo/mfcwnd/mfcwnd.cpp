#include <module/plugininc.h>
#include "mfcwnd.h"

#define ID_EXAMPLE_POPUPBTN     135
#define ID_EXAMPLE_SPLITBTN     136
#define ID_EXAMPLE_POPUP1       32771
#define ID_EXAMPLE_POPUP2       32772
#define ID_EXAMPLE_POPUP3       32773
#define ID_EXAMPLE_CHECKBOX1    142
#define ID_EXAMPLE_CHECKBOX2    143
#define ID_EXAMPLE_CHECKBOX3    144
#define ID_EXAMPLE_RADIOBTN1    145
#define ID_EXAMPLE_RADIOBTN2    146
#define ID_EXAMPLE_RADIOBTN3    147

CSimpleWnd::CSimpleWnd()
{
    m_checked134 = false;
    m_popupid = ID_EXAMPLE_POPUP1;
    ZeroMemory(m_checkbox, sizeof(m_checkbox));
}

CSimpleWnd::~CSimpleWnd()
{
}

BEGIN_MESSAGE_MAP(CSimpleWnd, CWnd)
	ON_WM_PAINT()

    ON_UPDATE_COMMAND_UI(ID_EXAMPLE_POPUPBTN, OnUpdateEnableItems)
    ON_COMMAND(ID_EXAMPLE_SPLITBTN, OnExampleSplitButton)
    ON_UPDATE_COMMAND_UI_RANGE(ID_EXAMPLE_POPUP1, ID_EXAMPLE_POPUP3, OnUpdatePopupItems)
    ON_COMMAND_RANGE(ID_EXAMPLE_POPUP1, ID_EXAMPLE_POPUP3, OnPopupItems)

    ON_COMMAND(138, OnExampleButton)
    ON_COMMAND(140, OnExampleSplitButton)
    ON_UPDATE_COMMAND_UI(139, OnUpdateEnableItems)

    ON_UPDATE_COMMAND_UI_RANGE(ID_EXAMPLE_CHECKBOX1, ID_EXAMPLE_CHECKBOX3, OnUpdateExampleCheckBox)
    ON_COMMAND_RANGE(ID_EXAMPLE_CHECKBOX1, ID_EXAMPLE_CHECKBOX3, OnExampleCheckBox)
    ON_UPDATE_COMMAND_UI_RANGE(ID_EXAMPLE_RADIOBTN1, ID_EXAMPLE_RADIOBTN3, OnUpdateExampleRadio)
    ON_COMMAND_RANGE(ID_EXAMPLE_RADIOBTN1, ID_EXAMPLE_RADIOBTN3, OnExampleRadio)
END_MESSAGE_MAP()

BOOL CSimpleWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

    cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

BOOL CSimpleWnd::CreateWnd(CWnd* pParentWnd, UINT id)
{
    return Create(NULL, L"SimpleWnd", WS_CHILD|WS_VISIBLE,
        CRect(0,0,0,0), pParentWnd, id);
}

void CSimpleWnd::OnPaint() 
{
	CPaintDC dc(this);
    CRect rect;

    GetClientRect(rect);

    dc.SetBkMode(TRANSPARENT);
    dc.DrawText(L"MFC Window in the mfcwnd plugin.", 
        rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CSimpleWnd::OnExampleButton()
{
    AfxMessageBox(L"CSimpleWnd::OnExampleButton() in the mfcwnd plugin.");
}

void CSimpleWnd::OnUpdateExampleToggle(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_checked134 ? 1 : 0);
}

void CSimpleWnd::OnExampleToggle()
{
    m_checked134 = !m_checked134;
}

void CSimpleWnd::OnUpdateEnableItems(CCmdUI* pCmdUI)
{
    pCmdUI->Enable();
}

void CSimpleWnd::OnUpdatePopupItems(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(m_popupid == pCmdUI->m_nID);
}

void CSimpleWnd::OnPopupItems(UINT id)
{
    m_popupid = id;

    CString str;
    str.Format(L"OnPopupItems(%d) in the mfcwnd plugin.", id);
    AfxMessageBox(str);
}

void CSimpleWnd::OnExampleSplitButton()
{
    OnPopupItems(m_popupid);
}

void CSimpleWnd::OnUpdateExampleCheckBox(CCmdUI* pCmdUI)
{
    UINT index = pCmdUI->m_nID - ID_EXAMPLE_CHECKBOX1;

    if (index >= 0 && index < 3)
    {
        pCmdUI->SetCheck(m_checkbox[index] ? 1 : 0);
    }
}

void CSimpleWnd::OnExampleCheckBox(UINT id)
{
    UINT index = id - ID_EXAMPLE_CHECKBOX1;

    if (index >= 0 && index < 3)
    {
        m_checkbox[index] = !m_checkbox[index];
    }
}

void CSimpleWnd::OnUpdateExampleRadio(CCmdUI* pCmdUI)
{
    UINT index = pCmdUI->m_nID - ID_EXAMPLE_RADIOBTN1;
    pCmdUI->SetRadio(index + ID_EXAMPLE_POPUP1 == m_popupid);
}

void CSimpleWnd::OnExampleRadio(UINT id)
{
    UINT index = id - ID_EXAMPLE_RADIOBTN1;
    if (index >= 0 && index < 3)
    {
        OnPopupItems(index + ID_EXAMPLE_POPUP1);
    }
}
