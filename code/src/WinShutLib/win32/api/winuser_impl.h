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

#include "./winuser.h"

namespace reth::simulation::windows_shutdown::win32::api
{
class winuser_impl:public winuser
{
    public:

        auto EnumThreadWindows( DWORD dwThreadId,
                                WNDENUMPROC lpfn,
                                LPARAM lParam   )const -> BOOL override;

        auto GetWindowTextLength( HWND hWnd )const -> int override;

        auto GetWindowText( HWND hWnd, LPWSTR lpString, int nMaxCount )const -> int override;
};
}