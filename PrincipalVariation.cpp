#include "PrincipalVariation.hpp"
#include "Board.hpp"

#include <ostream>

PrincipalVariation::PrincipalVariation(Board board, TimeInfo::Optional timeInfo)
    : board_(board), timeInfo_(timeInfo) {}

bool PrincipalVariation::isMate() const { Board::isMate(); }

int PrincipalVariation::score() const { return 0; }

std::size_t PrincipalVariation::length() const { return moves_.size(); }

PrincipalVariation::MoveIter PrincipalVariation::begin() const {
  return moves_.begin();
}

PrincipalVariation::MoveIter PrincipalVariation::end() const {
  return moves_.end();
}

std::ostream &operator<<(std::ostream &os, const PrincipalVariation &pv) {
  os << "Principal Variation:\n";
  for (const Move move : pv) {
    os << move << ' ';
  }
  os << '\n';
  return os;
}
