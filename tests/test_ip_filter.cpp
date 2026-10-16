#include "ip_filter.h"
#include <gtest/gtest.h>

TEST(ParseIP, CorrectAddress) {
    const auto result = parse_ip("192.168.1.10");
    EXPECT_EQ(result, (IP{192, 168, 1, 10}));
}

TEST(ParseIP, RejectsInvalidAddresses) {
    EXPECT_THROW(parse_ip("256.1.2.3"), std::invalid_argument);
    EXPECT_THROW(parse_ip("1.2.3"), std::invalid_argument);
    EXPECT_THROW(parse_ip("1.2.3.4.5"), std::invalid_argument);
    EXPECT_THROW(parse_ip("1..3.4"), std::invalid_argument);
    EXPECT_THROW(parse_ip("1.2.3."), std::invalid_argument);
    EXPECT_THROW(parse_ip("-0.1.2.3"), std::invalid_argument);
}

TEST(Sorting, NumericDescendingOrder) {
    IPPool pool{
        IP{2, 255, 255, 255}, IP{10, 0, 0, 1},
        IP{46, 70, 2, 100}, IP{46, 70, 10, 2}
    };
    sort_ips(pool);
    const IPPool expected{
        IP{46, 70, 10, 2}, IP{46, 70, 2, 100},
        IP{10, 0, 0, 1}, IP{2, 255, 255, 255}
    };
    EXPECT_EQ(pool, expected);
}

TEST(Filters, FirstOctet) {
    const IPPool pool{
        IP{46, 70, 1, 2}, IP{1, 100, 2, 3}, IP{1, 2, 3, 4}
    };
    const IPPool expected{IP{1, 100, 2, 3}, IP{1, 2, 3, 4}};
    EXPECT_EQ(filter_first(pool, 1), expected);
}

TEST(Filters, FirstTwoOctets) {
    const IPPool pool{
        IP{46, 71, 1, 2}, IP{46, 70, 200, 1}, IP{46, 70, 1, 2}
    };
    const IPPool expected{IP{46, 70, 200, 1}, IP{46, 70, 1, 2}};
    EXPECT_EQ(filter_first_two(pool, 46, 70), expected);
}

TEST(Filters, AnyOctetPreservesOrderAndDuplicates) {
    const IPPool pool{
        IP{100, 1, 46, 2}, IP{46, 55, 46, 98},
        IP{46, 55, 46, 98}, IP{1, 2, 3, 4}
    };
    const IPPool expected{
        IP{100, 1, 46, 2}, IP{46, 55, 46, 98}, IP{46, 55, 46, 98}
    };
    EXPECT_EQ(filter_any(pool, 46), expected);
}

TEST(Generation, KnownSeed) {
    EXPECT_EQ(target_ip, (IP{60, 137, 213, 242}));
}
