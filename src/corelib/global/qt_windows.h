// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QT_WINDOWS_H
#define QT_WINDOWS_H

#if 0
#pragma qt_sync_skip_header_check
#pragma qt_sync_stop_processing
#endif

#ifndef WINVER
#  define WINVER 0x0A00 // _WIN32_WINNT_WIN10
#endif
#ifndef _WIN32_WINNT
#  define _WIN32_WINNT 0x0A00
#endif
#ifndef _WIN32_IE
#  define _WIN32_IE 0x0A00
#endif
#ifndef NTDDI_VERSION
#  define NTDDI_VERSION 0x0A00000C // NTDDI_WIN10_NI
#endif

#ifndef NOMINMAX
#  define NOMINMAX
#endif
#include <winsock2.h>
#include <windows.h>

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

#include <d3d11.h>
#include <d3d12.h>
#include <d3d9.h>
#include <dwmapi.h>
#include <dwrite.h>
#include <dxgi1_3.h>
#include <iphlpapi.h>
#include <namedpipeapi.h>
#include <netioapi.h>
#include <ntsecapi.h>
#include <ntstatus.h>
#include <knownfolders.h>
#include <roapi.h>
#include <shellapi.h>
#include <shellscalingapi.h>
#include <shlobj.h>
#include <versionhelpers.h>
#include <winstring.h>
#include <winternl.h>
#include <ws2tcpip.h>

#pragma GCC diagnostic ignored "-Wcast-function-type"

#if !defined(_WIN64)

namespace Win32Thunk_5_1
{
    namespace AdvApi32
    {
        // documented as RtlGenRandom
        inline BOOLEAN SystemFunction036(
            _Out_ PVOID RandomBuffer,
            _In_ ULONG RandomBufferLength
        ) {
            using type = decltype(&SystemFunction036);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"advapi32.dll"), "SystemFunction036");
            if (real)
                return real(RandomBuffer, RandomBufferLength);

