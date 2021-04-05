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

#include <memory>

namespace reth::simulation::windows_shutdown::win32::api
{
class processthreadsapi;
class psapi;
class tlhelp32;
class winuser;
}

using namespace reth::simulation::windows_shutdown::win32::api;

namespace reth::simulation::windows_shutdown::win32
{
class windows_api
{
    public:
        
        static auto setup() -> void;
        static auto setup(  std::unique_ptr<const processthreadsapi> processthreadsapi,
                            std::unique_ptr<const psapi> psapi,
                            std::unique_ptr<const tlhelp32> tlhelp32,
                            std::unique_ptr<const winuser> winuser ) -> void;

        static auto cleanup() -> void;

        static auto get_processthreadsapi() -> const processthreadsapi&;
        static auto get_psapi() -> const psapi&;
        static auto get_tlhelp32() -> const tlhelp32&;
        static auto get_winuser() -> const winuser&;

    private:
        
        static std::unique_ptr<const processthreadsapi> _processthreadsapi;
        static std::unique_ptr<const psapi> _psapi;
        static std::unique_ptr<const tlhelp32> _tlhelp32;
        static std::unique_ptr<const winuser> _winuser;

        windows_api() = delete;
        windows_api( const windows_api& ) = delete;
        windows_api( windows_api&& ) = delete;

        auto operator=( const windows_api& ) -> windows_api& = delete;
        auto operator=( windows_api&& ) -> windows_api&& = delete;
};
}