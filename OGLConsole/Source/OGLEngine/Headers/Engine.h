// Master include for the folder: "OGLEngine"
#pragma once
#ifndef OGL_ENGINE_H
#define OGL_ENGINE_H

#include "Program.h"
#include "VertexObjects.h"
#include "Texture.h"

#define SCR_WIDTH 1000
#define SCR_HEIGHT 1000

namespace OGL
{
	typedef int OGLError;

	void BasicLoop(GLFWwindow*& Window);

	bool ShittyRender(GLFWwindow*& Window);

	inline OGLError _EndGLFW(void); // closes window and ALWAYS returns a -1

	OGLError start_ogl(GLFWwindow*& WindPtr, std::string Title, std::string dir_to_large_icon, std::string dir_to_small_icon, int width, int height);

	void render_texure(std::string dir, OGL::program& program_texure);

	void end_ogl(void);

	GLFWimage load_icon(std::string& dir);
}

#endif // OGL_ENGINE_H