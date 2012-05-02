#ifndef X3_UICORE_CMDMSG_EVENT_H
#define X3_UICORE_CMDMSG_EVENT_H

#include <observer/fireevent.h>
#include <string>

BEGIN_NAMESPACE_X3

struct UpdateCmdInfo
{
    bool            enabled;
    bool            checked;
    std::wstring    text;
};

//! Event type for dispatching a command message.
//  bool OnCommand(int id, bool test)
X3DEFINE_EVENT_2Break(EventCommand, int, bool, "uicore.x3");

//! Event type for dispatching the update notify of a command UI object.
//  bool OnUpdateUI(int id, x3::UpdateCmdInfo& info)
X3DEFINE_EVENT_2Break(EventUpdateCmd, int, UpdateCmdInfo&, "uicore.x3");

END_NAMESPACE_X3
#endif
