﻿// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include <functional>
#include <string>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

namespace util
{
    class CDownloadCallback : public IBindStatusCallback
    {
    public:
        CDownloadCallback() : m_LastPercent(0)
        {
        }
        virtual ~CDownloadCallback()
        {
        }
    public:
        int m_LastPercent;
        std::function<void(int, std::wstring)> fStatusCallback;
    public:
        HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved, __RPC__in_opt IBinding *pib)
        {
            return E_NOTIMPL;
        }
        HRESULT STDMETHODCALLTYPE GetPriority(__RPC__out LONG *pnPriority)
        {
            return E_NOTIMPL;
        }
        HRESULT STDMETHODCALLTYPE OnLowResource(DWORD reserved)
        {
            return E_NOTIMPL;
        }
        HRESULT STDMETHODCALLTYPE OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, __RPC__in_opt LPCWSTR szStatusText)
        {
            switch (ulStatusCode)
            {
            case BINDSTATUS_FINDINGRESOURCE:
                if (fStatusCallback != nullptr)
                {
                    fStatusCallback(-1, _T("Finding resource..."));
                }
                break;
            case BINDSTATUS_CONNECTING:
                if (fStatusCallback != nullptr)
                {
                    fStatusCallback(-1, _T("Connecting..."));
                }
                break;
            case BINDSTATUS_SENDINGREQUEST:
                if (fStatusCallback != nullptr)
                {
                    fStatusCallback(-1, _T("Sending request..."));
                }
                break;
            case BINDSTATUS_MIMETYPEAVAILABLE:
                if (fStatusCallback != nullptr)
                {
                    fStatusCallback(-1, _T("Mime type available"));
                }
                break;
            case BINDSTATUS_CACHEFILENAMEAVAILABLE:
                if (fStatusCallback != nullptr)
                {
                    fStatusCallback(-1, _T("Cache filename available"));
                }
                break;
            case BINDSTATUS_BEGINDOWNLOADDATA:
                if (fStatusCallback != nullptr)
                {
                    fStatusCallback(-1, _T("Begin download"));
                }
                break;
            case BINDSTATUS_DOWNLOADINGDATA:
            case BINDSTATUS_ENDDOWNLOADDATA:
            {
                int percent = (int)(100.0 * static_cast<double>(ulProgress) / static_cast<double>(ulProgressMax));
                if (m_LastPercent < percent)
                {
                    m_LastPercent = percent;
                    if (fStatusCallback != nullptr)
                    {
                        std::wstring szOutput = std::to_wstring(percent) + _T("%");
                        fStatusCallback(percent, szOutput);
                    }
                }
                if (ulStatusCode == BINDSTATUS_ENDDOWNLOADDATA)
                {
                    if (fStatusCallback != nullptr)
                    {
                        fStatusCallback(100, _T("End download"));
                    }
                }
            }
            break;
            default:
            {
                if (fStatusCallback != nullptr)
                {
                    std::wstring szOutput = _T("Status code : ") + std::to_wstring(ulStatusCode);
                    fStatusCallback(-1, szOutput);
                }
            }
            break;
            }
            return S_OK;
        }
        HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, __RPC__in_opt LPCWSTR szError)
        {
            return E_NOTIMPL;
        }
        HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD *grfBINDF, BINDINFO *pbindinfo)
        {
            return E_NOTIMPL;
        }
        HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed)
        {
            return E_NOTIMPL;
        }
        HRESULT STDMETHODCALLTYPE OnObjectAvailable(__RPC__in REFIID riid, __RPC__in_opt IUnknown *punk)
        {
            return E_NOTIMPL;
        }
        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
        {
            if (IsEqualIID(IID_IBindStatusCallback, riid) || IsEqualIID(IID_IUnknown, riid))
            {
                *ppvObject = reinterpret_cast<void*>(this);
                return S_OK;
            }
            return E_NOINTERFACE;
        }
        ULONG STDMETHODCALLTYPE AddRef(void)
        {
            return 2UL;
        }
        ULONG STDMETHODCALLTYPE Release(void)
        {
            return 1UL;
        }
    };

    class CDownload
    {
    public:
        CDownload()
        {
        }
        virtual ~CDownload()
        {
        }
    public:
        bool Download(const std::wstring& szUrl, const std::wstring& szPath, std::function<void(int, std::wstring)> fStatusCallback)
        {
            CDownloadCallback m_Callback;
            m_Callback.fStatusCallback = fStatusCallback;

            IBindStatusCallback* pBindStatusCallback = nullptr;
            m_Callback.QueryInterface(IID_IBindStatusCallback, reinterpret_cast<void**>(&pBindStatusCallback));

            HRESULT m_Result = URLDownloadToFile(nullptr, CT2W(szUrl), CT2W(szPath), 0, pBindStatusCallback);
            if (SUCCEEDED(m_Result))
            {
                return true;
            }
            return false;
        }
    };
}
