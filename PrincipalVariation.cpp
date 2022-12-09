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

void PrincipalVariation::addFront(std::unique_ptr<Move> move) {
  moves_.insert(moves_.begin(), std::move(move));
}

void PrincipalVariation::addBack(std::unique_ptr<Move> move) { moves_.push_back(std::move(move)); }

void PrincipalVariation::setMate(const bool mate) { isMate_ = mate; }

void PrincipalVariation::setScore(const int score) { score_ = score; }

std::ostream &operator<<(std::ostream &os, const PrincipalVariation &pv) {
  os << "PV: ";
  for (const Move move : pv) {
    os << move << ' ';
  }
  return os;
}
