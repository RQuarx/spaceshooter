#pragma once
#include <SDL3/SDL_gpu.h>

#include "sdl/gpu/texture.hh"
#include "sdl/types.hh"


namespace sdl::gpu
{
    using render_pass = sdl::resource<SDL_GPURenderPass, SDL_EndGPURenderPass>;


    class render_pass_builder
    {
    public:
        [[nodiscard]] auto set_clear_color(sdl::color color) noexcept -> render_pass_builder &;
        [[nodiscard]] auto set_loadop(SDL_GPULoadOp loadop) noexcept -> render_pass_builder &;
        [[nodiscard]] auto set_storeop(SDL_GPUStoreOp storeop) noexcept -> render_pass_builder &;
        [[nodiscard]] auto set_texture(texture &texture) noexcept -> render_pass_builder &;
        [[nodiscard]] auto begin(command_buffer &buff) -> render_pass;

    private:
        SDL_GPUColorTargetInfo m_info {};
    };
}
