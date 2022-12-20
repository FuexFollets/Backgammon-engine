#pragma once

#include <array>

namespace board {
    using point = signed char;

    const std::array<point, 24> default_position {2, 0, 0, 0, 0, -5, 0, -3, 0, 0, 0, 5, -5, 0, 0, 0, 3, 0, 5, 0, 0, 0, 0, -2};

    struct board {
        std::array<point, 24> position;
        point bar;
        point white_finish, black_finish;

        bool turn; // 0 = white, 1 = black
    };
}
