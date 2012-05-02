#include <module/plugininc.h>
#include "cmdhander1.h"

#define ID_EXAMPLE_BUTTON       133
#define ID_EXAMPLE_TOGGLE       134

X3_BEGIN_MSGMAP(CmdHandler)
    X3_ON_COMMAND(ID_EXAMPLE_BUTTON, OnExampleButton)
    X3_ON_COMMAND(ID_EXAMPLE_TOGGLE, OnExampleToggle)
    X3_ON_UPDATE_CMDUI(ID_EXAMPLE_TOGGLE, OnUpdateExampleToggle)
X3_END_MSGMAP()

CmdHandler::CmdHandler()
{
    m_checked = false;
}

CmdHandler::~CmdHandler()
{
}

void CmdHandler::OnExampleButton()
{
    AfxMessageBox(L"CmdHandler::OnExampleButton() in the cmdhander1 plugin.");
}

void CmdHandler::OnExampleToggle()
{
    m_checked = !m_checked;
}

void CmdHandler::OnUpdateExampleToggle(bool&, bool& checked, std::wstring&)
{
    checked = m_checked;
}
