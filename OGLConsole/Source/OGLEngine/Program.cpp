#include "Headers/Program.h"

namespace OGL
{
	void program::Attach(shader& Shader_)
	{
		glAttachShader(ProgramID, Shader_.ID());
	}

	StrError program::Link()
	{
		glLinkProgram(ProgramID);
		// check for linking errors
		int Success = false;
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			std::vector<char> InfoLog(512);
			glGetProgramInfoLog(ProgramID, 512, NULL, InfoLog.begin()._Ptr);
			return std::string().append(InfoLog.begin(), InfoLog.end());
		}

		return STR_SUCCESS;
	}

	void program::use()
	{
		glUseProgram(ProgramID);
	}

	void program::set_sampler_two_dim(std::string name, int slot)
	{
		glUniform1i(glGetUniformLocation(ProgramID, name.c_str()), slot); // set it manually
	}

	void program::set_vec4(std::string name, float v1, float v2, float v3, float v4)
	{
		glUniform4f(glGetUniformLocation(ProgramID, name.c_str()), v1, v2, v3, v4);
	}

	void program::set_vec3(std::string name, float v1, float v2, float v3)
	{
		glUniform3f(glGetUniformLocation(ProgramID, name.c_str()), v1, v2, v3);
	}
}