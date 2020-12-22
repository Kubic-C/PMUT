#include "Headers\pmut.h"

namespace pmut
{

	app::app(std::string title, int width, int height)
		: title(title), width(width), height(height), exit_code(0), 
		window(nullptr), _main_lock()
	{
	}

	void app::cleanup()
	{
		commands::end_command_if_possible();
		delete console::vars::pmut_console;
	}

	int app::start_console()
	{
		int console_good = console::high::start_console
		(
			window,
			input::character_callback,
			input::key_callback,
			input::active_input_buffer,
			PMUT_NAME,
			DIR_TO_LARGE_ICON,
			DIR_TO_SMALL_ICON,
			DIR_TO_SHADERS,
			FNT_FILE_NAME,
			BITMAP_FILE_NAME,
			SCR_WIDTH,
			SCR_HEIGHT
		);

		if (console_good == OGL_BAD)
		{
			flags::exit_app = true;
			exit_code = EXIT_FAILURE;
			return OGL_BAD;
		}

		console::high::set_callbacks(window, input::character_callback, input::key_callback);

		return OGL_GOOD;
	}


	void app::start()
	{
		start_console();
		commands::set_command_map();
		::pmut::_main_lock    = &this->_main_lock;

		console::vars::pmut_console->print_perm(0, console::vec3(1.0f, 1.0f, 0.0f), PMUT_NAME);
		console::vars::pmut_console->print_perm(1, console::vec3(1.0f, 0.6f, 0.6f), PMUT_ENGINE, glGetString(GL_VERSION));
		console::vars::pmut_console->print_perm(2, console::vec3(0.2f, 0.7f, 0.3f), "Type '{info}' to get some info about this version of PMUT");
	}

	void app::show_input_to_user()
	{
		console::vars::pmut_console->print("[PMUT]?>", console::vars::pmut_console->get_input(), "_");
	}

	void app::check_flags()
	{
		if (flags::update_settings)
			this->update_settings();

		if (!commands::active_command)
			return;

		if (flags::need_user_input && console::vars::pmut_console->get_submitted_input().size() != 0)
			_wait_for_input.notify_one();

		while (!flags::done_starting || flags::waiting || flags::need_user_input)
		{
			if (flags::waiting)
				_fast_main_cv.notify_one();
		}
	}

	void app::try_command()
	{
		if (console::vars::pmut_console->get_submitted_input().size() != 0)
		{
			if (!commands::active_command)
			{
				input::last_input_buffer = console::vars::pmut_console->get_submitted_input();
				commands::start_command(console::vars::pmut_console->get_submitted_input());
				console::vars::pmut_console->get_submitted_input().clear();
			}
			else
			{
				console::vars::pmut_console->print(RDR_ERR_P, true, V3_ERR,
					errors::cannot_call_a_command_already_active.error_);
				console::vars::pmut_console->get_submitted_input().clear();
			}
		}
	}

	void app::update_settings()
	{
		console::vars::pmut_console->set_swap_interval(settings::opengl::swap_interval);
		flags::update_settings = false;
		console::vars::pmut_console->print(19, true, V3_INFO, "settings have been updated");
	}

	int app::run()
	{
		start();

		while (!flags::exit_app && console::vars::pmut_console->window_closed())
		{
			preframe();
			pmut_frame();
			lateframe();
		}

		return exit_code;
	}
}

// app frames
namespace pmut
{
	void app::preframe()
	{
		check_flags();

		_main_lock.lock();
		flags::main_loop_unlocked = false;
		frametime.start_timer();

		console::vars::pmut_console->print(9, false, V3_INFO,
			PMUT + " currently active command: ", commands::active_command_name);
		console::vars::pmut_console->console_poll();
	}


	void app::pmut_frame()
	{
		console::vars::pmut_console->print(PMUT_ENGINE + " OpenGL error: ", glGetError());
		show_input_to_user();
		try_command();
		commands::end_command_if_possible();
	}

	void app::lateframe()
	{
		frametime.end_timer();
#pragma warning( push )
#pragma warning( disable : 26110 )
		_main_lock.unlock();
#pragma warning( pop )
		flags::main_loop_unlocked = true;
	}
}