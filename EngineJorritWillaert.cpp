#include "EngineJorritWillaert.hpp"
#include "PrincipalVariation.hpp"

EngineJorritWillaert::EngineJorritWillaert() {}

std::string EngineJorritWillaert::name() const { return "Jorrit Willaert"; }
std::string EngineJorritWillaert::version() const { return "1.0"; }
std::string EngineJorritWillaert::author() const { return "Jorrit Willaert"; }

void EngineJorritWillaert::newGame() {}

Move searchForBestMove(const Board &board, Board::MoveVec moves) {
  Move bestMove = moves[0];
  return bestMove; // TODO calculate the actual best move
}

int calculateScore(const Board &board) {
  return 0; // TODO calculate the actual score
}

PrincipalVariation
EngineJorritWillaert::pv(const Board &board,
                         const TimeInfo::Optional &timeInfo) {
  auto moves = Board::MoveVec();
  board.pseudoLegalMoves(moves);
  PrincipalVariation principVar = PrincipalVariation();
  bool check = board.isCheck();

  if (check && moves.empty()) {
    principVar.setMate(true);
    principVar.setScore(0); // In case of mate, score is number of plies (= 0)
  } else if (!check && moves.empty()) {
    principVar.setDraw(true);
    principVar.setScore(0);
  } else {
    Move move = searchForBestMove(board, moves);
    principVar.addMove(move);
    principVar.setScore(calculateScore(board));
  }
  return principVar;
}