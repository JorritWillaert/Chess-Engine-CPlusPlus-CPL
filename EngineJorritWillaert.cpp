#include "EngineJorritWillaert.hpp"
#include "PrincipalVariation.hpp"
#include <iostream>

EngineJorritWillaert::EngineJorritWillaert() {}

std::string EngineJorritWillaert::name() const { return "Jorrit Willaert"; }
std::string EngineJorritWillaert::version() const { return "1.0"; }
std::string EngineJorritWillaert::author() const { return "Jorrit Willaert"; }

void EngineJorritWillaert::newGame() {}

ResultWrapper EngineJorritWillaert::alphaBetaMax(int alpha, int beta, int depth,
                                                 int maxDepth,
                                                 const Board &board) {
  ResultWrapper result;
  result.score = 0;
  result.isStalemate = false;
  result.pv = PrincipalVariation();
  if (board.myKingDead()) {
  // if (board.isMate(board.turn())) {
    result.score = -50000 - depth;
    return result;
  }
  if (depth == maxDepth) {
    result.score = board.calculateScore();
    return result;
  }
  Board::MoveVec moves;
  board.pseudoLegalMoves(moves);
  if (moves.empty()) {
    result.score = 0;
    result.isStalemate = true;
    return result;
  }
  ResultWrapper bestResult;
  bestResult.pv = PrincipalVariation();
  int best_i = 0;
  bool noStalemateFound = false;
  for (std::vector<int>::size_type i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    Board newBoard = board;
    newBoard.makeMove(move);
    // if (newBoard.isCheck(board.turn())) {
    //   continue;
    // }
    ResultWrapper prevResult =
        alphaBetaMin(alpha, beta, depth + 1, maxDepth, newBoard);
    if (!prevResult.isStalemate) {
      noStalemateFound = true;
    }
    if (prevResult.score > 50000) {
      result.score = prevResult.score;
      result.pv = prevResult.pv;
      Move *newMove = new Move(move.from(), move.to(), move.promotion());
      result.pv.addFront(*newMove);
      return result;
    }
    if (prevResult.score >= beta) {
      result.score = beta;
      return result;
    }
    if (prevResult.score > alpha) {
      alpha = prevResult.score;
      bestResult = prevResult;
      best_i = i;
    }
  }
  if (!noStalemateFound) {
    result.score = 0;
    result.isStalemate = true;
    return result;
  } else {
    result.score = alpha;
    result.pv = bestResult.pv;
    Move *newMove = new Move(moves[best_i].from(), moves[best_i].to(),
                            moves[best_i].promotion());
    result.pv.addFront(*newMove);
    return result;
  }
}

ResultWrapper EngineJorritWillaert::alphaBetaMin(int alpha, int beta, int depth,
                                                 int maxDepth,
                                                 const Board &board) {
  ResultWrapper result;
  result.score = 0;
  result.isStalemate = false;
  result.pv = PrincipalVariation();
  // std::cout << "MIN" << std::endl;
  if (board.myKingDead()) {
  // if (board.isMate(board.turn())) {
    result.score = 50000 + depth;
    return result;
  }
  // std::cout << "End MIN" << std::endl;
  if (depth == maxDepth) {
    result.score = board.calculateScore();
    return result;
  }
  Board::MoveVec moves;
  board.pseudoLegalMoves(moves);
  if (moves.empty()) {
    result.score = 0;
    result.isStalemate = true;
    return result;
  }
  int best_i = 0;
  ResultWrapper bestResult;
  bestResult.pv = PrincipalVariation();
  bool noStalemateFound = true;
  for (std::vector<int>::size_type i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    Board newBoard = board;
    newBoard.makeMove(move);
    // if (newBoard.isCheck(board.turn())) {
    //   continue;
    // }
    ResultWrapper prevResult =
        alphaBetaMax(alpha, beta, depth + 1, maxDepth, newBoard);
    if (!prevResult.isStalemate) {
      noStalemateFound = true;
    }
    if (prevResult.score < -50000) {
      result.score = prevResult.score;
      result.pv = prevResult.pv;
      Move *newMove = new Move(move.from(), move.to(), move.promotion());
      result.pv.addFront(*newMove);
      return result;
    }
    if (prevResult.score <= alpha) {
      result.score = alpha;
      return result;
    }
    if (prevResult.score < beta) {
      beta = prevResult.score;
      bestResult = prevResult;
      best_i = i;
    }
  }
  if (!noStalemateFound) {
    result.score = 0;
    result.isStalemate = true;
    return result;
  } else {
    result.score = beta;
    result.pv = bestResult.pv;
    Move *newMove = new Move(moves[best_i].from(), moves[best_i].to(),
                            moves[best_i].promotion());
    result.pv.addFront(*newMove);
    return result;
  }
}

PrincipalVariation
EngineJorritWillaert::pv(const Board &board,
                         const TimeInfo::Optional &timeInfo) {
  PrincipalVariation principVarBest;
  ResultWrapper result;
  int depthToSearch = 6;
  for (int maxDepth = 1; maxDepth < depthToSearch; maxDepth++) {
    result = alphaBetaMax(-100000, 100000, 0, maxDepth, board);
    if (result.isStalemate) {
      principVarBest = PrincipalVariation();
      principVarBest.setScore(0);
      return principVarBest;
    }
    if (result.score > 50000) {
      principVarBest = result.pv;
      principVarBest.setMate(true);
      principVarBest.setScore(result.score - 50000);
      break;
    } else if (result.score < -50000) {
      principVarBest = result.pv;
      principVarBest.setMate(true);
      principVarBest.setScore(result.score + 50000);
    } else if (!(result.score > 500000) && !(result.score < -50000) && (result.score > principVarBest.score())) {
      principVarBest = result.pv;
      principVarBest.setMate(false);
      principVarBest.setScore(result.score);
    }
  }
  (void)timeInfo;
  return principVarBest;
}