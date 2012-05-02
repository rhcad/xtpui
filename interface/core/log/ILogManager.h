/*! \file ILogManager.h
 *  \brief Define interface of logging output observer manager: ILogManager
 *  \author Zhang Yungui, X3 C++ PluginFramework
 *  \date   2011.11.08
 */
#ifndef X3_LOG_ILOGMANAGER_H_
#define X3_LOG_ILOGMANAGER_H_

#include "IObject.h"

BEGIN_NAMESPACE_X3

//! Logging type.
enum LogType
{
    logType_Debug,     //!< debug info
    logType_Info,      //!< information
    logType_Warning,   //!< warning info.
    logType_Error,     //!< error info.
    logType_Fatal      //!< fatal error info.
};

class ILogObserver;

const char* const CLSID_LogManager = "bbf48a97-9aef-4368-9dc0-2d2937c326ec";

//! interface of logging output observer manager.
/*! Used by RegisterLogObserver, X3LogGroup and X3LOG_WARNING.
    \interface ILogManager
    \ingroup _GROUP_PLUGIN_LOG_
    \see x3::CLSID_LogManager, IStringTable
    \see x3::RegisterLogObserver, UnRegisterObserver
*/
class ILogManager : public IObject
{
public:
    X3DEFINE_IID(ILogManager);

    //! Add a logging output observer.
    virtual bool RegisterObserver(ILogObserver* observer, HMODULE fromdll) = 0;

    //! Remove a logging output observer.
    virtual void UnRegisterObserver(ILogObserver* observer) = 0;

    //! Beginning a new logging group.
    /*!
        \param msg group message text. 
            if the first char is '@' and leading as 'Module:StrID' format 
            then the manager will translate into localization text using IStringTable.
        \param extra additional context info.
        \param file source file name, __FILE__
        \param line code line, __LINE__
        \return true if successful.
        \see X3LogGroup, PopGroup
    */
    virtual bool PushGroup(const wchar_t* msg, const wchar_t* extra, 
        const char* file, long line) = 0;

    //! Ending a logging group.
    /*!
        \return true if successful.
        \see X3LogGroup, PushGroup
    */
    virtual bool PopGroup() = 0;

    //! Output a logging info.(UNICODE string)
    /*!
        \param type logging type.
        \param msg logging text.
            if the first char is '@' and leading as 'Module:StrID' format 
            then the manager will translate into localization text using IStringTable.
        \param extra additional context info.
        \param file source file name, __FILE__
        \param line code line, __LINE__
        \return true if successful.
        \see X3LOG_WARNING, X3LOG_WARNING2, X3LOG_ERROR, X3LOG_ERROR2
    */
    virtual bool WriteLog(LogType type, const wchar_t* msg, 
        const wchar_t* extra, const char* file, long line) = 0;

    //! Output a logging info.(ANSI string)
    /*!
        \param type logging type.
        \param msg logging text.
            if the first char is '@' and leading as 'Module:StrID' format 
            then the manager will translate into localization text using IStringTable.
        \param extra additional context info.
        \param file source file name, __FILE__
        \param line code line, __LINE__
        \return true if successful.
        \see X3LOG_WARNING, X3LOG_WARNING2, X3LOG_ERROR, X3LOG_ERROR2
    */
    virtual bool WriteLog(LogType type, const char* msg, 
        const char* extra, const char* file, long line) = 0;

    //! Show assert info, used by XCrtDbgReport.
    virtual int CrtDbgReport(const char* msg, const char* file, long line) = 0;
};

END_NAMESPACE_X3
#endif // X3_LOG_ILOGMANAGER_H_
