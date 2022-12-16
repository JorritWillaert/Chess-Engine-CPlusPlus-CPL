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
  orderMoves(moves, board);
  ResultWrapper bestResult;
  bestResult.pv = PrincipalVariation();
  int best_i = 0;
  // bool noStalemateFound = false;
  for (std::vector<int>::size_type i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    Board newBoard = board;
    newBoard.makeMove(move);
    // if (newBoard.isCheck(board.turn())) {
    //   continue;
    // }
    ResultWrapper prevResult =
        alphaBetaMin(alpha, beta, depth + 1, maxDepth, newBoard);
    // if (move.from() == Square::F3 && move.to() == Square::E5 && depth == 0) {
    //   std::cout << "Score prevresult: " << prevResult.score << std::endl;
    //   std::cout << "PV: " << prevResult.pv << std::endl;
    //   Board copyBoard = newBoard;
    //   std::cout << copyBoard << std::endl;
    //   for (const Move move : prevResult.pv) {
    //     copyBoard.makeMove(move);
    //     std::cout << copyBoard << std::endl;
    //     std::cout << copyBoard.calculateScore() << std::endl;
    //   }
    // }
    // if (move.from() == Square::F3 && move.to() == Square::G1 && depth == 0) {
    //   std::cout << "Score new: " << prevResult.score << std::endl;
    // }
    // if (!prevResult.isStalemate) {
    //   noStalemateFound = true;
    // }
    if (prevResult.score > 50000) {
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
  // if (!noStalemateFound) {
  //   result.score = 0;
  //   result.isStalemate = true;
  //   return result;
  // } else {
    result.score = alpha;
    result.pv = bestResult.pv;
    Move newMove = Move(moves[best_i].from(), moves[best_i].to(),
                            moves[best_i].promotion());
    result.pv.addFront(newMove);
    return result;
  // }
}

ResultWrapper EngineJorritWillaert::alphaBetaMin(int alpha, int beta, int depth,
                                                 int maxDepth,
                                                 Board &board) {
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
  orderMoves(moves, board);
  int best_i = 0;
  ResultWrapper bestResult;
  bestResult.pv = PrincipalVariation();
  // bool noStalemateFound = true;
  for (std::vector<int>::size_type i = 0; i < moves.size(); i++) {
    Move move = moves[i];
    Board newBoard = board;
    newBoard.makeMove(move);
    // if (newBoard.isCheck(board.turn())) {
    //   continue;
    // }
    ResultWrapper prevResult =
        alphaBetaMax(alpha, beta, depth + 1, maxDepth, newBoard);
    // if (!prevResult.isStalemate) {
    //   noStalemateFound = true;
    // }
    if (prevResult.score < -50000) {
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
  // if (!noStalemateFound) {
  //   result.score = 0;
  //   result.isStalemate = true;
  //   return result;
  // } else {
    result.score = beta;
    result.pv = bestResult.pv;
    Move newMove = Move(moves[best_i].from(), moves[best_i].to(),
                            moves[best_i].promotion());
    result.pv.addFront(newMove);
    return result;
  // }
}

PrincipalVariation
EngineJorritWillaert::pv(const Board &board,
                         const TimeInfo::Optional &timeInfo) {
  Board nonConstBoard = board;
  nonConstBoard.setMaximizerColor(board.turn());
  // std::cout << "Turn: " << board.turn() << std::endl;
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
    if (result.score > 50000) {
      principVarBest = result.pv;
      principVarBest.setMate(true);
      principVarBest.setScore(result.score - 50000);
      break;
    } else if (result.score < -50000) {
      principVarBest = result.pv;
      principVarBest.setMate(true);
      principVarBest.setScore(result.score + 50000);
      return principVarBest;
    } else if (!(result.score > 500000) && !(result.score < -50000)) {
      // std::cout << "Score: " << result.score << std::endl;
      principVarBest = result.pv;
      // std::cout << result.pv;
      principVarBest.setMate(false);
      principVarBest.setScore(result.score);
    }
  }
  // std::cout << "Score: " << principVarBest.score() << std::endl;
  // std::cout << principVarBest << std::endl;
  (void)timeInfo;
  return principVarBest;
}