            // the only use of this function is to fill buffer
            // simply fail, Qt will handle it
            SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
            return FALSE;
        }
    }

    namespace D3D9
    {
        inline IDirect3D9 *Direct3DCreate9(
            UINT SDKVersion
        ) {
            using type = decltype(&::Direct3DCreate9);
            static HMODULE module = LoadLibraryW(L"d3d9.dll");
            if (module) {
                static type real = (type)GetProcAddress(module, "Direct3DCreate9");
                if (real)
                    return real(SDKVersion);
            }

            // the only use is detecting opengl GPU
            // simply fail
            return nullptr;
        }
    }

    namespace Kernel32
    {
        inline BOOL CheckRemoteDebuggerPresent(
            _In_ HANDLE hProcess,
            _Inout_ PBOOL pbDebuggerPresent
        ) {
            using type = decltype(&::CheckRemoteDebuggerPresent);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "CheckRemoteDebuggerPresent");
            if (real)
                return real(hProcess, pbDebuggerPresent);

            *pbDebuggerPresent = (hProcess == GetCurrentProcess()) && IsDebuggerPresent();
            return TRUE;
        }

        inline int GetGeoInfoW(
            _In_ GEOID Location,
            _In_ GEOTYPE GeoType,
            _Out_writes_opt_(cchData) LPWSTR lpGeoData,
            _In_ int cchData,
            _In_ LANGID LangId
        ) {
            using type = decltype(&::GetGeoInfoW);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetGeoInfoW");
            if (real)
                return real(Location, GeoType, lpGeoData, cchData, LangId);

            // the only use in Qt is to determine system time zone
            // return CN (UTC+8), which covers most people in the world
            if (GeoType != GEO_ISO2) {
                SetLastError(ERROR_INVALID_PARAMETER);
                return 0;
            }
            lpGeoData[0] = L'C';
            lpGeoData[1] = L'N';
            lpGeoData[2] = 0;
            return 3;
        }

        inline BOOL GetModuleHandleExW(
            _In_ DWORD dwFlags,
            _In_opt_ LPCWSTR lpModuleName,
            _Out_ HMODULE *phModule
        ) {
            using type = decltype(&::GetModuleHandleExW);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetModuleHandleExW");
            if (real)
                return real(dwFlags, lpModuleName, phModule);

            // the only use in Qt is to prevent unloading of the plugin
            // simply fail, we are building static Qt
            SetLastError(ERROR_NOT_SUPPORTED);
            return FALSE;
        }

        inline void GetNativeSystemInfo(
            _Out_ LPSYSTEM_INFO lpSystemInfo
        ) {
            using type = decltype(&::GetNativeSystemInfo);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetNativeSystemInfo");
            if (real)
                return real(lpSystemInfo);

            GetSystemInfo(lpSystemInfo);
        }

        inline DWORD GetProcessId(
            _In_ HANDLE Process
        ) {
            using type = decltype(&::GetProcessId);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetProcessId");
            if (real)
                return real(Process);

            PROCESS_BASIC_INFORMATION processBasicInfo;
            NTSTATUS status = NtQueryInformationProcess(Process, ProcessBasicInformation, &processBasicInfo, sizeof(processBasicInfo), nullptr);
            if (NT_SUCCESS(status)) {
                return processBasicInfo.UniqueProcessId;
            } else {
                SetLastError(RtlNtStatusToDosError(status));
                return 0;
            }
        }

        inline GEOID GetUserGeoID(
            _In_ GEOCLASS GeoClass
        ) {
            using type = decltype(&::GetUserGeoID);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetUserGeoID");
            if (real)
                return real(GeoClass);

            // the only use in Qt is to determine system time zone
            // return CN (UTC+8), which covers most people in the world
            if (GeoClass != GEOCLASS_NATION) {
                SetLastError(ERROR_INVALID_PARAMETER);
                return GEOID_NOT_AVAILABLE;
            }
            return 45;
        }

        inline BOOL GetVolumePathNamesForVolumeNameW(
            _In_ LPCWSTR lpszVolumeName,
            _Out_writes_(cchBufferLength) LPWCH lpszVolumePathNames,
            _In_ DWORD cchBufferLength,
            _Out_ PDWORD lpcchReturnLength
        ) {
            using type = decltype(&::GetVolumePathNamesForVolumeNameW);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetVolumePathNamesForVolumeNameW");
            if (real)
                return real(lpszVolumeName, lpszVolumePathNames, cchBufferLength, lpcchReturnLength);

            // the only use in Qt is to convert volume UUID to drive letter
            // simply fail
            SetLastError(ERROR_NOT_SUPPORTED);
            return FALSE;
        }

        inline BOOL TzSpecificLocalTimeToSystemTime(
            _In_opt_ const TIME_ZONE_INFORMATION *lpTimeZoneInformation,
            _In_ const SYSTEMTIME *lpLocalTime,
            _Out_ LPSYSTEMTIME lpUniversalTime
        ) {
            using type = decltype(&::TzSpecificLocalTimeToSystemTime);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "TzSpecificLocalTimeToSystemTime");
            if (real)
                return real(lpTimeZoneInformation, lpLocalTime, lpUniversalTime);

            long long fileTimeNs;
            if (!SystemTimeToFileTime(lpLocalTime, (FILETIME *)&fileTimeNs))
                return FALSE;
            fileTimeNs += lpTimeZoneInformation->Bias * 60LL * 10000000LL;
            if (!FileTimeToSystemTime((FILETIME *)&fileTimeNs, lpUniversalTime))
                return FALSE;
            return TRUE;
        }

        inline DWORD WTSGetActiveConsoleSessionId()
        {
            using type = decltype(&::WTSGetActiveConsoleSessionId);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "WTSGetActiveConsoleSessionId");
            if (real)
                return real();

            return 0xFFFFFFFF;
        }
    }

    namespace Shell32
    {
        inline HRESULT SHDefExtractIconW(
            _In_ LPCWSTR pszIconFile,
            _In_ int iIconIndex,
            _In_ UINT uFlags,
            _Out_opt_ HICON *phiconLarge,
            _Out_opt_ HICON *phiconSmall,
            _In_ UINT nIconSize
        ) {
            using type = decltype(&::SHDefExtractIconW);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"shell32.dll"), "SHDefExtractIconW");
            if (real)
                return real(pszIconFile, iIconIndex, uFlags, phiconLarge, phiconSmall, nIconSize);

            // the only use in Qt is guarded by newer `SHGetStockIconInfo` (6.0)
            // simply fail
            return E_NOTIMPL;
        }
    }

    namespace User32
    {
        inline ATOM RegisterClassExW(
            _In_ const WNDCLASSEXW *unnamedParam1
        ) {
            // behaviour change
            if (IsWindowsXPOrGreater())
                return ::RegisterClassExW(unnamedParam1);

            // CS_DROPSHADOW is not supported on Windows 2000
            WNDCLASSEXW param = *unnamedParam1;
            param.style = unnamedParam1->style & ~CS_DROPSHADOW;
            return ::RegisterClassExW(&param);
        }
    }

    namespace Ws2_32
    {
        inline void WSAAPI freeaddrinfo(
            _In_ PADDRINFOA pAddrInfo
        ) {
            using type = decltype(&::freeaddrinfo);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"ws2_32.dll"), "freeaddrinfo");
            if (real)
                return real(pAddrInfo);

            while (pAddrInfo) {
                addrinfo *next = pAddrInfo->ai_next;
                free(pAddrInfo->ai_addr);
                free(pAddrInfo);
                pAddrInfo = next;
            }
        }

        namespace Detail {
            inline addrinfo *AddrInfoFromHostent(const hostent *host, int idx) {
                // this is the last entry
                if (host->h_addr_list[idx] == nullptr)
                    return nullptr;

                addrinfo *next = AddrInfoFromHostent(host, idx + 1);

                addrinfo *result = (addrinfo *)malloc(sizeof(addrinfo));
                if (!result) {
                    Ws2_32::freeaddrinfo(next);
                    return nullptr;
                }

                result->ai_flags = 0;
                result->ai_family = AF_INET;
                result->ai_socktype = SOCK_STREAM;
                result->ai_protocol = IPPROTO_TCP;
                result->ai_canonname = nullptr;
                result->ai_addrlen = sizeof(sockaddr_in);

                sockaddr_in *addr = (sockaddr_in *)malloc(sizeof(sockaddr_in));
                if (!addr) {
                    Ws2_32::freeaddrinfo(result);
                    Ws2_32::freeaddrinfo(next);
                    return nullptr;
                }

                addr->sin_family = AF_INET;
                addr->sin_port = 0;
                addr->sin_addr = *(in_addr *)host->h_addr;
                memset(addr->sin_zero, 0, sizeof(addr->sin_zero));
                result->ai_addr = (sockaddr *)addr;

                result->ai_next = next;
                return result;
            }
        }

        inline INT WSAAPI getaddrinfo(
            _In_opt_ PCSTR pNodeName,
            _In_opt_ PCSTR pServiceName,
            _In_opt_ const ADDRINFOA *pHints,
            _Out_ PADDRINFOA *ppResult
        ) {
            using type = decltype(&::getaddrinfo);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"ws2_32.dll"), "getaddrinfo");
            if (real)
                return real(pNodeName, pServiceName, pHints, ppResult);

            // the only use in Qt is simple lookup
            // implement it with gethostbyname
            if (!pNodeName)
                return WSAHOST_NOT_FOUND;
            if (pHints && pHints->ai_family != AF_UNSPEC)
                return WSAEAFNOSUPPORT;

            struct hostent *host = gethostbyname(pNodeName);
            if (!host)
                return WSANO_DATA;

            addrinfo *result = Detail::AddrInfoFromHostent(host, 0);
            if (!result)
                return WSA_NOT_ENOUGH_MEMORY;
            *ppResult = result;
            return 0;
        }

        inline INT WSAAPI getnameinfo(
            _In_ const SOCKADDR *pSockaddr,
            _In_ socklen_t SockaddrLength,
            _Out_ PCHAR pNodeBuffer,
            _In_ DWORD NodeBufferLength,
            _Out_ PCHAR pServiceBuffer,
            _In_ DWORD ServiceBufferLength,
            _In_ INT Flags
        ) {
            using type = decltype(&::getnameinfo);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"ws2_32.dll"), "getnameinfo");
            if (real)
                return real(pSockaddr, SockaddrLength, pNodeBuffer, NodeBufferLength, pServiceBuffer, ServiceBufferLength, Flags);

            // the only use in Qt is simple reverse lookup
            // implement it with gethostbyaddr
            if (SockaddrLength == sizeof(sockaddr_in)) {
                const sockaddr_in *addr = (const sockaddr_in *)pSockaddr;
                hostent *host = gethostbyaddr((const char *)&addr->sin_addr, sizeof(in_addr), AF_INET);
                if (!host)
                    return WSANO_DATA;

                if (pNodeBuffer && NodeBufferLength > 0) {
                    strncpy(pNodeBuffer, host->h_name, NodeBufferLength);
                    pNodeBuffer[NodeBufferLength - 1] = '\0';
                }
                if (pServiceBuffer && ServiceBufferLength > 0)
                    pServiceBuffer[0] = '\0';
                return 0;
            } else if (SockaddrLength == sizeof(sockaddr_in6)) {
                const sockaddr_in6 *addr = (const sockaddr_in6 *)pSockaddr;
                hostent *host = gethostbyaddr((const char *)&addr->sin6_addr, sizeof(in6_addr), AF_INET6);
                if (!host)
                    return WSANO_DATA;

                if (pNodeBuffer && NodeBufferLength > 0) {
                    strncpy(pNodeBuffer, host->h_name, NodeBufferLength);
                    pNodeBuffer[NodeBufferLength - 1] = '\0';
                }
                if (pServiceBuffer && ServiceBufferLength > 0)
                    pServiceBuffer[0] = '\0';
                return 0;
            } else {
                return WSAEFAULT;
            }
        }
    }
}

