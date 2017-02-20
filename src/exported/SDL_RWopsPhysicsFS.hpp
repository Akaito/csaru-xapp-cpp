#pragma once

// TODO : Can we just forward-declare SDL_RWops?
#ifdef WIN32
#	include <SDL.h>
#else
#	include <SDL2/SDL.h>
#endif

// SDL_RWops which allows SDL to transparently
// read/write using PhysicsFS.

namespace csaru {
namespace xapp {

// You MUST call SDL_RWclose() with this to allow it to
// both PHYSFS_close() and SDL_FreeRW()!
// DO *NOT* SDL_FreeRW() this yourself!
//
// path: platform-independent, PhysicsFS notation.
// mode: Any one of 'r', 'w', 'a' (read, write, append).
//
// returns nullptr on error (check SDL_GetError()).
SDL_RWops * AllocRwOpsPhysFs (const char * path, const char mode);

} // namespace xapp
} // namespace csaru

