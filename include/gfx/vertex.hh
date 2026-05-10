#pragma once
#include <array>
#include <span>

#include <SDL3/SDL_gpu.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "sdl/gpu/types.hh"
#include "sdl/types.hh"


namespace gfx
{
    struct vertex
    {
        glm::vec3  position;
        glm::vec2  uv; /* texture position */
        sdl::color color;


        [[nodiscard]]
        static constexpr auto
        get_attributes() -> std::array<sdl::gpu::vertex_attribute, 3>
        {
            return {
                sdl::gpu::vertex_attribute {
                                            .location    = 0,
                                            .buffer_slot = 0,
                                            .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                                            .offset      = 0,
                                            },
                sdl::gpu::vertex_attribute {
                                            .location    = 1,
                                            .buffer_slot = 0,
                                            .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
                                            .offset      = offsetof(vertex, uv),
                                            },
                sdl::gpu::vertex_attribute {
                                            .location    = 2,
                                            .buffer_slot = 0,
                                            .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
                                            .offset      = offsetof(vertex, color),
                                            },
            };
        }


        [[nodiscard]]
        static constexpr auto
        get_buffer_descriptions() -> std::array<sdl::gpu::vertex_buffer_description, 1>
        {
            return { { {
                .slot               = 0,
                .pitch              = sizeof(vertex),
                .input_rate         = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                .instance_step_rate = 0,
            } } };
        }
    };


    using quad = std::span<const vertex, 6>;
}
