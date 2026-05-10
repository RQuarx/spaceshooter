#pragma once
#include <memory>


namespace sdl
{
    namespace _impl
    {
        template <typename T, void (*Deleter)(T *)> struct resource_deleter
        {
            void
            operator()(T *instance) const
            {
                if (instance != nullptr) Deleter(instance);
            }
        };
    }


    template <typename T, void (*Deleter)(T *)>
    using resource = std::unique_ptr<T, _impl::resource_deleter<T, Deleter>>;
}
