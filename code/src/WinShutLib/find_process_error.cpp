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

#include "./find_process_error.h"

namespace reth::simulation::windows_shutdown
{
find_process_error::find_process_error( DWORD id, const std::wstring& description )
:
    _id{ id }, _description{ description }
{
}

auto find_process_error::operator==( const find_process_error& other )const -> bool
{
    return  ( this->_id == other._id ) &&
            ( this->_description == other._description );
}

auto find_process_error::operator!=( const find_process_error& other )const -> bool
{
    return !( *this == other );
}

[[nodiscard]]
auto find_process_error::get_id()const -> DWORD
{
    return this->_id;
}

[[nodiscard]]
auto find_process_error::get_description()const -> std::wstring
{
    return this->_description;
}
}