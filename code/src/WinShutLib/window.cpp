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

#include "./window.h"

#include <Windows.h>

#include "./win32/windows_api.h"
#include "./win32/api/winuser.h"

using namespace reth::simulation::windows_shutdown::win32;

namespace reth::simulation::windows_shutdown
{
window::window( HWND handle )
:
    _handle{ handle }
{
}

[[nodiscard]]
auto window::get_handle()const -> HWND
{
    return this->_handle;
}

[[nodiscard]]
auto window::get_text()const -> std::wstring
{
    std::wstring result;

    int length = windows_api::get_winuser().GetWindowTextLength( this->_handle ) + 1;

    result.resize( length );

    windows_api::get_winuser().GetWindowText(   this->_handle,
                                                &result[ 0 ],
                                                length  );

    result.resize( length - 1 );

    return result;
}

auto window::query_end_session( session_end_reasons reasons )const -> bool
{
    return static_cast<bool>(   ::SendMessage(  this->_handle,
                                                WM_QUERYENDSESSION,
                                                0,
                                                static_cast<LPARAM>( reasons ) )   );
}

auto window::end_session( session_end_reasons reasons, bool is_session_ending )const -> LRESULT
{
    return static_cast<bool>(   ::SendMessage(  this->_handle,
                                                WM_ENDSESSION,
                                                static_cast<WPARAM>( is_session_ending ),
                                                static_cast<LPARAM>( reasons ) )   );
}
}