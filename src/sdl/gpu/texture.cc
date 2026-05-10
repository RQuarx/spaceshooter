#include "sdl/exception.hh"
#include "sdl/gpu/texture.hh"

using sdl::gpu::texture;


auto
texture::wait_and_acquire_swapchain_texture(command_buffer &buffer, window &window) -> texture
{
    texture text;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(buffer.get(), window.get(), &text.m_texture,
                                               &text.m_width, &text.m_height))
        throw exception {};

    return text;
}


auto
texture::release() noexcept -> std::tuple<SDL_GPUTexture *, std::int32_t, std::int32_t>
{
    return std::make_tuple(std::exchange(m_texture, nullptr), std::exchange(m_width, 0),
                           std::exchange(m_height, 0));
}


auto
texture::get() const noexcept -> SDL_GPUTexture *
{ return m_texture; }


auto
texture::width() const noexcept -> std::uint32_t
{ return m_width; }


auto
texture::height() const noexcept -> std::uint32_t
{ return m_height; }
