#include <cstring>

#include "app.hh"
#include "logger.hh"

using ss::app;


void
app::build_frame()
{
    for (auto &batch : frame_batches) batch.clear();

    frame_vertices.clear();

    for (auto &e : m_entities)
        std::visit([this](auto &entity) { entity.build_render_vertices(*this); }, *e);

    for (auto &batch : frame_batches)
        frame_vertices.insert(frame_vertices.end(), batch.begin(), batch.end());
}


void
app::push_vertex(std::uint32_t layer, const gfx::vertex &v)
{ frame_batches[static_cast<std::size_t>(layer)].push_back(v); }


void
app::push_quad(std::uint32_t layer, const gfx::quad &quad)
{
    auto &batch = frame_batches[static_cast<std::size_t>(layer)];
    batch.insert(batch.end(), quad.begin(), quad.end());
}


void
app::add_event_callback(std::move_only_function<event_callback_signature> cb)
{ m_event_callbacks.emplace_back(std::move(cb)); }


auto
app::invoke_event_callbacks(const SDL_Event &e) -> SDL_AppResult
{
    for (auto &cb : m_event_callbacks)
        if (SDL_AppResult res = cb(e, *this); res != SDL_APP_CONTINUE) return res;
    return SDL_APP_CONTINUE;
}


auto
app::render(sdl::gpu::command_buffer &cmd_buff, sdl::gpu::render_pass &pass) -> SDL_AppResult
{
    if (frame_vertices.empty()) return SDL_APP_CONTINUE;

    SDL_BindGPUGraphicsPipeline(pass.get(), pipeline.get());

    SDL_GPUBufferBinding binding {};
    binding.buffer = vertex_buffer.get();
    binding.offset = 0;

    SDL_BindGPUVertexBuffers(pass.get(), 0, &binding, 1);

    ss::log[ss::log_level::info]("vertices: {}", frame_vertices.size());
    ss::log[ss::log_level::info]("{}, {}, {}", sizeof(gfx::vertex), offsetof(gfx::vertex, position),
                                 offsetof(gfx::vertex, color));
    SDL_DrawGPUPrimitives(pass.get(), static_cast<std::uint32_t>(frame_vertices.size()), 1, 0, 0);

    return SDL_APP_CONTINUE;
}


void
app::upload_frame(sdl::gpu::command_buffer &cmd_buff)
{
    if (frame_vertices.empty()) return;

    const std::size_t required_size = frame_vertices.size() * sizeof(gfx::vertex);

    std::println("frame");
    for (auto &vert : frame_vertices)
    {
        std::println("({}, {})", vert.position.x, vert.position.y);
    }

    if (required_size > vertex_buffer_capacity)
    {
        vertex_buffer_capacity = std::max(required_size, vertex_buffer_capacity * 2);

        if (vertex_buffer_capacity == 0) vertex_buffer_capacity = required_size;

        vertex_buffer = sdl::gpu::buffer_builder {}
                            .set_usage(SDL_GPU_BUFFERUSAGE_VERTEX)
                            .set_size(vertex_buffer_capacity)
                            .build(&gpu_device);

        transfer_buffer = sdl::gpu::transfer_buffer_builder {}
                              .set_usage(SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD)
                              .set_size(vertex_buffer_capacity)
                              .build(&gpu_device);
    }

    void *mapped = SDL_MapGPUTransferBuffer(gpu_device.get(), transfer_buffer.get(), false);

    if (!mapped)
    {
        ss::log[ss::log_level::error]("failed to map transfer buffer ({})", SDL_GetError());

        return;
    }


    std::memcpy(mapped, frame_vertices.data(), required_size);

    SDL_UnmapGPUTransferBuffer(gpu_device.get(), transfer_buffer.get());

    SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(cmd_buff.get());

    SDL_GPUTransferBufferLocation src {};
    src.transfer_buffer = transfer_buffer.get();
    src.offset          = 0;

    SDL_GPUBufferRegion dst {};
    dst.buffer = vertex_buffer.get();
    dst.offset = 0;
    dst.size   = required_size;

    SDL_UploadToGPUBuffer(copy_pass, &src, &dst, false);

    SDL_EndGPUCopyPass(copy_pass);
}

