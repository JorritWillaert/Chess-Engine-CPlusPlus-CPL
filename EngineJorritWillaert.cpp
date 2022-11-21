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

int EngineJorritWillaert::alphaBetaMax(int alpha, int beta, int depthLeft,
                                       Board &board, PrincipalVariation &pv) {
  if (depthLeft == 0) {
    return calculateScore(board);
  }
  Board::MoveVec moves;
  board.pseudoLegalMoves(moves);
  Move *bestMove;
  for (Move move : moves) {
    Board newBoard = board;
    newBoard.makeMove(move);
    int score = alphaBetaMin(alpha, beta, depthLeft - 1, newBoard, pv);
    if (score >= beta) {
      return beta;
    }
    if (score > alpha) {
      alpha = score;
      bestMove = &move;
    }
  }
  pv.addFront(*bestMove);
  return alpha;
}

int EngineJorritWillaert::alphaBetaMin(int alpha, int beta, int depthLeft,
                                       Board &board, PrincipalVariation &pv) {
  if (depthLeft == 0) {
    return calculateScore(board);
  }
  Board::MoveVec moves;
  board.pseudoLegalMoves(moves);
  Move *bestMove;
  for (Move move : moves) {
    Board newBoard = board;
    newBoard.makeMove(move);
    int score = alphaBetaMax(alpha, beta, depthLeft - 1, newBoard, pv);
    if (score <= alpha) {
      return alpha;
    }
    if (score < beta) {
      beta = score;
      bestMove = &move;
    }
  }
  pv.addFront(*bestMove);
  return beta;
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
    principVar.setScore(0); // In case of mate, score is number of plies (=
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