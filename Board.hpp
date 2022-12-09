#ifndef CHESS_ENGINE_BOARD_HPP
#define CHESS_ENGINE_BOARD_HPP

#include "CastlingRights.hpp"
#include "Move.hpp"
#include "Piece.hpp"
#include "Square.hpp"

#include <cstdint>
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
  uint64_t generate_pseudo_bishop_moves(const Square &from, const PieceColor &turn) const;
  void add_pseudo_rook_moves(const Square &from, Board::MoveVec &moves) const;
  uint64_t generate_pseudo_rook_moves(const Square &from, const PieceColor &turn) const;
  void add_pseudo_queen_moves(const Square &from, Board::MoveVec &moves) const;
  void add_pseudo_king_moves(const Square &from, Board::MoveVec &moves) const;

  uint64_t get_all_friendly_pieces(const PieceColor &turn) const;
  uint64_t get_all_opponent_pieces(const PieceColor &turn) const;
  uint64_t get_pawns(const PieceColor color) const;
  uint64_t get_knights(const PieceColor color) const;
  uint64_t get_bishops_and_queens(const PieceColor color) const;
  uint64_t get_rooks_and_queens(const PieceColor color) const;
  uint64_t get_king(const PieceColor color) const;

  uint64_t get_castle_moves(const Square &from) const;

  bool square_under_attack_by_color(const Square &square,
                                    const PieceColor color) const;

  bool check_pawn_to_square(const Square &square, const PieceColor color) const;
  bool check_knight_to_square(const Square &square,
                              const PieceColor color) const;
  bool check_bishop_and_queen_to_square(const Square &square,
                                        const PieceColor color) const;
  bool check_rook_and_queen_to_square(const Square &square,
                                      const PieceColor color) const;
  bool check_king_to_square(const Square &square, const PieceColor color) const;

  bool isCheck(PieceColor turn);
  bool isMate(PieceColor turn);
  bool myKingDead() const;

  int calculateScore() const;

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
