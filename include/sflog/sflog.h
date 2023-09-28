#ifndef SFLOG_SFLOG_H
#define SFLOG_SFLOG_H

#include <fmt/core.h>
#include <magic_enum.hpp>
#include <magic_enum_format.hpp>
#include <source_location>
#include <string_view>

#include <sflog/config.h>

namespace sflog {
    enum class level_t {
        ERROR = 0,
        WARNING,
        INFO,
        DEBUG,
    };

    struct level_location {
        level_t level;
        std::source_location loc;
    };

    constexpr auto max_size(const auto &cont) {
        const auto it = std::max_element(
            cont.begin(), cont.end(),
            [](const auto &a, const auto &b) { return a.size() < b.size(); });
        return it->size();
    }

    constexpr level_t LOG_LEVEL =
        magic_enum::enum_value<level_t>(SFLOG_LEVEL);

    constexpr std::size_t LOG_LEVEL_MAXLEN =
        max_size(magic_enum::enum_names<level_t>());

    constexpr std::size_t LOG_PATH_MAXLEN = 20;

    auto trim_end_filepath(const auto &fpath) {
        std::string_view sv(fpath);
        auto len = sv.size();
        if (len > LOG_PATH_MAXLEN) {
            auto start = len - LOG_PATH_MAXLEN;
            return sv.substr(start);
        }

        return sv;
    }

    template <typename... Args>
    static inline void
    println_formatted(level_t level, const std::source_location loc,
                      fmt::format_string<Args...> the_format, Args &&...args) {}

    static inline void print_level_and_location(
        level_t level,
        std::source_location loc = std::source_location::current()) {
        fmt::print("[{:^{}}] {:>{}}:{:<6}| ", level, LOG_LEVEL_MAXLEN,
                   trim_end_filepath(loc.file_name()), LOG_PATH_MAXLEN,
                   loc.line());
    }

    template <typename... Args>
    static inline void println_format(fmt::format_string<Args...> the_format,
                                      Args &&...args) {
        fmt::println(the_format, std::forward<Args>(args)...);
    }

} // namespace sflog

// Custom formatter for the pair level_t/source_location, used to avoid
// repeating the format for every log call, since the prefix is equal for each
// call
template <>
struct fmt::formatter<sflog::level_location> {
    constexpr auto parse(format_parse_context &ctx)
        -> format_parse_context::iterator {
        // Only empty '{}' accepted, will raise an ERROR otherwise at
        // compilation (typically) or runtime
        return ctx.begin();
    }

    auto format(const sflog::level_location &ll, format_context &ctx) const {
        return fmt::format_to(ctx.out(), "[{:^{}}] {:>{}}:{:<6}| ", ll.level,
                              sflog::LOG_LEVEL_MAXLEN,
                              sflog::trim_end_filepath(ll.loc.file_name()),
                              sflog::LOG_PATH_MAXLEN, ll.loc.line());
    }
};

#define LOG_FMT_LEVEL_LOCATION "{}"
#define LOG_ARG_LEVEL_LOCATION(level, loc)                                     \
    sflog::level_location {                                                    \
        level, loc                                                             \
    }

#define LOG(level, the_format, ...)                                            \
    do {                                                                       \
        if constexpr (level <= sflog::LOG_LEVEL) {                             \
            fmt::println(                                                      \
                LOG_FMT_LEVEL_LOCATION the_format,                             \
                LOG_ARG_LEVEL_LOCATION(level, std::source_location::current()) \
                    __VA_OPT__(, ) __VA_ARGS__);                               \
        }                                                                      \
    } while (0);

#define LOG_ERROR(the_format, ...)                                             \
    LOG(sflog::level_t::ERROR, the_format __VA_OPT__(, ) __VA_ARGS__)

#define LOG_WARNING(the_format, ...)                                           \
    LOG(sflog::level_t::WARNING, the_format __VA_OPT__(, ) __VA_ARGS__)

#define LOG_INFO(the_format, ...)                                              \
    LOG(sflog::level_t::INFO, the_format __VA_OPT__(, ) __VA_ARGS__)

#define LOG_DEBUG(the_format, ...)                                             \
    LOG(sflog::level_t::DEBUG, the_format __VA_OPT__(, ) __VA_ARGS__)

#endif // SFLOG_SFLOG_H