#undef RtlGenRandom
#define SystemFunction036 Win32Thunk_5_1::AdvApi32::SystemFunction036

#define Direct3DCreate9 Win32Thunk_5_1::D3D9::Direct3DCreate9

#undef GetGeoInfo
#undef GetModuleHandleEx
#undef GetVolumePathNamesForVolumeName
#define CheckRemoteDebuggerPresent      Win32Thunk_5_1::Kernel32::CheckRemoteDebuggerPresent
#define GetGeoInfo                      Win32Thunk_5_1::Kernel32::GetGeoInfoW
#define GetModuleHandleEx               Win32Thunk_5_1::Kernel32::GetModuleHandleExW
#define GetNativeSystemInfo             Win32Thunk_5_1::Kernel32::GetNativeSystemInfo
#define GetProcessId                    Win32Thunk_5_1::Kernel32::GetProcessId
#define GetUserGeoID                    Win32Thunk_5_1::Kernel32::GetUserGeoID
#define GetVolumePathNamesForVolumeName Win32Thunk_5_1::Kernel32::GetVolumePathNamesForVolumeNameW
#define TzSpecificLocalTimeToSystemTime Win32Thunk_5_1::Kernel32::TzSpecificLocalTimeToSystemTime
#define WTSGetActiveConsoleSessionId    Win32Thunk_5_1::Kernel32::WTSGetActiveConsoleSessionId

#undef SHDefExtractIcon
#define SHDefExtractIcon Win32Thunk_5_1::Shell32::SHDefExtractIconW

#undef RegisterClassEx
#define RegisterClassEx Win32Thunk_5_1::User32::RegisterClassExW

#define freeaddrinfo Win32Thunk_5_1::Ws2_32::freeaddrinfo
#define getaddrinfo  Win32Thunk_5_1::Ws2_32::getaddrinfo
#define getnameinfo  Win32Thunk_5_1::Ws2_32::getnameinfo

#endif // _WIN64

#if !defined(__aarch64)

namespace Win32Thunk_6_0
{
    namespace DwmApi
    {
        inline HRESULT DwmEnableBlurBehindWindow(
            _In_ HWND hWnd,
            _In_ const DWM_BLURBEHIND *pBlurBehind
        ) {
            using type = decltype(&::DwmEnableBlurBehindWindow);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"dwmapi.dll"), "DwmEnableBlurBehindWindow");
            if (real)
                return real(hWnd, pBlurBehind);

