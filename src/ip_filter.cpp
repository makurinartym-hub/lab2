#include "ip_filter.h"
#include <iostream>
#include <string>

int main() {
    try {
        IPPool pool;
        for (std::string line; std::getline(std::cin, line);) {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            const auto tab = line.find('\t');
            pool.push_back(parse_ip(line.substr(0, tab)));
        }
        sort_ips(pool);
        const auto print_pool = [](const IPPool& addresses) {
            for (const auto& ip : addresses) {
                print_ip(std::cout, ip);
                std::cout << '\n';
            }
        };
        print_pool(pool);
        print_pool(filter_first(pool, 1));
        print_pool(filter_first_two(pool, 46, 70));
        print_pool(filter_any(pool, 46));
        const auto found = std::find(pool.begin(), pool.end(), target_ip)
                           != pool.end();
        std::cerr << "Target IP: ";
        print_ip(std::cerr, target_ip);
        std::cerr << " — " << (found ? "найдено" : "не найдено") << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
