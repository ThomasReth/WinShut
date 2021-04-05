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
#include <optional>
#include <regex>
#include <string>
#include <tuple>
#include <vector>
#include <Windows.h>

#include "./find_process_error.h"
#include "./window.h"
#include "./win32/close_handle.h"

using namespace reth::simulation::windows_shutdown::win32;

namespace reth::simulation::windows_shutdown
{
class process
{
    public:

        static auto find_processes( const std::wstring_view& file_name ) -> std::tuple<std::vector<process>, std::vector<find_process_error>>;

        process(    DWORD id,
                    const std::wstring& file_name,
                    const std::wstring& file_path   );

        process( const process& other ) = default;
        process( process&& other )noexcept;

        virtual ~process() = default;

        auto operator=( const process& other ) -> process& = default;
        auto operator=( process&& other )noexcept -> process&;

        auto operator ==( const process& other )const -> bool;
        auto operator !=( const process& other )const -> bool;

        auto get_id()const -> DWORD;
        auto get_file_name()const -> std::wstring;
        auto get_file_path()const -> std::wstring;

        auto find_windows( const std::wregex& pattern )const -> std::vector<window>;

    private:

        static auto get_process( DWORD process_id, const std::wstring_view& file_name ) -> std::optional<process>;
        static auto get_process_ids() -> std::vector<DWORD>;
        static auto get_thread_window_handles( DWORD thread_id ) -> std::vector<HWND>;
        static auto get_module_info( DWORD process_id ) -> std::tuple<std::unique_ptr<HANDLE, close_handle>, HMODULE>;
        static auto get_process_file_name( HANDLE process_handle, HMODULE module_handle ) -> std::wstring;
        static auto get_process_file_path( HANDLE process_handle, HMODULE module_handle ) -> std::wstring;       

        auto get_thread_ids()const -> std::vector<DWORD>;

        static constexpr size_t PROCESS_ID_BUFFER_DEFAULT_SIZE{ 1024 };
        static constexpr DWORD INVALID_PROCESS_ID{ 0 };

        DWORD _id;
        
        std::wstring _file_name;
        std::wstring _file_path;
};
}