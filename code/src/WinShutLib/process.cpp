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

#include "./process.h"

#include <TlHelp32.h>
#include <Psapi.h>
#include <algorithm>
#include <execution>
#include <mutex>

#include "./find_process_error.h"
#include "./win32/close_handle.h"
#include "./win32/windows_api.h"
#include "./win32/windows_exception.h"
#include "./win32/api/processthreadsapi.h"
#include "./win32/api/psapi.h"
#include "./win32/api/tlhelp32.h"
#include "./win32/api/winuser.h"

using namespace reth::simulation::windows_shutdown::win32;
using namespace reth::simulation::windows_shutdown::win32::api;

namespace reth::simulation::windows_shutdown
{
auto process::find_processes( const std::wstring_view& file_name ) -> std::tuple<std::vector<process>, std::vector<find_process_error>>
{
    std::vector<process> processes;
    std::vector<find_process_error> errors;

    std::vector<DWORD> process_ids{ process::get_process_ids() };

    std::mutex sync_processes;
    std::mutex sync_errors;

    std::for_each(  std::execution::par,
                    process_ids.begin(),
                    process_ids.end(),
                    [ & ]( DWORD process_id )
                    {
                        try
                        {
                            std::optional<process> process{ process::get_process( process_id, file_name ) };

                            if( process.has_value() == true )
                            {
                                std::lock_guard guard{ sync_processes };

                                processes.push_back( std::move( *process ) );
                            }
                        }catch( const windows_exception& exception )
                        {
                            std::lock_guard guard{ sync_errors };

                            errors.emplace_back( process_id, exception.get_error_message() );
                        }
                    }   );

    return std::make_tuple( std::move( processes ), std::move( errors ) );
}

auto process::get_process( DWORD process_id, const std::wstring_view& file_name ) -> std::optional<process>
{
    if( process_id != process::INVALID_PROCESS_ID )
    {
        auto [ process_handle, module_handle ] = process::get_module_info( process_id );

        std::wstring current_file_name{ process::get_process_file_name( process_handle.get(), module_handle ) };

        int comparison{ file_name.compare( current_file_name ) };

        if( comparison == 0 )
        {
            std::wstring current_file_path{ process::get_process_file_path( process_handle.get(), module_handle ) };

            return process{ process_id, current_file_name, current_file_path };
        }
    }

    return {};
}

auto process::get_process_ids() -> std::vector<DWORD>
{
    std::vector<DWORD> result( process::PROCESS_ID_BUFFER_DEFAULT_SIZE );

    DWORD* buffer{ result.data() };
    DWORD buffer_size{ static_cast<DWORD>( result.size() * sizeof( DWORD ) ) };
    
    if( DWORD bytes_needed{ 0 }; windows_api::get_psapi().EnumProcesses( buffer, buffer_size, &bytes_needed ) == FALSE )
    {
        throw windows_exception( "Enumeration of processes failed." );
    }

    return result;
}

auto process::get_thread_window_handles( DWORD thread_id ) -> std::vector<HWND>
{
    std::vector<HWND> result;

    windows_api::get_winuser().EnumThreadWindows(   thread_id,
                                                    []( HWND hwnd, LPARAM lparam )
                                                    {
                                                        auto handles = reinterpret_cast<std::vector<HWND>*>( lparam );

                                                        handles->push_back( hwnd );

                                                        return TRUE;
                                                    },
                                                    reinterpret_cast<LPARAM>( &result )   );

    return result;
}

auto process::get_module_info( DWORD process_id ) -> std::tuple<std::unique_ptr<HANDLE, close_handle>, HMODULE>
{
    std::unique_ptr<HANDLE, close_handle> process_handle{   windows_api::get_processthreadsapi().OpenProcess(   PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                                                                                                FALSE,
                                                                                                                process_id  )   };

    HMODULE module_handle{ nullptr };

    if( process_handle != nullptr )
    {
        DWORD bytes_needed{ 0 };

        if( windows_api::get_psapi().EnumProcessModulesEx(  process_handle.get(),
                                                            &module_handle,
                                                            sizeof( &module_handle ),
                                                            &bytes_needed,
                                                            LIST_MODULES_DEFAULT    ) == 0 )
        {
            throw windows_exception( "Enumeration of process modules failed." );
        }
    }else
    {
        throw windows_exception( "Failed to open process." );
    }

    return std::make_tuple( std::move( process_handle ), module_handle );
}

auto process::get_process_file_name( HANDLE process_handle, HMODULE module_handle ) -> std::wstring
{
    std::wstring result;

    result.resize( MAX_PATH );
    
    if( windows_api::get_psapi().GetModuleBaseName( process_handle,
                                                    module_handle,
                                                    &result[ 0 ],
                                                    static_cast<DWORD>( result.length() )   ) == 0 )
    {
        throw windows_exception( "Failed to retrieve process file name." );
    }

    result.resize( ::wcslen( result.c_str() ) );
    result.shrink_to_fit();

    return result;
}

auto process::get_process_file_path( HANDLE process_handle, HMODULE module_handle ) -> std::wstring
{
    std::wstring result;

    result.resize( MAX_PATH );

    if( windows_api::get_psapi().GetModuleFileNameEx(   process_handle,
                                                        module_handle,
                                                        &result[ 0 ],
                                                        static_cast<DWORD>( result.length() )   ) == 0 )
    {
        throw windows_exception( "Failed to retrieve process file path." );
    }

    result.resize( ::wcslen( result.c_str() ) );
    result.shrink_to_fit();

    return result;
}

process::process(   DWORD id,
                    const std::wstring& file_name,
                    const std::wstring& file_path   )
:
    _id{ id }, _file_name{ file_name }, _file_path{ file_path }
{
}

process::process( process&& other )noexcept
:
    _id{ other._id },
    _file_name{ std::move( other._file_name ) },
    _file_path{ std::move( other._file_path ) }
{
}

auto process::operator=( process&& other )noexcept -> process&
{
    this->_id = other._id;
    this->_file_name = std::move( other._file_name );
    this->_file_path = std::move( other._file_path );

    return *this;
}

auto process::operator==( const process& other )const -> bool
{
    return  ( this->_id == other._id ) &&
            ( this->_file_name == other._file_name ) &&
            ( this->_file_path == other._file_path );
}

auto process::operator!=( const process& other )const -> bool
{
    return !( *this == other );
}

[[nodiscard]]
auto process::get_id()const -> DWORD
{
    return this->_id;
}

[[nodiscard]]
auto process::get_file_name()const -> std::wstring
{
    return this->_file_name;
}

[[nodiscard]]
auto process::get_file_path()const -> std::wstring
{
    return this->_file_path;
}

[[nodiscard]]
auto process::find_windows( const std::wregex& pattern )const -> std::vector<window>
{
    std::vector<window> result;

    std::vector<DWORD> thread_ids{ this->get_thread_ids() };

    for( DWORD thread_id:thread_ids )
    {
        std::vector<HWND> window_handles{ process::get_thread_window_handles( thread_id ) };

        std::for_each(  window_handles.cbegin(),
                        window_handles.cend(),
                        [ &pattern, &result ]( HWND handle )
                        {
                            window window{ handle };

                            std::wstring window_text{ window.get_text() };

                            if( std::regex_search( window_text, pattern ) == true )
                            {
                                result.push_back( window );
                            }
                        }   );
    }

    return result;
}

auto process::get_thread_ids()const -> std::vector<DWORD>
{
    std::vector<DWORD> result;

    const tlhelp32& toolhelp = windows_api::get_tlhelp32();
    
    std::unique_ptr<HANDLE, close_handle> snapshot( toolhelp.CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 ) );

    if( snapshot.get() == INVALID_HANDLE_VALUE )
    {       
        throw windows_exception( "Failed to created thread snapshot." );
    }

    THREADENTRY32 thread_entry;

    thread_entry.dwSize = sizeof( thread_entry );

    if( toolhelp.Thread32First( snapshot.get(), &thread_entry ) == TRUE )
    {
        do
        {
            if( thread_entry.dwSize >= FIELD_OFFSET( THREADENTRY32, th32OwnerProcessID ) + sizeof( thread_entry.th32OwnerProcessID ) &&
                this->_id == thread_entry.th32OwnerProcessID    )
            {
                result.push_back( thread_entry.th32ThreadID );   
            }
            
            thread_entry.dwSize = sizeof( thread_entry );
        }while( toolhelp.Thread32Next( snapshot.get(), &thread_entry ) );
    }else
    {
        throw windows_exception( "Failed to retrieve thread information from snapshot." );
    }

    return result;
}
}