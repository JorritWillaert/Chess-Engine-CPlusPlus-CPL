#ifndef CHESS_ENGINE_PRINCIPALVARIATION_HPP
#define CHESS_ENGINE_PRINCIPALVARIATION_HPP

#include "Board.hpp"
#include "Move.hpp"
#include "Piece.hpp"
#include "TimeInfo.hpp"

#include <cstddef>
#include <iosfwd>
#include <memory>
#include <vector>

class PrincipalVariation {
public:
  PrincipalVariation();

  using MoveIter = std::vector<Move>::const_iterator;

  bool isMate() const;
  int score() const;

  void addFront(const Move &move);
  void addBack(const Move &move);
  void setMate(const bool mate);
  void setScore(const int score);
  void removeLastTwoMoves();

  std::size_t length() const;
  MoveIter begin() const;
  MoveIter end() const;

private:
  TimeInfo::Optional timeInfo_;
  std::vector<Move> moves_;
  bool isMate_ = false;
  int score_ = 0;
};

std::ostream &operator<<(std::ostream &os, const PrincipalVariation &pv);

#endif
