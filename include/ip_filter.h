#pragma once
#include <algorithm>
#include <charconv>
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

using IP = std::tuple<int, int, int, int>;
using IPPool = std::vector<IP>;

inline IP parse_ip(const std::string& text) {
    int parts[4]{};
    std::size_t start = 0;
    for (int i = 0; i < 4; ++i) {
        const auto end = text.find('.', start);
        if ((i < 3 && end == std::string::npos) ||
            (i == 3 && end != std::string::npos)) {
            throw std::invalid_argument("IP must contain four octets");
        }
        const auto stop = end == std::string::npos ? text.size() : end;
        const auto* first = text.data() + start;
        const auto* last = text.data() + stop;
        const auto [ptr, error] = std::from_chars(first, last, parts[i]);
        if (error != std::errc{} || ptr != last ||
            parts[i] < 0 || parts[i] > 255 || *first == '-') {
            throw std::invalid_argument("Invalid IP octet");
        }
        start = stop + 1;
    }
    return {parts[0], parts[1], parts[2], parts[3]};
}

inline void print_ip(std::ostream& output, const IP& ip) {
    const auto [a, b, c, d] = ip;
    output << a << '.' << b << '.' << c << '.' << d;
}

inline void sort_ips(IPPool& pool) {
    std::sort(pool.begin(), pool.end(),
        [](const auto& left, const auto& right) {
            return left > right;
        });
}

template <typename Predicate>
IPPool filter_ips(const IPPool& pool, Predicate condition) {
    IPPool result;
    for (const auto& ip : pool) {
        if (condition(ip)) {
            result.push_back(ip);
        }
    }
    return result;
}

inline IPPool filter_first(const IPPool& pool, int first) {
    return filter_ips(pool, [first](const auto& ip) {
        return std::get<0>(ip) == first;
    });
}

inline IPPool filter_first_two(const IPPool& pool, int first, int second) {
    return filter_ips(pool, [first, second](const auto& ip) {
        return std::get<0>(ip) == first && std::get<1>(ip) == second;
    });
}

inline IPPool filter_any(const IPPool& pool, int value) {
    return filter_ips(pool, [value](const auto& ip) {
        const auto [a, b, c, d] = ip;
        return a == value || b == value || c == value || d == value;
    });
}

consteval IP generate_ip(std::uint32_t value) {
    for (int i = 0; i < 10000; ++i) {
        value ^= value << 13;
        value ^= value >> 17;
        value *= 0x45D9F3B;
        value ^= value >> 16;
    }
    return {
        static_cast<int>((value >> 24) & 255u),
        static_cast<int>((value >> 16) & 255u),
        static_cast<int>((value >> 8) & 255u),
        static_cast<int>(value & 255u)
    };
}

inline constexpr IP target_ip = generate_ip(12345u);
