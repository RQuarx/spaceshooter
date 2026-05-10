#pragma once
#include <cstdint>

#include <SDL3/SDL_gpu.h>

#include "sdl/gpu/device.hh"


namespace sdl::gpu
{
    using shader = resource<SDL_GPUShader, SDL_ReleaseGPUShader>;

    class shader_builder
    {
    public:
        shader_builder(const std::uint8_t *code, std::size_t size);


        [[nodiscard]]
        auto set_format(SDL_GPUShaderFormat shader_format) noexcept -> shader_builder &;

        [[nodiscard]]
        auto set_stage(SDL_GPUShaderStage shader_stage) noexcept -> shader_builder &;

        [[nodiscard]]
        auto set_samplers_amount(std::uint32_t amount) noexcept -> shader_builder &;

        [[nodiscard]]
        auto set_storage_textures_amount(std::uint32_t amount) noexcept -> shader_builder &;

        [[nodiscard]]
        auto set_storage_buffers_amount(std::uint32_t amount) noexcept -> shader_builder &;

        [[nodiscard]]
        auto set_uniform_buffers_amount(std::uint32_t amount) noexcept -> shader_builder &;

        [[nodiscard]]
        auto build(device *dev) -> shader;

    private:
        SDL_GPUShaderCreateInfo m_create_info {};
    };
}
