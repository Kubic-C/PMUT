#include "Headers/command_defintions.h"

namespace pmut
{
	namespace commands
	{
		namespace definitions
		{
			void print(COMMAND_PARAMS)
			{
				enum opt { str = 0 };
				auto mlock = make_cmd_unique_lock();
				wait(mlock);

				console::vars::pmut_console->print(RDR_SUCCESS_P, true, V3_SUCCESS,
					options[opt::str].second);
				
				cmd_done(mlock);
			}

			void print_two(COMMAND_PARAMS)
			{
				enum opt { str = 0, str2 = 1 };
				auto mlock = make_cmd_unique_lock();
				wait(mlock);

				console::vars::pmut_console->print(RDR_SUCCESS_P, true, V3_SUCCESS, 
					options[opt::str].second);
				console::vars::pmut_console->print(RDR_SUCCESS_P, true, V3_SUCCESS, 
					options[opt::str2].second);

				cmd_done(mlock);
			}

			void info(COMMAND_PARAMS)
			{
				auto mlock = make_cmd_unique_lock();
				wait(mlock);

				console::vars::pmut_console->print(RDR_INFO_P, true, V3_INFO, 
					PMUT_INFO);

				cmd_done(mlock);
			}

			void exit(COMMAND_PARAMS)
			{
				auto mlock = make_cmd_unique_lock();
				wait(mlock);

				flags::exit_app = true;

				cmd_done(mlock);
			}

			void set_interval(COMMAND_PARAMS)
			{
				enum opt { interval_speed = 0 };
				auto mlock = make_cmd_unique_lock();
				wait(mlock);

				try
				{
					int new_interval = std::stoi(options[interval_speed].second);
					settings::opengl::swap_interval = new_interval;
					flags::update_settings = true;
					console::vars::pmut_console->print(RDR_SUCCESS_P, true, V3_SUCCESS, 
						"swap interval successfully changed");
				}
				catch (std::invalid_argument& err)
				{
					console::vars::pmut_console->print(RDR_ERR_P, true, V3_ERR,
						err.what());
				}

				cmd_done(mlock);
			}

			void clear_screen(COMMAND_PARAMS)
			{
				auto mlock = make_cmd_unique_lock();
				wait(mlock);

				console::vars::pmut_console->clear_output_buffer();

				cmd_done(mlock);
			}

			void get_html(COMMAND_PARAMS)
			{
				enum opt { ip = 0 };

				asio::error_code err_code;
				asio::io_context context;
				asio::ip::tcp::endpoint ip_endpoint(asio::ip::make_address(options[opt::ip].second, err_code), 80);
				asio::ip::tcp::socket user_socket(context);
				user_socket.connect(ip_endpoint, err_code);

				auto mlock = make_cmd_unique_lock();

				std::string qrequest =
				    "GET /index.html HTTP/1.1\r\n"
					"HOST: example.com\r\n"
					"Connection: close\r\n\r\n";

				if (!err_code && user_socket.is_open())
				{
					using namespace std::chrono_literals;
					wait(mlock);
					console::vars::pmut_console->print(RDR_SUCCESS_P, true, V3_SUCCESS,
						"connection with socket(", options[opt::ip].second, ", 80", ") was successful!");
					user_socket.write_some(asio::buffer(qrequest.data(), qrequest.size()), err_code);
					std::this_thread::sleep_for(200ms);

					size_t bytes = user_socket.available();

					if (bytes > 0)
					{
						std::vector<char> input(bytes);
						user_socket.read_some(asio::buffer(input.data(), input.size()), err_code);
						std::string input_(input.begin(), input.end());
						console::vars::pmut_console->print(RDR_SUCCESS_P, true, V3_SUCCESS,
							"socket(", options[opt::ip].second, ", 80", ") sent: \n", input_);
					}
				}
				else
				{
					console::vars::pmut_console->print(RDR_ERR_P, true, V3_ERR, 
						"ASIO could not connect with error code: ", err_code.value());
				}

				user_socket.close();

				cmd_done(mlock);
			}

			void test(COMMAND_PARAMS)
			{
				auto mlock = make_cmd_unique_lock();
				wait(mlock);

				hlnet::get_html();

				cmd_done(mlock);
			}
		}

		void set_command_map()
		{
			commands_map["exit"]           = command(definitions::exit);
			commands_map["info"]           = command(definitions::info);
			commands_map["print"]          = command(definitions::print, "str");
			commands_map["print two"]      = command(definitions::print_two, "str", "str2");
			commands_map["swap interval"]  = command(definitions::set_interval, "i");
			commands_map["html"]           = command(definitions::get_html, "ip");
			commands_map["clear"]		   = command(definitions::clear_screen);
			commands_map["test"]		   = command(definitions::test);
		}

		std::unique_lock<std::mutex> make_cmd_unique_lock()
		{
			flags::done_starting = false;
			std::unique_lock<std::mutex> mlock(*_main_lock);
			flags::done_starting = true;
#pragma warning( push )
#pragma warning( disable : 26115 )
			return std::move(mlock);
#pragma warning( pop )
		}

		void wait(std::unique_lock<std::mutex>& mlock)
		{
			flags::waiting = true;
			_fast_main_cv.wait(mlock);
			flags::waiting = false;
		}

		void cmd_done(std::unique_lock<std::mutex>& mlock)
		{
			done = true;
#pragma warning( push )
#pragma warning( disable : 26110 )
			mlock.unlock();
#pragma warning( pop )
		}

	}
}