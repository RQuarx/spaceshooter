#pragma once
#include <exception>

#include <SDL3/SDL_error.h>


namespace sdl
{
    class exception final : public std::exception
    {
    public:
        exception() : msg { SDL_GetError() } {}


        [[nodiscard]]
        auto
        what() const noexcept -> const char * override
        { return msg; }

    private:
        const char *msg;
    };
}
