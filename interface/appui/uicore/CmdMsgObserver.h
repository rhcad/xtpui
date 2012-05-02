/*! \file CmdMsgObserver.h
 *  \brief Define window's command message observer: RawCmdMsgObserver
 *  \author Zhang Yungui, X3 C++ PluginFramework
 *  \date   2012.3.27
 */
#ifndef X3_UICORE_CMDMSG_OBSERVER_H
#define X3_UICORE_CMDMSG_OBSERVER_H

#include "CmdMsgEvent.h"

BEGIN_NAMESPACE_X3

struct MENU_MSG_MAP;

//! Window's command message observer.
/*! Add X3_DECLARE_MSGMAP() in the derived class (*.h).\n
    Add the command handler functions in implement files (*.cpp) as following:
    \code
    X3_BEGIN_MSGMAP(Class)
        X3_ON_COMMAND(id, memberFxn)
        X3_ON_COMMAND_RANGE(idMin, idMax, memberFxn)
        X3_ON_UPDATE_CMDUI(id, memberFxn)
        X3_ON_UPDATE_CMDUI_RANGE(idMin, idMax, memberFxn)
    X3_END_MSGMAP()
    \endcode
    \ingroup _GROUP_APPUI_
    \see MenuMsgMapFunctions
*/
class CmdMsgObserver
{
protected:
    CmdMsgObserver()
    {
    }

    virtual ~CmdMsgObserver()
    {
    }

    bool OnRawCommand(int id, bool test);
    bool OnRawUpdateCmdUI(int id, bool& enabled, bool& checked, std::wstring& text);

private:
    virtual const MENU_MSG_MAP* GetMsgMapEntry() const = 0;
};

typedef void (CmdMsgObserver::*PFN_MSG)(void);

//! Command handler function typedefs.
/*! Add X3_DECLARE_MSGMAP() in the derived class of CmdMsgObserver (*.h).\n
    Add the command handler functions in implement files (*.cpp) as following:
    \code
    X3_BEGIN_MSGMAP(Class)
        X3_ON_COMMAND(id, memberFxn)
        X3_ON_COMMAND_RANGE(idMin, idMax, memberFxn)
        X3_ON_UPDATE_CMDUI(id, memberFxn)
        X3_ON_UPDATE_CMDUI_RANGE(idMin, idMax, memberFxn)
    X3_END_MSGMAP()
    \endcode
    \see CmdMsgObserver
*/
union MenuMsgMapFunctions
{
    PFN_MSG pfn;

    //! Regular command handler function, correspond to X3_ON_COMMAND.
    void (CmdMsgObserver::*pfn_COMMAND)();

    //! Command handler function of contiguous range of command IDs, correspond to X3_ON_COMMAND_RANGE.
    void (CmdMsgObserver::*pfn_COMMAND_RANGE)(int id);

    //! Regular update message handler function, correspond to X3_ON_UPDATE_CMDUI.
    void (CmdMsgObserver::*pfn_UPDATE_CMDUI)(
        bool& enabled, bool& checked, std::wstring& text);

    //! Update message handler function of contiguous range of command IDs, correspond to X3_ON_UPDATE_CMDUI_RANGE.
    void (CmdMsgObserver::*pfn_UPDATE_CMDUI_RANGE)(
        int id, bool& enabled, bool& checked, std::wstring& text);
};

//! Command handler function types.
enum MenuMsgType
{
    kMenuMsgNull,
    kMenuMsgCommand,
    kMenuMsgCommandRange,
    kMenuMsgUpdateUI,
    kMenuMsgUpdateUIRange,
};

//! Entry item of command handler functions.
struct MENU_MSG_MAP
{
    int         id;
    int         lastid;
    MenuMsgType type;
    PFN_MSG     pfn;
};

//! Define command handler function entries in H file.
/*! It's used in the derived class of CmdMsgObserver (*.h).
    \see MenuMsgMapFunctions, X3_BEGIN_MSGMAP
*/
#define X3_DECLARE_MSGMAP()  \
private:    \
    const x3::MENU_MSG_MAP* GetMsgMapEntry() const; \
    static const x3::MENU_MSG_MAP messageMap[];     \
    static bool _OnCommand(int id, bool test);      \
    static bool _OnUpdateUI(int id, x3::UpdateCmdInfo& info);   \
public:     \
    static bool Register();

