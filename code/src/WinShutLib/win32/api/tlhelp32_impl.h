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

#include "./tlhelp32.h"

namespace reth::simulation::windows_shutdown::win32::api
{
class tlhelp32_impl:public tlhelp32
{
    public:

        auto CreateToolhelp32Snapshot( DWORD dwFlags, DWORD th32ProcessID )const -> HANDLE override;
        
        auto Thread32First( HANDLE hSnapshot, LPTHREADENTRY32 lpte )const -> BOOL override;
        auto Thread32Next( HANDLE hSnapshot, LPTHREADENTRY32 lpte )const -> BOOL override;
};
}