#pragma once

#include <unordered_map>
#include <vector>

#ifdef WIN32
#	include <SDL.h> // SDL_Event couldn't be reasonably forward-declared.
#else
#	include <SDL2/SDL.h>
#endif

namespace csaru {
namespace xapp {

// forward declares
class Window;


class Application {
protected:
	std::vector<Window *>                        m_windows;
	std::unordered_map<const char *, TTF_Font *> m_fonts; // Could be better placed.  Works fine for now.
	bool                                         m_isQuitting = false;
	TTF_Font *                                   m_debugFont  = nullptr;

	bool TryHandleWindowEvent (const SDL_Event & e);

public:
	virtual ~Application ();

	virtual bool Init (int argc, const char * const * argv); // Call this Init from your own first!
	virtual void Close (); // Call this one *after* yours.

	// Suggest copy-pasting this one's contents into yours to start.
	// Don't call this from yours!
	virtual void PollEvents ();

	void AddWindow (Window * window);
	bool HasOpenWindows () { return !m_windows.empty(); }
	bool IsQuitting ()     { return m_isQuitting; }

	void SetFont (const char * key, TTF_Font * font); // Application Close destroys these.  Setting to nullptr doesn't.
	void SetDebugFont (const char * key);

	// Helpers for simple applications.
	void ClearWindows ();
	void RenderWindows ();
};

} // namespace xapp
} // namespace csaru

