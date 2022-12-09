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

  using MoveIter = std::vector<std::unique_ptr<Move>>::const_iterator;

  bool isMate() const;
  int score() const;

  void addFront(std::unique_ptr<Move> move);
  void addBack(std::unique_ptr<Move> move);
  void setMate(const bool mate);
  void setScore(const int score);

  std::size_t length() const;
  MoveIter begin() const;
  MoveIter end() const;

private:
  TimeInfo::Optional timeInfo_;
  std::vector<std::unique_ptr<Move>> moves_;
  bool isMate_ = false;
  int score_ = 0;
};

std::ostream &operator<<(std::ostream &os, const PrincipalVariation &pv);

#endif
