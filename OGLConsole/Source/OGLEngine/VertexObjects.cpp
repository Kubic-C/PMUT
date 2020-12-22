#include "Headers/VertexObjects.h"

namespace OGL
{
	VBO::VBO()
		: Type(GL_ARRAY_BUFFER), ID(0)
	{
		glGenBuffers(1, &ID); // init. the VBO
	}

	VBO::VBO(bind_on_start)
		: Type(GL_ARRAY_BUFFER), ID(0)
	{
		glGenBuffers(1, &ID); // init. the VBO
		bind(); 
	}

	VBO::VBO(GLenum Target)
		: Type(Target), ID(0)
	{
		glGenBuffers(1, &ID); // init. the VBO
	}

	VBO::VBO(GLenum Target, bind_on_start)
		: Type(Target), ID(0)
	{
		glGenBuffers(1, &ID); // init. the VBO
		bind();
	}

	VBO::VBO(GLsizeiptr Size, void* PtrToData, GLenum Usage)
		: Type(GL_ARRAY_BUFFER), ID(0)
	{
		glGenBuffers(1, &ID); // init. the VBO
		buffer_data(Size, PtrToData, Usage);
	}

	VBO::VBO(GLenum Target, GLsizeiptr Size, void* PtrToData, GLenum Usage)
		: Type(Target), ID(0)
	{
		glGenBuffers(1, &ID); // init. the VBO
		buffer_data(Size, PtrToData, Usage);
	}

	VBO::VBO(GLenum Target, GLsizeiptr Size, void* PtrToData, GLenum Usage, bind_on_start)
	{
		glGenBuffers(1, &ID); // init. the VBO
		buffer_data(Size, PtrToData, Usage, NoUnbind());
		bind();
	}

	VBO::~VBO()
	{
		glDeleteBuffers(1, &ID);
	}

	void VBO::bind()
	{
		glBindBuffer(Type, ID);
	}

	void VBO::buffer_data(GLsizeiptr Size, void* PtrToData, GLenum Usage)
	{
		bind();
		glBufferData(Type, Size, PtrToData, Usage);
		unbind_buffer();
	}

	void VBO::buffer_data(GLsizeiptr Size, void* PtrToData, GLenum Usage, NoUnbind)
	{
		bind();
		glBufferData(Type, Size, PtrToData, Usage);
	}

	void VBO::set_target(GLenum target)
	{
		Type = target;
	}

	void VBO::update_mem(GLsizeiptr size, void* ptr_to_data)
	{
		bind();
		glBufferSubDataARB(Type, 0, size, ptr_to_data);
		unbind_buffer();
	}

	inline void unbind_buffer()
	{
		// the target param does not matter since were are binding it nothing
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void unbind_array()
	{
		glBindVertexArray(0);
	}

	void SetBufferDataIntoBoundVAO(GLuint Index, GLint Size, GLenum typename_, GLenum Norm, GLint Stride, const void* Start)
	{
		glVertexAttribPointer(Index, Size, typename_, Norm, Stride, Start);
		glEnableVertexAttribArray(Index);
	}

	void SetVboToDefault(VBO& VBO_)
	{
		VBO_.buffer_data(sizeof(Triangle), Triangle, GL_STATIC_DRAW, NoUnbind());
		SetBufferDataIntoBoundVAO(0, 3, GL_FLOAT, GL_FALSE, sizeof(Triangle) / 3, nullptr/*<-begginning of array*/);
	}

	VAO::VAO()
	{
		generate();
	}

	VAO::VAO(bind_on_start)
	{
		generate();
		bind();
	}

	VAO::~VAO()
	{
		glDeleteVertexArrays(1, &ID);
	}

	void VAO::generate()
	{
		glGenVertexArrays(1, &ID);
	}

	void VAO::bind()
	{
		glBindVertexArray(ID);
	}

	void VAO::set_buffer(GLuint Index, GLint Size, GLenum typename_, GLenum Norm, GLint Stride, const void* Start)
	{
		bind();
		glVertexAttribPointer(Index, Size, typename_, Norm, Stride, Start);
		glEnableVertexAttribArray(Index);
		unbind_array();
	}
}