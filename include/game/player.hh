#pragma once
#include <array>

#include <SDL3/SDL_init.h>

#include "game/entity.hh"
#include "gfx/vertex.hh"


namespace ss
{
    class app;


    class player
    {
    public:
        glm::vec3  position {};
        glm::vec2  size {};
        sdl::color color {};

        void build_render_vertices(app &app);
        void update(float dt);

    private:
        glm::vec2 m_cursor_position {};
        glm::vec2 m_position {};
        bool      m_propel = false;

        mutable std::array<gfx::vertex, 12> m_vertices {};
        mutable int                         m_vert_count = 0;


        auto on_event(const SDL_Event &event, app &app) -> SDL_AppResult;
    };


    static_assert(is_entity<player>);
}
