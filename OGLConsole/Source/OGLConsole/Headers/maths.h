#pragma once
#ifndef CONSOLE_INPUT_H
#define CONSOLE_INPUT_H

#include "PCH\OGLPCH.h"
#include "../OGLEngine/Headers/Engine.h"

#define FLOAT_MAX ~(float(0.0f))
#define cast(typename_, value) static_cast<typename_>(value)
#define unsafe_cast(typename_, value) reinterpret_cast<typename_>(value)

namespace console
{
	struct vec2
	{
		float x, y;

		vec2(float x, float y);
		std::vector<float> unpack();
	};

	struct vec3
	{
		float x, y, z;

		vec3(float x, float y, float z);
		std::vector<float> unpack();
	};

	struct vec4
	{
		float x, y, z, w;

		vec4(float x, float y, float z, float w);
		std::vector<float> unpack();
	};

	struct vertex
	{
		float pos[3];
		float rgb[3];
		float tex_coords[2];
	};

	// has pos, rgb, and tex_coords for quad
	struct quad
	{
		float pos[12];       // vec3 pos
		float rgb[12];       // vec3 rgb
		float tex_coords[8]; // vec2 tex coords
	};

	// translate a quad!
	void translate_quad(quad& quad, vec3 ve3);

	// add a quad to float vector
	void add_quad_to_float_vector(quad& quad, std::vector<float>& vector);

	// add a quad to float array
	void add_quad_to_float_array(quad& quad, float* vector);

	// change the rgb value of a quad to reflect the rgb values in the parameter
	void set_rgb_value_for_quad(quad& quad, vec3 rgb);

	// misc.

	inline int passive_stoi(const std::string& _Str, size_t* _Idx = nullptr, int _Base = 10) {
		// convert string to int
		int& _Errno_ref = errno; // Nonzero cost, pay it once
		const char* _Ptr = _Str.c_str();
		char* _Eptr;
		_Errno_ref = 0;
		const long _Ans = _CSTD strtol(_Ptr, &_Eptr, _Base);

		if (_Ptr == _Eptr) {
			return INT_MAX;
		}

		if (_Errno_ref == ERANGE || _Ans < INT_MIN || INT_MAX < _Ans) {
			return INT_MAX;
		}

		if (_Idx) {
			*_Idx = static_cast<size_t>(_Eptr - _Ptr);
		}

		return static_cast<int>(_Ans);
	}
}

inline float passie_stof(const std::string& _Str, size_t* _Idx = nullptr) { // convert string to float
	int& _Errno_ref = errno; // Nonzero cost, pay it once
	const char* _Ptr = _Str.c_str();
	char* _Eptr;
	_Errno_ref = 0;
	const float _Ans = _CSTD strtof(_Ptr, &_Eptr);

	if (_Ptr == _Eptr) {
		return cast(float, INT_MAX);
	}

	if (_Errno_ref == ERANGE) {
		return cast(float, INT_MAX);
	}

	if (_Idx) {
		*_Idx = static_cast<size_t>(_Eptr - _Ptr);
	}

	return _Ans;
}

#endif // CONSOLE_INPUT_H