            return 0x80263001; // DWM_E_COMPOSITIONDISABLED
        }

        inline HRESULT DwmGetWindowAttribute(
            _In_ HWND hwnd,
            _In_ DWORD dwAttribute,
            _Out_ PVOID pvAttribute,
            _In_ DWORD cbAttribute
        ) {
            using type = decltype(&::DwmGetWindowAttribute);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"dwmapi.dll"), "DwmGetWindowAttribute");
            if (real)
                return real(hwnd, dwAttribute, pvAttribute, cbAttribute);

            // the only use of this function is to detect dark mode
            // report dark mode is not supported
            return E_INVALIDARG;
        }

        inline HRESULT DwmIsCompositionEnabled(
            _Out_ BOOL *pfEnabled
        ) {
            using type = decltype(&::DwmIsCompositionEnabled);
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

        inline HRESULT DwmSetWindowAttribute(
            _In_ HWND hwnd,
            _In_ DWORD dwAttribute,
            _In_ LPCVOID pvAttribute,
            _In_ DWORD cbAttribute
        ) {
            using type = decltype(&::DwmSetWindowAttribute);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"dwmapi.dll"), "DwmSetWindowAttribute");
            if (real)
                return real(hwnd, dwAttribute, pvAttribute, cbAttribute);

            // the only use of this function is to set dark mode
            // report dark mode is not supported
            return E_INVALIDARG;
        }
    }
    
    namespace IpHlpApi
    {
        namespace Detail
        {
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
        }

        inline NETIO_STATUS ConvertInterfaceIndexToLuid(
            _In_ NET_IFINDEX InterfaceIndex,
            _Out_ NET_LUID *InterfaceLuid
        ) {
            using type = decltype(&::ConvertInterfaceIndexToLuid);
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

        inline NETIO_STATUS ConvertInterfaceLuidToGuid(
            _In_ const NET_LUID *InterfaceLuid,
            _Out_ GUID *InterfaceGuid
        ) {
            using type = decltype(&::ConvertInterfaceLuidToGuid);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"iphlpapi.dll"), "ConvertInterfaceLuidToGuid");
            if (real)
                return real(InterfaceLuid, InterfaceGuid);

            // the only use is in network connection / status monitor
            // simply fail
            return ERROR_CALL_NOT_IMPLEMENTED;
        }

        inline NETIO_STATUS ConvertInterfaceLuidToIndex(
            _In_ const NET_LUID *InterfaceLuid,
            _Out_ NET_IFINDEX *InterfaceIndex
        ) {
            using type = decltype(&::ConvertInterfaceLuidToIndex);
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

        inline NETIO_STATUS ConvertInterfaceLuidToNameW(
            _In_ const NET_LUID *InterfaceLuid,
            _Out_ PWSTR InterfaceName,
            _In_ SIZE_T Length
        ) {
            using type = decltype(&::ConvertInterfaceLuidToNameW);
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

        inline NETIO_STATUS ConvertInterfaceNameToLuidW(
            _In_ const WCHAR *InterfaceName,
            _Out_ NET_LUID *InterfaceLuid
        ) {
            using type = decltype(&::ConvertInterfaceNameToLuidW);
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

        inline ULONG GetAdaptersAddresses(
            _In_ ULONG Family,
            _In_ ULONG Flags,
            _In_ PVOID Reserved,
            _Inout_ IP_ADAPTER_ADDRESSES_LH *AdapterAddresses,
            _Inout_ PULONG SizePointer
        ) {
            // introduced in Windows XP; behaviour changed in Windows Vista
#if defined(_WIN64)
            if (IsWindowsVistaOrGreater())
                return ::GetAdaptersAddresses(Family, Flags, Reserved, AdapterAddresses, SizePointer);
#else
            using type = decltype(&::GetAdaptersAddresses);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"iphlpapi.dll"), "GetAdaptersAddresses");
            if (real && IsWindowsVistaOrGreater())
                return real(Family, Flags, Reserved, AdapterAddresses, SizePointer);
#endif

            // the result is not used by Red Panda C++
            // simply fail, Qt will handle it
            SetLastError(ERROR_NOT_SUPPORTED);
            return ERROR_NOT_SUPPORTED;
        }
    }

    namespace Kernel32
    {
        namespace Detail
        {
            inline const std::map<LANGID, const wchar_t *> LangidToLocaleNameMap = {
                // only supported languages
                { 0x0404, L"zh-TW" },
                { 0x0409, L"en-US" },
                { 0x0416, L"pt-BR" },
                { 0x0804, L"zh-CN" },
            };
        }

        inline void AcquireSRWLockExclusive(
            _Inout_ PSRWLOCK SRWLock
        ) {
            using type = decltype(&::AcquireSRWLockExclusive);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "AcquireSRWLockExclusive");
            if (real)
                return real(SRWLock);

            // the only use is d3d12 / vulkan memory allocation
            // do nothing
            return;
        }

        inline void AcquireSRWLockShared(
            _Inout_ PSRWLOCK SRWLock
        ) {
            using type = decltype(&::AcquireSRWLockShared);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "AcquireSRWLockShared");
            if (real)
                return real(SRWLock);

            // the only use is d3d12 / vulkan memory allocation
            // do nothing
            return;
        }

        inline BOOL CancelIoEx(
            _In_ HANDLE hFile,
            _In_opt_ LPOVERLAPPED lpOverlapped
        ) {
            using type = decltype(&::CancelIoEx);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "CancelIoEx");
            if (real)
                return real(hFile, lpOverlapped);

            return CancelIo(hFile);
        }

        inline int CompareStringEx(
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
            using type = decltype(&::CompareStringEx);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "CompareStringExW");
            if (real)
                return real(lpLocaleName, dwCmpFlags, lpString1, cchCount1, lpString2, cchCount2, lpVersionInformation, lpReserved, lParam);

            return CompareStringW(LOCALE_USER_DEFAULT, dwCmpFlags, lpString1, cchCount1, lpString2, cchCount2);
        }

        inline HANDLE CreateNamedPipeW(
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

        inline BOOL GetFileInformationByHandleEx(
            _In_ HANDLE hFile,
            _In_ FILE_INFO_BY_HANDLE_CLASS FileInformationClass,
            _Out_writes_bytes_(dwBufferSize) LPVOID lpFileInformation,
            _In_ DWORD dwBufferSize
        ) {
            using type = decltype(&::GetFileInformationByHandleEx);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetFileInformationByHandleEx");
            if (real)
                return real(hFile, FileInformationClass, lpFileInformation, dwBufferSize);

            // the only use of this function is to get the file id.
            // simply return false
            SetLastError(ERROR_INVALID_FUNCTION);
            return FALSE;
        }

        inline ULONGLONG GetTickCount64()
        {
            using type = decltype(&::GetTickCount64);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetTickCount64");
            if (real)
                return real();

            return GetTickCount();
        }

        inline BOOL GetUserPreferredUILanguages(
            _In_ DWORD dwFlags,
            _Out_ PULONG pulNumLanguages,
            _Out_opt_ PZZWSTR pwszLanguagesBuffer,
            _Inout_ PULONG pcchLanguagesBuffer
        ) {
            using type = decltype(&::GetUserPreferredUILanguages);
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

                unsigned long length = wcslen(localeName);
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

        inline void ReleaseSRWLockExclusive(
            _Inout_ PSRWLOCK SRWLock
        ) {
            using type = decltype(&::ReleaseSRWLockExclusive);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "ReleaseSRWLockExclusive");
            if (real)
                return real(SRWLock);

            // the only use is d3d12 / vulkan memory allocation
            // do nothing
            return;
        }

        inline void ReleaseSRWLockShared(
            _Inout_ PSRWLOCK SRWLock
        ) {
            using type = decltype(&::ReleaseSRWLockShared);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "ReleaseSRWLockShared");
            if (real)
                return real(SRWLock);

            // the only use is d3d12 / vulkan memory allocation
            // do nothing
            return;
        }
    }

