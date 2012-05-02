#ifndef EXAMPLE_MFCWND_IMPL_H
#define EXAMPLE_MFCWND_IMPL_H

class CSimpleWnd : public CWnd
{
public:
	CSimpleWnd();
    virtual ~CSimpleWnd();

    BOOL CreateWnd(CWnd* pParentWnd, UINT id);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	afx_msg void OnPaint();
    afx_msg void OnExampleButton();
    afx_msg void OnUpdateExampleToggle(CCmdUI* pCmdUI);
    afx_msg void OnExampleToggle();
    afx_msg void OnUpdateEnableItems(CCmdUI* pCmdUI);
    afx_msg void OnUpdatePopupItems(CCmdUI* pCmdUI);
    afx_msg void OnPopupItems(UINT id);
    afx_msg void OnExampleSplitButton();
    afx_msg void OnUpdateExampleCheckBox(CCmdUI* pCmdUI);
    afx_msg void OnExampleCheckBox(UINT id);
    afx_msg void OnUpdateExampleRadio(CCmdUI* pCmdUI);
    afx_msg void OnExampleRadio(UINT id);
	DECLARE_MESSAGE_MAP()

private:
    bool    m_checked134;
    UINT    m_popupid;
    bool    m_checkbox[3];
};

#endif