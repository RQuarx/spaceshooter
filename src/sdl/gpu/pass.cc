#include "sdl/exception.hh"
#include "sdl/gpu/pass.hh"

using sdl::gpu::render_pass;
using sdl::gpu::render_pass_builder;


auto
render_pass_builder::set_clear_color(sdl::color color) noexcept -> render_pass_builder &
{
    m_info.clear_color = color;
    return *this;
}


auto
render_pass_builder::set_loadop(SDL_GPULoadOp loadop) noexcept -> render_pass_builder &
{
    m_info.load_op = loadop;
    return *this;
}


auto
render_pass_builder::set_storeop(SDL_GPUStoreOp storeop) noexcept -> render_pass_builder &
{
    m_info.store_op = storeop;
    return *this;
}


auto
render_pass_builder::set_texture(texture &texture) noexcept -> render_pass_builder &
{
    m_info.texture = texture.get();
    return *this;
}


auto
render_pass_builder::begin(command_buffer &buff) -> render_pass
{
    render_pass p { SDL_BeginGPURenderPass(buff.get(), &m_info, 1, nullptr) };
    if (p.get() == nullptr) throw exception {};
    return p;
}


