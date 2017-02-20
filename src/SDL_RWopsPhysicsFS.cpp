#include "exported/SDL_RWopsPhysicsFS.hpp"

#include <cstddef> // size_t

#include <physfs.h>


namespace {

//==============================================================================
int SDLCALL close (SDL_RWops * context) {

	PHYSFS_File * file = reinterpret_cast<PHYSFS_File *>(context->hidden.unknown.data1);

	const int physFsCloseResult = PHYSFS_close(file);
	SDL_assert_paranoid(physFsCloseResult);
	if (!PHYSFS_close(file)) {
		SDL_SetError(
			"Failed to close PhysicsFS file; handle remains open: %s",
			PHYSFS_getLastError()
		);
	}

	SDL_FreeRW(context);
	// return 0 on success; -1 if failed to flush to disk, etc.
	return physFsCloseResult == 0 ? -1 : 0;

}

//==============================================================================
std::size_t SDLCALL read (
	SDL_RWops * context,
	void *      ptr,
	std::size_t size,
	std::size_t maxnum
) {

	PHYSFS_File * file = reinterpret_cast<PHYSFS_File *>(context->hidden.unknown.data1);

	// Return 0 if at EOF.
	if (PHYSFS_eof(file))
		return 0;

	PHYSFS_sint64 readResult = PHYSFS_read(
		file,
		ptr    /* buffer */,
		size   /* object size */,
		maxnum /* object count */
	);

	// PHYSFS_read returns -1 on complete failure.
	SDL_assert_paranoid(readResult != -1);
	if (readResult == -1) {
		SDL_SetError(
			"RWops read complete failure from PhysFS: %s",
			PHYSFS_getLastError()
		);
		return 0;
	}

	return static_cast<std::size_t>(readResult);

}

//==============================================================================
Sint64 SDLCALL seek (SDL_RWops * context, Sint64 offset, int whence) {

	SDL_assert(whence == RW_SEEK_SET || whence == RW_SEEK_CUR || whence == RW_SEEK_END);

	PHYSFS_File * file = reinterpret_cast<PHYSFS_File *>(context->hidden.unknown.data1);

	PHYSFS_uint64 targetPos = static_cast<PHYSFS_uint64>(-1);
	switch (whence) {
		case RW_SEEK_SET:
			targetPos = static_cast<PHYSFS_uint64>(offset);
			break;

		case RW_SEEK_CUR: {
			PHYSFS_sint64 curFilePos = PHYSFS_tell(file);
			SDL_assert_paranoid(curFilePos != -1);
			if (curFilePos == -1) {
				SDL_SetError(
					"RWops PhysicsFS seek-cur error getting pos: %s",
					PHYSFS_getLastError()
				);
				return -1;
			}
			targetPos = static_cast<PHYSFS_uint64>(curFilePos + offset);
		} break;

		case RW_SEEK_END: {
			PHYSFS_sint64 endPos = PHYSFS_fileLength(file);
			SDL_assert_paranoid(endPos != -1);
			if (endPos == -1) {
				SDL_SetError(
					"RWops PhysicsFS seek-end error getting file length; streaming archive?: %s",
					PHYSFS_getLastError()
				);
				return -1;
			}
			targetPos = static_cast<PHYSFS_uint64>(endPos + offset);
		} break;

		default:
			SDL_SetError(
				"Invalid SDL_RWops seek whence {%d}.  (PhysicsFS RWops).  "
					"Use RW_SEEK_SET, RW_SEEK_CUR, or RW_SEEK_END (if you're feeling brave).  "
					"These come from #include <cstdio>.",
				whence
			);
			return -1;
			break;
	}

	int seekResult = PHYSFS_seek(file, targetPos);
	SDL_assert_paranoid(seekResult);
	if (!seekResult) {
		SDL_SetError("SDL_RWops for PhysicsFS error seeking: %s", PHYSFS_getLastError());
		return -1;
	}

	return static_cast<Sint64>(targetPos); // note uint64 -> int64

}

//==============================================================================
Sint64 SDLCALL size (SDL_RWops * context) {

	PHYSFS_File * file = reinterpret_cast<PHYSFS_File *>(context->hidden.unknown.data1);

	PHYSFS_sint64 length = PHYSFS_fileLength(file);
	SDL_assert_paranoid(length != -1);
	if (length == -1) {
		SDL_SetError(
			"RWops PhysicsFS size error getting file length; streaming archive?: %s",
			PHYSFS_getLastError()
		);
		return -1;
	}

	return length;

}

//==============================================================================
std::size_t SDLCALL write (
	SDL_RWops *  context,
	const void * ptr,
	std::size_t  size,
	std::size_t  num
) {

	PHYSFS_File * file = reinterpret_cast<PHYSFS_File *>(context->hidden.unknown.data1);

	// TODO : Should we return 0 from here if already at EOF?
	// http://hg.libsdl.org/SDL/file/default/include/SDL_rwops.h
	// https://wiki.libsdl.org/SDL_RWops

	PHYSFS_sint64 writeResult = PHYSFS_write(
		file /* file to write to */,
		ptr  /* buffer to write from */,
		size /* size of each object */,
		num  /* number of objects */
	);
	SDL_assert_paranoid(writeResult != -1);
	if (writeResult == -1) {
		SDL_SetError("RWops write error from PhysicsFS: %s", PHYSFS_getLastError());
		return 0;
	}

	// return number of objects written.  SDL header casually indicates this
	// should be 0 if less than num param.  Possibly a mistake; docs say count
	// written should be returned, which is more useful.
	return writeResult;

}

} // anonymous namespace


namespace csaru {
namespace xapp {

//==============================================================================
SDL_RWops * AllocRwOpsPhysFs (const char * path, const char mode) {

	SDL_assert(mode == 'a' || mode == 'r' || mode == 'w');

	PHYSFS_File * physFsFile = nullptr;
	switch (mode) {
		case 'a': physFsFile = PHYSFS_openAppend(path); break;
		case 'r': physFsFile = PHYSFS_openRead(path);   break;
		case 'w': physFsFile = PHYSFS_openWrite(path);  break;

		default:
			SDL_SetError("Invalid PhysicsFS RWops mode {%c}.  Use one of a,r,w.", mode);
			return nullptr;
	}
	if (!physFsFile) {
		SDL_SetError("PhysicsFS error while creating RWops: %s", PHYSFS_getLastError());
		return nullptr;
	}

	SDL_RWops * rwOps = SDL_AllocRW();
	rwOps->close                = close;
	rwOps->read                 = read;
	rwOps->seek                 = seek;
	rwOps->size                 = size;
	rwOps->write                = write;
	rwOps->type                 = SDL_RWOPS_UNKNOWN; // application-defined RWops type
	rwOps->hidden.unknown.data1 = physFsFile;
	rwOps->hidden.unknown.data2 = nullptr;

	// Must be SDL_RWclose()'d to be freed by SDL_FreeRW()!
	return rwOps;

}

} // namespace xapp
} // namespace csaru

