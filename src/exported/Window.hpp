#pragma once

#include <stdint.h>

#ifdef WIN32
#	include <SDL.h> // SDL_Event couldn't be forward-declared.
#else
#	include <SDL2/SDL.h>
#endif

namespace csaru {
namespace xapp {

class Window {
private:
	uint32_t       m_areaWidth  = 0;
	uint32_t       m_areaHeight = 0;
	SDL_Window *   m_window     = nullptr;
	SDL_Renderer * m_renderer   = nullptr;

	bool SetupDebugFont (const char * path);

public:
	virtual ~Window ();

	bool Init (const char * title, uint32_t width, uint32_t height);
	void Destroy ();

	void HandleEvent (const SDL_Event & e);
	void Clear ();
	void Render ();

	void DebugPrint (const char * message);

	bool IsAlive ()               { return m_window != nullptr; }

	SDL_Window * SdlWindow ()     { return m_window; }
	SDL_Renderer * SdlRenderer () { return m_renderer; }
};

} // namespace xapp
} // namespace csaru

