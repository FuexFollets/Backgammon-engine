#pragma once

#include <array>
#include <vector>

namespace board {
    using point = signed char;

    static constexpr const std::array<point, 24> default_position {2, 0, 0, 0, 0, -5, 0, -3, 0, 0, 0, 5, -5, 0, 0, 0, 3, 0, 5, 0, 0, 0, 0, -2};

    struct board {
        std::array<point, 24> position;
        point bar;
        point white_finish, black_finish;
        bool turn; // 0 = white, 1 = black

        struct complete_move {
            struct individual_move {
                individual_move() {}
                individual_move(point p); // Bar move
                individual_move(point p_start, point p_end); // Normal move

                point start, end;

                bool move_type(); // false = normal move, true = moving off the bar
            };

            complete_move(const std::initializer_list<individual_move> moves) : complete_move_composition {moves} {}

            std::vector<individual_move> complete_move_composition;
        };

        void make_move(const complete_move&);
    };
}