#if !defined(_UCRT)
    namespace Msvcrt
    {
        inline errno_t _wgetenv_s(
            size_t *pReturnValue,
            wchar_t *buffer,
            size_t numberOfElements,
            const wchar_t *varname
        ) {
            // global `::_wgetenv_s` is overloaded
            using type = decltype(&_wgetenv_s);
            (void)static_cast<type>(&::_wgetenv_s);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"msvcrt.dll"), "_wgetenv_s");
            if (real)
                return real(pReturnValue, buffer, numberOfElements, varname);

            if (!pReturnValue || !varname)
                return EINVAL;
            if (!buffer && numberOfElements > 0)
                return EINVAL;

            wchar_t *env = _wgetenv(varname);
            if (env) {
                size_t len = wcslen(env) + 1;
                if (len > numberOfElements) {
                    *pReturnValue = len;
                    if (buffer)
                        buffer[0] = L'\0';
                    return ERANGE;
                } else {
                    *pReturnValue = len;
                    wcscpy(buffer, env);
                    return 0;
                }
            } else {
                *pReturnValue = 0;
                if (buffer && numberOfElements > 0)
                    buffer[0] = L'\0';
                return 0;
            }
        }
    }
#endif

    namespace Shell32
    {
        namespace Detail
        {
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

        inline HRESULT SHCreateItemFromIDList(
            _In_ PCIDLIST_ABSOLUTE pidl,
            _In_ REFIID riid,
            _Out_ void **ppv
        ) {
            using type = decltype(&::SHCreateItemFromIDList);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"shell32.dll"), "SHCreateItemFromIDList");
            if (real)
                return real(pidl, riid, ppv);

            // the only use in Qt is to create file with clsid url scheme
            // simply fail since SHGetKnownFolderIDList is not implemented
            return __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
        }

        inline HRESULT SHCreateItemFromParsingName(
            _In_ PCWSTR pszPath,
            _In_opt_ IBindCtx *pbc,
            _In_ REFIID riid,
            _Out_ void **ppv
        ) {
            using type = decltype(&::SHCreateItemFromParsingName);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"shell32.dll"), "SHCreateItemFromParsingName");
            if (real)
                return real(pszPath, pbc, riid, ppv);

            // usages in Qt:
            //   1. moveToTrash: only applies to Windows 8+
            //   2. file dialog setDirectory: only applies to Windows Vista+
            // simply fail
            return __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
        }

        inline HRESULT SHGetKnownFolderIDList(
            _In_ REFKNOWNFOLDERID rfid,
            _In_ DWORD dwFlags,
            _In_opt_ HANDLE hToken,
            _Out_ PIDLIST_ABSOLUTE *ppidl
        ) {
            using type = decltype(&::SHGetKnownFolderIDList);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"shell32.dll"), "SHGetKnownFolderIDList");
            if (real)
                return real(rfid, dwFlags, hToken, ppidl);

            // the only use in Qt is to resolve clsid url scheme
            // simply fail
            return __HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
        }

        inline HRESULT SHGetKnownFolderPath(
            _In_ REFKNOWNFOLDERID rfid,
            _In_ DWORD dwFlags,
            _In_opt_ HANDLE hToken,
            _Outptr_ PWSTR *ppszPath
        ) {
            using type = decltype(&::SHGetKnownFolderPath);
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

        inline HRESULT SHGetStockIconInfo(
            _In_ SHSTOCKICONID siid,
            _In_ UINT uFlags,
            _Inout_ SHSTOCKICONINFO *psii
        ) {
            using type = decltype(&::SHGetStockIconInfo);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"shell32.dll"), "SHGetStockIconInfo");
            if (real)
                return real(siid, uFlags, psii);

            return E_NOTIMPL;
        }
    }

    namespace User32
    {
        inline HPOWERNOTIFY RegisterPowerSettingNotification(
            _In_ HANDLE hRecipient,
            _In_ LPCGUID PowerSettingGuid,
            _In_ DWORD Flags
        ) {
            using type = decltype(&::RegisterPowerSettingNotification);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "RegisterPowerSettingNotification");
            if (real)
                return real(hRecipient, PowerSettingGuid, Flags);

            return malloc(1);
        }

        inline BOOL UnregisterPowerSettingNotification(
            _In_ HPOWERNOTIFY Handle
        ) {
            using type = decltype(&::UnregisterPowerSettingNotification);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "UnregisterPowerSettingNotification");
            if (real)
                return real(Handle);

            if (Handle)
                free(Handle);
            return TRUE;
        }

        inline BOOL UpdateLayeredWindowIndirect(
            _In_ HWND hwnd,
            _In_ const UPDATELAYEREDWINDOWINFO *pULWInfo
        ) {
            using type = decltype(&::UpdateLayeredWindowIndirect);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "UpdateLayeredWindowIndirect");
            if (real)
                return real(hwnd, pULWInfo);

            return UpdateLayeredWindow(hwnd, pULWInfo->hdcDst, (POINT *)pULWInfo->pptDst, (SIZE *)pULWInfo->psize, pULWInfo->hdcSrc, (POINT *)pULWInfo->pptSrc, pULWInfo->crKey, (BLENDFUNCTION *)pULWInfo->pblend, pULWInfo->dwFlags);
        }
    }
}

