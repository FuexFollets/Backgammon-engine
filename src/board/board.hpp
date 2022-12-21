#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <ostream>
#include <vector>
#include <functional>
#include <exception>
#include <string>
#include <cstring>

namespace board {
    using point = signed char; // For each point, a positive number is the number of white pieces and a negative number is the number of black pieces

    constexpr int number_of_points {24};

    struct invalid_move : std::exception {
        using std::exception::what;

        const char* what() {
            const std::string exception_string;

            char* c_exception_string {new char[std::strlen(exception_string.c_str())]};

            return std::strcpy(
                    c_exception_string,
                    exception_string.c_str()
                    );
        }
    };

    struct board {
        board() : position {default_position} {} // Default initializer
        board(const board& other_board) : position {other_board.position} {}

        static constexpr const std::array<point, 24> default_position {2, 0, 0, 0, 0, -5, 0, -3, 0, 0, 0, 5, -5, 0, 0, 0, 3, 0, 5, 0, 0, 0, 0, -2};
        enum TurnType { White = 0, Black = 1}; // Enum indicating turn

        std::array<point, number_of_points> position;
        point bar {};
        point white_finish {}, black_finish {};
        TurnType turn {TurnType::White}; // 0 = white, 1 = black

        struct complete_move {
            struct partial_move {
                partial_move() = default;
                explicit partial_move(uint8_t); // Bar move
                partial_move(uint8_t , uint8_t); // Normal move
                partial_move& operator=(const partial_move&);

                partial_move(const partial_move&); // Copy constructor

                uint8_t start {}, end {}; // Start and ending positions for moves

                enum MoveType { NormalMove = 0, BarMove = 1 };

                [[nodiscard]] MoveType get_move_type() const;
                friend std::ostream& operator<<(std::ostream&, const partial_move&);
                friend std::istream& operator>>(std::istream&, partial_move&);
                void fancy_print();
            };

            complete_move(const std::initializer_list<partial_move> moves) : complete_move_composition {moves} {}

            const std::vector<partial_move> complete_move_composition; // A full move consists of multiple sub-moves
            friend std::ostream& operator<<(std::ostream&, const complete_move&);
            friend std::istream& operator>>(std::istream&, complete_move&);
            void fancy_print();
        };

        [[nodiscard]] bool is_valid_move(const complete_move&) const;
        [[nodiscard]] bool is_valid_move(const complete_move::partial_move&) const;
        void make_complete_move(const complete_move&);
        void make_partial_move(const complete_move::partial_move&);

        friend std::ostream& operator<<(std::ostream&, const board&);
        friend std::istream& operator>>(std::istream&, board&);
        void fancy_print();

        using partial_move = complete_move::partial_move;
    }; // End of struct board


    using MoveType = board::partial_move::MoveType;

    board::partial_move::partial_move(uint8_t point_number) : 
        start {UINT8_MAX}, end {point_number} {} // Bar move initializer
    board::partial_move::partial_move(uint8_t p_start, uint8_t p_end) : start {p_start}, end {p_end} {} // Normal move initializer
    board::partial_move::partial_move(const partial_move& move) = default; //  : start {move.start}, end {move.end} {}
    // (const board::partial_move::partial_move& move) {}
    board::partial_move& board::partial_move::operator=(const board::partial_move& move) = default;

    MoveType board::partial_move::get_move_type() const { // Based off of `start` member variable
        return (start == UINT8_MAX) ? MoveType::BarMove : MoveType::NormalMove;
    }

    bool board::is_valid_move(const complete_move::partial_move& move) const {
        static const std::function<bool(uint8_t)> has_one_or_less_of_opposite_turn { // Decides if the player at the current turn is able to move to the point in the parameter
            [this] (uint8_t point_number) -> bool {
                return (turn == TurnType::White) ?
                    (position.at(point_number) >= -1) : // If the turn is whites turn
                    (position.at(point_number) <= 1); // if the turn is blacks turn
            }
        };

        if (move.get_move_type() == MoveType::BarMove) {
            return has_one_or_less_of_opposite_turn(move.end);
        }

        return (
                has_one_or_less_of_opposite_turn(move.end) &&
                (turn == TurnType::White) ? (position.at(move.start) > 0) : position.at(move.start) < 0
            );
    }

    bool board::is_valid_move(const complete_move& move) const {
        return std::all_of(
                move.complete_move_composition.begin(),
                move.complete_move_composition.end(),
                [this] (const complete_move::partial_move& p_move) -> bool { return is_valid_move(p_move); }
            );
    }

    void board::make_partial_move(const complete_move::partial_move& move) {
        point difference {((turn == TurnType::White) ? point {-1} : point {1})}; // Added to the `point` moved from and subtracted from the `point` moved to
        if (move.get_move_type() == MoveType::BarMove) {

            bar += difference;
            position.at(move.end) -= difference;

            return;
        }
        // Normal move case

        position.at(move.start) += difference;
        position.at(move.end) -= difference;
    }

    void board::make_complete_move(const complete_move& move) {
        if (!is_valid_move(move)) { throw invalid_move {}; };

        for (const auto& partial_move : move.complete_move_composition) {
            make_partial_move(partial_move);
        }
    }
} // End of namespace board
