#pragma once

#include "Window.hpp"

typedef void * SDL_GLContext;

namespace csaru {
namespace xapp {

class WindowOpenGl : public Window {
protected:
	SDL_GLContext m_glContext = nullptr;
	float         m_clearColor[4];

	void SetClearColor (float r, float g, float b, float a);

public:
	virtual ~WindowOpenGl ();

	SDL_GLContext SdlGlContext () { return m_glContext; }

public: // csaru::xapp::Window
	virtual bool Init (const char * title, uint32_t width, uint32_t height) override;
	virtual void Destroy () override;
	virtual void Clear () override;
	virtual void Render () override;
};

} // namespace xapp
} // namespace csaru

