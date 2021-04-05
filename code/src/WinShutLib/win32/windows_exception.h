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

#include <exception>
#include <string>
#include <Windows.h>

namespace reth::simulation::windows_shutdown::win32
{
class windows_exception:public std::exception
{
    public:
        
        static auto format_message( DWORD error_code ) -> std::wstring;

        explicit windows_exception( const std::string& what )noexcept;
        explicit windows_exception( const std::string& what, DWORD error_code )noexcept;

        ~windows_exception()override = default;

        auto get_error_code()const -> DWORD;
        auto get_error_message()const -> std::wstring;

        auto what()const noexcept -> const char* override;

    private:

        DWORD _error_code;

        std::string _what;
        std::wstring _error_message;
};
}