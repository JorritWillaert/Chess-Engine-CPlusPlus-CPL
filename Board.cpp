#include "Board.hpp"

#include <ostream>
#include <cassert>
#include <cmath>

Board::Board()
    : pawn_white_(0), pawn_black_(0), rook_white_(0), rook_black_(0),
      bishop_white_(0), bishop_black_(0), knight_white_(0), knight_black_(0),
      queen_white_(0), queen_black_(0), king_white_(0), king_black_(0) {}

void Board::setPiece(const Square& square, const Piece::Optional& piece) {
    (void)square;
    (void)piece;
}

Piece::Optional Board::piece(const Square& square) const {
    (void)square;
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
