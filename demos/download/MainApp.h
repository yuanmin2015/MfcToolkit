// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"

class CMFCApplication1App : public CWinApp
{
public:
    CMFCApplication1App();
public:
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

extern CMFCApplication1App theApp;
