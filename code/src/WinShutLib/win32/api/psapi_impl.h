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

#pragma once

#include "./psapi.h"

namespace reth::simulation::windows_shutdown::win32::api
{
class psapi_impl:public psapi
{
    public:

        auto EnumProcesses( DWORD* lpidProcess,
                            DWORD cb,  
                            LPDWORD lpcbNeeded  )const -> BOOL override;

        auto EnumProcessModulesEx(  HANDLE hProcess,
                                    HMODULE* lphModule,
                                    DWORD cb,
                                    LPDWORD lpcbNeeded,
                                    DWORD dwFilterFlag  )const -> BOOL override;

        auto GetModuleBaseName( HANDLE hProcess,
                                HMODULE hModule,
                                LPWSTR lpBaseName,
                                DWORD nSize )const -> DWORD override;

        auto GetModuleFileNameEx(   HANDLE hProcess,
                                    HMODULE hModule,
                                    LPWSTR lpFilename,
                                    DWORD nSize )const -> DWORD override;
};
}