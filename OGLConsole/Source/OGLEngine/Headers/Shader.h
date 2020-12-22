// Shader class file
#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "PCH/OGLPCH.h"
#include "ShaderParser.h"

// String Success
#define STR_SUCCESS "\12\27"

namespace OGL
{
	typedef std::string StrError;

	class shader
	{
	public:
		shader(std::string Source, GLenum Type);
		shader(std::string Dir, std::string ShaderName, GLenum Type);
		shader(std::string Dir, std::string ShaderName, GLenum Type, std::string& StrSuccess); 
		~shader();

		StrError Compile(); // Compile shader

		GLuint ID(); // Getter for this->ShaderID

		void delete_(); // Delete the shader

		std::string* _GetSourceHolder();

		const char* _GetSource();

	protected:
		GLuint ShaderID = 0;
		GLenum Type;
		const char* Source = SourceHolder.c_str();
		std::string SourceHolder;
	};
}


#endif // SHADER_H