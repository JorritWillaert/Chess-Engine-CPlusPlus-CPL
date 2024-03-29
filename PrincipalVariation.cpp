#include "PrincipalVariation.hpp"
#include "Board.hpp"

#include <ostream>

PrincipalVariation::PrincipalVariation() : isMate_(false), score_(0) {}

bool PrincipalVariation::isMate() const { return isMate_; }

int PrincipalVariation::score() const { return score_; }

std::size_t PrincipalVariation::length() const { return moves_.size(); }

PrincipalVariation::MoveIter PrincipalVariation::begin() const {
  return moves_.begin();
}

PrincipalVariation::MoveIter PrincipalVariation::end() const {
  return moves_.end();
}

void PrincipalVariation::addFront(const Move &move) {
  moves_.insert(moves_.begin(), move);
}

void PrincipalVariation::addBack(const Move &move) { moves_.push_back(move); }

void PrincipalVariation::setMate(const bool mate) { isMate_ = mate; }

void PrincipalVariation::setScore(const int score) { score_ = score; }

void PrincipalVariation::removeLastTwoMoves() {
  moves_.pop_back();
  moves_.pop_back();
}

std::ostream &operator<<(std::ostream &os, const PrincipalVariation &pv) {
  os << "PV: ";
  for (const Move move : pv) {
    os << move << ' ';
  }
  return os;
}
