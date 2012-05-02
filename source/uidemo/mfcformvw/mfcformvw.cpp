#include "stdafx.h"
#include "mfcformvw.h"

#define ID_EXAMPLE_BUTTON       133

CMyFormView::CMyFormView() : CFormView(CMyFormView::IDD)
{
}

CMyFormView::~CMyFormView()
{
}

BOOL CMyFormView::CreateWnd(CWnd* pParentWnd, UINT id)
{
    CLocalResource localRes;
    return Create(NULL, NULL, WS_CHILD|WS_VISIBLE,
        CRect(0,0,0,0), pParentWnd, id, NULL);
}

void CMyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyFormView, CFormView)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyFormView::OnBnClickedButton1)
    ON_COMMAND(ID_EXAMPLE_BUTTON, OnExampleButton)
END_MESSAGE_MAP()

void CMyFormView::OnBnClickedButton1()
{
    AfxMessageBox(L"CMyFormView::OnBnClickedButton1()");
}

void CMyFormView::OnExampleButton()
{
    AfxMessageBox(L"CMyFormView::OnExampleButton() in the mfcformvw plugin.");
}

//--------------------------

CMyDlgView::CMyDlgView()
{
}

CMyDlgView::~CMyDlgView()
{
}

BOOL CMyDlgView::CreateWnd(CWnd* pParentWnd, UINT /*id*/)
{
    CLocalResource localRes;
    return Create(IDD, pParentWnd) && ShowWindow(SW_SHOW);
}

void CMyDlgView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyDlgView, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, &CMyDlgView::OnBnClickedButton1)
    ON_COMMAND(ID_EXAMPLE_BUTTON, OnExampleButton)
END_MESSAGE_MAP()

BOOL CMyDlgView::DestroyWindow()
{
    EndDialog(0);
    return CDialog::DestroyWindow();
}

void CMyDlgView::OnOK()
{
}

void CMyDlgView::OnCancel()
{
}

void CMyDlgView::OnBnClickedButton1()
{
    AfxMessageBox(L"CMyDlgView::OnBnClickedButton1()");
}

void CMyDlgView::OnExampleButton()
{
    AfxMessageBox(L"CMyDlgView::OnExampleButton() in the mfcformvw plugin.");
}
