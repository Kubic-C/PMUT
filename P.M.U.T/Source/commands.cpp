#include "Headers/commands.h"

#define ERR_STR "\r error"

namespace pmut
{
	namespace commands
	{
		command::command(command_fun command_ptr)
			: command_ptr(command_ptr)
		{
		}

		command::command()
			: command_ptr(nullptr)
		{
		}

		void command::check_if_input_is_valid(std::vector<std::pair<std::string, std::string>> option_inputs)
		{
			size_t sizeof_oi = option_inputs.size();
			if (sizeof_oi != option_names.size())
				throw errors::invalid_amount_of_options(option_inputs.size());

			if (option_names.size() == 0)
				return;

			for (size_t index = 0; index < sizeof_oi; index++)
			{
				std::string option_name = option_inputs[index].first;

				if (option_name != option_names[index])
					throw errors::uknown_option("option[" + option_name + "]");
			}
		}

		void command::invoke(std::vector<std::pair<std::string, std::string>> option_inputs)
		{
			command_ptr(option_inputs);
		}

		command_fun command::get_fun()
		{
			return command_ptr;
		}

		std::string extract(std::string start, std::string end, std::string& str)
		{
			int i = 0;
			bool write = false;
			std::string buffer = ERR_STR;
			for (;;)
			{
				if (start.find(str[0]) != std::string::npos)
				{
					buffer = "";
					write = true;
				}
				else if (end.find(str[0]) != std::string::npos)
					break;

				else if (write)
					buffer += str[0];


				if (str.size() >= 2)
					str.erase(str.begin(), str.begin() + 1);
				else
					break;
			}

			if (str.size() >= 2)
				str.erase(str.begin(), str.begin() + 1);
			return buffer;
		}

		std::vector<std::string> extract_boxes(std::string str)
		{
			std::string remainder = str;
			std::vector<std::string> boxes;
			for (;;)
			{
				if (remainder.size() == 0)
					break;

				size_t index = remainder.find("[");
				if (index == std::string::npos)
					break;

				size_t end_index = remainder.find("]") + 1;
				if (end_index == std::string::npos)
					throw errors::no_terminating_end_character_found(end_index);

				std::string box = extract("[", "]", remainder);
				if (box == ERR_STR)
					throw errors::pmut_ct_error(ERR_STR);

				boxes.push_back(box);
			}

			return boxes;
		}

		std::vector<std::pair<std::string, std::string>> extract_option_pair(std::string str)
		{
			std::vector<std::string> option_boxes = extract_boxes(str);
			std::vector<std::pair<std::string, std::string>> options = {};
			for (std::string& box : option_boxes)
			{
				std::pair<std::string, std::string> option_buffer;

				size_t seperator = box.find(':');
				if (seperator == std::string::npos)
					throw errors::no_seperator_found(box);

				option_buffer.first = box.substr(0, seperator);
				option_buffer.first.erase(std::remove(option_buffer.first.begin(), option_buffer.first.end(), ' '), option_buffer.first.end());
				option_buffer.second = box.substr(seperator + 1, box.size());
				options.push_back(option_buffer);
			}

			return options;
		}

		void start_command(std::string input)
		{
			try
			{
				std::string command = extract_command(input);
				if (commands_map.find(command) != commands_map.end())
				{
					std::vector<std::pair<std::string, std::string>> arguments = extract_option_pair(input);
					commands_map[command].check_if_input_is_valid(arguments);
					active_command_name = command;
					active_command = new std::thread(commands_map[command].get_fun(), arguments);
				}
				else
					throw errors::uknown_command(command);
			}
			catch (errors::error& err)
			{
				console::vars::pmut_console->print(RDR_ERR_P, true, V3_ERR, err.error_, ": ", err.how);
			}
		}

		void end_command_if_possible()
		{
			if (done)
			{
				active_command->join();
				delete active_command;
				active_command_name = "";
				active_command = nullptr;
				done = false;
			}
		}

		std::string extract_command(std::string& str)
		{
			std::string command = "";
			size_t index = str.find("{");
			size_t end_index = str.find("}");
			if (index >= end_index || index == std::string::npos || end_index == std::string::npos)
				throw errors::invalid_postion_of_command_extractor_operators("{} not }{ nor without them");

			command = str.substr(index + 1, end_index - 1);
			str.erase(index, end_index + 1);
			return command;
		}
	}
}