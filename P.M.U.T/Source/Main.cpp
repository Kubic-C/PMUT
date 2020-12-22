#include "Headers/pmut.h"
#undef GLOBAL
#define GLOBAL

// GLOBAL FILE VARS
namespace pmut
{
	GLOBAL std::mutex* _main_lock                 = nullptr;
	GLOBAL std::condition_variable _fast_main_cv           ;
	GLOBAL std::condition_variable _wait_for_input         ;

	namespace flags
	{
		GLOBAL std::atomic<bool> waiting            = false;
		GLOBAL std::atomic<bool> main_loop_unlocked = false;
		GLOBAL std::atomic<bool> done_starting      = true ;
		GLOBAL std::atomic<bool> update_settings    = false;
		GLOBAL std::atomic<bool> need_user_input    = false;
		GLOBAL bool exit_app = false;
	}

	namespace settings
	{
		namespace opengl
		{
			GLOBAL int swap_interval = 2;
		}
	}

	namespace commands
	{
		GLOBAL std::unordered_map<std::string, command> commands_map;
		GLOBAL std::thread* active_command     = nullptr;
		GLOBAL std::string active_command_name = "";
		GLOBAL bool done                       = false;
	}

	namespace input
	{
		GLOBAL bool pressed_v = false;
		GLOBAL bool pressed_c = false;
		GLOBAL bool         alt_mode			= false;
		GLOBAL std::string* active_input_buffer	= nullptr;
		GLOBAL std::string last_input_buffer    = "";
	}
}

int main()
{ 
#ifdef RELEASE_
#ifdef _WINDOWS_
	ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif
#elif defined(_LINUX_)
	// do something to hide the linux console window 
#endif

	pmut::app pmut(PMUT_NAME, 1000, 1000);
	int exit_code = pmut.run();
	pmut.cleanup();
	return exit_code;
}