/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QT_WINDOWS_H
#define QT_WINDOWS_H

#if 0
#pragma qt_sync_skip_header_check
#pragma qt_sync_stop_processing
#endif

#if defined(Q_CC_MINGW)
// mingw's windows.h does not set _WIN32_WINNT, resulting breaking compilation
#  ifndef WINVER
#    define WINVER 0x0501
#  endif
#  ifndef _WIN32_WINNT
#    define _WIN32_WINNT 0x0501
#  endif
#  ifndef NTDDI_VERSION
#    define NTDDI_VERSION 0x05010300
#  endif
#endif

#ifndef NOMINMAX
#  define NOMINMAX
#endif
#include <winsock2.h>
#include <windows.h>

#if defined(_WIN32_IE) && _WIN32_IE < 0x0501
#  undef _WIN32_IE
#endif
#if !defined(_WIN32_IE)
#  define _WIN32_IE 0x0501
#endif

// already defined when compiled with WINVER >= 0x0500
#ifndef SPI_SETMENUANIMATION
#define SPI_SETMENUANIMATION 0x1003
#endif
#ifndef SPI_SETMENUFADE
#define SPI_SETMENUFADE 0x1013
#endif
#ifndef SPI_SETCOMBOBOXANIMATION
#define SPI_SETCOMBOBOXANIMATION 0x1005
#endif
#ifndef SPI_SETTOOLTIPANIMATION
#define SPI_SETTOOLTIPANIMATION 0x1017
#endif
#ifndef SPI_SETTOOLTIPFADE
#define SPI_SETTOOLTIPFADE 0x1019
#endif
#ifndef SPI_SETUIEFFECTS
#define SPI_SETUIEFFECTS 0x103F
#endif
#ifndef SPI_GETMENUANIMATION
#define SPI_GETMENUANIMATION 0x1002
#endif
#ifndef SPI_GETMENUFADE
#define SPI_GETMENUFADE 0x1012
#endif
#ifndef SPI_GETCOMBOBOXANIMATION
#define SPI_GETCOMBOBOXANIMATION 0x1004
#endif
#ifndef SPI_GETTOOLTIPANIMATION
#define SPI_GETTOOLTIPANIMATION 0x1016
#endif
#ifndef SPI_GETTOOLTIPFADE
#define SPI_GETTOOLTIPFADE 0x1018
#endif
#ifndef SPI_GETUIEFFECTS
#define SPI_GETUIEFFECTS 0x103E
#endif
#ifndef SPI_GETKEYBOARDCUES
#define SPI_GETKEYBOARDCUES 0x100A
#endif
#ifndef SPI_GETGRADIENTCAPTIONS
#define SPI_GETGRADIENTCAPTIONS 0x1008
#endif
#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)
#endif
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL 0x020A
#endif
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef ETO_PDY
#define ETO_PDY 0x2000
#endif
#ifndef COLOR_GRADIENTACTIVECAPTION
#define COLOR_GRADIENTACTIVECAPTION 27
#endif
#ifndef COLOR_GRADIENTINACTIVECAPTION
#define COLOR_GRADIENTINACTIVECAPTION 28
#endif

// already defined when compiled with WINVER >= 0x0600
#ifndef SPI_GETFLATMENU
#define SPI_GETFLATMENU 0x1022
#endif
#ifndef CS_DROPSHADOW
#define CS_DROPSHADOW 0x00020000
#endif
#ifndef CLEARTYPE_QUALITY
#define CLEARTYPE_QUALITY 5
#endif

#include <map>

#include <dwmapi.h>
#include <iphlpapi.h>
#include <namedpipeapi.h>
#include <netioapi.h>
#include <ntstatus.h>
#include <knownfolders.h>
#include <shellapi.h>
#include <shlobj.h>
#include <versionhelpers.h>

