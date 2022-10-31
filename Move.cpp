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
            default:
                return std::nullopt;
        }
        return Move(fromSquare, toSquare, promotion);
    } else {
        return Move(fromSquare, toSquare);
    }
}

Square Move::from() const {
    return from_;
}

Square Move::to() const {
    return to_;
}

std::optional<PieceType> Move::promotion() const {
    return promotion_;
}

std::ostream& operator<<(std::ostream& os, const Move& move) {
    os << move.from() << move.to();
    if (move.promotion().has_value()) {
        switch (move.promotion().value()) {
            case PieceType::Queen:
                os << 'q';
                break;
            case PieceType::Rook:
                os << 'r';
                break;
            case PieceType::Bishop:
                os << 'b';
                break;
            case PieceType::Knight:
                os << 'n';
                break;
            default: 
                break;
        }
    }
    return os;
}


bool operator<(const Move& lhs, const Move& rhs) {
    if (lhs.from() < rhs.from()) {
        return true;
    } else if (lhs.from() > rhs.from()) {
        return false;
    }
    /* 'from' equal */

    if (lhs.to() < rhs.to()) {
        return true;
    } else if (lhs.to() > rhs.to()) {
        return false;
    }
    /* 'from' and 'to' equal */

    if (lhs.promotion().has_value() && rhs.promotion().has_value()) {
        return lhs.promotion().value() < rhs.promotion().value();
    } else if (rhs.promotion().has_value()) {
        return true;
    } 
    return false;
}

bool operator==(const Move& lhs, const Move& rhs) {
    if (lhs.from() != rhs.from() || lhs.to() != rhs.to()) {
        return false;
    }
    if (lhs.promotion().has_value() != rhs.promotion().has_value()) {
        return false;
    }
    if (lhs.promotion().has_value()) { /* Both have a promotion value */
        return lhs.promotion().value() == rhs.promotion().value();
    }
    return true;
}
