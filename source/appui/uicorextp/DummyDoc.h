#pragma once

class CDummyDoc : public CDocument
{
protected: // create from serialization only
    CDummyDoc();
    DECLARE_DYNCREATE(CDummyDoc)

// Overrides
public:
    virtual BOOL OnNewDocument();
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
    virtual BOOL DoFileSave();
    virtual BOOL CanCloseFrame(CFrameWnd* pFrame);

// Implementation
public:
    virtual ~CDummyDoc();

// Generated message map functions
protected:
    afx_msg void OnFileClose();
    DECLARE_MESSAGE_MAP()
};
