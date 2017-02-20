#ifdef WIN32
#	include <SDL_ttf.h>
#else
#	include <SDL2/SDL_ttf.h>
#endif

#include "exported/SDL_RWopsPhysicsFS.hpp"
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
void Window::DebugPrint (const char * message) {
	SDL_Log(message);

	if (!m_debugFont)
		return;

	DebugText line;
	line.framesRemaining = 60 * 3;
	line.text    = message;
	line.surface = TTF_RenderText_Solid(m_debugFont, line.text.c_str(), SDL_Color{0xFF, 0x00, 0xFF, 0xFF});
	if (!line.surface) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to render debug text {%s} to SDL surface.  %s\n", message, TTF_GetError());
		return;
	}

	m_debugMessages.push_back(std::move(line));
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

	// set the clear color to some arbitrary, inoffensive default
	SDL_SetRenderDrawColor(m_renderer, 0x20, 0x20, 0x20, 0xFF);

	m_areaWidth  = width;
	m_areaHeight = height;
	return true;
}

//======================================================================
void Window::Render () {
	// render debug text
	{
		SDL_Surface * windowSurface = SDL_GetWindowSurface(m_window);
		SDL_Rect      windowRect;
		SDL_GetClipRect(windowSurface, &windowRect);
		for (auto && message : m_debugMessages) {
			SDL_BlitSurface(message.surface, nullptr, windowSurface, nullptr);
			SDL_Rect textRect;
			SDL_GetClipRect(message.surface, &textRect);
			windowRect.y += textRect.y;
			--message.framesRemaining;
			SDL_Log("Drew {%s}.\n", message.text.c_str());
		}

		// delete any debug text that's out of time
		for (int32_t i = m_debugMessages.size() - 1; i >= 0; --i) {
			auto && message = m_debugMessages[i];
			if (message.framesRemaining <= 0) {
				SDL_FreeSurface(message.surface);
				m_debugMessages.erase(m_debugMessages.begin() + i);
			}
		}
	}

	// TODO : Switch to using SDL_Texture from SDL_Surface for everything.
	SDL_UpdateWindowSurface(m_window);
	//SDL_RenderPresent(m_renderer);
}

//======================================================================
void Window::SetDebugFont (TTF_Font * font) {
	m_debugFont = font;
}

} // namespace xapp
} // namespace csaru

