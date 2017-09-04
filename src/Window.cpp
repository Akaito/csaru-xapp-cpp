#ifdef _WIN32
#	include <SDL_ttf.h>
#else
#	include <SDL2/SDL_ttf.h>
#endif

#include <csaru-loaders/everything.hpp>
#include "exported/Window.hpp"

extern int SDL_LOG_CATEGORY_CSARU_XAPP;

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
	SDL_LogInfo(SDL_LOG_CATEGORY_CSARU_XAPP, "%s", message);

	if (!m_debugFont)
		return;

	DebugText line;
	line.framesRemaining = 60 * 2;
	line.text            = message;

	SDL_Surface * textSurface = TTF_RenderText_Solid(m_debugFont, line.text.c_str(), SDL_Color{0xFF, 0x00, 0xFF, 0xFF});
	if (!textSurface) {
		SDL_LogError(SDL_LOG_CATEGORY_CSARU_XAPP, "Failed to render debug text {%s} to SDL surface.  %s\n", message, TTF_GetError());
		return;
	}
	line.texture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
	SDL_FreeSurface(textSurface);
	if (!line.texture) {
		SDL_LogError(SDL_LOG_CATEGORY_CSARU_XAPP, "Failed to converte debug text {%s} surface to texture.  %s\n", message, SDL_GetError());
		return;
	}
	uint32_t format = 0; // don't actually care about this
	int      access = 0; // or this
	if (SDL_QueryTexture(line.texture, &format, &access, &line.textureRect.w, &line.textureRect.h)) {
		SDL_LogError(SDL_LOG_CATEGORY_CSARU_XAPP, "Failed to get debug text {%s} texture dimensions.  %s\n", message, SDL_GetError());
		return;
	}

	m_debugMessages.push_back(std::move(line));
}

//======================================================================
void Window::Destroy () {
	for (auto && message : m_debugMessages)
		SDL_DestroyTexture(message.texture);
	m_debugMessages.clear();

	if (m_renderer) {
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
	if (m_window) {
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}

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
		SDL_LogError(SDL_LOG_CATEGORY_CSARU_XAPP, "SDL failed to create a window.  %s\n", SDL_GetError());
		return false;
	}

	// create renderer
	m_renderer = SDL_CreateRenderer(
		m_window,
		-1, // rendering driver index; -1 for first-available
		SDL_RENDERER_ACCELERATED
	);
	if (!m_renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_CSARU_XAPP, "SDL failed to create a renderer.  %s\n", SDL_GetError());
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
		int yOffset = 0;
		for (auto && message : m_debugMessages) {
			SDL_Rect destRect = message.textureRect;
			destRect.y += yOffset;
			SDL_RenderCopy(m_renderer, message.texture, nullptr, &destRect);

			yOffset += message.textureRect.h;
			--message.framesRemaining;
		}

		// delete any debug text that's out of time
		for (int32_t i = m_debugMessages.size() - 1; i >= 0; --i) {
			auto && message = m_debugMessages[i];
			if (message.framesRemaining <= 0) {
				SDL_DestroyTexture(message.texture);
				m_debugMessages.erase(m_debugMessages.begin() + i);
			}
		}
	}

	SDL_RenderPresent(m_renderer);
}

//======================================================================
void Window::SetDebugFont (TTF_Font * font) {
	m_debugFont = font;
}

} // namespace xapp
} // namespace csaru

