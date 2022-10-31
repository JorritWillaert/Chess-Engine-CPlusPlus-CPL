#include "Move.hpp"

#include <ostream>

Move::Move(const Square& from, const Square& to,
           const std::optional<PieceType>& promotion)
    : from_(from), to_(to), promotion_(promotion)
{
}

Move::Optional Move::fromUci(const std::string& uci) {
    /* Correct size */
    if (uci.size() != 4 || uci.size() != 5) {
        return std::nullopt;
    }

    /* Valid squares */
    if (uci[0] < 'a' || uci[0] > 'h' || uci[1] < '1' || uci[1] > '8' ||
        uci[2] < 'a' || uci[2] > 'h' || uci[3] < '1' || uci[3] > '8') {
        return std::nullopt;
    }

    /* Valid promotion */
    if (uci.size() == 5) {
        if (uci[4] != 'q' && uci[4] != 'r' && uci[4] != 'b' && uci[4] != 'n') {
            return std::nullopt;
        }
    }
    
    /* Create move */
    auto from = Square::fromCoordinates(uci[0] - 'a', uci[1] - '1');
    auto to = Square::fromCoordinates(uci[2] - 'a', uci[3] - '1');
    if (!from.has_value() || !to.has_value()) {
        return std::nullopt;
    } 
    Square fromSquare = from.value();
    Square toSquare = to.value();
    
    if (uci.size() == 5) {
        PieceType promotion;
        switch (uci[4]) {
            case 'q':
                promotion = PieceType::Queen;
                break;
            case 'r':
                promotion = PieceType::Rook;
                break;
            case 'b':
                promotion = PieceType::Bishop;
                break;
            case 'n':
                promotion = PieceType::Knight;
                break;
        }
        return Move(fromSquare, toSquare, promotion);
    } else {
        return Move(fromSquare, toSquare);
    }
}

Square Move::from() const {
    return Square::A1;
}

Square Move::to() const {
    return Square::A1;
}

std::optional<PieceType> Move::promotion() const {
    return std::nullopt;
}

std::ostream& operator<<(std::ostream& os, const Move& move) {
    (void)move;
    return os;
}


bool operator<(const Move& lhs, const Move& rhs) {
    (void)lhs;
    (void)rhs;
    return false;
}

bool operator==(const Move& lhs, const Move& rhs) {
    (void)lhs;
    (void)rhs;
    return false;
}
