#include "Headers/output.h"

namespace console
{
	character_placement::character_placement()
	{
		memset(this, 0, sizeof(*this));
	}

	character_placement::character_placement(char character, float x, float y, float width, float height, float xoffset, float yoffset, float advance)
		: character(character), x(x), y(y), width(width), height(height), xoffset(xoffset), yoffset(yoffset), advance(advance)
	{
	}

	void character_placement::set(char character, float x, float y, float width, float height, float xoffset, float yoffset, float advance)
	{
		this->character = character;
		this->x         = x;
		this->y         = y;
		this->width     = width;
		this->height    = height;
		this->xoffset   = xoffset;
		this->yoffset   = yoffset;
		this->advance   = advance;
	}

	bitmap::bitmap(std::string dir_to_folder, std::string fnt_file, std::string texture_atlas_file)
	{
		texture_atlas.load_bitmap(dir_to_folder + texture_atlas_file);
		load_characters(dir_to_folder + fnt_file);
		calc_quads_for_char_key();
	}

	void bitmap::bind_slot()
	{
		texture_atlas.activate_and_bind(GL_TEXTURE0);
	}

	float bitmap::get_width()
	{
		return cast(float, texture_atlas.width);
	}

	float bitmap::get_height()
	{
		return cast(float, texture_atlas.height);
	}

	void bitmap::load_characters(std::string dir)
	{

		std::ifstream fnt_file(dir);
		if (!fnt_file.is_open())
			return;

		int i = 0;
		std::string line;
		while (std::getline(fnt_file, line))
		{
			std::vector<std::string> split_line;
			if (line.find("kernings count=") != std::string::npos)
				return;

			split(split_line, line, " =");
			load_characters_switch(split_line, i);
		}
		return;
	}

	void bitmap::load_characters_switch(std::vector<std::string>& str_v, int& i)
	{
		switch (i)
		{
		case 0: case 1: case 2: case 3:
			break;

		default:
		{
			std::vector<float> set_params;
			for (std::string& str : str_v)
			{
				float possible = passie_stof(str);
				if (possible != float(INT_MAX))
					set_params.emplace_back(possible);
			}
			char_key[static_cast<int>(set_params[0])].first.set
			(
				static_cast<int>(set_params[0]),
				set_params[1],
				set_params[2],
				set_params[3],
				set_params[4],
				set_params[5],
				set_params[6],
				set_params[7]
			);
		}
			break;
		}

		i++;
	}

	void bitmap::calc_quads_for_char_key()
	{
		for (std::pair<const char, std::pair<character_placement, quad>>& char_ : char_key)
		{
			character_placement& char_fnt = char_.second.first;
			quad& char_quad = char_.second.second;

			float norm_width =  (char_fnt.width)  / texture_atlas.width;
			float norm_height = (char_fnt.height) / texture_atlas.height;

			float pos_pre[] =
			{
					norm_width,   norm_height, 0.3f,
					norm_width,  -norm_height, 0.3f,
				   -norm_width,  -norm_height, 0.3f,
				   -norm_width,   norm_height, 0.3f
			};

			for (int i = 0; i < 12; i += 3)
			{
				pos_pre[i + 0] -= char_fnt.xoffset / texture_atlas.width;
				pos_pre[i + 1] -= char_fnt.yoffset / texture_atlas.height;
			}

			float rgb_pre[] =
			{
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f
			};

			float tex_coords_pre[] =
			{
				(char_fnt.x + char_fnt.width) / texture_atlas.width, char_fnt.y                     / texture_atlas.height,
				(char_fnt.x + char_fnt.width) / texture_atlas.width, (char_fnt.y + char_fnt.height) / texture_atlas.height,
				char_fnt.x                    / texture_atlas.width, (char_fnt.y + char_fnt.height) / texture_atlas.height,
				char_fnt.x                    / texture_atlas.width, char_fnt.y                     / texture_atlas.height
			};


			memcpy(&char_quad.pos       , &pos_pre       , sizeof(pos_pre)       );
			memcpy(&char_quad.rgb       , &rgb_pre       , sizeof(rgb_pre)       );
			memcpy(&char_quad.tex_coords, &tex_coords_pre, sizeof(tex_coords_pre));
		}
	}

