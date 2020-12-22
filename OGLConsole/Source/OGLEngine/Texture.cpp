#include "Headers/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Headers/Depend/stb_image.h"

namespace OGL
{
	texture::texture()
		: ok(true),  ID(0), width(0), height(0), 
		image(nullptr), nr_channels(0)
	{
		glGenTextures(1, &ID);
	}

	texture::texture(std::string dir)
	{
		glGenTextures(1, &ID);
		load_whole_texture(dir, texture_params());
	}

	void texture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	void texture::activate_and_bind(GLenum slot)
	{
		glActiveTexture(slot); // activate the texture unit first before binding texture
		bind();
	}

	void texture::load_whole_texture(const std::string& dir, texture_params texture_p)
	{
		bind();
	
		glTexParameteri(GL_TEXTURE_2D, texture_p.pname_one, texture_p.pname_param_one);
		glTexParameteri(GL_TEXTURE_2D, texture_p.pname_two, texture_p.pname_param_two);
		glTexParameteri(GL_TEXTURE_2D, texture_p.filter_one, texture_p.filter_param_one);
		glTexParameteri(GL_TEXTURE_2D, texture_p.filter_two, texture_p.filter_param_two);

		//stbi_set_flip_vertically_on_load(true);
		image = stbi_load(dir.c_str(), &width, &height, &nr_channels, 0);
		if (!image)
		{
			ok = false;
			printf("something went wrong :(\n");
			return; // skip the rest
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
	}

	void texture::load_bitmap(std::string dir)
	{
		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//stbi_set_flip_vertically_on_load(true);
		image = stbi_load(dir.c_str(), &width, &height, &nr_channels, 0);
		if (!image)
		{
			ok = false;
			return; // skip the rest
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
	}

	OGL::texture texture::load_sub_texture(const std::string& dir, texture_params texture_p, int width, int height, int xoff, int yoff, int zoff)
	{
		bind();

		//glTexParameteri(GL_TEXTURE_2D, texture_p.pname_one, texture_p.pname_param_one);
		//glTexParameteri(GL_TEXTURE_2D, texture_p.pname_two, texture_p.pname_param_two);
		//glTexParameteri(GL_TEXTURE_2D, texture_p.filter_one, texture_p.filter_param_one);

		//glTexParameteri(GL_TEXTURE_2D, texture_p.filter_two, texture_p.filter_param_two);
		// load image, create texture and generate mipmaps

		stbi_set_flip_vertically_on_load(true);
		image = stbi_load(dir.c_str(), &width, &height, &nr_channels, 0);
		if (!image)
		{
			ok = false;
			return OGL::texture(); // skip the rest
		}

		glGetTextureSubImage(GL_TEXTURE_2D, 0, xoff, yoff, zoff, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, 1, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
		return OGL::texture();
	}

	void unbind_texture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	texture_params::texture_params()
		:
		pname_one(GL_TEXTURE_WRAP_S), filter_one(GL_TEXTURE_MIN_FILTER),
		pname_two(GL_TEXTURE_WRAP_T), filter_two(GL_TEXTURE_MAG_FILTER),
		pname_param_one(GL_CLAMP_TO_BORDER), filter_param_one(GL_LINEAR),
		pname_param_two(GL_CLAMP_TO_BORDER), filter_param_two(GL_LINEAR)
	{
	}
}