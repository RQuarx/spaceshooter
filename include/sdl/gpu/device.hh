#pragma once
#include <utility>

#include <SDL3/SDL_gpu.h>

#include "sdl/resource.hh"


namespace sdl::gpu
{
    using device = sdl::resource<SDL_GPUDevice, SDL_DestroyGPUDevice>;


    template <typename T, void (*Deleter)(SDL_GPUDevice *, T *)> class resource
    {
    public:
        using pointer = T *;


        resource(device *dev) : m_dev { dev } {}
        ~resource()
        {
            if (m_obj != nullptr) Deleter(m_dev->get(), m_obj);
        }

        resource(resource &&other) noexcept
            : m_dev { std::exchange(other.m_dev, nullptr) },
              m_obj { std::exchange(other.m_obj, nullptr) }
        {
        }

        auto
        operator=(resource &&other) noexcept -> resource &
        {
            if (this != &other)
            {
                m_dev = std::exchange(other.m_dev, nullptr);
                m_obj = std::exchange(other.m_obj, nullptr);
            }

            return *this;
        }

        resource(const resource &)                     = delete;
        auto operator=(const resource &) -> resource & = delete;


        [[nodiscard]]
        auto
        release() noexcept -> pointer
        { return std::exchange(m_obj, nullptr); }


        void
        reset(pointer p) noexcept
        {
            if (m_obj != nullptr)
            {
                Deleter(m_dev->get(), m_obj);
                m_obj = nullptr;
            }
            m_obj = p;
        }


        [[nodiscard]]
        auto
        get() const noexcept -> pointer
        { return m_obj; }


        operator bool() { return m_obj != nullptr; }

    private:
        device *m_dev;
        pointer m_obj = nullptr;
    };
}
