#include <module/plugininc.h> // or stdafx.h
#include <mfc/FrameAppImpl.h>

class CMyApp : public CFrameApp
{
public:
    CMyApp()
    {
    }

private:
    virtual LPCWSTR GetSingletonAppUID() const
    {
        return L"";
    }
};

CMyApp theApp;
