#include <cstdio>

#ifdef WIN32
#	include <SDL.h>
#	include <SDL_ttf.h>
#else
#	include <SDL2/SDL.h>
#	include <SDL2/SDL_ttf.h>
#endif

#include <physfs.h>

#include <csaru-core-cpp/csaru-core-cpp.hpp> // unused()

#include "exported/Application.hpp"
#include "exported/SDL_RWopsPhysicsFS.hpp"
#include "exported/Window.hpp"

namespace csaru {
namespace xapp {

//======================================================================
Application::~Application () {
	Close();
}

//======================================================================
void Application::AddWindow (Window * window) {
	m_windows.push_back(window);
	window->SetDebugFont(m_debugFont);
}

//======================================================================
void Application::ClearWindows () {
	for (auto && window : m_windows) {
		window->Clear();
	}
}

//======================================================================
void Application::Close () {
	for (auto && window : m_windows) {
		window->Destroy();
	}
	m_windows.clear();

	for (auto fontIter : m_fonts) {
		TTF_CloseFont(fontIter.second);
	}
	m_fonts.clear();

	if (!PHYSFS_deinit()) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "PhysicsFS couldn't deinit properly.  Open files refusing to close?");
	}

	TTF_Quit();
	SDL_Quit();
}

//======================================================================
bool Application::TryHandleWindowEvent (const SDL_Event & e) {
	uint32_t eventWindowId = 0;
	switch (e.type) {
		case SDL_MOUSEBUTTONUP:   eventWindowId = e.button.windowID; break;
		case SDL_MOUSEBUTTONDOWN: eventWindowId = e.button.windowID; break;
		case SDL_MOUSEMOTION:     eventWindowId = e.motion.windowID; break;
		case SDL_WINDOWEVENT:     eventWindowId = e.window.windowID; break;
		default: return false;
	}

	for (auto && window : m_windows) {
		uint32_t windowId = SDL_GetWindowID(window->SdlWindow());
		if (windowId != eventWindowId)
			continue;
		window->HandleEvent(e);
		return true;
	}
	//SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_WINDOWEVENT windowID {%u} unmatched in Application.", e.window.windowID);
	return false;
}

//======================================================================
bool Application::Init (int argc, const char * const * argv) {
	unused(argc);

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::fprintf(stderr, "SDL failed to initialize.  %s\n", SDL_GetError());
		return false;
	}

	// SDL_ttf
	if (TTF_Init() == -1) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_ttf failed to initialize.  %s\n", TTF_GetError());
		return false;
	}

	// PhysicsFS
	if (!PHYSFS_init(argv[0])) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "PhysicsFS failed to initialize.  %s\n", PHYSFS_getLastError());
		return false;
	}

	return true;
}

//======================================================================
void Application::PollEvents () {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
			case SDL_APP_TERMINATING:
				m_isQuitting = true;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
					// hacky quick quit key while testing
					case SDLK_ESCAPE: m_isQuitting = true; break;
				} // end keydown keysym switch
				//break; // fall-through
			default:
				TryHandleWindowEvent(e);
		} // end event type switch
	} // end event loop
}

//======================================================================
void Application::RenderWindows () {
	for (auto && window : m_windows) {
		window->Render();
	}
}

//======================================================================
void Application::SetFont (const char * key, TTF_Font * font) {
	// If setting to null, don't free the font.  Assume user is doing that.
	if (!font) {
		if (m_debugFont == m_fonts.at(key))
			m_debugFont = nullptr;
		m_fonts.erase(key);
		return;
	}

	if (m_fonts.count(key) && m_debugFont == m_fonts.at(key))
		m_debugFont = font;
	m_fonts[key] = font;
}

//======================================================================
void Application::SetDebugFont (const char * key) {
	m_debugFont = m_fonts.at(key);
	for (auto && window : m_windows) {
		window->SetDebugFont(m_debugFont);
	}
}

} // namespace xapp
} // namespace csaru

