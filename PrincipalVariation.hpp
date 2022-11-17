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
  PrincipalVariation(Board board, TimeInfo::Optional timeInfo);

  using MoveIter = std::vector<Move>::const_iterator;

  bool isMate() const;
  int score() const;

  std::size_t length() const;
  MoveIter begin() const;
  MoveIter end() const;

private:
  Board board_;
  TimeInfo::Optional timeInfo_;
  std::vector<Move> moves_;
};

std::ostream &operator<<(std::ostream &os, const PrincipalVariation &pv);

#endif
