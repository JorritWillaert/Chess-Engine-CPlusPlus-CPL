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
  if (depth == maxDepth) {
    result.score = board.calculateScore();
    return result;
  }
  // if (board.isMate()) {
  //   std::cout << "Detected check max" << std::endl;
  //   return -50000 - depth;
  // }
  Board::MoveVec moves;
  board.pseudoLegalMoves(moves);
  ResultWrapper bestResult;
  int best_i = 0;
  for (std::vector<int>::size_type i = 0; i < moves.size(); ++i) {
    Move move = moves[i];
    Board newBoard = board;
    newBoard.makeMove(move);
    ResultWrapper prevResult =
        alphaBetaMax(alpha, beta, depth + 1, maxDepth, newBoard);
    // if (score > 50000 || score < -50000) {
    //   Move *moveHeap =
    //       new Move(moves[i].from(), moves[i].to(), moves[i].promotion());
    //   principVar.addFront(*moveHeap);
    //   return score;
    // }
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
  result.pv.addFront(moves[best_i]);
  return result;
}

ResultWrapper EngineJorritWillaert::alphaBetaMin(int alpha, int beta, int depth,
                                                 int maxDepth,
                                                 const Board &board) {
  ResultWrapper result;
  if (depth == maxDepth) {
    result.score = -board.calculateScore();
    return result;
  }
  // if (board.isMate()) {
  //   std::cout << "Detected check min" << std::endl;
  //   return 50000 + depth;
  // }
  Board::MoveVec moves;
  board.pseudoLegalMoves(moves);
  int best_i = 0;
  ResultWrapper bestResult;
  for (std::vector<int>::size_type i = 0; i < moves.size(); ++i) {
    Move move = moves[i];
    Board newBoard = board;
    newBoard.makeMove(move);
    ResultWrapper prevResult =
        alphaBetaMax(alpha, beta, depth + 1, maxDepth, newBoard);
    // if (score > 50000 || score < -50000) {
    //   Move *moveHeap =
    //       new Move(moves[i].from(), moves[i].to(), moves[i].promotion());
    //   principVar.addFront(*moveHeap);
    //   return score;
    // }
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
  result.pv.addFront(moves[best_i]);
  return result;
}

PrincipalVariation
EngineJorritWillaert::pv(const Board &board,
                         const TimeInfo::Optional &timeInfo) {
  auto principVar = PrincipalVariation();
  auto followingPrincipVar = PrincipalVariation();
  ResultWrapper result = alphaBetaMax(-100000, 100000, 0, 3, board);
  principVar = followingPrincipVar;
  std::cout << "Final score" << result.score << std::endl;
  if (result.score > 100 || result.score < -50000) {
    principVar.setMate(true);

    // TODO add mate in so many steps

    // if (result.scrf > 50000) {
    //   principVar.setScore(finalScore - 50000);
    // } else {
    //   principVar.setScore(-finalScore - 50000);
    // }
  }
  principVar.setScore(result.score);
  std::cout << principVar << std::endl;
  (void)timeInfo;
  return principVar;
}