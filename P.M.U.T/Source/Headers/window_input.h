#pragma once
#ifndef WINDOW_INPUT_H
#define WINDOW_INPUT_H

#include "error.h"

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
		ONE_DEF bool ctrl_mode = false;
		GLOBAL std::string last_input_buffer;

		void character_callback(GLFWwindow* window, unsigned int codepoint);
		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void set_callbacks(GLFWwindow* window);
	}
}

#endif