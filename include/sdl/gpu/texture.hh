#pragma once
#include <cstdint>

#include <SDL3/SDL_gpu.h>

#include "sdl/gpu/command_buffer.hh"
#include "sdl/types.hh"


namespace sdl::gpu
{
    class texture
    {
    public:
        [[nodiscard]]
        static auto wait_and_acquire_swapchain_texture(command_buffer &buffer, window &window)
            -> texture;


        ~texture()                                   = default;
        texture(const texture &)                     = delete;
        texture(texture &&)                          = default;
        auto operator=(const texture &) -> texture & = delete;
        auto operator=(texture &&) -> texture &      = default;


        [[nodiscard]] auto get() const noexcept -> SDL_GPUTexture *;
        [[nodiscard]] auto width() const noexcept -> std::uint32_t;
        [[nodiscard]] auto height() const noexcept -> std::uint32_t;
        [[nodiscard]]
        auto release() noexcept -> std::tuple<SDL_GPUTexture *, std::int32_t, std::int32_t>;

    private:
        SDL_GPUTexture *m_texture;

        std::uint32_t m_width;
        std::uint32_t m_height;


        texture() = default;
    };
}
