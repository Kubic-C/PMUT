#include "Headers/maths.h"

namespace console
{
	vec2::vec2(float x, float y)
		: x(x), y(y)
	{
	}

	std::vector<float> vec2::unpack()
	{
		return { x, y };
	}

	vec3::vec3(float x, float y, float z)
		: x(x), y(y), z(z)
	{
	}

	std::vector<float> vec3::unpack()
	{
		return { x, y, z };
	}

	vec4::vec4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{
	}

	std::vector<float> vec4::unpack()
	{
		return { x, y, z, w };
	}

	void translate_quad(quad& quad, vec3 vec3_)
	{
		for (int i = 0; i < 12; i += 3)
		{
			quad.pos[i    ] += vec3_.x;
			quad.pos[i + 1] += vec3_.y;
			quad.pos[i + 2] += vec3_.z;
		}
	}

	void add_quad_to_float_vector(quad& quad, std::vector<float>& vector)
	{
		vector.reserve(32);

		// vertex 1, 2, 3 , 4
		int vertex_offset = 3;
		for (int v_i = 0; v_i < 4; v_i++)
		{
			for (int i = 0; i < 3; i++)
				vector.push_back(quad.pos[i + (vertex_offset * v_i)]); 

			for (int i = 0; i < 3; i++)
				vector.push_back(quad.rgb[i + (vertex_offset * v_i)]);

			for (int i = 0; i < 2; i++)
				vector.push_back(quad.tex_coords[i + ((vertex_offset - 1) * v_i)]);
		}

	}

	void add_quad_to_float_array(quad& quad, float* vector)
	{
		memcpy(vector, &quad, sizeof(quad));
	}

	void set_rgb_value_for_quad(quad& quad, vec3 rgb)
	{
		for (float* i = quad.rgb; i <= &quad.rgb[11]; i += 3)
		{
			*(i + 0) = rgb.x;
			*(i + 1) = rgb.y;
			*(i + 2) = rgb.z;
		}
	}
}