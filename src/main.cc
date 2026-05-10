#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "app.hh"
#include "logger.hh"
#include "sdl/gpu/shader.hh"
#include "sdl/gpu/texture.hh"

#include "shaders.h"


namespace
{
    void
    create_pipeline(ss::app &app)
    {
        sdl::gpu::device &dev = app.gpu_device;

        auto vertex   = sdl::gpu::shader_builder { shader_vertex, shader_vertex_size }
                            .set_stage(SDL_GPU_SHADERSTAGE_VERTEX)
                            .set_samplers_amount(0)
                            .set_storage_buffers_amount(0)
                            .set_storage_textures_amount(0)
                            .set_uniform_buffers_amount(1)
                            .build(&dev);
        auto fragment = sdl::gpu::shader_builder { shader_fragment, shader_fragment_size }
                            .set_stage(SDL_GPU_SHADERSTAGE_FRAGMENT)
                            .set_samplers_amount(0)
                            .set_storage_buffers_amount(0)
                            .set_storage_textures_amount(0)
                            .set_uniform_buffers_amount(0)
                            .build(&dev);

        std::array<sdl::gpu::color_target_description, 1> color_target_desc {};
        color_target_desc[0].format = SDL_GetGPUSwapchainTextureFormat(dev.get(), app.window.get());

        app.pipeline
            = sdl::gpu::graphics_pipeline_builder { vertex, fragment }
                  .set_primitive_type(SDL_GPU_PRIMITIVETYPE_TRIANGLELIST)
                  .set_vertex_input_state({
                      .vertex_buffer_descriptions = gfx::vertex::get_buffer_descriptions().data(),
                      .num_vertex_buffers         = gfx::vertex::get_buffer_descriptions().size(),
                      .vertex_attributes          = gfx::vertex::get_attributes().data(),
                      .num_vertex_attributes      = gfx::vertex::get_attributes().size(),
                  })
                  .set_target_info({ .color_target_descriptions = color_target_desc.data(),
                                     .num_color_targets         = color_target_desc.size() })
                  .build(&dev);
    }
}


auto
SDL_AppInit(void **appstate, int /* argc */, char ** /* argv */) -> SDL_AppResult
{
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        ss::log[ss::log_level::fatal]("failed to initialize video subsystem ({})", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
    {
        ss::log[ss::log_level::fatal]("failed to initialize audio subsystem ({})", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    auto *app = new ss::app;

    app->window.reset(SDL_CreateWindow("spaceshooter", 800, 500,
                                       SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE));
    if (!app->window)
    {
        ss::log[ss::log_level::fatal]("failed to create window and renderer ({})", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    app->gpu_device.reset(SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, nullptr));
    if (!app->gpu_device)
    {
        ss::log[ss::log_level::fatal]("failed to create gpu device ({})", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_ClaimWindowForGPUDevice(app->gpu_device.get(), app->window.get());

    create_pipeline(*app);

    app->add_event_callback(
        [](const SDL_Event &event, ss::app &) -> SDL_AppResult
        { return event.type == SDL_EVENT_QUIT ? SDL_APP_SUCCESS : SDL_APP_CONTINUE; });

    app->add_entity(ss::player {});

    *appstate = app;

    return SDL_APP_CONTINUE;
}


void
SDL_AppQuit(void *appstate, SDL_AppResult /* result */)
{ delete static_cast<ss::app *>(appstate); }


auto
SDL_AppEvent(void *appstate, SDL_Event *event) -> SDL_AppResult
{ return static_cast<ss::app *>(appstate)->invoke_event_callbacks(*event); }


auto
SDL_AppIterate(void *appstate) -> SDL_AppResult
{
    auto *app = static_cast<ss::app *>(appstate);

    auto cmd_buffer = sdl::gpu::command_buffer::acquire(app->gpu_device);

    app->build_frame();
    app->upload_frame(cmd_buffer);

    auto swapchain = sdl::gpu::texture::wait_and_acquire_swapchain_texture(cmd_buffer, app->window);

    if (swapchain.get() == nullptr) return SDL_APP_CONTINUE;

    auto render_pass = sdl::gpu::render_pass_builder {}
                           .set_clear_color({ 0, 0, 0, 1 })
                           .set_loadop(SDL_GPU_LOADOP_CLEAR)
                           .set_storeop(SDL_GPU_STOREOP_STORE)
                           .set_texture(swapchain)
                           .begin(cmd_buffer);


    SDL_PushGPUVertexUniformData(cmd_buffer.get(), 0, &app->uniform, sizeof app->uniform);

    if (auto res = app->render(cmd_buffer, render_pass); res != SDL_APP_CONTINUE) return res;

    return SDL_APP_CONTINUE;
}
