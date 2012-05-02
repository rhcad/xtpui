#include "stdafx.h"
#include <module/pluginimpl.h>
#include <module/modulemacro.h>
#include <observer/observerimpl.h>

#include "FrmWndFactory.h"
#include "UIOptions.h"

XBEGIN_DEFINE_MODULE()
    XDEFINE_CLASSMAP_ENTRY_Singleton(CFrameWndFactory)
    XDEFINE_CLASSMAP_ENTRY_Singleton(CUIOptions)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
    return true;
}

OUTAPI void x3UninitializePlugin()
{
}

std::wstring GetFileModifyTime(const std::wstring& filename)
{
    wchar_t filetime[20] = { 0 };
    HANDLE hfile = NULL;

    hfile = ::CreateFileW(filename.c_str(),
        GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

    if (hfile != INVALID_HANDLE_VALUE)
    {
        FILETIME ftCreate, ftAccess, ftWrite;
        SYSTEMTIME stUTC, stLocal;

        if (::GetFileTime(hfile, &ftCreate, &ftAccess, &ftWrite))
        {
            FileTimeToSystemTime(&ftWrite, &stUTC);
            SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

            swprintf_s(filetime, _countof(filetime), L"%04d-%02d-%02d %02d:%02d:%02d",
                stLocal.wYear, stLocal.wMonth, stLocal.wDay,
                stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
        }

        ::CloseHandle(hfile);
    }

    return filetime;
}
