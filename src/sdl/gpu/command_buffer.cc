#include "sdl/exception.hh"
#include "sdl/gpu/command_buffer.hh"

using sdl::gpu::command_buffer;


auto
command_buffer::acquire(device &dev) -> command_buffer
{
    command_buffer b;
    if (b.m_buffer = SDL_AcquireGPUCommandBuffer(dev.get()); b.m_buffer == nullptr)
        throw exception {};
    return b;
}


command_buffer::~command_buffer()
{
    submit();
}


command_buffer::command_buffer(command_buffer &&other) noexcept
    : m_buffer { std::exchange(other.m_buffer, nullptr) }
{
}


auto
command_buffer::operator=(command_buffer &&other) noexcept -> command_buffer &
{
    if (this != &other) m_buffer = std::exchange(other.m_buffer, nullptr);
    return *this;
}


void
command_buffer::submit()
{
    if (m_buffer != nullptr) SDL_SubmitGPUCommandBuffer(m_buffer);
}


auto
command_buffer::get() const noexcept -> SDL_GPUCommandBuffer *
{
    return m_buffer;
}
