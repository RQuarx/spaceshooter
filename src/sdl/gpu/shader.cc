#include <cstring>

#include "sdl/exception.hh"
#include "sdl/gpu/shader.hh"

using sdl::gpu::shader;
using sdl::gpu::shader_builder;


shader_builder::shader_builder(const std::uint8_t *code, std::size_t size)
{
    m_create_info.code      = code;
    m_create_info.code_size = size;
    m_create_info.entrypoint = "main";

    m_create_info.format = SDL_GPU_SHADERFORMAT_SPIRV;
}


auto
shader_builder::set_format(SDL_GPUShaderFormat shader_format) noexcept -> shader_builder &
{
    m_create_info.format = shader_format;
    return *this;
}


auto
shader_builder::set_stage(SDL_GPUShaderStage shader_stage) noexcept -> shader_builder &
{
    m_create_info.stage = shader_stage;
    return *this;
}


auto
shader_builder::set_samplers_amount(std::uint32_t amount) noexcept -> shader_builder &
{
    m_create_info.num_samplers = amount;
    return *this;
}


auto
shader_builder::set_storage_textures_amount(std::uint32_t amount) noexcept -> shader_builder &
{
    m_create_info.num_storage_textures = amount;
    return *this;
}


auto
shader_builder::set_storage_buffers_amount(std::uint32_t amount) noexcept -> shader_builder &
{
    m_create_info.num_storage_buffers = amount;
    return *this;
}


auto
shader_builder::set_uniform_buffers_amount(std::uint32_t amount) noexcept -> shader_builder &
{
    m_create_info.num_uniform_buffers = amount;
    return *this;
}


auto
shader_builder::build(device *dev) -> shader
{
    shader s { dev };
    s.reset(SDL_CreateGPUShader(dev->get(), &m_create_info));
    if (!s) throw exception {};
    return s;
}

