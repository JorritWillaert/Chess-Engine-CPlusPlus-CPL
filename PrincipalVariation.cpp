#include "PrincipalVariation.hpp"
#include "Board.hpp"

#include <ostream>

PrincipalVariation::PrincipalVariation()
    : isMate_(false), isDraw_(false), score_(0) {}

bool PrincipalVariation::isMate() const {
  return isMate_;
  // if (!boards_.back().isCheck()) {
  //   return false;
  // }
  // Board::MoveVec moves;
  // boards_.back().pseudoLegalMoves(moves);
  // for (const Move &move : moves) {
  //   Board new_board = boards_.back();
  //   if (!makeMoveIsCheck(move, new_board)) {
  //     return false;
  //   }
  // }
  // return true;
}

int PrincipalVariation::score() const { return score_; }

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
