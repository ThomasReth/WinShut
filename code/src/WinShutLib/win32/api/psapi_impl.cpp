// Copyright (C) 2021  Thomas Reth

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.

// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "./psapi_impl.h"

namespace reth::simulation::windows_shutdown::win32::api
{
auto psapi_impl::EnumProcesses( DWORD* lpidProcess,
                                DWORD cb,  
                                LPDWORD lpcbNeeded  )const -> BOOL
{
    return ::EnumProcesses( lpidProcess, cb, lpcbNeeded );
}

auto psapi_impl::EnumProcessModulesEx(  HANDLE hProcess,
                                        HMODULE* lphModule,
                                        DWORD cb,
                                        LPDWORD lpcbNeeded,
                                        DWORD dwFilterFlag  )const -> BOOL
{
    return ::EnumProcessModulesEx( hProcess, lphModule, cb, lpcbNeeded, dwFilterFlag );
}

auto psapi_impl::GetModuleBaseName( HANDLE hProcess,
                                    HMODULE hModule,
                                    LPWSTR lpBaseName,
                                    DWORD nSize )const -> DWORD
{
    return ::GetModuleBaseName( hProcess, hModule, lpBaseName, nSize );
}

auto psapi_impl::GetModuleFileNameEx(   HANDLE hProcess,
                                        HMODULE hModule,
                                        LPWSTR lpFilename,
                                        DWORD nSize )const -> DWORD
{
    return ::GetModuleFileNameEx( hProcess, hModule, lpFilename, nSize );
}
}