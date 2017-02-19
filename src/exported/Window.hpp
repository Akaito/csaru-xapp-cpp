#pragma once

#include <stdint.h>

// SDL forward declares
struct SDL_Window;
struct SDL_Renderer;

namespace csaru {
namespace xapp {

class Window {
private:
	uint32_t       m_areaWidth  = 0;
	uint32_t       m_areaHeight = 0;
	SDL_Window *   m_window     = nullptr;
	SDL_Renderer * m_renderer   = nullptr;

public:
	virtual ~Window ();

	bool Init (const char * title, uint32_t width, uint32_t height);
	void Destroy ();

	void SendQuit ();
	void Clear ();
	void Render ();

	bool IsAlive ()            { return m_window != nullptr; }

	operator SDL_Window * ()   { return m_window; }
	operator SDL_Renderer * () { return m_renderer; }
};

} // namespace xapp
} // namespace csaru

