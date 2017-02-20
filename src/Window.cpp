#ifdef WIN32
#	include <SDL.h>
#else
#	include <SDL2/SDL.h>
#endif

#include "exported/Window.hpp"

namespace csaru {
namespace xapp {

//======================================================================
Window::~Window () {
	Destroy();
}

//======================================================================
void Window::Clear () {
	SDL_RenderClear(m_renderer);
}

//======================================================================
void Window::Destroy () {
	if (m_renderer)
		SDL_DestroyRenderer(m_renderer);
	if (m_window)
		SDL_DestroyWindow(m_window);

	m_renderer   = nullptr;
	m_window     = nullptr;
	m_areaWidth  = 0;
	m_areaHeight = 0;
}

//======================================================================
void Window::HandleEvent (const SDL_Event & e) {
	switch (e.window.event) {
		case SDL_WINDOWEVENT_CLOSE: {
			Destroy();
			break;
		}
	}
}

//======================================================================
bool Window::Init (const char * title, uint32_t width, uint32_t height) {
	Destroy();

	// create window
	m_window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN
	);
	if (!m_window) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to create a window.  %s\n", SDL_GetError());
		return false;
	}

	// create renderer
	m_renderer = SDL_CreateRenderer(
		m_window,
		-1, // rendering driver index; -1 for first-available
		SDL_RENDERER_ACCELERATED
	);
	if (!m_renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to create a renderer.  %s\n", SDL_GetError());
		return false;
	}

	// Set the clear color to some arbitrary "see this color? you may
	// want to change it, this is the default" color.
	SDL_SetRenderDrawColor(m_renderer, 0x34, 0xE3, 0xFF, 0xFF);

	m_areaWidth  = width;
	m_areaHeight = height;
	return true;
}

//======================================================================
void Window::Render () {
	SDL_RenderPresent(m_renderer);
}

} // namespace xapp
} // namespace csaru

