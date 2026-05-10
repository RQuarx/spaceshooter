#pragma once
#include <functional>
#include <variant>
#include <vector>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include "game/entity.hh"
#include "game/player.hh"
#include "sdl/gpu/buffer.hh"
#include "sdl/gpu/command_buffer.hh"
#include "sdl/gpu/device.hh"
#include "sdl/gpu/pass.hh"
#include "sdl/gpu/pipeline.hh"


namespace ss
{
    class app;

    using event_callback_signature = auto(const SDL_Event &, app &app) -> SDL_AppResult;
    using render_callback_signature
        = auto(app &app, sdl::gpu::command_buffer &cmd_buff, sdl::gpu::render_pass &render_pass)
            -> SDL_AppResult;

    using entities = std::variant<player>; /* more later */


    struct uniform_buffer
    {
        glm::vec2 camera_position { 0, 0 };
    };


    class app
    {
    public:
        using vertex_batch = std::vector<gfx::vertex>;

        sdl::window window;

        sdl::gpu::device gpu_device;

        sdl::gpu::graphics_pipeline pipeline { &gpu_device };

        sdl::gpu::transfer_buffer transfer_buffer { &gpu_device };
        sdl::gpu::buffer          vertex_buffer { &gpu_device };
        std::size_t               vertex_buffer_capacity = 0;

        std::array<vertex_batch, 5> frame_batches;
        std::vector<gfx::vertex> frame_vertices;

        uniform_buffer uniform;

        app()                       = default;
        app(const app &)            = delete;
        app &operator=(const app &) = delete;
        app(app &&)                 = delete;
        app &operator=(app &&)      = delete;


        template <is_entity T>
        void
        add_entity(T &&entity)
        {
            auto ptr = std::make_unique<std::variant<player>>(std::forward<T>(entity));
            m_entities.emplace_back(std::move(ptr));
        }

        void build_frame();
        void push_vertex(std::uint32_t layer, const gfx::vertex &vertex);
        void push_quad(std::uint32_t layer, const gfx::quad &quad);

        void upload_frame(sdl::gpu::command_buffer &cmd_buff);
        auto render(sdl::gpu::command_buffer &cmd_buff, sdl::gpu::render_pass &pass)
            -> SDL_AppResult;


        void add_event_callback(std::move_only_function<event_callback_signature> cb);
        auto invoke_event_callbacks(const SDL_Event &e) -> SDL_AppResult;

    private:
        std::vector<std::unique_ptr<entities>>                         m_entities;
        std::vector<std::move_only_function<event_callback_signature>> m_event_callbacks;
    };
}
