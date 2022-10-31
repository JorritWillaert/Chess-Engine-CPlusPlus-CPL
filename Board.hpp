#ifndef CHESS_ENGINE_BOARD_HPP
#define CHESS_ENGINE_BOARD_HPP

#include "Piece.hpp"
#include "Square.hpp"
#include "Move.hpp"
#include "CastlingRights.hpp"

#include <optional>
#include <iosfwd>
#include <vector>

class Board {
public:

    using Optional = std::optional<Board>;
    using MoveVec = std::vector<Move>;

    Board();

    void setPiece(const Square& square, const Piece::Optional& piece);
    Piece::Optional piece(const Square& square) const;
    void setTurn(PieceColor turn);
    PieceColor turn() const;
    void setCastlingRights(CastlingRights cr);
    CastlingRights castlingRights() const;
    void setEnPassantSquare(const Square::Optional& square);
    Square::Optional enPassantSquare() const;

    void makeMove(const Move& move);

    void pseudoLegalMoves(MoveVec& moves) const;
    void pseudoLegalMovesFrom(const Square& from, MoveVec& moves) const;

private:
    uint64_t pawn_white_;
    uint64_t pawn_black_;
    uint64_t rook_white_;
    uint64_t rook_black_;
    uint64_t bishop_white_;
    uint64_t bishop_black_;
    uint64_t knight_white_;
    uint64_t knight_black_;
    uint64_t queen_white_;
    uint64_t queen_black_;
    uint64_t king_white_;
    uint64_t king_black_;

    PieceColor turn_;
};

std::ostream& operator<<(std::ostream& os, const Board& board);

#endif
