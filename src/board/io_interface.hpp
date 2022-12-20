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
}
