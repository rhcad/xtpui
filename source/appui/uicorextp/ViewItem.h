#pragma once

//! View creation information
struct ViewItem
{
    CDocTemplate*   pTemplate;  //!< used to create the document and view.
    UINT            frameID;    //!< resource id of the child frame window.
    std::wstring    layout;     //!< the folder name in which contains PaneFactory.xml
    std::wstring    caption;    //!< the panel's caption.
    std::wstring    clsid;      //!< the embed window (IWndFactory)'s class id.
};

ViewItem* FindViewItem(CDocTemplate* pTemplate);
