#include "exported/WindowOpenGl.hpp"

#ifdef WIN32
#	include <SDL_opengl.h>
#else
#	include <SDL2/SDL_opengl.h>
#endif

namespace csaru {
namespace xapp {

//======================================================================
WindowOpenGl::~WindowOpenGl () {
	Destroy();
}

//======================================================================
void WindowOpenGl::Clear () {
	glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);
}

//======================================================================
void WindowOpenGl::Destroy () {
	csaru::xapp::Window::Destroy();
}

//======================================================================
bool WindowOpenGl::Init (const char * title, uint32_t width, uint32_t height) {
	Destroy();

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_DisplayMode currentDisplayMode;
	SDL_GetCurrentDisplayMode(0, &currentDisplayMode);

	// create window
	m_window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	);
	if (!m_window) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to create a window.  %s\n", SDL_GetError());
		return false;
	}

	m_glContext = SDL_GL_CreateContext(m_window);
	if (!m_glContext) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to create a GL context.  %s\n", SDL_GetError());
		return false;
	}

	m_areaWidth  = width;
	m_areaHeight = height;
	return true;
}

//======================================================================
void WindowOpenGl::Render () {
	SDL_GL_SwapWindow(m_window);
}

//======================================================================
void WindowOpenGl::SetClearColor (float r, float g, float b, float a) {
	m_clearColor[0] = r;
	m_clearColor[1] = g;
	m_clearColor[2] = b;
	m_clearColor[3] = a;
}

} // namespace csaru
} // namespace xapp

