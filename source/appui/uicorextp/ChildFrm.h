#pragma once

class CChildFrame : public CMDIChildWnd, CXTPOffice2007FrameHook
{
protected:
    CChildFrame();
    DECLARE_DYNCREATE(CChildFrame)

// Overrides
public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
    virtual ~CChildFrame();

// Generated message map functions
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    DECLARE_MESSAGE_MAP()
};
