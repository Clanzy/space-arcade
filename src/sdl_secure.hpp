#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <memory>
#include <stdexcept>

// unique_ptr wrapper
template <typename T, void (*destroy)(T *), typename Ptr = std::unique_ptr<T, decltype(destroy)> >
struct HandleUnique : public Ptr {
	HandleUnique(T *&&p = nullptr)
		: Ptr{ p, destroy }
	{
		if (p != nullptr && !*this) { throw std::runtime_error(SDL_GetError()); }
	}
};

// shared_ptr wrapper
template <typename T, void (*destroy)(T *), typename Ptr = std::shared_ptr<T> >
struct HandleShared : public Ptr {
	HandleShared(T *&&p = nullptr)
		: Ptr{ p, destroy }
	{
		if (p != nullptr && !*this) { throw std::runtime_error(SDL_GetError()); }
	}
};

using SDL_Window_U = HandleUnique<SDL_Window, SDL_DestroyWindow>;
using SDL_Renderer_U = HandleUnique<SDL_Renderer, SDL_DestroyRenderer>;
using SDL_Surface_U = HandleUnique<SDL_Surface, SDL_FreeSurface>;
using SDL_Texture_U = HandleUnique<SDL_Texture, SDL_DestroyTexture>;
using Mix_Music_U = HandleUnique<Mix_Music, Mix_FreeMusic>;
using Mix_Chunk_U = HandleUnique<Mix_Chunk, Mix_FreeChunk>;

using SDL_Texture_S = HandleShared<SDL_Texture, SDL_DestroyTexture>;
