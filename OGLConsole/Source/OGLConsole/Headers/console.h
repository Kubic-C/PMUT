// File for console class
#pragma once
#ifndef OGL_CONSOLE_H
#define OGL_CONSOLE_H

#include "PCH\OGLPCH.h"
#include "output.h"

namespace console
{
	
	/*
	 * A console master object is responsible for the startup
	 * cleanup, and rendering/output of the PMUT window. It 
	 * is the PMUT window in a sense. It also will take care/
	 * cover the keyboard input. It's purpose is to abstract
	 * the proccess of input, output, and rendering of the 
	 * PMUT window. The creation of this object should only
	 * happen one time. Any more then that can lead to undefined
	 * behaviour or unwanted behaviour.
	 * 
	 * 
	*/
	class console_master
	{
	public:
		bool submitted_input_is_new = false;

		console_master(std::string dir, std::string fnt_file, std::string texture_atlas_file, GLFWwindow* window);
		~console_master();

		std::string* get_console_internal_keyboard_buffer();
		std::string get_input(); // returns live input
		std::string& get_submitted_input(); // returns input that the user sumbitted
		bool get_new_submitted_input();
		void clear_keyboard_buffer();
		void set_background_color(float red, float green, float blue);
		void set_default_background(float red, float green, float blue);
		void set_background_color(vec3 rgb);
		void set_background_default();
		void prepare_window();
		void console_poll(); // poll should be the first thing called every frame
		void clear_submitted_input_buffer();
		void clear_output_buffer();
		void submit_input();
		void set_swap_interval(int interval);

		bool window_closed();

		template<typename ... params>
		void print(const params& ... args)
		{
			output.print(args...);
		}

		template<typename ... params>
		void print(int render_piority, bool is_static, vec3 rgb, const params& ... args)
		{
			output.print(render_piority, is_static, rgb, args...);
		}

		template<typename ... params>
		void print_perm(int render_piority, vec3 rgb, const params& ... args)
		{
			output.print_perm(render_piority , rgb, args...);
		}

		template<typename ... params>
		void print_perm(const params& ... args)
		{
			output.print_perm(args...);
		}

		std::string keyboard_buffer;
		std::string entered_input;

	private:
		text_renderer output;
		OGL::program font_program;
		std::string console_errors;
		GLFWwindow* window;
		vec3 background_color;
	};
}

#endif // OGL_CONSOLE_H