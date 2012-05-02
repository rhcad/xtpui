#ifndef X3_UICORE_MFC_FIRE_CMDMSG_H
#define X3_UICORE_MFC_FIRE_CMDMSG_H

#include "CmdMsgEvent.h"

BEGIN_NAMESPACE_X3

static inline bool FireCmdMsgEvent(int id, int code, void* extra, void* handler)
{
    bool ret = false;

    if (CN_COMMAND == code)
    {
        ret = FireEventCommand(id, handler != NULL).fireEvent().handled();
    }
    else if (CN_UPDATE_COMMAND_UI == code)
    {
        UpdateCmdInfo info;
        info.enabled = true;
        info.checked = false;
        info.text = L"~@#$";

        ret = FireEventUpdateCmd(id, info).fireEvent().handled();

        if (ret)
        {
            CCmdUI* pCmdUI = (CCmdUI*)extra;
            pCmdUI->Enable(info.enabled);
            pCmdUI->SetCheck(info.checked ? 1 : 0);
            if (info.text != L"~@#$")
            {
                pCmdUI->SetText(info.text.c_str());
            }
        }
    }

    return ret;
}

END_NAMESPACE_X3
#endif
