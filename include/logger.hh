#pragma once
#include <cstdint>
#include <format>
#include <print>
#include <source_location>
#include <string_view>

#include <unistd.h>


namespace ss
{
    enum class log_level : std::uint8_t
    {
        trace,
        debug,
        info,
        warn,
        error,
        fatal
    };


    namespace _impl
    {
        [[nodiscard]]
        static inline auto
        log_level_to_string(log_level level) -> std::string_view
        {
            bool use_color = ::isatty(STDERR_FILENO);
            switch (level)
            {
                using enum log_level;
            case trace: return use_color ? "\033[38;2;156;163;175mtrace\033[0m" : "trace";
            case debug: return use_color ? "\033[38;2;59;130;246mdebug\033[0m" : "debug";
            case info:  return use_color ? "\033[38;2;34;211;238minfo\033[0m" : "info";
            case warn:  return use_color ? "\033[38;2;250;204;21mwarn\033[0m" : "warn";
            case error: return use_color ? "\033[38;2;239;68;68merror\033[0m" : "error";
            case fatal: return use_color ? "\033[38;2;192;38;211mfatal\033[0m" : "fatal";
            }
        }


        struct logger
        {
            struct printer
            {
                log_level            level;
                std::source_location source;


                template <typename... Args>
                void
                operator()(std::format_string<Args...> fmt, Args &&...args)
                {
                    if (level >= log_level::error)
                        std::println(stderr, "[{} at {}:{}]: {}", log_level_to_string(level),
                                     source.file_name(), source.line(),
                                     std::format(fmt, std::forward<Args>(args)...));
                    else
                        std::println(stderr, "[{}]: {}", log_level_to_string(level),
                                     std::format(fmt, std::forward<Args>(args)...));
                }
            };


            [[nodiscard]]
            static constexpr auto
            operator[](log_level                   level,
                       const std::source_location &source = std::source_location::current())
                -> printer
            { return printer { level, source }; }
        };
    }


    constexpr inline _impl::logger log;
}
