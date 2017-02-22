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

class Window {
private:
	struct DebugText {
		SDL_Texture * texture         = nullptr;
		uint32_t      framesRemaining = 0;
		SDL_Rect      textureRect     = {0, 0, 0, 0};
		std::string   text;
	};

	uint32_t               m_areaWidth  = 0;
	uint32_t               m_areaHeight = 0;
	SDL_Window *           m_window     = nullptr;
	SDL_Renderer *         m_renderer   = nullptr;
	TTF_Font *             m_debugFont  = nullptr;
	std::vector<DebugText> m_debugMessages;

public:
	virtual ~Window ();

	virtual bool Init (const char * title, uint32_t width, uint32_t height);
	virtual void Destroy ();

	virtual void HandleEvent (const SDL_Event & e);
	virtual void Clear ();
	virtual void Render ();

	void SetDebugFont (TTF_Font * font);
	void DebugPrint (const char * message);

	bool IsAlive ()               { return m_window != nullptr; }

	SDL_Window * SdlWindow ()     { return m_window; }
	SDL_Renderer * SdlRenderer () { return m_renderer; }
};

} // namespace xapp
} // namespace csaru

