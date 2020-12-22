// file for Program class
#pragma once
#ifndef PROGRAM_H
#define PROGRAM_H

#include "PCH/OGLPCH.h"
#include "Shader.h"

namespace OGL
{
	class program
	{
	public:
		// Note the paremter pack should only be OGL::Shader objects
		template<class ... Shaders>
		program(StrError& StrError, Shaders& ... Shader_)
			: ProgramID(glCreateProgram())
		{
			((Attach(Shader_)), ...);
			StrError = Link();
			((Shader_.delete_()), ...);
		}

		program() : ProgramID(0) {}

		void Attach(shader& Shader_);
		StrError Link();
		void use();

		template<class ... Shaders>
		void link_program(StrError& StrError, Shaders& ... Shader_)
		{
			glDeleteProgram(ProgramID);
			ProgramID = glCreateProgram();

			((Attach(Shader_)), ...);
			StrError = Link();
			((Shader_.delete_()), ...);
		}

		unsigned int get_id() const
		{
			return ProgramID;
		}

		void set_sampler_two_dim(std::string name, int slot);
		
		void set_vec4(std::string name, float v1, float v2, float v3, float v4);

		void set_vec3(std::string name, float v1, float v2, float v3);

	protected:
		GLuint ProgramID;

	};
}

#endif // PROGRAM_H