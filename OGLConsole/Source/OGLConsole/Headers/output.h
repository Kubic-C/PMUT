#pragma once
#ifndef CONSOLE_OUTPUT_H
#define CONSOLE_OUTPUT_H

#include "PCH\OGLPCH.h"
#include "maths.h"
#include <unordered_map>
#include <algorithm>

#define UKNOWN_CHARACTER (int)'~'
#define CHARACTER_RENDER_LIMIT 10000 // lol
#define STATIC_OUTPUT_STR_LIMIT 50
#define DEFAULT_RENDER_PIORITY 10

namespace console
{

	// can hold and may hold values for
	// the x, and y values of a character
	// on a texture_atlas
	struct character_placement
	{
		char character;
		float x;
		float y;
		float width;
		float height;
		float xoffset;
		float yoffset;
		float advance;

		character_placement();
		character_placement(char character, float x, float y, float width, float height, float xoffset, float yoffset, float advance);
		void set(char character, float x, float y, float width, float height, float xoffset, float yoffset, float advance);
	};

	struct string_output_wrap
	{
		vec3 color;
		bool is_static;
		bool perm;
		std::string str;
		int render_piority;

		string_output_wrap() : 
			color(0.0f, 0.0f, 0.0f), 
			is_static(false), 
			str("~!"),
			render_piority(DEFAULT_RENDER_PIORITY),
			perm(false) {};

		bool operator==(string_output_wrap strow);
	};

	static bool strow_lesser_then_render_piority(const string_output_wrap& strow_1, const string_output_wrap& strow_2)
	{ 
		return strow_1.render_piority < strow_2.render_piority; 
	}

	static bool strow_remove_pred(string_output_wrap stow)
	{
		int i = ((int)stow.perm) + ((int)stow.is_static);
		switch (i)
		{
		case 0:
			return true;
		}

		return false;
	}

	static bool strow_perm_remove_pred(string_output_wrap stow)
	{
		return !stow.perm;
	}

	/* BITMAP CLASS
	*  The bitmap class is resposible for
	*  loading character information such 
	*  as the x, and y values of a character
	*  on the texture atlas. It will
	*  also be resposible for loading
	*  the texture atlas image.
	* 
	* 
	*/
	class bitmap
	{
	public:
		bitmap(std::string dir_to_folder, std::string fnt_file, std::string texture_atlas_file);
		void bind_slot();

		float get_width();
		float get_height();

		std::unordered_map<char, std::pair<character_placement, quad>> char_key;
	private:
		void load_characters(std::string dir);
		void load_characters_switch(std::vector<std::string>& str_v, int& i);
		void calc_quads_for_char_key();

		OGL::texture texture_atlas;
	};

	/* TEXT_RENDERER CLASS
	* The text renderer class is responsible 
	* for the rendering of character(s)
	* off a bitmap. It also controls
	* the rendering order and the color
	* of the characters. It uses batch
	* rendering for faster rendering time
	* of a string of characters.
	* 
	* 
	*/
	class text_renderer
	{
	public:
		text_renderer(std::string dir_to_folder, std::string fnt_file, std::string texture_atlas_file);

		void set_ref_program(OGL::program& program);

		void print_poll();

		void prepare_output();

		void clear_output_buffer(); // clear output buffer will clear EVERYTHING besides perm objects
		
		void clear_if_max_static_objects();

		int count_amount_of_static_text();

		template<typename ... params>
		void print(int render_piority, bool is_static, vec3 rgb, const params& ... args)
		{
			((append_to_output_buffer(render_piority, false, is_static, rgb, args)), ...);
			append_to_output_buffer(render_piority, false, is_static, rgb, "\n");
		}

		template<typename ... params>
		void print(const params& ... args)
		{
			((append_to_output_buffer(DEFAULT_RENDER_PIORITY, false, false, vec3(1.0f, 1.0f, 1.0f), args)), ...);
			append_to_output_buffer(DEFAULT_RENDER_PIORITY, false, false, vec3(1.0f, 1.0f, 1.0f), "\n");
		}

		template<typename ... params>
		void print_perm(int render_piority, vec3 rgb, const params& ... args)
		{
			((append_to_output_buffer(render_piority, true, false, rgb, args)), ...);
			append_to_output_buffer(render_piority, true, false, rgb, "\n");
		}

		template<typename ... params>
		void print_perm(const params& ... args)
		{
			((append_to_output_buffer(DEFAULT_RENDER_PIORITY, true, false, vec3(1.0f, 1.0f, 1.0f), args)), ...);
			append_to_output_buffer(DEFAULT_RENDER_PIORITY, true, false, vec3(1.0f, 1.0f, 1.0f), "\n");
		}

	private:

		void append_to_output_buffer(int render_piority, bool is_perm, bool is_static, vec3 rgb, std::string str)
		{
			string_output_wrap output_str;
			output_str.is_static = is_static;
			output_str.color = rgb;
			output_str.str = str;
			output_str.perm = is_perm;
			output_str.render_piority = render_piority;
			output_buffer.emplace_back(output_str);
		}

		template<typename T>
		void append_to_output_buffer(int render_piority, bool is_perm, bool is_static, vec3 rgb, T str_t)
		{
			append_to_output_buffer(render_piority, is_perm, is_static, rgb, std::to_string(str_t));
		}
		
		void append_to_output_buffer(int render_piority, bool is_perm, bool is_static, vec3 rgb, const unsigned char* str_c)
		{
			append_to_output_buffer(render_piority, is_perm, is_static, rgb, std::string((char*)str_c));
		}

		void append_to_output_buffer(int render_piority, bool is_perm, bool is_static, vec3 rgb, char* str_c)
		{
			append_to_output_buffer(render_piority, is_perm, is_static, rgb, std::string(str_c));
		}

		void append_to_output_buffer(int render_piority, bool is_perm, bool is_static, vec3 rgb, const char* str_c)
		{
			append_to_output_buffer(render_piority, is_perm, is_static, rgb, std::string(str_c));
		}

		bitmap bitmap;
		OGL::VAO vertex_array_buffer;
		OGL::VBO quad_buffer;
		OGL::VBO indices_b;
		OGL::program* program;

		std::vector<string_output_wrap> output_buffer;
		std::vector<float>       vertex_text;
		float x = -1.9f, y = 1.9f; // where the tex will print
	};

	// split a string by an deleimier
	void split(std::vector<std::string>& str_v, std::string& str, std::string delimiter);
}

#endif // CONSOLE_OUTPUT_H
