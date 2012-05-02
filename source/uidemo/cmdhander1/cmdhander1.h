#ifndef EXAMPLE_CMD_HANDLER_H
#define EXAMPLE_CMD_HANDLER_H

#include <CmdMsgObserver.h>

class CmdHandler : public x3::CmdMsgObserver
{
public:
    CmdHandler();
    virtual ~CmdHandler();

private:
    void OnExampleButton();
    void OnExampleToggle();
    void OnUpdateExampleToggle(bool& enabled, bool& checked, std::wstring& text);
    X3_DECLARE_MSGMAP()

private:
    bool    m_checked;
};

#endif