#define DwmEnableBlurBehindWindow Win32Thunk_6_0::DwmApi::DwmEnableBlurBehindWindow
#define DwmGetWindowAttribute     Win32Thunk_6_0::DwmApi::DwmGetWindowAttribute
#define DwmIsCompositionEnabled   Win32Thunk_6_0::DwmApi::DwmIsCompositionEnabled
#define DwmSetWindowAttribute     Win32Thunk_6_0::DwmApi::DwmSetWindowAttribute

#define ConvertInterfaceIndexToLuid Win32Thunk_6_0::IpHlpApi::ConvertInterfaceIndexToLuid
#define ConvertInterfaceLuidToGuid  Win32Thunk_6_0::IpHlpApi::ConvertInterfaceLuidToGuid
#define ConvertInterfaceLuidToIndex Win32Thunk_6_0::IpHlpApi::ConvertInterfaceLuidToIndex
#define ConvertInterfaceLuidToNameW Win32Thunk_6_0::IpHlpApi::ConvertInterfaceLuidToNameW
#define ConvertInterfaceNameToLuidW Win32Thunk_6_0::IpHlpApi::ConvertInterfaceNameToLuidW
#define GetAdaptersAddresses        Win32Thunk_6_0::IpHlpApi::GetAdaptersAddresses

#undef CreateNamedPipe
#define AcquireSRWLockExclusive      Win32Thunk_6_0::Kernel32::AcquireSRWLockExclusive
#define AcquireSRWLockShared         Win32Thunk_6_0::Kernel32::AcquireSRWLockShared
#define CancelIoEx                   Win32Thunk_6_0::Kernel32::CancelIoEx
#define CompareStringEx              Win32Thunk_6_0::Kernel32::CompareStringEx
#define CreateNamedPipe              Win32Thunk_6_0::Kernel32::CreateNamedPipeW
#define GetFileInformationByHandleEx Win32Thunk_6_0::Kernel32::GetFileInformationByHandleEx
#define GetTickCount64               Win32Thunk_6_0::Kernel32::GetTickCount64
#define GetUserPreferredUILanguages  Win32Thunk_6_0::Kernel32::GetUserPreferredUILanguages
#define ReleaseSRWLockExclusive      Win32Thunk_6_0::Kernel32::ReleaseSRWLockExclusive
#define ReleaseSRWLockShared         Win32Thunk_6_0::Kernel32::ReleaseSRWLockShared

#ifndef _UCRT
# define _wgetenv_s Win32Thunk_6_0::Msvcrt::_wgetenv_s
#endif

#define SHCreateItemFromIDList      Win32Thunk_6_0::Shell32::SHCreateItemFromIDList
#define SHCreateItemFromParsingName Win32Thunk_6_0::Shell32::SHCreateItemFromParsingName
#define SHGetKnownFolderIDList      Win32Thunk_6_0::Shell32::SHGetKnownFolderIDList
#define SHGetKnownFolderPath        Win32Thunk_6_0::Shell32::SHGetKnownFolderPath
#define SHGetStockIconInfo          Win32Thunk_6_0::Shell32::SHGetStockIconInfo

#define RegisterPowerSettingNotification   Win32Thunk_6_0::User32::RegisterPowerSettingNotification
#define UnregisterPowerSettingNotification Win32Thunk_6_0::User32::UnregisterPowerSettingNotification
#define UpdateLayeredWindowIndirect        Win32Thunk_6_0::User32::UpdateLayeredWindowIndirect

namespace Win32Thunk_6_1
{
    namespace D3D11
    {
        inline HRESULT D3D11CreateDevice(
            _In_opt_ IDXGIAdapter *pAdapter,
            D3D_DRIVER_TYPE DriverType,
            HMODULE Software,
            UINT Flags,
            _In_opt_ const D3D_FEATURE_LEVEL *pFeatureLevels,
            UINT FeatureLevels,
            UINT SDKVersion,
            _Out_opt_ ID3D11Device **ppDevice,
            _Out_opt_ D3D_FEATURE_LEVEL *pFeatureLevel,
            _Out_opt_ ID3D11DeviceContext **ppImmediateContext
        ) {
            using type = decltype(&::D3D11CreateDevice);
            static HMODULE module = LoadLibraryW(L"d3d11.dll");
            if (module) {
                static type real = (type)GetProcAddress(module, "D3D11CreateDevice");
                if (real)
                    return real(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);
            }

            // the only usage is rhi creating device
            // simply fail
            return E_NOTIMPL;
        }
    }

