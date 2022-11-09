#ifndef CHESS_ENGINE_BOARD_HPP
#define CHESS_ENGINE_BOARD_HPP

#include "CastlingRights.hpp"
#include "Move.hpp"
#include "Piece.hpp"
#include "Square.hpp"

#include <iosfwd>
#include <optional>
#include <vector>

class Board {
public:
  using Optional = std::optional<Board>;
  using MoveVec = std::vector<Move>;

  Board();

  void setPiece(const Square &square, const Piece::Optional &piece);
  void removePiece(const Square &square, const Piece::Optional &piece);
  Piece::Optional piece(const Square &square) const;
  void setTurn(PieceColor turn);
  PieceColor turn() const;
  void setCastlingRights(CastlingRights cr);
  CastlingRights castlingRights() const;
  void setEnPassantSquare(const Square::Optional &square);
  Square::Optional enPassantSquare() const;

  void makeMove(const Move &move);

  void pseudoLegalMoves(MoveVec &moves) const;
  void pseudoLegalMovesFrom(const Square &from, MoveVec &moves) const;

  void add_pseudo_pawn_moves(const Square &from, Board::MoveVec &moves,
                             const PieceColor color) const;
  void add_pseudo_knight_moves(const Square &from, Board::MoveVec &moves) const;
  void add_pseudo_bishop_moves(const Square &from, Board::MoveVec &moves) const;
  void add_pseudo_rook_moves(const Square &from, Board::MoveVec &moves) const;
  void add_pseudo_queen_moves(const Square &from, Board::MoveVec &moves) const;
  void add_pseudo_king_moves(const Square &from, Board::MoveVec &moves) const;

  uint64_t get_all_friendly_pieces() const;
  uint64_t get_all_opponent_pieces() const;

private:
  uint64_t all_bitmaps_[12]; // WhitePawn, WhiteKnight, WhiteBishop, WhiteRook,
                             // WhiteQueen, WhiteKing, BlackPawn, BlackKnight,
                             // BlackBishop, BlackRook, BlackQueen, BlackKing
  PieceColor turn_;
  CastlingRights castling_rights_;
  std::optional<Square> en_passant_square_;
};

std::ostream &operator<<(std::ostream &os, const Board &board);

#endif
