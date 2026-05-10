#pragma once
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_video.h>

#include "sdl/resource.hh"


namespace sdl
{
    using color  = SDL_FColor;
    using window = resource<SDL_Window, SDL_DestroyWindow>;
}
