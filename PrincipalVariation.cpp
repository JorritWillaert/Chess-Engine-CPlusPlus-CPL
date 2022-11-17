#include "PrincipalVariation.hpp"

#include <ostream>

bool PrincipalVariation::isMate() const { return false; }

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
