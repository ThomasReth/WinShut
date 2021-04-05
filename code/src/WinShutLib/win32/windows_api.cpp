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

#include "./windows_api.h"

#include "./api/processthreadsapi_impl.h"
#include "./api/psapi_impl.h"
#include "./api/tlhelp32_impl.h"
#include "./api/winuser_impl.h"

#include <memory>

namespace reth::simulation::windows_shutdown::win32
{
std::unique_ptr<const processthreadsapi> windows_api::_processthreadsapi{ std::unique_ptr<const processthreadsapi>{ nullptr } };
std::unique_ptr<const psapi> windows_api::_psapi{ std::unique_ptr<const psapi>{ nullptr } };
std::unique_ptr<const tlhelp32> windows_api::_tlhelp32{ std::unique_ptr<const tlhelp32>{ nullptr } };
std::unique_ptr<const winuser> windows_api::_winuser{ std::unique_ptr<const winuser>{ nullptr } };

auto windows_api::setup() -> void
{
    windows_api::_processthreadsapi = std::make_unique<const processthreadsapi_impl>();
    windows_api::_psapi = std::make_unique<const psapi_impl>();
    windows_api::_tlhelp32 = std::make_unique<const tlhelp32_impl>();
    windows_api::_winuser = std::make_unique<const winuser_impl>();
}

auto windows_api::setup(    std::unique_ptr<const processthreadsapi> processthreadsapi,
                            std::unique_ptr<const psapi> psapi,
                            std::unique_ptr<const tlhelp32> tlhelp32,
                            std::unique_ptr<const winuser> winuser  ) -> void
{
    windows_api::_processthreadsapi = std::move( processthreadsapi );
    windows_api::_psapi = std::move( psapi );
    windows_api::_tlhelp32 = std::move( tlhelp32 );
    windows_api::_winuser = std::move( winuser );
}

auto windows_api::cleanup() -> void
{
    windows_api::_psapi = nullptr;
    windows_api::_tlhelp32 = nullptr;
    windows_api::_winuser = nullptr;
}

[[nodiscard]]
auto windows_api::get_processthreadsapi() -> const processthreadsapi&
{
    return *( windows_api::_processthreadsapi );
}

[[nodiscard]]
auto windows_api::get_psapi() -> const psapi&
{
    return *( windows_api::_psapi );
}

[[nodiscard]]
auto windows_api::get_tlhelp32() -> const tlhelp32&
{
    return *( windows_api::_tlhelp32 );
}

[[nodiscard]]
auto windows_api::get_winuser() -> const winuser&
{
    return *( windows_api::_winuser );
}
}