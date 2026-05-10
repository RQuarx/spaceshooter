#include "sdl/exception.hh"
#include "sdl/gpu/pipeline.hh"

using sdl::gpu::graphics_pipeline_builder;


graphics_pipeline_builder::graphics_pipeline_builder(shader &vertex, shader &fragment) noexcept
{
    m_create_info.vertex_shader = vertex.get();
    m_create_info.fragment_shader = fragment.get();
}


auto
graphics_pipeline_builder::set_primitive_type(SDL_GPUPrimitiveType type) noexcept
    -> graphics_pipeline_builder &
{
    m_create_info.primitive_type = type;
    return *this;
}


auto
graphics_pipeline_builder::set_rasterizer_state(const SDL_GPURasterizerState &state) noexcept
    -> graphics_pipeline_builder &
{
    m_create_info.rasterizer_state = state;
    return *this;
}


auto
graphics_pipeline_builder::set_multisample_state(const SDL_GPUMultisampleState &state) noexcept
    -> graphics_pipeline_builder &
{
    m_create_info.multisample_state = state;
    return *this;
}


auto
graphics_pipeline_builder::set_depth_stencil_state(const SDL_GPUDepthStencilState &state) noexcept
    -> graphics_pipeline_builder &
{
    m_create_info.depth_stencil_state = state;
    return *this;
}


auto
graphics_pipeline_builder::set_vertex_input_state(const SDL_GPUVertexInputState &state) noexcept
    -> graphics_pipeline_builder &
{
    m_create_info.vertex_input_state = state;
    return *this;
}


auto
graphics_pipeline_builder::set_target_info(
    const SDL_GPUGraphicsPipelineTargetInfo &target_info) noexcept -> graphics_pipeline_builder &
{
    m_create_info.target_info = target_info;
    return *this;
}


auto
graphics_pipeline_builder::build(device *dev) -> graphics_pipeline
{
    graphics_pipeline p { dev };
    p.reset(SDL_CreateGPUGraphicsPipeline(dev->get(), &m_create_info));
    if (!p) throw exception {};
    return p;
}
