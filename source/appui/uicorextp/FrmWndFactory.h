#pragma once

#include <IFrmWndFactory.h>

class CFrameWndFactory : public IFrameWndFactory
{
    X3BEGIN_CLASS_DECLARE(CFrameWndFactory, clsidFrameWndFactory)
        X3DEFINE_INTERFACE_ENTRY(IFrameWndFactory)
    X3END_CLASS_DECLARE()
protected:
    CFrameWndFactory();
    virtual ~CFrameWndFactory();

private:
    virtual bool CheckAppInstance(const wchar_t* appid);
    virtual bool CreateFrameWnd(const wchar_t* factoryFile);
    virtual void OnQuit();
    virtual bool OnIdle(long count);

private:
    void RegisterDocTemplate(bool mdi, 
        const ConfigSection& frame, const ConfigSection& views);
    bool ProcessShellCommand();

private:
    std::wstring    m_appid;
};
