#include "Headers/Engine.h"
#include "Headers/Depend/stb_image.h"

namespace OGL
{

	// .cpp only function vvv
	void opengl_load_core()
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	// .cpp only function vvv
	void load_and_set_icons(GLFWwindow* wind_ptr, std::string dir_to_large_icon, std::string dir_to_small_icon)
	{
		GLFWimage images[2];
		images[0] = load_icon(dir_to_large_icon);
		images[1] = load_icon(dir_to_small_icon);

		glfwSetWindowIcon(wind_ptr, 2, images);
		stbi_image_free(images[0].pixels);
		stbi_image_free(images[1].pixels);
	}

	// usless
	void frame_callback_and_resize(GLFWwindow* window, int width, int height)
	{
		if (width == 0 || height == 0)
			return;

		glViewport(0, 0, (width/height)*SCR_WIDTH, (width/height)*SCR_HEIGHT);
	}

	void BasicLoop(GLFWwindow*& Window)
	{
		OGL::VAO AnArray;
		AnArray.bind();

		OGL::VBO AFuckingTriangle;
		OGL::SetVboToDefault(AFuckingTriangle);

		while (OGL::ShittyRender(Window)) { AnArray.bind();  OGL::SetVboToDefault(AFuckingTriangle); }
	}

	bool ShittyRender(GLFWwindow*& Window)
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(Window);

		/* Poll for and process events */
		glfwPollEvents();

		return !glfwWindowShouldClose(Window);
	}

	inline OGLError _EndGLFW()
	{
		glfwTerminate(); // closes window
		return OGL_BAD;
	}

	OGLError OGL::start_ogl(GLFWwindow*& wind_ptr, std::string Title, std::string dir_to_large_icon, std::string dir_to_small_icon, int width, int height)
	{
		int GLFWGood = glfwInit();
		if (GLFWGood != GLFW_TRUE)
			return _EndGLFW();

		opengl_load_core();
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		wind_ptr = glfwCreateWindow(width, height, Title.c_str(), nullptr, nullptr);
		if (!wind_ptr)
			return _EndGLFW();
		
		glfwMakeContextCurrent(wind_ptr);
		glfwSetFramebufferSizeCallback(wind_ptr, frame_callback_and_resize);

		int GLEWGood = glewInit();
		if (GLEWGood != GLEW_OK)
			return _EndGLFW();

		load_and_set_icons(wind_ptr, dir_to_large_icon, dir_to_small_icon);

		glEnable(GL_BLEND);  // blending for dem text and textures
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_DEPTH_TEST)

		return OGL_GOOD;
	}

	void render_texure(std::string dir, OGL::program& texture_program)
	{
		float vertices_[] =
		{
			1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f
		};

		unsigned int indices_[] =
		{
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
		};

		float tex_cords_[] =
		{
		  1.0f, 1.0f,   // top right
		  1.0f, 0.0f,   // bottom right
		  0.0f, 0.0f,   // bottom left
		  0.0f, 1.0f    // top left 
		};


		OGL::VAO texture_buffer = OGL::bind_on_start();

		OGL::VBO vertices(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);
		vertices.bind();
		texture_buffer.set_buffer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, nullptr);

		OGL::VBO tex_cords(GL_ARRAY_BUFFER, sizeof(tex_cords_), tex_cords_, GL_STATIC_DRAW);
		tex_cords.bind();
		texture_buffer.set_buffer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

		OGL::VBO indices(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);
		OGL::texture texture(dir);
		texture.activate_and_bind(GL_TEXTURE0);

		texture_program.set_sampler_two_dim("texture1", 0);

		float color = (sin(glfwGetTime()) * cos(glfwGetTime()));
		texture_program.set_vec4("cpu_color", color, color / 1.1f, color * 0.4f, 0.5f);

		indices.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	void end_ogl(void)
	{
		_EndGLFW();
	}

	GLFWimage load_icon(std::string& dir)
	{
		GLFWimage image;
		image.pixels = stbi_load(dir.c_str(), &image.width, &image.height, nullptr, 4);
		return image;
	}
}