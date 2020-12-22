#ifndef PMUT_BASE_H
#define PMUT_BASE_H

#define PLATFORM_WINDOWS

#ifdef PLATFORM_WINDOWS
#pragma warning( push )
#pragma warning( disable : 4005 )
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#pragma warning( pop )
#endif 

#ifdef PLATFORM_LINUX
#pragma warning( push )
#pragma warning( disable : 4005 )
// include some linux librarys
#pragma warning( pop )
#endif

// disable "enum class error" 
#pragma warning( push )
#pragma warning( disable : 26812 )

//#include "OGLHeaders/Include.h"; no reason to include this since it is already included in Network base.h
#include "Network\Headers\base.h"

// std librarys
#include <chrono>

#define PMUT_NAME "Porter Multi Use Terminal [PMUT] | BUILD(" + std::string(__DATE__) + " " + std::string(__TIME__) + ") | Creator : Sawyer Porter"

#define PMUT_INFO "Most if not all commands are in the Help.txt file\n"\
				  "if you are intrested in joining PMUT's discord server\n"\
				  "it should also be included in the Help.txt file. this\n"\
				  "build is in https://github.com/Kubic-C/PMUT_BUILDS."

#define COMMAND_PARAMS std::vector<std::pair<std::string, std::string>> options

#define PMUT        std::string("[PMUT]")
#define PMUT_TIMER  std::string("[PMUT::TIMER]")
#define PMUT_ENGINE std::string("[PMUT::ENGINE]")

#define ONE_DEF static

namespace console
{
	namespace vars
	{
		GLOBAL console::console_master* pmut_console;
	}
}

// misc_
namespace pmut
{
	typedef long double timer_byte;
	typedef std::chrono::high_resolution_clock clock;

	class timer
	{	
	public:
		timer() 
			: duration() {}

		void reset_frametimes();
		void start_timer();
		void end_timer();

	private:
		timer_byte shortest_frame_time = 100.0L;
		timer_byte largest_frame_time = 0.0L;
		timer_byte current_frame_time = 0.0L;
		clock::time_point start; 
		clock::time_point end;
		std::chrono::duration<timer_byte> duration;
	};
}

#endif // !PMUT_BASE_H
