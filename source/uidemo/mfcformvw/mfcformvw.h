#pragma once

#include "resource.h"

class CMyFormView : public CFormView
{
public:
	CMyFormView();
	virtual ~CMyFormView();

    BOOL CreateWnd(CWnd* pParentWnd, UINT id);

public:
	enum { IDD = IDD_FORMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
    afx_msg void OnBnClickedButton1();
    afx_msg void OnExampleButton();
	DECLARE_MESSAGE_MAP()
};

class CMyDlgView : public CDialog
{
public:
	CMyDlgView();
	virtual ~CMyDlgView();

    BOOL CreateWnd(CWnd* pParentWnd, UINT id);

public:
	enum { IDD = IDD_FORMVIEW };
    virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
    virtual void OnOK();
	virtual void OnCancel();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnExampleButton();
	DECLARE_MESSAGE_MAP()
};