inline namespace WinXPType {
    struct CHANGEFILTERSTRUCT {
        DWORD cbSize;
        DWORD ExtStatus;
    };
    using PCHANGEFILTERSTRUCT = CHANGEFILTERSTRUCT *;

    enum FDAP { };

    enum FDE_OVERWRITE_RESPONSE { };

    enum FDE_SHAREVIOLATION_RESPONSE { };

    using FILEOPENDIALOGOPTIONS = DWORD;

    enum FILE_INFO_BY_HANDLE_CLASS {};

    using HPOWERNOTIFY = HANDLE;
    using PHPOWERNOTIFY = HPOWERNOTIFY *;

    using HTOUCHINPUT = HANDLE;

    struct IFileDialogEvents;
    struct IFileDialog : public IModalWindow
    {
        virtual HRESULT STDMETHODCALLTYPE SetFileTypes(
            UINT cFileTypes,
            const COMDLG_FILTERSPEC *rgFilterSpec) = 0;
        virtual HRESULT STDMETHODCALLTYPE SetFileTypeIndex(
            UINT iFileType) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetFileTypeIndex(
            UINT *piFileType) = 0;
        virtual HRESULT STDMETHODCALLTYPE Advise(
            IFileDialogEvents *pfde,
            DWORD *pdwCookie) = 0;
        virtual HRESULT STDMETHODCALLTYPE Unadvise(
            DWORD dwCookie) = 0;
        virtual HRESULT STDMETHODCALLTYPE SetOptions(
            FILEOPENDIALOGOPTIONS fos) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetOptions(
            FILEOPENDIALOGOPTIONS *pfos) = 0;
        virtual HRESULT STDMETHODCALLTYPE SetDefaultFolder(
            IShellItem *psi) = 0;
        virtual HRESULT STDMETHODCALLTYPE SetFolder(
            IShellItem *psi) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetFolder(
            IShellItem **ppsi) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetCurrentSelection(
            IShellItem **ppsi) = 0;
        virtual HRESULT STDMETHODCALLTYPE SetFileName(
            LPCWSTR pszName) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetFileName(
            LPWSTR *pszName) = 0;
        virtual HRESULT STDMETHODCALLTYPE SetTitle(
            LPCWSTR pszTitle) = 0;
        virtual HRESULT STDMETHODCALLTYPE SetOkButtonLabel(
            LPCWSTR pszText) = 0;
        virtual HRESULT STDMETHODCALLTYPE SetFileNameLabel(
            LPCWSTR pszLabel) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetResult(
            IShellItem **ppsi) = 0;
        virtual HRESULT STDMETHODCALLTYPE AddPlace(
            IShellItem *psi,
            FDAP fdap) = 0;
        virtual HRESULT STDMETHODCALLTYPE SetDefaultExtension(
            LPCWSTR pszDefaultExtension) = 0;
        virtual HRESULT STDMETHODCALLTYPE Close(
            HRESULT hr) = 0;
        virtual HRESULT STDMETHODCALLTYPE SetClientGuid(
            REFGUID guid) = 0;
        virtual HRESULT STDMETHODCALLTYPE ClearClientData(
            ) = 0;
        virtual HRESULT STDMETHODCALLTYPE SetFilter(
            IShellItemFilter *pFilter) = 0;
    };

    struct IFileDialog2 : public IFileDialog
    {
        virtual HRESULT STDMETHODCALLTYPE SetCancelButtonLabel(
            LPCWSTR pszLabel) = 0;
        virtual HRESULT STDMETHODCALLTYPE SetNavigationRoot(
            IShellItem *psi) = 0;
    };

    struct IFileDialogEvents : public IUnknown
    {
        virtual HRESULT STDMETHODCALLTYPE OnFileOk(
            IFileDialog *pfd) = 0;
        virtual HRESULT STDMETHODCALLTYPE OnFolderChanging(
            IFileDialog *pfd,
            IShellItem *psiFolder) = 0;
        virtual HRESULT STDMETHODCALLTYPE OnFolderChange(
            IFileDialog *pfd) = 0;
        virtual HRESULT STDMETHODCALLTYPE OnSelectionChange(
            IFileDialog *pfd) = 0;
        virtual HRESULT STDMETHODCALLTYPE OnShareViolation(
            IFileDialog *pfd,
            IShellItem *psi,
            FDE_SHAREVIOLATION_RESPONSE *pResponse) = 0;
        virtual HRESULT STDMETHODCALLTYPE OnTypeChange(
            IFileDialog *pfd) = 0;
        virtual HRESULT STDMETHODCALLTYPE OnOverwrite(
            IFileDialog *pfd,
            IShellItem *psi,
            FDE_OVERWRITE_RESPONSE *pResponse) = 0;
    };

    struct IFileOpenDialog : public IFileDialog
    {
        virtual HRESULT STDMETHODCALLTYPE GetResults(
            IShellItemArray **ppenum) = 0;
        virtual HRESULT STDMETHODCALLTYPE GetSelectedItems(
            IShellItemArray **ppsai) = 0;
    };

    struct NONCLIENTMETRICSW {
        UINT cbSize;
        int iBorderWidth;
        int iScrollWidth;
        int iScrollHeight;
        int iCaptionWidth;
        int iCaptionHeight;
        LOGFONTW lfCaptionFont;
        int iSmCaptionWidth;
        int iSmCaptionHeight;
        LOGFONTW lfSmCaptionFont;
        int iMenuWidth;
        int iMenuHeight;
        LOGFONTW lfMenuFont;
        LOGFONTW lfStatusFont;
        LOGFONTW lfMessageFont;
        int iPaddedBorderWidth;
    };

    struct NOTIFYICONDATAW {
        DWORD cbSize;
        HWND hWnd;
        UINT uID;
        UINT uFlags;
        UINT uCallbackMessage;
        HICON hIcon;
        WCHAR szTip[128];
        DWORD dwState;
        DWORD dwStateMask;
        WCHAR szInfo[256];
        union {
            UINT uTimeout;
            UINT uVersion;
        };
        WCHAR szInfoTitle[64];
        DWORD dwInfoFlags;
        GUID guidItem;
        HICON hBalloonIcon;
    };

    using PEN_FLAGS = UINT32;
    using PEN_MASK = UINT32;
    enum POINTER_BUTTON_CHANGE_TYPE { };
    using POINTER_FLAGS = UINT32;
    using POINTER_INPUT_TYPE = DWORD;
    using TOUCH_FLAGS = UINT32;
    using TOUCH_MASK = UINT32;
    struct POINTER_INFO {
        POINTER_INPUT_TYPE pointerType;
        UINT32 pointerId;
        UINT32 frameId;
        POINTER_FLAGS pointerFlags;
        HANDLE sourceDevice;
        HWND hwndTarget;
        POINT ptPixelLocation;
        POINT ptHimetricLocation;
        POINT ptPixelLocationRaw;
        POINT ptHimetricLocationRaw;
        DWORD dwTime;
        UINT32 historyCount;
        INT32 InputData;
        DWORD dwKeyStates;
        UINT64 PerformanceCount;
        POINTER_BUTTON_CHANGE_TYPE ButtonChangeType;
    };

    struct POINTER_PEN_INFO {
        POINTER_INFO pointerInfo;
        PEN_FLAGS penFlags;
        PEN_MASK penMask;
        UINT32 pressure;
        UINT32 rotation;
        INT32 tiltX;
        INT32 tiltY;
    };

    struct POINTER_TOUCH_INFO {
        POINTER_INFO pointerInfo;
        TOUCH_FLAGS touchFlags;
        TOUCH_MASK touchMask;
        RECT rcContact;
        RECT rcContactRaw;
        UINT32 orientation;
        UINT32 pressure;
    };

    struct POWERBROADCAST_SETTING {
        GUID PowerSetting;
        DWORD DataLength;
        UCHAR Data[1];
    };
    using PPOWERBROADCAST_SETTING = POWERBROADCAST_SETTING *;

    enum SHSTOCKICONID {
        SIID_DOCNOASSOC = 0,
        SIID_FOLDER = 3,
        SIID_FOLDEROPEN = 4,
        SIID_DRIVE35 = 6,
        SIID_DRIVEFIXED = 8,
        SIID_DRIVENET = 9,
        SIID_DRIVECD = 11,
        SIID_HELP = 23,
        SIID_RECYCLER = 31,
        SIID_DRIVEDVD = 59,
        SIID_SHIELD = 77,
        SIID_WARNING = 78,
        SIID_INFO = 79,
        SIID_ERROR = 80,
    };

    struct SHSTOCKICONINFO {
        DWORD cbSize;
        HICON hIcon;
        int iSysImageIndex;
        int iIcon;
        WCHAR szPath[MAX_PATH];
    } ;

    struct TOUCHINPUT {
        LONG x;
        LONG y;
        HANDLE hSource;
        DWORD dwID;
        DWORD dwFlags;
        DWORD dwMask;
        DWORD dwTime;
        ULONG_PTR dwExtraInfo;
        DWORD cxContact;
        DWORD cyContact;
    };
    using PTOUCHINPUT = TOUCHINPUT *;
    using PCTOUCHINPUT = const TOUCHINPUT *;

    struct WSAPOLLFD {
        SOCKET fd;
        short  events;
        short  revents;
    };
    using LPWSAPOLLFD = WSAPOLLFD *;
} // namespace WinXPType

