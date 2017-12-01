// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "afxcmn.h"
#include "afxwin.h"
#include "..\..\download\Download.h"

class CMFCApplication1Dlg : public CDialog
{
public:
    CMFCApplication1Dlg(CWnd* pParent = NULL);
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
protected:
    HICON m_hIcon;
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
public:
    CEdit m_UrlEdit;
    CEdit m_PathEdit;
    CProgressCtrl m_ProgressCtrl;
    CButton m_DownloadButton;
public:
    CDownload m_Download;
};
