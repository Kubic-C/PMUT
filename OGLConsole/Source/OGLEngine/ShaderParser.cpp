#include "Headers/ShaderParser.h"

namespace OGL
{
	std::string GetExeDir()
	{
#ifdef _WINDOWS_ 
		char result[MAX_PATH];
		return std::string(result, GetModuleFileName(NULL, (wchar_t*)*result, MAX_PATH));

#elif defined(LINUX_)
		char result[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
		return std::string(result, (count > 0) ? count : 0);
#else
		return "UKNOWN SYSTEM";
#endif
	}
}