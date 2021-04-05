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

#include <string>
#include <Windows.h>

#include "./session_end_reasons.h"

namespace reth::simulation::windows_shutdown
{
class window
{
    public:

        explicit window( HWND handle );

        virtual ~window() = default;

        auto get_handle()const -> HWND;
        auto get_text()const -> std::wstring;

        auto query_end_session( session_end_reasons reasons )const -> bool;
        auto end_session( session_end_reasons reasons, bool is_session_ending )const -> LRESULT;

    private:

        HWND _handle{ nullptr };
};
}