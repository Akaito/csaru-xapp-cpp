#pragma once

#include <vector>


namespace csaru {
namespace xapp {

// forward declares
class Window;


class Application {
private:
	std::vector<Window *> m_windows;
	bool                  m_isQuitting = false;

	void HandleWindowEvent (const SDL_Event & e);

public:
	virtual ~Application ();

	bool Init ();
	void Close ();

	void PollEvents ();

	void AddWindow (Window * window);
	bool HasOpenWindows () { return !m_windows.empty(); }
	bool IsQuitting ()     { return m_isQuitting; }
};

} // namespace xapp
} // namespace csaru