inline namespace WinXPConstant {
    constexpr HRESULT DWM_E_COMPOSITIONDISABLED = 0x80263001;

    constexpr int FOS_OVERWRITEPROMPT = 0x2;
    constexpr int FOS_PICKFOLDERS = 0x20;
    constexpr int FOS_FORCEFILESYSTEM = 0x40;
    constexpr int FOS_ALLOWMULTISELECT = 0x200;
    constexpr int FOS_PATHMUSTEXIST = 0x800;
    constexpr int FOS_FILEMUSTEXIST = 0x1000;
    constexpr int FOS_NOREADONLYRETURN = 0x8000;
    constexpr int FOS_NODEREFERENCELINKS = 0x100000;
    constexpr int FOS_FORCESHOWHIDDEN = 0x10000000;

    constexpr IID IID_IFileDialog2 = {0x61744fc7, 0x85b5, 0x4791, 0xa9, 0xb0, 0x27, 0x22, 0x76, 0x30, 0x9b, 0x13};
    constexpr IID IID_IFileDialogEvents = {0x973510db, 0x7d7f, 0x452b, 0x89, 0x75, 0x74, 0xa8, 0x58, 0x28, 0xd3, 0x54};
    constexpr IID IID_IFileOpenDialog   = {0xd57c7288, 0xd4ad, 0x4768, 0xbe, 0x02, 0x9d, 0x96, 0x95, 0x32, 0xd9, 0x60};
    constexpr IID IID_IFileSaveDialog   = {0x84bccd23, 0x5fde, 0x4cdb,0xae, 0xa4, 0xaf, 0x64, 0xb8, 0x3d, 0x78, 0xab};

    constexpr int KF_FLAG_DONT_VERIFY = 0x00004000;

    constexpr const wchar_t *LOCALE_NAME_USER_DEFAULT = nullptr;
    constexpr const wchar_t *LOCALE_NAME_INVARIANT = L"";
    constexpr const wchar_t *LOCALE_NAME_SYSTEM_DEFAULT = L"!x-sys-default-locale";

    constexpr int LOCALE_SSORTLOCALE = 0x0000007b;

    constexpr int MUI_LANGUAGE_ID = 0x4;
    constexpr int MUI_LANGUAGE_NAME = 0x8;

    constexpr int MSGFLT_ALLOW = 1;

    constexpr int NID_INTEGRATED_TOUCH = 0x00000001;
    constexpr int NID_EXTERNAL_TOUCH = 0x00000002;
    constexpr int NID_READY = 0x00000080;

    constexpr int NIF_SHOWTIP = 0x00000080;

    constexpr int NIIF_LARGE_ICON = 0x00000020;

    constexpr int NOTIFYICON_VERSION_4 = 4;

    constexpr int PBT_POWERSETTINGCHANGE = 32787;

    constexpr int PEN_FLAG_BARREL = 0x00000001;
    constexpr int PEN_FLAG_INVERTED = 0x00000002;
    constexpr int PEN_FLAG_ERASER = 0x00000004;

    constexpr int PEN_MASK_PRESSURE = 0x00000001;
    constexpr int PEN_MASK_ROTATION = 0x00000002;
    constexpr int PEN_MASK_TILT_X = 0x00000004;
    constexpr int PEN_MASK_TILT_Y = 0x00000008;

    constexpr int POINTER_FLAG_DOWN = 0x00010000;
    constexpr int POINTER_FLAG_UP = 0x00040000;

    constexpr int POINTER_MESSAGE_FLAG_INCONTACT = 0x00000004;

    constexpr int SHGSI_LINKOVERLAY = SHGFI_LINKOVERLAY;

    constexpr SHSTOCKICONID SIID_INVALID = SHSTOCKICONID(-1);

    constexpr int SM_DIGITIZER = 94;
    constexpr int SM_MAXIMUMTOUCHES = 95;

    constexpr int TOUCHEVENTF_DOWN = 0x0002;
    constexpr int TOUCHEVENTF_UP = 0x0004;

    constexpr int TOUCH_MASK_CONTACTAREA = 0x00000001;
    constexpr int TOUCH_MASK_PRESSURE = 0x00000004;

    constexpr int TOUCHINPUTMASKF_CONTACTAREA = 0x0004;

    constexpr int WM_DWMNCRENDERINGCHANGED = 0x031f;
} // namespace WinXPConstant

