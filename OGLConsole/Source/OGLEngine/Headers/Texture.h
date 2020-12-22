// File meant for classes VAO(Vertex Array Object), and VBO(Vertex Buffer object)
#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "PCH/OGLPCH.h"

namespace OGL
{

	void unbind_texture();

	struct texture_params
	{
		GLenum pname_one, filter_one;
		GLenum pname_two, filter_two;
		GLenum pname_param_one, filter_param_one;
		GLenum pname_param_two, filter_param_two;

		texture_params();
	};

	// The type of texture is GL_TEXTURE_2D as no other texture type is needed
	class texture
	{
	public:
		bool ok; // Constructor will set member to false(something went wrong with loading) or true(all ok)

		texture();
		texture(std::string dir);

		void bind();

		void activate_and_bind(GLenum slot);

		// loads .png
		void load_whole_texture(const std::string& dir, texture_params texture_p);

		// loads file meant for bitmap
		void load_bitmap(std::string dir);

		// load a sub texture from a texuture
		OGL::texture load_sub_texture(const std::string& dir, texture_params texture_p, int width, int height, int xoff, int yoff, int zoff);

		unsigned int ID;
		unsigned char* image;
		int width, height, nr_channels;
	};
}

#endif // TEXTURE_H