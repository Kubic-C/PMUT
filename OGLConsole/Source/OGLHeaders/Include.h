// Master include for the entirety of this project
#pragma once
#ifndef OGL_MASTER_INCLUDE_H
#define OGL_MASTER_INCLUDE_H

#include "..\OGLConsole\Headers\console.h"

#define GLOBAL extern

#define DIR_TO_SHADERS    "../Misc/Bitmap/"
#define DIR_TO_LARGE_ICON "../Misc/Icon/large_pmut.png"
#define DIR_TO_SMALL_ICON "../Misc/Icon/small_icon.png"

#define FNT_FILE_NAME    "PMUT_FONT.fnt"
#define BITMAP_FILE_NAME "PMUT_FONT.png"

namespace
{
	const auto V3_ERR = console::vec3(1.0f, 0.0f, 0.0f);
	const auto V3_SUCCESS = console::vec3(0.0f, 1.0f, 0.0f);
	const auto V3_INFO = console::vec3(0.4f, 0.8f, 0.4f);

	const auto RDR_ERR_P = 15;
	const auto RDR_SUCCESS_P = 14;
	const auto RDR_INFO_P = 13;
}

namespace console
{
	namespace vars
	{
		GLOBAL console::console_master* pmut_console;
	}

	namespace high // "high level"
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
			int height);

		void set_callbacks(GLFWwindow* window, GLFWcharfun char_callback, GLFWkeyfun key_callback);
	}
}

#endif // OGL_MASTER_INCLUDE_H