inline namespace WinXPMacro {
    constexpr inline WORD GET_POINTERID_WPARAM(DWORD wParam) {
        return LOWORD(wParam);
    }

    constexpr inline bool IS_POINTER_FLAG_SET_WPARAM(DWORD wParam, int flag) {
        return (HIWORD(wParam) & flag) == flag;
    }

    constexpr inline bool IS_POINTER_INCONTACT_WPARAM(DWORD wParam) {
        return IS_POINTER_FLAG_SET_WPARAM(wParam, POINTER_MESSAGE_FLAG_INCONTACT);
    }
} // namespace WinXPMacro

namespace WinXPThunk {
    namespace DwmApi {
        // Windows Vista
        inline HRESULT WINAPI DwmEnableBlurBehindWindow(
            _In_ HWND hWnd,
            _In_ const DWM_BLURBEHIND *pBlurBehind
        ) {
            using type = decltype(&DwmEnableBlurBehindWindow);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"dwmapi.dll"), "DwmEnableBlurBehindWindow");
            if (real)
                return real(hWnd, pBlurBehind);

            return DWM_E_COMPOSITIONDISABLED;
        }

        // Windows Vista
        inline HRESULT WINAPI DwmGetWindowAttribute(
            _In_ HWND hwnd,
            _In_ DWORD dwAttribute,
            _Out_ PVOID pvAttribute,
            _In_ DWORD cbAttribute
        ) {
            using type = decltype(&DwmGetWindowAttribute);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"dwmapi.dll"), "DwmGetWindowAttribute");
            if (real)
                return real(hwnd, dwAttribute, pvAttribute, cbAttribute);

            // the only use of this function is to detect dark mode
            // report dark mode is not supported
            return E_INVALIDARG;
        }

        // Windows Vista
        inline HRESULT WINAPI DwmIsCompositionEnabled(
            _Out_ BOOL *pfEnabled
        ) {
            using type = decltype(&DwmIsCompositionEnabled);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"dwmapi.dll"), "DwmIsCompositionEnabled");
            if (real)
                return real(pfEnabled);

            if (pfEnabled) {
                *pfEnabled = FALSE;
                return S_OK;
            } else {
                SetLastError(ERROR_INVALID_PARAMETER);
                return E_INVALIDARG;
            }
        }

        // Windows Vista
        inline HRESULT WINAPI DwmSetWindowAttribute(
            _In_ HWND hwnd,
            _In_ DWORD dwAttribute,
            _In_ LPCVOID pvAttribute,
            _In_ DWORD cbAttribute
        ) {
            using type = decltype(&DwmSetWindowAttribute);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"dwmapi.dll"), "DwmSetWindowAttribute");
            if (real)
                return real(hwnd, dwAttribute, pvAttribute, cbAttribute);

            // the only use of this function is to set dark mode
            // report dark mode is not supported
            return E_INVALIDARG;
        }
    } // namespace DwmApi

    namespace IpHlpApi {
        namespace Detail {
            inline MIB_IFTABLE *IfTable = nullptr;

            inline bool InitIfTable() {
                if (IfTable)
                    return true;
                DWORD size = 0;
                GetIfTable(IfTable, &size, false);
                IfTable = (MIB_IFTABLE *)malloc(size);
                if (!IfTable) {
                    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
                    return false;
                }
                if (GetIfTable(IfTable, &size, false) != NO_ERROR) {
                    free(IfTable);
                    IfTable = nullptr;
                    return false;
                }
                return true;
            }
        } // namespace Detail

        // Windows Vista
        inline NETIO_STATUS WINAPI ConvertInterfaceIndexToLuid(
            _In_ NET_IFINDEX InterfaceIndex,
            _Out_ NET_LUID *InterfaceLuid
        ) {
            using type = decltype(&ConvertInterfaceIndexToLuid);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"iphlpapi.dll"), "ConvertInterfaceIndexToLuid");
            if (real)
                return real(InterfaceIndex, InterfaceLuid);

            if (!Detail::InitIfTable())
                return GetLastError();
            for (DWORD i = 0; i < Detail::IfTable->dwNumEntries; ++i) {
                auto *row = Detail::IfTable->table + i;
                if (row->dwIndex == InterfaceIndex) {
                    InterfaceLuid->Info.NetLuidIndex = row->dwIndex;
                    InterfaceLuid->Info.IfType = row->dwType;
                    return NO_ERROR;
                }
            }
            return ERROR_NOT_FOUND;
        }

        // Windows Vista
        inline NETIO_STATUS WINAPI ConvertInterfaceLuidToIndex(
            _In_ const NET_LUID *InterfaceLuid,
            _Out_ NET_IFINDEX *InterfaceIndex
        ) {
            using type = decltype(&ConvertInterfaceLuidToIndex);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"iphlpapi.dll"), "ConvertInterfaceLuidToIndex");
            if (real)
                return real(InterfaceLuid, InterfaceIndex);

            if (!Detail::InitIfTable())
                return GetLastError();
            for (DWORD i = 0; i < Detail::IfTable->dwNumEntries; ++i) {
                auto *row = Detail::IfTable->table + i;
                if (row->dwIndex == InterfaceLuid->Info.NetLuidIndex && row->dwType == InterfaceLuid->Info.IfType) {
                    *InterfaceIndex = row->dwIndex;
                    return NO_ERROR;
                }
            }
            return ERROR_NOT_FOUND;
        }

        // Windows Vista
        inline NETIO_STATUS WINAPI ConvertInterfaceLuidToNameW(
            _In_ const NET_LUID *InterfaceLuid,
            _Out_ PWSTR InterfaceName,
            _In_ SIZE_T Length
        ) {
            using type = decltype(&ConvertInterfaceLuidToNameW);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"iphlpapi.dll"), "ConvertInterfaceLuidToNameW");
            if (real)
                return real(InterfaceLuid, InterfaceName, Length);

            if (!Detail::InitIfTable())
                return GetLastError();
            for (DWORD i = 0; i < Detail::IfTable->dwNumEntries; ++i) {
                auto *row = Detail::IfTable->table + i;
                if (row->dwIndex == InterfaceLuid->Info.NetLuidIndex && row->dwType == InterfaceLuid->Info.IfType) {
                    if (Length < wcslen(row->wszName) + 1)
                        return ERROR_INSUFFICIENT_BUFFER;
                    wcscpy(InterfaceName, row->wszName);
                    return NO_ERROR;
                }
            }
            return ERROR_NOT_FOUND;
        }

        // Windows Vista
        inline NETIO_STATUS WINAPI ConvertInterfaceNameToLuidW(
            _In_ const WCHAR *InterfaceName,
            _Out_ NET_LUID *InterfaceLuid
        ) {
            using type = decltype(&ConvertInterfaceNameToLuidW);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"iphlpapi.dll"), "ConvertInterfaceNameToLuidW");
            if (real)
                return real(InterfaceName, InterfaceLuid);

            if (!Detail::InitIfTable())
                return GetLastError();
            for (DWORD i = 0; i < Detail::IfTable->dwNumEntries; ++i) {
                auto *row = Detail::IfTable->table + i;
                if (wcscmp(row->wszName, InterfaceName) == 0) {
                    InterfaceLuid->Info.NetLuidIndex = row->dwIndex;
                    InterfaceLuid->Info.IfType = row->dwType;
                    return NO_ERROR;
                }
            }
            return ERROR_NOT_FOUND;
        }

        // Windows XP
        // behaviour changed in Windows Vista
        inline ULONG WINAPI GetAdaptersAddresses(
            _In_ ULONG Family,
            _In_ ULONG Flags,
            _In_ PVOID Reserved,
            _Inout_ IP_ADAPTER_ADDRESSES_LH *AdapterAddresses,
            _Inout_ PULONG SizePointer
        ) {
            if (IsWindowsVistaOrGreater())
                return ::GetAdaptersAddresses(Family, Flags, Reserved, (IP_ADAPTER_ADDRESSES_XP *)AdapterAddresses, SizePointer);

            // Windows XP: simply fail
            // TODO: convert data format
            SetLastError(ERROR_NOT_SUPPORTED);
            return ERROR_NOT_SUPPORTED;
        }
    } // namespace IpHlpApi

    namespace Kernel32 {
        namespace Detail {
            inline const std::map<LANGID, const wchar_t *> LangidToLocaleNameMap = {
                // only supported languages
                { 0x0404, L"zh-TW" },
                { 0x0409, L"en-US" },
                { 0x0416, L"pt-BR" },
                { 0x0804, L"zh-CN" },
            };
        } // namespace Detail

        // Windows Vista
        inline BOOL WINAPI CancelIoEx(
            _In_ HANDLE hFile,
            _In_opt_ LPOVERLAPPED lpOverlapped
        ) {
            using type = decltype(&CancelIoEx);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "CancelIoEx");
            if (real)
                return real(hFile, lpOverlapped);

            return CancelIo(hFile);
        }

        // Windows Vista
        inline int WINAPI CompareStringExW(
            _In_opt_ LPCWSTR lpLocaleName,
            _In_ DWORD dwCmpFlags,
            _In_ LPCWSTR lpString1,
            _In_ int cchCount1,
            _In_ LPCWSTR lpString2,
            _In_ int cchCount2,
            _In_opt_ LPNLSVERSIONINFO lpVersionInformation,
            _In_opt_ LPVOID lpReserved,
            _In_opt_ LPARAM lParam
        ) {
            using type = decltype(&CompareStringExW);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "CompareStringExW");
            if (real)
                return real(lpLocaleName, dwCmpFlags, lpString1, cchCount1, lpString2, cchCount2, lpVersionInformation, lpReserved, lParam);

            return CompareStringW(LOCALE_USER_DEFAULT, dwCmpFlags, lpString1, cchCount1, lpString2, cchCount2);
        }

        // Windows 2000
        // behaviour changed in Windows Vista
        inline HANDLE WINAPI CreateNamedPipeW(
            _In_ LPCWSTR lpName,
            _In_ DWORD dwOpenMode,
            _In_ DWORD dwPipeMode,
            _In_ DWORD nMaxInstances,
            _In_ DWORD nOutBufferSize,
            _In_ DWORD nInBufferSize,
            _In_ DWORD nDefaultTimeOut,
            _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes
        ) {
            if (IsWindowsVistaOrGreater())
                return ::CreateNamedPipeW(lpName, dwOpenMode, dwPipeMode, nMaxInstances, nOutBufferSize, nInBufferSize, nDefaultTimeOut, lpSecurityAttributes);

            // Windows XP: remove flag PIPE_REJECT_REMOTE_CLIENTS
            dwPipeMode &= ~PIPE_REJECT_REMOTE_CLIENTS;
            return ::CreateNamedPipeW(lpName, dwOpenMode, dwPipeMode, nMaxInstances, nOutBufferSize, nInBufferSize, nDefaultTimeOut, lpSecurityAttributes);
        }

        // Windows Vista
        inline BOOL GetFileInformationByHandleEx(
            _In_ HANDLE hFile,
            _In_ FILE_INFO_BY_HANDLE_CLASS FileInformationClass,
            _Out_writes_bytes_(dwBufferSize) LPVOID lpFileInformation,
            _In_ DWORD dwBufferSize
        ) {
            using type = decltype(&GetFileInformationByHandleEx);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetFileInformationByHandleEx");
            if (real)
                return real(hFile, FileInformationClass, lpFileInformation, dwBufferSize);

            // the only use of this function is to get the file id.
            // simply return false
            SetLastError(ERROR_INVALID_FUNCTION);
            return FALSE;
        }

        // Windows Vista
        inline ULONGLONG GetTickCount64() {
            using type = decltype(&GetTickCount64);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetTickCount64");
            if (real)
                return real();

            return GetTickCount();
        }

        // Windows Vista
        inline BOOL GetUserPreferredUILanguages(
            _In_ DWORD dwFlags,
            _Out_ PULONG pulNumLanguages,
            _Out_opt_ PZZWSTR pwszLanguagesBuffer,
            _Inout_ PULONG pcchLanguagesBuffer
        ) {
            using type = decltype(&GetUserPreferredUILanguages);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetUserPreferredUILanguages");
            if (real)
                return real(dwFlags, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);

            LANGID langId = GetUserDefaultUILanguage();

            bool querySize = *pcchLanguagesBuffer == 0 && pwszLanguagesBuffer == nullptr;
            if (dwFlags == MUI_LANGUAGE_ID) {
                if (querySize) {
                    *pcchLanguagesBuffer = 6;
                    *pulNumLanguages = 1;
                    return TRUE;
                }
                if (*pcchLanguagesBuffer < 6) {
                    *pcchLanguagesBuffer = 6;
                    *pulNumLanguages = 0;
                    SetLastError(ERROR_INSUFFICIENT_BUFFER);
                    return FALSE;
                }
                *pcchLanguagesBuffer = 6;
                *pulNumLanguages = 1;
                wsprintf(pwszLanguagesBuffer, L"%04x", langId);
                pwszLanguagesBuffer[5] = L'\0';
                return TRUE;
            } else {
                const wchar_t *localeName = L"en-US";
                if (
                    auto it = Detail::LangidToLocaleNameMap.find(langId);
                    it != Detail::LangidToLocaleNameMap.end()
                ) {
                    localeName = it->second;
                }

                int length = wcslen(localeName);
                if (querySize) {
                    *pcchLanguagesBuffer = length + 2; // double null-terminated
                    *pulNumLanguages = 1;
                    return TRUE;
                }
                if (*pcchLanguagesBuffer < length + 2) {
                    *pcchLanguagesBuffer = length + 2;
                    *pulNumLanguages = 0;
                    SetLastError(ERROR_INSUFFICIENT_BUFFER);
                    return FALSE;
                }
                *pcchLanguagesBuffer = length + 2;
                *pulNumLanguages = 1;
                wcscpy(pwszLanguagesBuffer, localeName);
                pwszLanguagesBuffer[length + 1] = L'\0';  // double null-terminated
                return TRUE;
            }
        }

        // Windows 7
        inline void WINAPI RaiseFailFastException(
            _In_opt_ PEXCEPTION_RECORD pExceptionRecord,
            _In_opt_ PCONTEXT pContextRecord,
            _In_ DWORD dwFlags
        ) {
            using type = decltype(&RaiseFailFastException);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "RaiseFailFastException");
            if (real)
                return real(pExceptionRecord, pContextRecord, dwFlags);

            TerminateProcess(GetCurrentProcess(), pExceptionRecord ? pExceptionRecord->ExceptionCode : STATUS_FAIL_FAST_EXCEPTION);
        }
    } // namespace Kernel32

    namespace Shell32 {
        namespace Detail {
            struct GuidComp {
                bool operator()(const GUID &lhs, const GUID &rhs) const {
                    return memcmp(&lhs, &rhs, sizeof(GUID)) < 0;
                }
            };

            inline const std::map<GUID, int, GuidComp> KnownFolderClsidMap = {
                { FOLDERID_Desktop, CSIDL_DESKTOP },
                { FOLDERID_Documents, CSIDL_MYDOCUMENTS },
                { FOLDERID_Fonts, CSIDL_FONTS },
                { FOLDERID_LocalAppData, CSIDL_LOCAL_APPDATA },
                { FOLDERID_LocalAppDataLow, CSIDL_LOCAL_APPDATA },
                { FOLDERID_Music, CSIDL_MYMUSIC },
                { FOLDERID_Pictures, CSIDL_MYPICTURES },
                { FOLDERID_ProgramData, CSIDL_COMMON_APPDATA },
                { FOLDERID_Programs, CSIDL_PROGRAMS },
                { FOLDERID_RoamingAppData, CSIDL_APPDATA },
                { FOLDERID_Videos, CSIDL_MYVIDEO },
                // Not mapped: FOLDERID_Downloads - ok, Qt handles it
            };
        }

        // Windows Vista
        inline HRESULT WINAPI SHCreateItemFromIDList(
            _In_ PCIDLIST_ABSOLUTE pidl,
            _In_ REFIID riid,
            _Out_ void **ppv
        ) {
            using type = decltype(&SHCreateItemFromIDList);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"shell32.dll"), "SHCreateItemFromIDList");
            if (real)
                return real(pidl, riid, ppv);

            *ppv = nullptr;
            IPersistIDList *ppidl;
            IShellItem *psi;
            HRESULT hr = SHCoCreateInstance(nullptr, &CLSID_ShellItem, nullptr, __uuidof(IShellItem), (void **)&psi);
            if (SUCCEEDED(hr)) {
                hr = psi->QueryInterface(riid, ppv);
                psi->Release();

                hr = ppidl->SetIDList(pidl);
                if (SUCCEEDED(hr))
                    hr = ppidl->QueryInterface(riid, ppv);
                ppidl->Release();
            }
            return hr;
        }

        // Windows Vista
        inline HRESULT WINAPI SHCreateItemFromParsingName(
            _In_ PCWSTR pszPath,
            _In_opt_ IBindCtx *pbc,
            _In_ REFIID riid,
            _Out_ void **ppv
        ) {
            using type = decltype(&SHCreateItemFromParsingName);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"shell32.dll"), "SHCreateItemFromParsingName");
            if (real)
                return real(pszPath, pbc, riid, ppv);

            *ppv = nullptr;
            PIDLIST_ABSOLUTE pidl;
            HRESULT hr = SHParseDisplayName(pszPath, pbc, &pidl, 0, nullptr);
            if (SUCCEEDED(hr)) {
                hr = SHCreateItemFromIDList(pidl, riid, ppv);
                ILFree(pidl);
            }
            return hr;
        }

        // Windows Vista
        inline HRESULT WINAPI SHGetKnownFolderIDList(
            _In_ REFKNOWNFOLDERID rfid,
            _In_ DWORD dwFlags,
            _In_opt_ HANDLE hToken,
            _Out_ PIDLIST_ABSOLUTE *ppidl
        ) {
            using type = decltype(&SHGetKnownFolderIDList);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"shell32.dll"), "SHGetKnownFolderIDList");
            if (real)
                return real(rfid, dwFlags, hToken, ppidl);

            // the only use in Qt is to resolve clsid url scheme
            // simply fail
            return __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
        }

        // Windows Vista
        inline HRESULT WINAPI SHGetKnownFolderPath(
            _In_ REFKNOWNFOLDERID rfid,
            _In_ DWORD dwFlags,
            _In_opt_ HANDLE hToken,
            _Outptr_ PWSTR *ppszPath
        ) {
            using type = decltype(&SHGetKnownFolderPath);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"shell32.dll"), "SHGetKnownFolderPath");
            if (real)
                return real(rfid, dwFlags, hToken, ppszPath);

            if (!ppszPath)
                return E_INVALIDARG;
            *ppszPath = nullptr;

            if (
                auto it = Detail::KnownFolderClsidMap.find(rfid);
                it != Detail::KnownFolderClsidMap.end()
            ) {
                int csidl = it->second;
                wchar_t *buf = (wchar_t *)CoTaskMemAlloc(MAX_PATH * sizeof(wchar_t));
                if (!buf)
                    return E_OUTOFMEMORY;

                HRESULT hr = SHGetFolderPathW(nullptr, dwFlags | csidl, hToken, 0, buf);
                if (SUCCEEDED(hr))
                    *ppszPath = buf;
                else
                    CoTaskMemFree(buf);
                return hr;
            } else
                return __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
        }

        // Windows Vista
        inline HRESULT WINAPI SHGetStockIconInfo(
            _In_ SHSTOCKICONID siid,
            _In_ UINT uFlags,
            _Inout_ SHSTOCKICONINFO *psii
        ) {
            using type = decltype(&SHGetStockIconInfo);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"shell32.dll"), "SHGetStockIconInfo");
            if (real)
                return real(siid, uFlags, psii);

            return E_NOTIMPL;
        }

        // Windows XP
        inline BOOL WINAPI Shell_NotifyIconW(
            _In_ DWORD dwMessage,
            _In_ WinXPType::NOTIFYICONDATAW *lpData
        ) {
            return ::Shell_NotifyIconW(dwMessage, (_NOTIFYICONDATAW *)lpData);
        }

        // Windows 7
        inline HRESULT WINAPI Shell_NotifyIconGetRect(
            _In_ const NOTIFYICONIDENTIFIER *identifier,
            _Out_ RECT *iconLocation
        ) {
            using type = decltype(&Shell_NotifyIconGetRect);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"shell32.dll"), "Shell_NotifyIconGetRect");
            if (real)
                return real(identifier, iconLocation);

            return E_NOTIMPL;
        }
    } // namespace Shell32

    namespace User32 {
        // Windows 7
        inline BOOL WINAPI ChangeWindowMessageFilterEx(
            _In_ HWND hWnd,
            _In_ UINT message,
            _In_ DWORD action,
            _Inout_opt_ PCHANGEFILTERSTRUCT pChangeFilterStruct
        ) {
            using type = decltype(&ChangeWindowMessageFilterEx);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "ChangeWindowMessageFilterEx");
            if (real)
                return real(hWnd, message, action, pChangeFilterStruct);

            if (pChangeFilterStruct)
                pChangeFilterStruct->ExtStatus = 0;
            return TRUE;
        }

        // Windows 7
        inline BOOL WINAPI CloseTouchInputHandle(
            _In_ HTOUCHINPUT hTouchInput
        ) {
            using type = decltype(&CloseTouchInputHandle);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "CloseTouchInputHandle");
            if (real)
                return real(hTouchInput);

            SetLastError(ERROR_INVALID_HANDLE);
            return FALSE;
        }

        // Windows 7
        inline BOOL WINAPI GetTouchInputInfo(
            _In_ HTOUCHINPUT hTouchInput,
            _In_ UINT cInputs,
            _Out_ PTOUCHINPUT pInputs,
            _In_ int cbSize
        ) {
            using type = decltype(&GetTouchInputInfo);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "GetTouchInputInfo");
            if (real)
                return real(hTouchInput, cInputs, pInputs, cbSize);

            SetLastError(ERROR_INVALID_HANDLE);
            return FALSE;
        }

        // Windows 7
        inline BOOL WINAPI IsTouchWindow(
            _In_ HWND hwnd,
            _Out_opt_ PULONG pulFlags
        ) {
            using type = decltype(&IsTouchWindow);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "IsTouchWindow");
            if (real)
                return real(hwnd, pulFlags);

            if (pulFlags)
                *pulFlags = 0;
            return FALSE;
        }

        // Windows Vista
        inline HPOWERNOTIFY WINAPI RegisterPowerSettingNotification(
            _In_ HANDLE hRecipient,
            _In_ LPCGUID PowerSettingGuid,
            _In_ DWORD Flags
        ) {
            using type = decltype(&RegisterPowerSettingNotification);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "RegisterPowerSettingNotification");
            if (real)
                return real(hRecipient, PowerSettingGuid, Flags);

            return malloc(1);
        }

        // Windows 7
        inline BOOL WINAPI RegisterTouchWindow(
            _In_ HWND hwnd,
            _In_ ULONG ulFlags
        ) {
            using type = decltype(&RegisterTouchWindow);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "RegisterTouchWindow");
            if (real)
                return real(hwnd, ulFlags);

            return TRUE;
        }

        // Windows Vista
        inline BOOL WINAPI UnregisterPowerSettingNotification(
            _In_ HPOWERNOTIFY Handle
        ) {
            using type = decltype(&UnregisterPowerSettingNotification);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "UnregisterPowerSettingNotification");
            if (real)
                return real(Handle);

            if (Handle)
                free(Handle);
            return TRUE;
        }

        // Windows 7
        inline BOOL WINAPI UnregisterTouchWindow(
            _In_ HWND hWnd
        ) {
            using type = decltype(&UnregisterTouchWindow);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "UnregisterTouchWindow");
            if (real)
                return real(hWnd);

            return TRUE;
        }

        // Windows Vista
        inline BOOL WINAPI UpdateLayeredWindowIndirect(
            _In_ HWND hwnd,
            _In_ const UPDATELAYEREDWINDOWINFO *pULWInfo
        ) {
            using type = decltype(&UpdateLayeredWindowIndirect);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "UpdateLayeredWindowIndirect");
            if (real)
                return real(hwnd, pULWInfo);

            return UpdateLayeredWindow(hwnd, pULWInfo->hdcDst, (POINT *)pULWInfo->pptDst, (SIZE *)pULWInfo->psize, pULWInfo->hdcSrc, (POINT *)pULWInfo->pptSrc, pULWInfo->crKey, (BLENDFUNCTION *)pULWInfo->pblend, pULWInfo->dwFlags);
        }
    } // namespace User32
} // namespace WinXPThunk

