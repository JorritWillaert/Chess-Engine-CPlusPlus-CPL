#ifndef CHESS_ENGINE_PIECE_HPP
#define CHESS_ENGINE_PIECE_HPP

#include <optional>
#include <iosfwd>

enum class PieceColor {
    White,
    Black
};

enum class PieceType {
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

class Piece {
public:

    using Optional = std::optional<Piece>;

    Piece(PieceColor color, PieceType type);

    static Optional fromSymbol(char symbol);

    PieceColor color() const;
    PieceType type() const;

private:
    PieceColor piece_color_;
    PieceType piece_type_;
};

bool operator==(const Piece& lhs, const Piece& rhs);
std::ostream& operator<<(std::ostream& os, const Piece& piece);
std::ostream& operator<<(std::ostream& os, const PieceColor& pieceColor);

// Invert a color (White becomes Black and vice versa)
PieceColor operator!(PieceColor color);

#endif