//! Implement command handler function entries in implement files (*.cpp).
/*!
    \see MenuMsgMapFunctions, X3_END_MSGMAP, X3_DECLARE_MSGMAP
*/
#define X3_BEGIN_MSGMAP(theClass)       \
    static theClass _the ## theClass;   \
    bool theClass::_OnCommand(int id, bool test) {  \
        return _the ## theClass.OnRawCommand(id, test); \
    }   \
    bool theClass::_OnUpdateUI(int id, x3::UpdateCmdInfo& info) {  \
        return _the ## theClass.OnRawUpdateCmdUI(   \
            id, info.enabled, info.checked, info.text);    \
    }   \
    bool theClass::Register() {         \
        return X3_REGISTER_OBSERVER(x3::EventCommand, _OnCommand)   \
            && X3_REGISTER_OBSERVER(x3::EventUpdateCmd, _OnUpdateUI); \
    }   \
    const x3::MENU_MSG_MAP* theClass::GetMsgMapEntry() const    \
        { return messageMap; }  \
    const x3::MENU_MSG_MAP theClass::messageMap[] = {

//! This macro maps a command message to a member function.
/*!
    \see X3_BEGIN_MSGMAP, MenuMsgMapFunctions
*/
#define X3_ON_COMMAND(id, memberFxn)  \
        { id, id, x3::kMenuMsgCommand, (x3::PFN_MSG)&memberFxn },

//! Use this macro to map a contiguous range of command IDs to a single message handler function.
/*!
    \see X3_BEGIN_MSGMAP, MenuMsgMapFunctions
*/
#define X3_ON_COMMAND_RANGE(idMin, idMax, memberFxn)  \
        { idMin, idMax, x3::kMenuMsgCommandRange, \
        (x3::PFN_MSG)(void (x3::CmdMsgObserver::*)(int))&memberFxn },

//! This macro indicates which function will handle a user-interface update command message.
/*!
    \see X3_BEGIN_MSGMAP, MenuMsgMapFunctions
*/
#define X3_ON_UPDATE_CMDUI(id, memberFxn) \
        { id, id, x3::kMenuMsgUpdateUI, \
        (x3::PFN_MSG)(void (x3::CmdMsgObserver::*)(bool&,bool&,std::wstring&))&memberFxn },

//! Maps a contiguous range of command IDs to a single update message handler function.
/*!
    \see X3_BEGIN_MSGMAP, MenuMsgMapFunctions
*/
#define X3_ON_UPDATE_CMDUI_RANGE(idMin, idMax, memberFxn) \
        { idMin, idMax, x3::kMenuMsgUpdateUIRange, \
        (x3::PFN_MSG)(void (x3::CmdMsgObserver::*)(int,bool&,bool&,std::wstring&))&memberFxn },

//! End item correspond to X3_BEGIN_MSGMAP.
/*!
    \see X3_BEGIN_MSGMAP, MenuMsgMapFunctions
*/
#define X3_END_MSGMAP()  \
        { 0, 0, x3::kMenuMsgNull, (x3::PFN_MSG)0 }  \
    };

inline bool CmdMsgObserver::OnRawCommand(int id, bool test)
{
    const MENU_MSG_MAP* pMsgMap = GetMsgMapEntry();

    for (int i = 0; pMsgMap[i].id != 0; i++)
    {
        union MenuMsgMapFunctions mmf;
        mmf.pfn = pMsgMap[i].pfn;

        if (pMsgMap[i].type == kMenuMsgCommand
            && id == pMsgMap[i].id)
        {
            if (!test)
                (this->*mmf.pfn_COMMAND)();
            return true;
        }
        else if (pMsgMap[i].type == kMenuMsgCommandRange
            && id >= pMsgMap[i].id && id <= pMsgMap[i].lastid)
        {
            if (!test)
                (this->*mmf.pfn_COMMAND_RANGE)(id);
            return true;
        }
    }

    return false;
}

inline bool CmdMsgObserver::OnRawUpdateCmdUI(
    int id, bool& enabled, bool& checked, std::wstring& text)
{
    int i = 0;
    const MENU_MSG_MAP* pMsgMap = GetMsgMapEntry();

    for (; pMsgMap[i].id != 0; i++)
    {
        union MenuMsgMapFunctions mmf;
        mmf.pfn = pMsgMap[i].pfn;

        if (pMsgMap[i].type == kMenuMsgUpdateUI
            && id == pMsgMap[i].id)
        {
            (this->*mmf.pfn_UPDATE_CMDUI)(enabled, checked, text);
            break;
        }
        else if (pMsgMap[i].type == kMenuMsgUpdateUIRange
            && id >= pMsgMap[i].id && id <= pMsgMap[i].lastid)
        {
            (this->*mmf.pfn_UPDATE_CMDUI_RANGE)(id, enabled, checked, text);
            break;
        }
    }

    if (pMsgMap[i].id == 0)
    {
        for (i = 0; pMsgMap[i].id != 0; i++)
        {
            if (id >= pMsgMap[i].id && id <= pMsgMap[i].lastid)
                break;
        }
    }

    return pMsgMap[i].id != 0;
}

END_NAMESPACE_X3
#endif
