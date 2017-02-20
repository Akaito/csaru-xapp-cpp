#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#ifdef WIN32
#	include <SDL.h> // SDL_Event couldn't be reasonably forward-declared.
#else
#	include <SDL2/SDL.h>
#endif

// SDL_ttf font object forward declare
struct _TTF_Font;
typedef _TTF_Font TTF_Font;


namespace csaru {
namespace xapp {

struct DebugText {
	SDL_Surface * surface         = nullptr;
	uint32_t      framesRemaining = 0;
	std::string   text;
};


class Window {
private:
	uint32_t               m_areaWidth  = 0;
	uint32_t               m_areaHeight = 0;
	SDL_Window *           m_window     = nullptr;
	SDL_Renderer *         m_renderer   = nullptr;
	TTF_Font *             m_debugFont  = nullptr;
	std::vector<DebugText> m_debugMessages;

public:
	virtual ~Window ();

	bool Init (const char * title, uint32_t width, uint32_t height);
	void Destroy ();

	void HandleEvent (const SDL_Event & e);
	void Clear ();
	void Render ();

	void SetDebugFont (TTF_Font * font);
	void DebugPrint (const char * message);

	bool IsAlive ()               { return m_window != nullptr; }

	SDL_Window * SdlWindow ()     { return m_window; }
	SDL_Renderer * SdlRenderer () { return m_renderer; }
};

} // namespace xapp
} // namespace csaru

