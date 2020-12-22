#include "Headers/Shader.h"

namespace OGL
{
	shader::shader(std::string Source, GLenum Type)
		: SourceHolder(Source), Type(Type), ShaderID(glCreateShader(Type))
	{
		Source = SourceHolder.c_str();
	}

	shader::shader(std::string Dir, std::string ShaderName, GLenum Type)
		: SourceHolder(ParseForLines(Dir, ShaderName)), Type(Type), ShaderID(glCreateShader(Type))
	{
		Source = SourceHolder.c_str();
	}

	shader::shader(std::string Dir, std::string ShaderName, GLenum Type, std::string& StrSuccess)
		: SourceHolder(ParseForLines(Dir, ShaderName)), Type(Type), ShaderID(glCreateShader(Type))
	{
		Source = SourceHolder.c_str();
		StrSuccess = Compile();
	}

	shader::~shader()
	{
		delete_();
	}

	StrError shader::Compile()
	{
		
		glShaderSource(ShaderID, 1, &Source, 0); // Tell opengl member Source is the source code of this shader
		glCompileShader(ShaderID); // Compile shader

		GLint Success = false;
		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Success); // See if it was a success
		if (!Success)
		{
			std::vector<char> InfoLog(512);
			glGetShaderInfoLog(ShaderID, 512, NULL, InfoLog.begin()._Ptr);
			return std::string().append(InfoLog.begin(), InfoLog.end());
		}

		return STR_SUCCESS;
	}

	GLuint shader::ID() 
	{
		return ShaderID;
	}

	void shader::delete_()
	{
		glDeleteShader(ShaderID);
	}

	std::string* shader::_GetSourceHolder()
	{
		return &SourceHolder;
	}

	const char* shader::_GetSource()
	{
		return Source;
	}
}