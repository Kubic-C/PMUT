#pragma once
#ifndef XMAIN_H
#define XMAIN_H

#include "command_defintions.h"

namespace console
{
	namespace vars
	{
		GLOBAL console::console_master* pmut_console;
	}
}

namespace pmut
{
	GLOBAL std::mutex* _main_lock		          ;
	GLOBAL std::condition_variable _fast_main_cv  ;
	GLOBAL std::condition_variable _wait_for_input;

	namespace input
	{
		GLOBAL std::string* active_input_buffer;
		GLOBAL std::string last_input_buffer;
	}

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
		GLOBAL std::thread* active_command;
		GLOBAL command* active_command_obj;
		GLOBAL bool done;
	}

	class app
	{
	public:
		app(std::string title, int width, int height);

		int run();
		void pmut_frame();
		void cleanup();

	private:
		timer frametime;
		std::mutex _main_lock;
		GLFWwindow* window;
		std::string title;
		int width, height;
		int exit_code;

		void start();
		void preframe();
		void lateframe();
		int start_console();
		void show_input_to_user();
		void check_flags();
		void try_command();
		void update_settings();
	};
}

#endif