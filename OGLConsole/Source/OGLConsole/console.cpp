#include "Headers/Console.h"

#define FONT_SHADER_DIR "../Misc/Shaders/font.pshader"

namespace console
{
	console_master::console_master(std::string dir, std::string fnt_file, std::string texture_atlas_file, GLFWwindow* window)
		: keyboard_buffer(""), background_color(0.0f, 0.0f, 0.0f), window(window), output(dir, fnt_file, texture_atlas_file)
	{

		std::string vertex_errors = "";
		OGL::shader font_vertex_shader(FONT_SHADER_DIR, "font_vertex_shader", GL_VERTEX_SHADER, vertex_errors);
		font_vertex_shader.Compile();

		std::string fragment_errors = "";
		OGL::shader font_fragment_shader(FONT_SHADER_DIR, "font_fragemnt_shader", GL_FRAGMENT_SHADER, fragment_errors);
		font_fragment_shader.Compile();

		std::string program_errors = "";
		font_program.link_program(program_errors, font_vertex_shader, font_fragment_shader);

		printf(vertex_errors.c_str());
		printf("\n");
		printf(fragment_errors.c_str());
		printf("\n");
		printf(program_errors.c_str());
		printf("\n");
		output.set_ref_program(font_program);
		
		set_swap_interval(2);
	}

	console_master::~console_master()
	{
		
	}

	std::string* console_master::get_console_internal_keyboard_buffer()
	{
		return &keyboard_buffer;
	}

	void console_master::clear_keyboard_buffer()
	{
		keyboard_buffer.clear();
	}

	void console_master::set_background_color(float red, float green, float blue)
	{
		glClearColor(red, green, blue, 1.0f);
	}

	void console_master::set_default_background(float red, float green, float blue)
	{
		background_color = vec3(red, green, blue);
	}

	void console_master::set_background_color(vec3 rgb)
	{
		glClearColor(rgb.x, rgb.y, rgb.z, 1.0f);
	}

	void console_master::set_background_default()
	{
		glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
	}

	void console_master::prepare_window()
	{
		glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
		output.prepare_output();
	}

	void console_master::console_poll()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		output.clear_if_max_static_objects();
		output.print_poll();
		
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	void console_master::clear_submitted_input_buffer()
	{
		entered_input.clear();
		submitted_input_is_new = false;
	}

	void console_master::clear_output_buffer()
	{
		output.clear_output_buffer();
	}

	void console_master::submit_input()
	{
		entered_input.append(keyboard_buffer);
		keyboard_buffer.clear();
		submitted_input_is_new = true;
	}

	void console_master::set_swap_interval(int interval)
	{
		glfwSwapInterval(interval);
	}

	std::string console_master::get_input()
	{
		return keyboard_buffer;
	}

	std::string& console_master::get_submitted_input()
	{
		return entered_input;
	}

	bool console_master::get_new_submitted_input()
	{
		return this->submitted_input_is_new;
	}

	bool console_master::window_closed()
	{
		return !glfwWindowShouldClose(window);
	}
}