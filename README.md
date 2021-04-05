# WinShut
This is a simple Windows shutdown simulation library. It can be used to find running processes by a given name. For each found process containing windows can be listed by a window text pattern. Methods are provided to send WM_QUERYENDSESSION/WM_ENDSESSION to the according window. That way the shutdown of a windows system is simulated to an application. This is helpful to debug code for shutdown purposes without actually shutting down Windows itself.

Maybe there are better ways or tools available to achieve that. But after some time I was longing for good old C++ code :grin:.

I compiled and linked it with Visual Studio 2019 and used C++17 as language standard.

# Example
The example below shows how to use WinShut library. It assumes a running Windows Forms application called 'WinFormsApp1.exe'. Within that application a window with the text pattern '.NET-BroadcastEventWindow' is searched for. For each found window -probably not more than one- a WM_QUERYENDSESSION message is sent. If the result of this message is `true`, a WM_ENDSESSION is sent afterwards.

All Windows API calls are hidden behind interfaces to increase testability of the library classes.

```C++
#include <algorithm>
#include <future>
#include <iostream>
#include <memory>

#include "./find_process_error.h"
#include "./process.h"
#include "./win32/windows_exception.h"
#include "./win32/windows_api.h"

using namespace reth::simulation::windows_shutdown;
using namespace reth::simulation::windows_shutdown::win32;

int main()
{
    windows_api::setup();

    try
    {
        std::future<std::tuple<std::vector<process>, std::vector<find_process_error>>> find_result{ std::async( std::launch::async,
                                                                                                                process::find_processes,
                                                                                                                TEXT( "WinFormsApp1.exe" ) ) };

        auto [ processes, errors ] = find_result.get();

        for( const auto& process:processes )
        {
            std::vector<window> windows{ process.find_windows( std::wregex( TEXT( "^\\.NET-BroadcastEventWindow\\." ), std::regex_constants::ECMAScript ) ) };

            std::for_each(  windows.cbegin(),
                            windows.cend(),
                            []( const window& window )
                            {
                                std::wcout << window.get_text() << std::endl;

                                bool is_session_ending{ window.query_end_session( session_end_reasons::logoff ) };

                                if( is_session_ending == true )
                                {
                                    window.end_session( session_end_reasons::logoff, true );
                                }
                            }   );
        }
    }catch( const windows_exception& exception )
    {
        std::cout << "Error occurred: ";
        std::cout << exception.what() << std::endl;
    }

    windows_api::cleanup();

    return 0;
}
```
