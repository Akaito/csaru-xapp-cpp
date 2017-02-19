#pragma once

#include <vector>


namespace csaru {
namespace xapp {

// forward declares
class Window;


class Application {
private:
	std::vector<Window *> m_windows;

public:
	virtual ~Application ();

	bool Init ();
	void Close ();

	void AddWindow (Window * window);

	bool HasOpenWindows () { return !m_windows.empty(); }
};

} // namespace xapp
} // namespace csaru