template<> inline const GUID &__mingw_uuidof<WinXPType::IFileDialogEvents>() {
    return WinXPConstant::IID_IFileDialogEvents;
}

#define IP_ADAPTER_ADDRESSES IP_ADAPTER_ADDRESSES_LH
#define PIP_ADAPTER_ADDRESSES PIP_ADAPTER_ADDRESSES_LH
#define IP_ADAPTER_UNICAST_ADDRESS IP_ADAPTER_UNICAST_ADDRESS_LH
#define PIP_ADAPTER_UNICAST_ADDRESS PIP_ADAPTER_UNICAST_ADDRESS_LH

#define IFileDialog WinXPType::IFileDialog
#define IFileDialog2 WinXPType::IFileDialog2
#define IFileDialogEvents WinXPType::IFileDialogEvents
#define IFileOpenDialog WinXPType::IFileOpenDialog
#define NONCLIENTMETRICS WinXPType::NONCLIENTMETRICSW
#define NOTIFYICONDATA WinXPType::NOTIFYICONDATAW

#define DwmEnableBlurBehindWindow WinXPThunk::DwmApi::DwmEnableBlurBehindWindow
#define DwmGetWindowAttribute WinXPThunk::DwmApi::DwmGetWindowAttribute
#define DwmIsCompositionEnabled WinXPThunk::DwmApi::DwmIsCompositionEnabled
#define DwmSetWindowAttribute WinXPThunk::DwmApi::DwmSetWindowAttribute

