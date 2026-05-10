#pragma once
#include "sdl/gpu/device.hh"


namespace sdl::gpu
{
    class command_buffer
    {
    public:
        static auto acquire(device &dev) -> command_buffer;


        ~command_buffer();
        command_buffer(command_buffer &&other) noexcept;
        command_buffer(const command_buffer &) = delete;

        auto operator=(command_buffer &&other) noexcept -> command_buffer &;
        auto operator=(const command_buffer &) -> command_buffer & = delete;

        void submit();

        [[nodiscard]] auto get() const noexcept -> SDL_GPUCommandBuffer *;

    private:
        SDL_GPUCommandBuffer *m_buffer;


        command_buffer() = default;
    };
}
