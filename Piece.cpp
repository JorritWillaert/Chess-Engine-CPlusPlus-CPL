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
    char symbol;
    switch(piece.type()) {
        case PieceType::Pawn: symbol = 'p'; break;
        case PieceType::Knight: symbol = 'n'; break;
        case PieceType::Bishop: symbol = 'b'; break;
        case PieceType::Rook: symbol = 'r'; break;
        case PieceType::Queen: symbol = 'q'; break;
        case PieceType::King: symbol = 'k'; break;
    }
    if (piece.color() == PieceColor::White) {
        symbol = toupper(symbol);
    }
    os << symbol;
    return os;
}

PieceColor operator!(PieceColor color) {
    return (color == PieceColor::White) ? PieceColor::Black : PieceColor::White;
}
