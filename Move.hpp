#ifndef CHESS_ENGINE_MOVE_HPP
#define CHESS_ENGINE_MOVE_HPP

#include "Piece.hpp"
#include "Square.hpp"

#include <iosfwd>
#include <optional>
#include <string>

class Move {
public:
  using Optional = std::optional<Move>;

  Move(const Square &from, const Square &to,
       const std::optional<PieceType> &promotion = std::nullopt);

  static Optional fromUci(const std::string &uci);

  Square from() const;
  Square to() const;
  std::optional<PieceType> promotion() const;

  int tempValue() const;
  void setTempValue(int value);

private:
  Square from_;
  Square to_;
  std::optional<PieceType> promotion_;
  int tempValue_;
};

std::ostream &operator<<(std::ostream &os, const Move &move);

// Needed for std::map, std::set
bool operator<(const Move &lhs, const Move &rhs);
bool operator==(const Move &lhs, const Move &rhs);

#endif
