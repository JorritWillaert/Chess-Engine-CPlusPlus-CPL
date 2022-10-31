#include "Board.hpp"

#include <ostream>
#include <cassert>
#include <cmath>

enum class BitMapType {
    PawnWhite,
    PawnBlack,
    RookWhite,
    RookBlack,
    BishopWhite,
    BishopBlack,
    KnightWhite,
    KnightBlack,
    QueenWhite,
    QueenBlack,
    KingWhite,
    KingBlack,
    Last // Only used for iteration
}

void initialize_all_bitmaps() {
    for (int i = BitMapType.PawnWhite; i < BitMapType.Last; i++) {
        all_bitmaps_[i] = 0;
    }
}

Board::Board()
{
    initialize_all_bitmaps();
}

void Board::setPiece(const Square& square, const Piece::Optional& piece) {
    (void)square;
    (void)piece;
}

Piece::Optional Board::piece(const Square& square) const {
    for (int i = BitMapType.PawnWhite; i < BitMapType.Last; i++) {
        // TODO
    }
    return std::nullopt;
}

void Board::setTurn(PieceColor turn) {
    turn_ = turn;
}

PieceColor Board::turn() const {
    return turn_;
}

void Board::setCastlingRights(CastlingRights cr) {
    (void)cr;
}

CastlingRights Board::castlingRights() const {
    return CastlingRights::None;
}

void Board::setEnPassantSquare(const Square::Optional& square) {
    (void)square;
}

Square::Optional Board::enPassantSquare() const {
    return std::nullopt;
}

void Board::makeMove(const Move& move) {
    (void)move;
}

void Board::pseudoLegalMoves(MoveVec& moves) const {
    (void)moves;
}

void Board::pseudoLegalMovesFrom(const Square& from,
                                 Board::MoveVec& moves) const {
    (void)from;
    (void)moves;
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
    (void)board;
    return os;
}
