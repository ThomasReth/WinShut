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

#include "./winuser_impl.h"

namespace reth::simulation::windows_shutdown::win32::api
{
auto winuser_impl::EnumThreadWindows(   DWORD dwThreadId,
                                        WNDENUMPROC lpfn,
                                        LPARAM lParam   )const -> BOOL
{
    return ::EnumThreadWindows( dwThreadId, lpfn, lParam );
}

auto winuser_impl::GetWindowTextLength( HWND hWnd )const -> int
{
    return ::GetWindowTextLength( hWnd );
}

auto winuser_impl::GetWindowText( HWND hWnd, LPWSTR lpString, int nMaxCount )const -> int
{
    return ::GetWindowText( hWnd, lpString, nMaxCount );
}
}