	void split(std::vector<std::string>& str_v, std::string& str, std::string delimiter)
	{
		std::string token;
		for (char& char_ : str)
		{
			if (delimiter.find(char_) != std::string::npos)
			{
				str_v.push_back(token);
				token.clear();
			}
			else
				token += char_;
		}
	}

	text_renderer::text_renderer(std::string dir_to_folder, std::string fnt_file, std::string texture_atlas_file)
		: bitmap(dir_to_folder, fnt_file, texture_atlas_file), vertex_array_buffer(), quad_buffer(GL_ARRAY_BUFFER), indices_b(GL_ELEMENT_ARRAY_BUFFER),
		program((OGL::program*)nullptr)
	{
		vertex_array_buffer.bind();
		quad_buffer.buffer_data(sizeof(quad)*CHARACTER_RENDER_LIMIT,           nullptr, GL_DYNAMIC_DRAW);

		unsigned int* indices = new unsigned int[6 * CHARACTER_RENDER_LIMIT];
		unsigned int offset = 0;
		for (int i = 0; i < 6*CHARACTER_RENDER_LIMIT; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 3 + offset;

			indices[i + 3] = 1 + offset;
			indices[i + 4] = 2 + offset;
			indices[i + 5] = 3 + offset;

			offset += 4;
		}

		indices_b.buffer_data((sizeof(unsigned int)*6)*CHARACTER_RENDER_LIMIT, indices, GL_DYNAMIC_DRAW);
		quad_buffer.bind();
		delete[] indices;

		GLint stride = sizeof(float[3 + 3 + 2]);
		vertex_array_buffer.set_buffer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr); // position
		quad_buffer.bind();
		vertex_array_buffer.set_buffer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)12); // color/rgb
		quad_buffer.bind();
		vertex_array_buffer.set_buffer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)24); // tex coord
	}

	void text_renderer::set_ref_program(OGL::program& program)
	{
		this->program = &program;
	}

	void text_renderer::print_poll()
	{
		switch (output_buffer.size())
		{
		case 0:
			return;
		}

		std::stable_sort(output_buffer.begin(), output_buffer.end(), &strow_lesser_then_render_piority);
		int indices_count = 0;
		float x_cpy = x, y_cpy = y;
		for (string_output_wrap& str_output : output_buffer)
			for (char& char_ : str_output.str)
			{
				auto character = bitmap.char_key[char_];
				x_cpy += -character.first.xoffset / bitmap.get_width();
				switch (character.first.character)
				{
				case '\n':
					y_cpy -= 0.1f;
					x_cpy = x;
					break;
				}

				if (x_cpy >= 1.9f)
				{
					y_cpy -= 0.1f;
					x_cpy = x;
				}
				else if (y_cpy <= -y)
				{
					goto render;
				}

				set_rgb_value_for_quad(character.second, str_output.color);
				translate_quad(character.second, vec3(x_cpy, y_cpy, 0.0f));
				add_quad_to_float_vector(character.second, vertex_text);

				x_cpy += ((character.first.advance) / bitmap.get_width());
				indices_count += 6;
			}
	render:
		quad_buffer.update_mem(sizeof(float) * vertex_text.size(), &vertex_text[0]);

		indices_b.bind();
		glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr);
		output_buffer.erase(std::remove_if(output_buffer.begin(), output_buffer.end(), strow_remove_pred), output_buffer.end());
		vertex_text.clear();
	}

	void text_renderer::prepare_output()
	{
		program->use();
		bitmap.bind_slot();
		program->set_sampler_two_dim("our_texture", 0);
		vertex_array_buffer.bind();
	}

	void text_renderer::clear_output_buffer()
	{
		output_buffer.erase(std::remove_if(output_buffer.begin(), output_buffer.end(), &strow_perm_remove_pred), output_buffer.end());
	}

	void text_renderer::clear_if_max_static_objects()
	{
		if (count_amount_of_static_text() > STATIC_OUTPUT_STR_LIMIT)
			clear_output_buffer();
	}

	int text_renderer::count_amount_of_static_text()
	{
		int amount_of_static = 0;
		for (string_output_wrap& stow : output_buffer)
		{
			switch (stow.is_static)
			{
			case true:
				amount_of_static++;
				break;
			}
		}
		return amount_of_static;
	}

	bool string_output_wrap::operator==(string_output_wrap strow)
	{
		return is_static == strow.is_static;
	}
}
		