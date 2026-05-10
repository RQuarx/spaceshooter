#pragma once
#include <cstdint>

#include <SDL3/SDL_gpu.h>

#include "sdl/gpu/device.hh"


namespace sdl::gpu
{
    using buffer          = resource<SDL_GPUBuffer, SDL_ReleaseGPUBuffer>;
    using transfer_buffer = resource<SDL_GPUTransferBuffer, SDL_ReleaseGPUTransferBuffer>;

    class buffer_builder
    {
    public:
        [[nodiscard]] auto set_usage(SDL_GPUBufferUsageFlags usage) noexcept -> buffer_builder &;
        [[nodiscard]] auto set_size(std::uint32_t size) noexcept -> buffer_builder &;
        [[nodiscard]] auto build(device *dev) -> buffer;

    private:
        SDL_GPUBufferCreateInfo m_info {};
    };


    class transfer_buffer_builder
    {
    public:
        [[nodiscard]]
        auto set_usage(SDL_GPUTransferBufferUsage usage) noexcept -> transfer_buffer_builder &;
        [[nodiscard]] auto set_size(std::uint32_t size) noexcept -> transfer_buffer_builder &;
        [[nodiscard]] auto build(device *dev) -> transfer_buffer;

    private:
        SDL_GPUTransferBufferCreateInfo m_info {};
    };
}