    namespace Dwrite
    {
        inline HRESULT DWriteCreateFactory(
            _In_ DWRITE_FACTORY_TYPE factoryType,
            _In_ REFIID iid,
            _Out_ IUnknown **factory
        ) {
            using type = decltype(&::DWriteCreateFactory);
            static HMODULE module = LoadLibraryW(L"dwrite.dll");
            if (module) {
                static type real = (type)GetProcAddress(module, "DWriteCreateFactory");
                if (real)
                    return real(factoryType, iid, factory);
            }

            // the only usage is rhi creating device
            // simply fail
            return E_NOTIMPL;
        }
    }

    namespace Kernel32
    {
        inline void RaiseFailFastException(
            _In_opt_ PEXCEPTION_RECORD pExceptionRecord,
            _In_opt_ PCONTEXT pContextRecord,
            _In_ DWORD dwFlags
        ) {
            using type = decltype(&::RaiseFailFastException);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "RaiseFailFastException");
            if (real)
                return real(pExceptionRecord, pContextRecord, dwFlags);

            TerminateProcess(GetCurrentProcess(), pExceptionRecord ? pExceptionRecord->ExceptionCode : STATUS_FAIL_FAST_EXCEPTION);
        }
    }

    namespace Shell32
    {
        inline HRESULT Shell_NotifyIconGetRect(
            _In_ const NOTIFYICONIDENTIFIER *identifier,
            _Out_ RECT *iconLocation
        ) {
            using type = decltype(&::Shell_NotifyIconGetRect);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"shell32.dll"), "Shell_NotifyIconGetRect");
            if (real)
                return real(identifier, iconLocation);

            return E_NOTIMPL;
        }
    }
    
    namespace User32
    {
        inline BOOL ChangeWindowMessageFilterEx(
            _In_ HWND hWnd,
            _In_ UINT message,
            _In_ DWORD action,
            _Inout_opt_ PCHANGEFILTERSTRUCT pChangeFilterStruct
        ) {
            using type = decltype(&::ChangeWindowMessageFilterEx);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "ChangeWindowMessageFilterEx");
            if (real)
                return real(hWnd, message, action, pChangeFilterStruct);

            if (pChangeFilterStruct)
                pChangeFilterStruct->ExtStatus = 0;
            return TRUE;
        }

        inline BOOL CloseTouchInputHandle(
            _In_ HTOUCHINPUT hTouchInput
        ) {
            using type = decltype(&::CloseTouchInputHandle);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "CloseTouchInputHandle");
            if (real)
                return real(hTouchInput);

            SetLastError(ERROR_INVALID_HANDLE);
            return FALSE;
        }

        inline BOOL GetTouchInputInfo(
            _In_ HTOUCHINPUT hTouchInput,
            _In_ UINT cInputs,
            _Out_ PTOUCHINPUT pInputs,
            _In_ int cbSize
        ) {
            using type = decltype(&::GetTouchInputInfo);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "GetTouchInputInfo");
            if (real)
                return real(hTouchInput, cInputs, pInputs, cbSize);

            SetLastError(ERROR_INVALID_HANDLE);
            return FALSE;
        }

        inline BOOL IsTouchWindow(
            _In_ HWND hwnd,
            _Out_opt_ PULONG pulFlags
        ) {
            using type = decltype(&::IsTouchWindow);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "IsTouchWindow");
            if (real)
                return real(hwnd, pulFlags);

            if (pulFlags)
                *pulFlags = 0;
            return FALSE;
        }

        inline BOOL RegisterTouchWindow(
            _In_ HWND hwnd,
            _In_ ULONG ulFlags
        ) {
            using type = decltype(&::RegisterTouchWindow);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "RegisterTouchWindow");
            if (real)
                return real(hwnd, ulFlags);

            return TRUE;
        }

        inline BOOL UnregisterTouchWindow(
            _In_ HWND hWnd
        ) {
            using type = decltype(&::UnregisterTouchWindow);
            static type real = (type)GetProcAddress(GetModuleHandleW(L"user32.dll"), "UnregisterTouchWindow");
            if (real)
                return real(hWnd);

            return TRUE;
        }
    }
}

#define D3D11CreateDevice Win32Thunk_6_1::D3D11::D3D11CreateDevice

#define DWriteCreateFactory Win32Thunk_6_1::Dwrite::DWriteCreateFactory

#define RaiseFailFastException Win32Thunk_6_1::Kernel32::RaiseFailFastException

#define Shell_NotifyIconGetRect Win32Thunk_6_1::Shell32::Shell_NotifyIconGetRect

#define ChangeWindowMessageFilterEx Win32Thunk_6_1::User32::ChangeWindowMessageFilterEx
#define CloseTouchInputHandle       Win32Thunk_6_1::User32::CloseTouchInputHandle
#define GetTouchInputInfo           Win32Thunk_6_1::User32::GetTouchInputInfo
#define IsTouchWindow               Win32Thunk_6_1::User32::IsTouchWindow
#define RegisterTouchWindow         Win32Thunk_6_1::User32::RegisterTouchWindow
#define UnregisterTouchWindow       Win32Thunk_6_1::User32::UnregisterTouchWindow

namespace Win32Thunk_6_2
{
    namespace Combase
    {
        inline HRESULT RoGetActivationFactory(
            _In_ HSTRING activatableClassId,
            _In_ REFIID iid,
            _Out_ void **factory
        ) {
            using type = decltype(&::RoGetActivationFactory);
            static HMODULE module = LoadLibraryW(L"combase.dll");
            if (module) {
                static type real = (type)GetProcAddress(GetModuleHandleW(L"combase.dll"), "RoGetActivationFactory");
                if (real)
                    return real(activatableClassId, iid, factory);
            }

            // the only usage is to detect tablet mode
            // simply fail
            return E_NOTIMPL;
        }

