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

#include "./windows_exception.h"

#include <string>

namespace reth::simulation::windows_shutdown::win32
{
auto windows_exception::format_message( DWORD error_code ) -> std::wstring
{
    LPWSTR buffer{ nullptr };
    
    DWORD minimum_size{ 256 };

    DWORD stored_characters{    ::FormatMessage(    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                                                    nullptr,
                                                    error_code,
                                                    MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
                                                    ( LPWSTR )&buffer,
                                                    minimum_size,
                                                    nullptr )   };

    std::wstring result;

    if( stored_characters != 0 )
    {
        result = buffer;
    }    

    ::LocalFree( buffer );

    return result;
}

windows_exception::windows_exception( const std::string& what )noexcept
:
    _what{ what }
{
    this->_error_code = ::GetLastError();
    this->_error_message = windows_exception::format_message( this->_error_code );
}

windows_exception::windows_exception( const std::string& what, DWORD error_code )noexcept
:
    _what{ what },
    _error_code{ error_code }
{
    this->_error_message = windows_exception::format_message( this->_error_code );
}

[[nodiscard]]
auto windows_exception::get_error_code()const -> DWORD
{
    return this->_error_code;
}

[[nodiscard]]
auto windows_exception::get_error_message()const -> std::wstring
{
    return this->_error_message;
}

auto windows_exception::what()const noexcept -> const char*
{
    return this->_what.c_str();
}
}