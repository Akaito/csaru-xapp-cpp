#pragma once

#include <unordered_map>
#include <vector>

// SDL_ttf font object forward declare
struct _TTF_Font;
typedef _TTF_Font TTF_Font;

namespace csaru {
namespace xapp {

// forward declares
class Window;


class Application {
private:
	std::vector<Window *>                        m_windows;
	std::unordered_map<const char *, TTF_Font *> m_fonts; // Could be better placed.  Works fine for now.
	bool                                         m_isQuitting = false;
	TTF_Font *                                   m_debugFont  = nullptr;

	void HandleWindowEvent (const SDL_Event & e);

public:
	virtual ~Application ();

	bool Init (int argc, const char * const * argv);
	void Close ();

	void PollEvents ();

	void AddWindow (Window * window);
	bool HasOpenWindows () { return !m_windows.empty(); }
	bool IsQuitting ()     { return m_isQuitting; }

	void SetFont (const char * key, TTF_Font * font); // Application Close destroys these.  Setting to nullptr doesn't.
	void SetDebugFont (const char * key);
};

} // namespace xapp
} // namespace csaru

