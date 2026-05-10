#include <cmath>
#include <cstring>

#include <glm/glm.hpp>

#include "app.hh"
#include "game/player.hh"
#include "logger.hh"

using ss::player;

namespace ss
{

}


auto
player::on_event(const SDL_Event &event, app &app) -> SDL_AppResult
{
    switch (event.type)
    {
    case SDL_EVENT_MOUSE_MOTION:
        {
            int win_w;
            int win_h;
            int pix_w;
            int pix_h;

            SDL_GetWindowSize(app.window.get(), &win_w, &win_h);
            SDL_GetWindowSizeInPixels(app.window.get(), &pix_w, &pix_h);

            float window_width  = static_cast<float>(pix_w) / static_cast<float>(win_w);
            float window_height = static_cast<float>(pix_h) / static_cast<float>(win_h);

            float ndc_x = ((event.motion.x / window_width) * 2.0F) - 1.0F;
            float ndc_y = 1.0F - ((event.motion.y / window_height) * 2.0F);

            m_cursor_position = { ndc_x, ndc_y };
            break;
        }
    case SDL_EVENT_KEY_DOWN:
        switch (event.key.scancode)
        {
        case SDL_SCANCODE_W: m_propel = true; break;
        default:             break;
        }
        break;
    case SDL_EVENT_KEY_UP:
        switch (event.key.scancode)
        {
        case SDL_SCANCODE_W: m_propel = false; break;
        default:             break;
        }
        break;
    default: break;
    }

    return SDL_APP_CONTINUE;
}


void
player::build_render_vertices(app &app)
{
    using glm::length;
    using glm::normalize;
    using glm::vec2;
    using glm::vec3;

    constexpr float ship_length   = 0.08F;
    constexpr float wing_width    = 0.05F;
    constexpr float arm_thickness = 0.015F;

    constexpr sdl::color ship_color { 1, 1, 1, 1 };

    vec2 pos2d { position.x, position.y };
    vec2 to_cursor = m_cursor_position - pos2d;

    if (length(to_cursor) <= 0.0001F) to_cursor = { 0.F, -1.F };

    vec2 forward = normalize(to_cursor);
    vec2 right { -forward.y, forward.x };
    vec2 nose = pos2d + forward * ship_length;
    vec2 left_outer = pos2d - forward * 0.02F - right * wing_width;
    vec2 right_outer = pos2d - forward * 0.02F + right * wing_width;
    vec2 left_inner = pos2d - right * (wing_width - arm_thickness);
    vec2 right_inner = pos2d + right * (wing_width + arm_thickness);
    vec2 rear = pos2d - forward * 0.035F;

    auto make_vertex = [&](vec2 p)
    {
        return gfx::vertex {
            .position = vec3 { p.x, p.y, 0.1 },
            .uv       = { 0.F, 0.F },
            .color    = ship_color,
        };
    };

    /* left arm */
    app.push_vertex(1, make_vertex(left_outer));
    app.push_vertex(1, make_vertex(nose));
    app.push_vertex(1, make_vertex(rear));

    /* right arm */
    app.push_vertex(1, make_vertex(right_outer));
    app.push_vertex(1, make_vertex(nose));
    app.push_vertex(1, make_vertex(rear));
}
