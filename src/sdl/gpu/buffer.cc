#include "sdl/exception.hh"
#include "sdl/gpu/buffer.hh"

using sdl::gpu::buffer_builder;
using sdl::gpu::transfer_buffer_builder;


auto
buffer_builder::set_usage(SDL_GPUBufferUsageFlags usage) noexcept -> buffer_builder &
{
    m_info.usage = usage;
    return *this;
}


auto
buffer_builder::set_size(std::uint32_t size) noexcept -> buffer_builder &
{
    m_info.size = size;
    return *this;
}


auto
buffer_builder::build(device *dev) -> buffer
{
    buffer b { dev };
    b.reset(SDL_CreateGPUBuffer(dev->get(), &m_info));
    if (!b) throw exception {};
    return b;
}


auto
transfer_buffer_builder::set_usage(SDL_GPUTransferBufferUsage usage) noexcept
    -> transfer_buffer_builder &
{
    m_info.usage = usage;
    return *this;
}


auto
transfer_buffer_builder::set_size(std::uint32_t size) noexcept -> transfer_buffer_builder &
{
    m_info.size = size;
    return *this;
}


auto
transfer_buffer_builder::build(device *dev) -> transfer_buffer
{
    transfer_buffer b { dev };
    b.reset(SDL_CreateGPUTransferBuffer(dev->get(), &m_info));
    if (!b) throw exception {};
    return b;
}