        inline HRESULT WindowsCreateStringReference(
            PCWSTR sourceString,
            UINT32 length,
            HSTRING_HEADER *hstringHeader,
            HSTRING *string
        ) {
            using type = decltype(&::WindowsCreateStringReference);
            static HMODULE module = LoadLibraryW(L"combase.dll");
            if (module) {
                static type real = (type)GetProcAddress(module, "WindowsCreateStringReference");
                if (real)
                    return real(sourceString, length, hstringHeader, string);
            }

            // the only usage is to detect tablet mode
            // simply fail
            return E_NOTIMPL;
        }
    }
}

#define RoGetActivationFactory       Win32Thunk_6_2::Combase::RoGetActivationFactory
#define WindowsCreateStringReference Win32Thunk_6_2::Combase::WindowsCreateStringReference

namespace Win32Thunk_6_3
{
    namespace Dxgi
    {
        inline HRESULT CreateDXGIFactory2(
            UINT Flags,
            REFIID riid,
            _Out_ void **ppFactory
        ) {
            using type = decltype(&::CreateDXGIFactory2);
            static HMODULE module = LoadLibraryW(L"dxgi.dll");
            if (module) {
                static type real = (type)GetProcAddress(module, "CreateDXGIFactory2");
                if (real)
                    return real(Flags, riid, ppFactory);
            }

            // the only usage is rhi creating device
            // simply fail
            return E_NOTIMPL;
        }
    }

    namespace Shcore
    {
        namespace Detail
        {
            inline int GetDpiFromDeviceCaps()
            {
                HDC hdc = GetDC(nullptr);
                int dpi = 96;
                if (hdc)
                    dpi = GetDeviceCaps(hdc, LOGPIXELSX);
                ReleaseDC(nullptr, hdc);
                return dpi;
            }
        }

        inline HRESULT GetDpiForMonitor(
            _In_ HMONITOR hmonitor,
            _In_ MONITOR_DPI_TYPE dpiType,
            _Out_ UINT *dpiX,
            _Out_ UINT *dpiY
        ) {
            using type = decltype(&::GetDpiForMonitor);
            static HMODULE module = LoadLibraryW(L"shcore.dll");
            if (module) {
                static type real = (type)GetProcAddress(module, "GetDpiForMonitor");
                if (real)
                    return real(hmonitor, dpiType, dpiX, dpiY);
            }

            static int dpi = Detail::GetDpiFromDeviceCaps();
            if (!dpiX || !dpiY)
                return E_INVALIDARG;
            *dpiX = dpi;
            *dpiY = dpi;
            return S_OK;
        }
    }
}

#define CreateDXGIFactory2 Win32Thunk_6_3::Dxgi::CreateDXGIFactory2

#define GetDpiForMonitor Win32Thunk_6_3::Shcore::GetDpiForMonitor

namespace Win32Thunk_10_0
{
    namespace D3D12
    {
        inline HRESULT D3D12CreateDevice(
            _In_opt_ IUnknown *pAdapter,
            D3D_FEATURE_LEVEL MinimumFeatureLevel,
            _In_ REFIID riid,
            _Out_opt_ void **ppDevice
        ) {
            using type = decltype(&::D3D12CreateDevice);
            static HMODULE module = LoadLibraryW(L"d3d12.dll");
            if (module) {
                static type real = (type)GetProcAddress(module, "D3D12CreateDevice");
                if (real)
                    return real(pAdapter, MinimumFeatureLevel, riid, ppDevice);
            }

            // the only usage is rhi creating device
            // simply fail
            return E_NOTIMPL;
        }

        inline HRESULT D3D12GetDebugInterface(
            _In_ REFIID riid,
            _Out_opt_ void **ppvDebug
        ) {
            using type = decltype(&::D3D12GetDebugInterface);
            static HMODULE module = LoadLibraryW(L"d3d12.dll");
            if (module) {
                static type real = (type)GetProcAddress(module, "D3D12GetDebugInterface");
                if (real)
                    return real(riid, ppvDebug);
            }

            // guarded by `D3D12CreateDevice`
            // simply fail
            return E_NOTIMPL;
        }

        inline HRESULT D3D12SerializeVersionedRootSignature(
            _In_ const D3D12_VERSIONED_ROOT_SIGNATURE_DESC *pRootSignatureDesc,
            _Out_ ID3DBlob **ppBlob,
            _Out_opt_ ID3DBlob **ppErrorBlob
        ) {
            using type = decltype(&::D3D12SerializeVersionedRootSignature);
            static HMODULE module = LoadLibraryW(L"d3d12.dll");
            if (module) {
                static type real = (type)GetProcAddress(module, "D3D12SerializeVersionedRootSignature");
                if (real)
                    return real(pRootSignatureDesc, ppBlob, ppErrorBlob);
            }

            // guarded by `D3D12CreateDevice`
            // simply fail
            return E_NOTIMPL;
        }
    }
}

#define D3D12CreateDevice                    Win32Thunk_10_0::D3D12::D3D12CreateDevice
#define D3D12GetDebugInterface               Win32Thunk_10_0::D3D12::D3D12GetDebugInterface
#define D3D12SerializeVersionedRootSignature Win32Thunk_10_0::D3D12::D3D12SerializeVersionedRootSignature

#endif // __aarch64

#pragma GCC diagnostic warning "-Wcast-function-type"

#endif // QT_WINDOWS_H
