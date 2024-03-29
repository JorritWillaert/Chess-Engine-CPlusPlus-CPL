#include "EngineJorritWillaert.hpp"
#include "PrincipalVariation.hpp"
#include <iostream>
#include <algorithm>

EngineJorritWillaert::EngineJorritWillaert() {}

std::string EngineJorritWillaert::name() const { return "Jorrit Willaert"; }
std::string EngineJorritWillaert::version() const { return "1.0"; }
std::string EngineJorritWillaert::author() const { return "Jorrit Willaert"; }

void EngineJorritWillaert::newGame() {}

void orderMoves(Board::MoveVec &moves, Board &board) {
  for (std::vector<int>::size_type i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    Board copy = board;
    copy.makeMove(move);
    moves[i].setTempValue(copy.calculateScore());
  }
  std::sort(moves.begin(), moves.end(),
            [](const Move &a, const Move &b) -> bool {
              return a.tempValue() > b.tempValue();
            });
}

ResultWrapper EngineJorritWillaert::alphaBetaMax(int alpha, int beta, int depth,
                                                 int maxDepth,
                                                 Board &board) {
  ResultWrapper result;
  result.score = 0;
  result.isStalemate = false;
  result.pv = PrincipalVariation();
  if (board.myKingDead()) {
    result.score = -50000 - depth + 2; // Was mate 2 moves earlier
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
  orderMoves(moves, board);
  ResultWrapper bestResult;
  bestResult.pv = PrincipalVariation();
  bestResult.isStalemate = false;
  int best_i = 0;
  bool noStalemateFound = false;
  for (std::vector<int>::size_type i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    Board newBoard = board;
    newBoard.makeMove(move);
    ResultWrapper prevResult =
        alphaBetaMin(alpha, beta, depth + 1, maxDepth, newBoard);
    if (prevResult.isStalemate) {
      bestResult.isStalemate = true;
      break;
    }
    if (prevResult.score == -50000 - depth) {
      Board testBoard = board;
      testBoard.setTurn(!testBoard.turn());
      ResultWrapper testWrapper = alphaBetaMin(alpha, beta, depth + 1, depth + 2, testBoard);
      if (testWrapper.score != -50000 - depth && depth <= 1) {
        continue;
      }
    }
    noStalemateFound = true;
    if (prevResult.score >= 50000) {
      result.score = prevResult.score;
      result.pv = prevResult.pv;
      Move newMove = Move(move.from(), move.to(), move.promotion());
      result.pv.addFront(newMove);
      return result;
    }
    if (prevResult.score >= beta) {
      result.score = beta;
      result.pv = bestResult.pv;
      Move newMove = Move(moves[best_i].from(), moves[best_i].to(),
                              moves[best_i].promotion());
      result.pv.addFront(newMove);
      return result;
    }
    if (prevResult.score > alpha) {
      alpha = prevResult.score;
      bestResult = prevResult;
      best_i = i;
    }
  }
  if (bestResult.isStalemate || (!noStalemateFound)) {
    result.score = 0;
    result.isStalemate = true;
    return result;
  } else {
    result.score = alpha;
    result.pv = bestResult.pv;
    Move newMove = Move(moves[best_i].from(), moves[best_i].to(),
                            moves[best_i].promotion());
    result.pv.addFront(newMove);
    return result;
  }
}

ResultWrapper EngineJorritWillaert::alphaBetaMin(int alpha, int beta, int depth,
                                                 int maxDepth,
                                                 Board &board) {
  ResultWrapper result;
  result.score = 0;
  result.isStalemate = false;
  result.pv = PrincipalVariation();
  if (board.myKingDead()) {
    result.score = 50000 + depth - 2; // Was mate 2 moves earlier
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
  orderMoves(moves, board);
  int best_i = 0;
  ResultWrapper bestResult;
  bestResult.pv = PrincipalVariation();
  bestResult.isStalemate = false;
  bool noStalemateFound = false;
  for (std::vector<int>::size_type i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    Board newBoard = board;
    newBoard.makeMove(move);
    ResultWrapper prevResult =
        alphaBetaMax(alpha, beta, depth + 1, maxDepth, newBoard);
    if (prevResult.isStalemate) {
      bestResult.isStalemate = true;
      break;
    }
    if (prevResult.score == 50000 + depth) {
      Board testBoard = board;
      testBoard.setTurn(!testBoard.turn());
      ResultWrapper testWrapper = alphaBetaMax(alpha, beta, depth + 1, depth + 2, testBoard);
      if (testWrapper.score != 50000 + depth && depth <= 1) {
        continue;
      }
    }
    noStalemateFound = true;
    if (prevResult.score <= -50000) {
      result.score = prevResult.score;
      result.pv = prevResult.pv;
      Move newMove = Move(move.from(), move.to(), move.promotion());
      result.pv.addFront(newMove);
      return result;
    }
    if (prevResult.score <= alpha) {
      result.score = alpha;
      result.pv = bestResult.pv;
      Move newMove = Move(moves[best_i].from(), moves[best_i].to(),
                              moves[best_i].promotion());
      result.pv.addFront(newMove);
      return result;
    }
    if (prevResult.score < beta) {
      beta = prevResult.score;
      bestResult = prevResult;
      best_i = i;
    }
  }
  if (bestResult.isStalemate || (!noStalemateFound)) {
    result.score = 0;
    result.isStalemate = true;
    return result;
  } else {
    result.score = beta;
    result.pv = bestResult.pv;
    Move newMove = Move(moves[best_i].from(), moves[best_i].to(),
                            moves[best_i].promotion());
    result.pv.addFront(newMove);
    return result;
  }
}

PrincipalVariation
EngineJorritWillaert::pv(const Board &board,
                         const TimeInfo::Optional &timeInfo) {
  Board nonConstBoard = board;
  nonConstBoard.setMaximizerColor(board.turn());
  PrincipalVariation principVarBest;
  ResultWrapper result;
  int depthToSearch = 5;
  for (int maxDepth = 1; maxDepth <= depthToSearch; maxDepth++) {
    result = alphaBetaMax(-100000, 100000, 0, maxDepth, nonConstBoard);
    if (result.isStalemate) {
      principVarBest = PrincipalVariation();
      principVarBest.setScore(0);
      return principVarBest;
    }
    if (result.score >= 50000) {
      principVarBest = result.pv;
      principVarBest.removeLastTwoMoves();
      principVarBest.setMate(true);
      principVarBest.setScore(result.score - 50000);
      break;
    } else if (result.score <= -50000) {
      principVarBest = result.pv;
      principVarBest.removeLastTwoMoves();
      principVarBest.setMate(true);
      principVarBest.setScore(result.score + 50000);
      break;
    } else {
      principVarBest = result.pv;
      principVarBest.removeLastTwoMoves();
      principVarBest.setMate(false);
      principVarBest.setScore(result.score);
    }
  }
  (void)timeInfo;
  return principVarBest;
}