#pragma once

#include <iostream>
#include <iterator>

#include "board.hpp"

namespace board {
    std::ostream& operator<<(std::ostream& output_stream, const board& printed_board) {
        std::copy(
                printed_board.position.begin(),
                printed_board.position.end(),
                std::ostream_iterator<point>(std::cout, " ")
        );

        output_stream
            << printed_board.bar << ' '
            << printed_board.white_finish << ' '
            << ((printed_board.turn == board::TurnType::White) ? 
                    (0) : (1));

        return output_stream;
    }

    std::istream& operator>>(std::istream& input_stream, board& input_board) {
        std::copy_n(
                std::istream_iterator<int>(std::cin),
                24,
                input_board.position.begin()
        );

        int turn_int; // 0 if white, 1 if black

        input_stream
            >> input_board.bar
            >> input_board.white_finish
            >> turn_int;

        input_board.turn = (turn_int == 0) ?
            board::TurnType::White : board::TurnType::Black;

        return input_stream;
    }
}
