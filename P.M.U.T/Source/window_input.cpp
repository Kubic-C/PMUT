#include "Headers/window_input.h"

namespace pmut
{
	namespace input
	{
		void character_callback(GLFWwindow* window, unsigned int codepoint)
		{
			console::vars::pmut_console->keyboard_buffer += cast(char, codepoint);
		}

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			switch (key)
			{
			case GLFW_KEY_BACKSPACE:
				if (!console::vars::pmut_console->keyboard_buffer.size() || action != GLFW_PRESS)
					return;

				if (alt_mode)
					console::vars::pmut_console->keyboard_buffer.clear();
				else
					console::vars::pmut_console->keyboard_buffer.pop_back();
				return;

			case GLFW_KEY_ENTER:
				console::vars::pmut_console->submit_input();
				return;

			case GLFW_KEY_LEFT_ALT:
				alt_mode = true;
				return;

			case GLFW_KEY_F1:
				console::vars::pmut_console->clear_output_buffer();
				return;

			case GLFW_KEY_F2:
				console::vars::pmut_console->print(12, true, console::vec3(0.0f, 1.0f, 0.2f), "static string test");
				return;

			case GLFW_KEY_F3:
				console::vars::pmut_console->print(12, false, console::vec3(0.0f, 1.0f, 0.2f), "non-static string test");
				return;

			case GLFW_KEY_UP:
				if (action == GLFW_PRESS)
				{
					std::string& current_input = console::vars::pmut_console->keyboard_buffer;
					console::vars::pmut_console->keyboard_buffer = last_input_buffer;
					last_input_buffer = current_input;
				}
				return;

			case GLFW_KEY_LEFT_CONTROL:
				ctrl_mode = true;
				return;
				
			case GLFW_KEY_V:
				if (ctrl_mode && action == GLFW_PRESS)
				{
					const char* clipboard = glfwGetClipboardString(window);
					if (strlen(clipboard) == 0)
						return;
					console::vars::pmut_console->keyboard_buffer += std::string(clipboard);
				}
				return;

			case GLFW_KEY_C:
				if (ctrl_mode && action == GLFW_PRESS)
					glfwSetClipboardString(window, console::vars::pmut_console->keyboard_buffer.c_str());
				return;

			default:
				alt_mode = false;
				ctrl_mode = false;;
				break;
			}
		}
	}
}