#pragma once

class CEmbedWnd;

//! 面板管理器类，每个面板内置一个 IWndFactory 子窗口
/*!
    \ingroup _GROUP_APPUI_
*/
class CPaneManager
{
public:
    //! 构造函数
    /*!
        \param paneOwner 面板窗口的拥有者
        \param manager 停靠面板管理对象
        \param folderName 面板布局文件保存位置，子文件夹名
    */
    CPaneManager(CWnd* paneOwner, CXTPDockingPaneManager* manager, 
        const std::wstring& folderName = L"");

    //! 析构函数
    virtual ~CPaneManager();

public:
    //! 根据视图类ID查找面板子窗口对象, CEmbedWnd
    CWnd* FindPaneWnd(const char* clsidView, UINT* pID = NULL) const;

    //! 显示一个面板
    /*!
        \param id 面板窗口的ID
        \param activate 是否将该面板设置为当前活动面板
        \return 是否执行成功
    */
    bool ShowPane(UINT id, bool activate);

    //! 隐藏一个面板
    bool HidePane(UINT id);

    //! 切换到指定标识名称的面板布局中
    bool SwitchPaneLayout(const std::wstring& name);

    //! 返回面板窗口个数
    long GetViewCount() const;

    //! 返回指定序号的面板子窗口对象, CEmbedWnd
    CWnd* GetView(long index) const;

    //! 返回面板子窗口中的内嵌 IWndFactory 对象
    AnyObject GetViewObject(CWnd* pWnd) const;

public:
    //! 创建面板窗口
    bool CreateDockingPanes();

    //! 响应面板通知消息
    bool OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);

    //! 保存面板布局位置
    void SaveDockingPanes();

    //! 销毁所有面板窗口
    void DestroyPanes();

    //! 传递命令消息给面板子窗口
    BOOL DoCmdMsg(UINT id, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pInfo);

private:
    void CreatePanes(const std::wstring& filename);
    void ResetPaneOptions(const std::wstring& filename);
    void LoadPaneLayout(const std::wstring& filename);
    std::wstring MakeFileName(const std::wstring& shortName) const;
    bool IsSameTimeStamp(const std::wstring& userFile, const std::wstring& initialFile);
    bool SaveTimeStamp(const std::wstring& userFile, const std::wstring& initialFile);

private:
    std::map<UINT, CEmbedWnd*>  m_mapView;
    bool                    m_designMode;
    std::wstring            m_folderName;
    CWnd*                   m_paneOwner;
    CXTPDockingPaneManager* m_manager;
};
