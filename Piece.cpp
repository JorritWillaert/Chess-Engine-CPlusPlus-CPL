#include "Piece.hpp"

#include <ostream>
#include <cctype>

Piece::Piece(PieceColor color, PieceType type)
 : piece_color_(color), piece_type_(type) {}

Piece::Optional Piece::fromSymbol(char symbol) {
    PieceColor color = isupper(symbol) ? PieceColor::White : PieceColor::Black;
    switch(tolower(symbol)) {
        case 'p': return Piece(color, PieceType::Pawn);
        case 'n': return Piece(color, PieceType::Knight);
        case 'b': return Piece(color, PieceType::Bishop);
        case 'r': return Piece(color, PieceType::Rook);
        case 'q': return Piece(color, PieceType::Queen);
        case 'k': return Piece(color, PieceType::King);
    }
    return std::nullopt;
}

PieceColor Piece::color() const {
    return piece_color_;
}

PieceType Piece::type() const {
    return piece_type_;
}

bool operator==(const Piece& lhs, const Piece& rhs) {
    if (lhs.color() == rhs.color() && lhs.type() == rhs.type()) {
        return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const Piece& piece) {
    (void)piece;
    return os;
}

PieceColor operator!(PieceColor color) {
    (void)color;
    return PieceColor::White;
}
