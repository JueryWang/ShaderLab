#pragma once
#include <string>

namespace SDL_UTILS
{
	enum SDL_CONTEXT_STATUS_BIT
	{
		ArchiveUpdateBit = 0x0001,
		LoggInBit = 0x0002,
	};

	extern std::string author;
	
	extern unsigned int sdl_context;
}
