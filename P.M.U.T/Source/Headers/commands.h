#pragma once
#ifndef COMMANDS_H

#include <condition_variable>
#include <thread>
#include <mutex>
#include <atomic>
#include "window_input.h"

namespace console
{
	namespace vars
	{
		GLOBAL console::console_master* pmut_console;
	}
}

namespace pmut
{
	namespace input
	{
		GLOBAL bool alt_mode;
		GLOBAL std::string* active_input_buffer;
	}

	namespace commands
	{
		class command;
		GLOBAL std::unordered_map<std::string, command> commands_map;
		typedef void(*command_fun)(COMMAND_PARAMS);

		GLOBAL std::thread* active_command;
		GLOBAL std::string active_command_name;
		GLOBAL bool done;

		class command
		{
		public:
			template<typename ... params>
			command(command_fun command_ptr, const params& ... options)
				: command_ptr(command_ptr)
			{
				((option_names.emplace_back(options)), ...);
			}

			command(command_fun command_ptr);
			command();

			void check_if_input_is_valid(std::vector<std::pair<std::string, std::string>> option_inputs);
			void invoke(std::vector<std::pair<std::string, std::string>> option_inputs);

			command_fun get_fun();

		protected:
			std::string command_sig;
			command_fun command_ptr;
			std::vector<std::string> option_names = {};
		};

		std::string extract_command(std::string& str);
		std::vector<std::pair<std::string, std::string>> extract_option_pair(std::string str);

		void start_command(std::string input);
		void end_command_if_possible();
	}
}


#endif // !COMMANDS_H
