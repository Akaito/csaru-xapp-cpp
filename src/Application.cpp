#include <cstdio>

#ifdef WIN32
#	include <SDL.h>
#else
#	include <SDL2/SDL.h>
#endif

#include "exported/Application.hpp"
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
}

//======================================================================
void Application::Close () {
	for (auto && window : m_windows) {
		window->Destroy();
	}
	m_windows.clear();

	SDL_Quit();
}

//======================================================================
bool Application::Init () {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::fprintf(stderr, "SDL failed to initialize.  %s\n", SDL_GetError());
		return false;
	}

	return true;
}

//======================================================================
void Application::PollEvents () {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT || e.type == SDL_APP_TERMINATING) {
			m_isQuitting = true;
			break;
		}
	}
}

} // namespace xapp
} // namespace csaru

