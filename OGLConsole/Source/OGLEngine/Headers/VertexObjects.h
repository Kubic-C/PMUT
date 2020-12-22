// File meant for classes VAO(Vertex Array Object), and VBO(Vertex Buffer object)
#pragma once
#ifndef VERTEX_OBJECTS_H
#define VERTEX_OBJECTS_H

#include "PCH/OGLPCH.h"

namespace OGL
{
	// 2D Triangle
	static float Triangle[]
	{
		-0.5f, -0.5f, 0.f,
		 0.5f, -0.5f, 0.f,
		 0.5f, 0.5f, 0.f
	};

	// For the world's coolest tag dispatching structure, we bring you BindOnStart
	struct bind_on_start {};
	struct NoUnbind {};

	// Unbnd the vertex buffer object
	void unbind_buffer();

	// Unbind the vertex array object
	void unbind_array();

	// Set buffer data into the bound VAO
	void SetBufferDataIntoBoundVAO(GLuint Index, GLint Size, GLenum typename_, GLenum Norm, GLint Stride, const void* Start);

	class VBO;

	void SetVboToDefault(VBO& VBO_);

	template<typename T>
	inline int size_of_vector_elements(int amount_of_elements)
	{
		return sizeof(T) * amount_of_elements;
	}

	// NOTE : VBO is short for Vertex Buffer Object
	class VBO
	{
	public:
		VBO(); // default target/type is GL_VERTEX_ARRAY
		VBO(bind_on_start); // Binds as soon as it is created
		VBO(GLenum Target); // default target/type is GL_VERTEX_ARRAY
		VBO(GLenum Target, bind_on_start); // Binds as soon as it is created
		VBO(GLsizeiptr Size, void* PtrToData, GLenum Usage); // target/type is GL_VERTEX_ARRAY
		VBO(GLenum Target, GLsizeiptr Size, void* PtrToData, GLenum Usage);
		VBO(GLenum Target, GLsizeiptr Size, void* PtrToData, GLenum Usage, bind_on_start);
		~VBO();


		void bind();
		void buffer_data(GLsizeiptr size, void* PtrToData, GLenum Usage);
		void buffer_data(GLsizeiptr size, void* PtrToData, GLenum Usage, NoUnbind);
		void set_target(GLenum target);
		// The buffer data has to be set
		void update_mem(GLsizeiptr size, void* PtrToData);

	protected:
		GLuint ID;
		GLenum Type;
	};

	// NOTE : VAO is short for Vertex Array Object
	class VAO
	{
	public:
		VAO();
		VAO(bind_on_start); // Binds as soon as it is created
		~VAO();

		void generate();
		void bind();
		// If typename_(the parameter) is other then float(GL_FLOAT), Norm parameter should be GL_TRUE
		void set_buffer(GLuint Index, GLint Size, GLenum typename_, GLenum Norm, GLint Stride, const void* Start);

	protected:
		GLuint ID;
	};
}

#endif