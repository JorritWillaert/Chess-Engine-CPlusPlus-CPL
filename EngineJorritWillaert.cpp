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
  result.pv = PrincipalVariation();
  if (board.myKingDead()) {
    result.score = -depth;
    result.isMate = true;
    return result;
  }
  if (depth == maxDepth) {
    result.score = board.calculateScore();
    return result;
  }
  Board::MoveVec moves;
  board.pseudoLegalMoves(moves);
  ResultWrapper bestResult;
  bestResult.pv = PrincipalVariation();
  int best_i = 0;
  for (std::vector<int>::size_type i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    // std::cout << "Move: " << move.from() << move.to() << std::endl;
    Board newBoard = board;
    newBoard.makeMove(move);
    ResultWrapper prevResult =
        alphaBetaMin(alpha, beta, depth + 1, maxDepth, newBoard);
    if (prevResult.isMate && prevResult.score > 0) {
      result.score = prevResult.score;
      result.pv = prevResult.pv;
      result.isMate = true;
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
  result.score = alpha;
  result.pv = bestResult.pv;

  Move *newMove = new Move(moves[best_i].from(), moves[best_i].to(),
                           moves[best_i].promotion());
  result.pv.addFront(*newMove);
  return result;
}

ResultWrapper EngineJorritWillaert::alphaBetaMin(int alpha, int beta, int depth,
                                                 int maxDepth,
                                                 const Board &board) {
  ResultWrapper result;
  result.pv = PrincipalVariation();
  if (board.myKingDead()) {
    result.score = depth;
    result.isMate = true;
    return result;
  }
  if (depth == maxDepth) {
    result.score = board.calculateScore();
    return result;
  }
  Board::MoveVec moves;
  board.pseudoLegalMoves(moves);
  int best_i = 0;
  ResultWrapper bestResult;
  bestResult.pv = PrincipalVariation();
  for (std::vector<int>::size_type i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    Board newBoard = board;
    newBoard.makeMove(move);
    ResultWrapper prevResult =
        alphaBetaMax(alpha, beta, depth + 1, maxDepth, newBoard);
    if (prevResult.isMate && prevResult.score < 0) {
      result.score = prevResult.score;
      result.pv = prevResult.pv;
      result.isMate = true;
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
  result.score = beta;
  result.pv = bestResult.pv;
  Move *newMove = new Move(moves[best_i].from(), moves[best_i].to(),
                           moves[best_i].promotion());
  result.pv.addFront(*newMove);
  return result;
}

PrincipalVariation
EngineJorritWillaert::pv(const Board &board,
                         const TimeInfo::Optional &timeInfo) {
  PrincipalVariation principVarBest;
  for (int maxDepth = 3; maxDepth < 7; maxDepth++) {
    ResultWrapper result = alphaBetaMax(-100000, 100000, 0, maxDepth, board);
    // std::cout << "Final pv" << result.pv << std::endl;
    // std::cout << "Final score" << result.score << std::endl;
    if (result.isMate && result.score > 0) {
      principVarBest = result.pv;
      principVarBest.setMate(true);
      principVarBest.setScore(result.score);
      break;
    }
    if (result.score > principVarBest.score()) {
      principVarBest = result.pv;
      if (result.isMate && result.score < 0) {
        principVarBest.setMate(true);
      }
      principVarBest.setScore(result.score);
    }
  }
  (void)timeInfo;
  return principVarBest;
}