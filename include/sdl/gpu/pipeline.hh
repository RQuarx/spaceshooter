#pragma once
#include <SDL3/SDL_gpu.h>

#include "sdl/gpu/device.hh"
#include "sdl/gpu/shader.hh"


namespace sdl::gpu
{
    using graphics_pipeline = resource<SDL_GPUGraphicsPipeline, SDL_ReleaseGPUGraphicsPipeline>;

    class graphics_pipeline_builder
    {
    public:
        graphics_pipeline_builder(shader &vertex, shader &fragment) noexcept;

        [[nodiscard]]
        auto set_primitive_type(SDL_GPUPrimitiveType type) noexcept -> graphics_pipeline_builder &;

        [[nodiscard]]
        auto set_rasterizer_state(const SDL_GPURasterizerState &state) noexcept
            -> graphics_pipeline_builder &;

        [[nodiscard]]
        auto set_multisample_state(const SDL_GPUMultisampleState &state) noexcept
            -> graphics_pipeline_builder &;

        [[nodiscard]]
        auto set_depth_stencil_state(const SDL_GPUDepthStencilState &state) noexcept
            -> graphics_pipeline_builder &;

        [[nodiscard]]
        auto set_vertex_input_state(const SDL_GPUVertexInputState &state) noexcept
            -> graphics_pipeline_builder &;

        [[nodiscard]]
        auto set_target_info(const SDL_GPUGraphicsPipelineTargetInfo &target_info) noexcept
            -> graphics_pipeline_builder &;

        [[nodiscard]]
        auto build(device *dev) -> graphics_pipeline;

    private:
        SDL_GPUGraphicsPipelineCreateInfo m_create_info {};
    };
}
