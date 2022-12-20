#pragma once

#include <array>
#include <vector>

namespace board {
    using point = signed char;

    struct board {
        board() : position {default_position} {} // Default initializer

        static constexpr const std::array<point, 24> default_position {2, 0, 0, 0, 0, -5, 0, -3, 0, 0, 0, 5, -5, 0, 0, 0, 3, 0, 5, 0, 0, 0, 0, -2};

        std::array<point, 24> position;
        point bar;
        point white_finish, black_finish;
        bool turn; // 0 = white, 1 = black

        struct complete_move {
            struct partial_move {
                partial_move() {}
                partial_move(point p); // Bar move
                partial_move(point p_start, point p_end); // Normal move

                point start, end;

                bool move_type(); // false = normal move, true = moving off the bar
            };

            complete_move(const std::initializer_list<partial_move> moves) : complete_move_composition {moves} {}

            std::vector<partial_move> complete_move_composition; // A full move consists of multiple sub-moves
        };

        void make_complete_move(const complete_move&);
        void make_partial_move(const complete_move::partial_move&);
    };
}
