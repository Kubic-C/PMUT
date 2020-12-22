#pragma once
#ifndef COMMAND_DEFINITIONS_H
#define COMMAND_DEFINITIONS_H

#include "commands.h"

GLOBAL bool rainbow_mode_var;

namespace console
{
	namespace vars
	{
		GLOBAL console::console_master* pmut_console;
	}
}
namespace pmut
{
	GLOBAL std::mutex* _main_lock				  ;
	GLOBAL std::condition_variable _fast_main_cv  ;
	GLOBAL std::condition_variable _wait_for_input;

	namespace flags
	{
		GLOBAL std::atomic<bool> waiting;
		GLOBAL std::atomic<bool> main_loop_unlocked;
		GLOBAL std::atomic<bool> done_starting;
		GLOBAL std::atomic<bool> update_settings;
		GLOBAL std::atomic<bool> need_user_input;
		GLOBAL bool exit_app;
	}

	namespace settings
	{
		namespace opengl
		{
			GLOBAL int swap_interval;
		}
	}

	namespace commands
	{
		GLOBAL std::unordered_map<std::string, command> commands_map;
		GLOBAL bool done;
		void set_command_map();
		std::unique_lock<std::mutex> make_cmd_unique_lock();
		void wait(std::unique_lock<std::mutex>& mlock);
		void cmd_done(std::unique_lock<std::mutex>& mlock);
	}
}

#endif //!COMMAND_DEFINITIONS_H