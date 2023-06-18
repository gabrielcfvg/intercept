#pragma once

// builtin
#include <fmt/core.h>
#include <string>

// extern
#include <fmt/color.h>
#include <fmt/format.h>



[[noreturn]]
inline void panic(std::string const& message) {

    auto fmt_message = fmt::format("[PANIC]: {}", message);
    fmt::println("{}", fmt::styled(fmt_message, fmt::fg(fmt::color::red)));
    exit(1);
}


void rb_runtime_assert(bool result, std::string const& message);

inline void rb_assert(bool result, std::string const& message = "") {
#if !defined NDEBUG

    rb_runtime_assert(result, message);

#endif
}

inline void rb_runtime_assert(bool result, std::string const& message = "") {
    if (result == true) [[unlikely]]
        return;

    std::string panic_message;

    if (message.empty())
        panic_message = "assertion failed";
    else
        panic_message = fmt::format("assertion failed: {}", message);

    panic(panic_message);
}