#define ConvertInterfaceIndexToLuid WinXPThunk::IpHlpApi::ConvertInterfaceIndexToLuid
#define ConvertInterfaceLuidToIndex WinXPThunk::IpHlpApi::ConvertInterfaceLuidToIndex
#define ConvertInterfaceLuidToNameW WinXPThunk::IpHlpApi::ConvertInterfaceLuidToNameW
#define ConvertInterfaceNameToLuidW WinXPThunk::IpHlpApi::ConvertInterfaceNameToLuidW
#define GetAdaptersAddresses WinXPThunk::IpHlpApi::GetAdaptersAddresses

#define CancelIoEx WinXPThunk::Kernel32::CancelIoEx
#define CompareStringEx WinXPThunk::Kernel32::CompareStringExW
#define CreateNamedPipe WinXPThunk::Kernel32::CreateNamedPipeW
#define GetFileInformationByHandleEx WinXPThunk::Kernel32::GetFileInformationByHandleEx
#define GetTickCount64 WinXPThunk::Kernel32::GetTickCount64
#define GetUserPreferredUILanguages WinXPThunk::Kernel32::GetUserPreferredUILanguages
#define RaiseFailFastException WinXPThunk::Kernel32::RaiseFailFastException

#define SHCreateItemFromIDList WinXPThunk::Shell32::SHCreateItemFromIDList
#define SHCreateItemFromParsingName WinXPThunk::Shell32::SHCreateItemFromParsingName
#define SHGetKnownFolderIDList WinXPThunk::Shell32::SHGetKnownFolderIDList
#define SHGetKnownFolderPath WinXPThunk::Shell32::SHGetKnownFolderPath
#define SHGetStockIconInfo WinXPThunk::Shell32::SHGetStockIconInfo
#define Shell_NotifyIcon WinXPThunk::Shell32::Shell_NotifyIconW
#define Shell_NotifyIconGetRect WinXPThunk::Shell32::Shell_NotifyIconGetRect

#define ChangeWindowMessageFilterEx WinXPThunk::User32::ChangeWindowMessageFilterEx
#define CloseTouchInputHandle WinXPThunk::User32::CloseTouchInputHandle
#define GetTouchInputInfo WinXPThunk::User32::GetTouchInputInfo
#define IsTouchWindow WinXPThunk::User32::IsTouchWindow
#define RegisterPowerSettingNotification WinXPThunk::User32::RegisterPowerSettingNotification
#define RegisterTouchWindow WinXPThunk::User32::RegisterTouchWindow
#define UnregisterPowerSettingNotification WinXPThunk::User32::UnregisterPowerSettingNotification
#define UnregisterTouchWindow WinXPThunk::User32::UnregisterTouchWindow
#define UpdateLayeredWindowIndirect WinXPThunk::User32::UpdateLayeredWindowIndirect

#endif // QT_WINDOWS_H
