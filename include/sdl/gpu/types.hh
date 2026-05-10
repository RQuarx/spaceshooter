#pragma once
#include <SDL3/SDL_gpu.h>

#include "sdl/resource.hh"


namespace sdl::gpu
{
    using color_target_info             = SDL_GPUColorTargetInfo;
    using buffer_create_info            = SDL_GPUBufferCreateInfo;
    using transfer_buffer_create_info   = SDL_GPUTransferBufferCreateInfo;
    using transfer_buffer_location      = SDL_GPUTransferBufferLocation;
    using buffer_region                 = SDL_GPUBufferRegion;
    using shader_create_info            = SDL_GPUShaderCreateInfo;
    using graphics_pipeline_create_info = SDL_GPUGraphicsPipelineCreateInfo;
    using vertex_buffer_description     = SDL_GPUVertexBufferDescription;
    using vertex_attribute              = SDL_GPUVertexAttribute;
    using color_target_description      = SDL_GPUColorTargetDescription;
    using buffer_binding                = SDL_GPUBufferBinding;
}
