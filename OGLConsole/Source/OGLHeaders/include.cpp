#include "Include.h"

namespace console
{
	namespace high
	{
		int start_console(
			GLFWwindow*& window,
			GLFWcharfun char_callback,
			GLFWkeyfun key_callback,
			std::string*& input_buffer,
			std::string title,
			std::string dir_to_large_icon,
			std::string dir_to_small_icon,
			std::string dir_to_shaders,
			std::string fnt_file_name,
			std::string bitmap_file_name,
			int width,
			int height)
		{
			if (OGL::start_ogl(window, title, dir_to_large_icon, dir_to_small_icon, width, height) == OGL_BAD)
			{
				return OGL_BAD;
			}

			vars::pmut_console = new console::console_master(dir_to_shaders, fnt_file_name, bitmap_file_name, window);
			vars::pmut_console->prepare_window();
			set_callbacks(window, char_callback, key_callback);

			return OGL_GOOD;
		}

		void set_callbacks(GLFWwindow* window, GLFWcharfun char_callback, GLFWkeyfun key_callback)
		{
			glfwSetCharCallback(window, char_callback);
			glfwSetKeyCallback(window, key_callback);
		}
	}
}