//  include for all things OpenGL, not meant for things i have written or made
#pragma once
#ifndef OGL_PCH
#define OGL_PCH

#define OGL_GOOD 0
#define OGL_BAD -1

// GLEW_STATIC is defined, just not in a file
#pragma warning( push )
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma warning( pop  )

// Usual includes...
#include <vector>
#include <string>

#endif // OGL_PCH
