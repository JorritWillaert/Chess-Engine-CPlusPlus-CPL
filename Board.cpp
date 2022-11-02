#include "Board.hpp"

#include <cstdint>
#include <ostream>
#include <cassert>
#include <cmath>

#define set_bit(bitmap, index) (bitmap |= (1ULL << index))
#define get_bit(bitmap, index) (bitmap & (1ULL << index))
#define clear_bit(bitmap, index) (bitmap &= ~(1ULL << index))
#define get_LSB(bitmap) (__builtin_ctzll(bitmap))

const uint64_t FILE_A = 0x8080808080808080ULL; // 1000 0000 1000 000 ...
const uint64_t FILE_B = 0x4040404040404040ULL; // 0100 0000 0100 000 ...
const uint64_t FILE_C = 0x2020202020202020ULL; // 0010 0000 0010 000 ...
const uint64_t FILE_D = 0x1010101010101010ULL; // 0001 0000 0001 000 ...
const uint64_t FILE_E = 0x0808080808080808ULL; // 0000 1000 0000 100 ...
const uint64_t FILE_F = 0x0404040404040404ULL; // 0000 0100 0000 010 ...
const uint64_t FILE_G = 0x0202020202020202ULL; // 0000 0010 0000 001 ...
const uint64_t FILE_H = 0x0101010101010101ULL; // 0000 0001 0000 000 ...

const uint64_t RANK_1 = 0x00000000000000FFULL; // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111
const uint64_t RANK_2 = 0x000000000000FF00ULL; // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111 0000 0000
const uint64_t RANK_3 = 0x0000000000FF0000ULL; // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111 0000 0000 0000 0000
const uint64_t RANK_4 = 0x00000000FF000000ULL; // 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111 0000 0000 0000 0000 0000 0000
const uint64_t RANK_5 = 0x000000FF00000000ULL; // 0000 0000 0000 0000 0000 0000 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000
const uint64_t RANK_6 = 0x0000FF0000000000ULL; // 0000 0000 0000 0000 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
const uint64_t RANK_7 = 0x00FF000000000000ULL; // 0000 0000 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
const uint64_t RANK_8 = 0xFF00000000000000ULL; // 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000

constexpr uint64_t west(uint64_t bitmap) {
    return (bitmap & ~FILE_A) << 1;
}
constexpr uint64_t east(uint64_t bitmap) {
    return (bitmap & ~FILE_H) << 1;
}

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
};

Board::Board()
{
    for (int i = 0; i < 12; i++) {
        all_bitmaps_[i] = 0;
    }
}

void Board::setPiece(const Square& square, const Piece::Optional& piece) {
    (void)square;
    (void)piece;
}

Piece::Optional Board::piece(const Square& square) const {
    for (int i = 0; i < 12; i++) {
        (void)square;
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
