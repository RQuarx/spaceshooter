#pragma once
#include <concepts>

#include <SDL3/SDL_rect.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "sdl/types.hh"


namespace ss
{
    class app;


    template <typename T, typename U>
    concept same_decay_as = std::same_as<std::remove_cvref_t<T>, U>;


    template <typename T>
    concept is_entity = requires(T instance, app &app) {
        { instance.position } -> same_decay_as<glm::vec3>;
        { instance.size } -> same_decay_as<glm::vec2>;
        { instance.color } -> same_decay_as<sdl::color>;
        { instance.build_render_vertices(app) };
    };


    struct entity
    {
        glm::vec3  position {};
        glm::vec2  size {};
        sdl::color color {};

        virtual void build_render_vertices(app &app) = 0;
    };


    static_assert(is_entity<entity>);
}
