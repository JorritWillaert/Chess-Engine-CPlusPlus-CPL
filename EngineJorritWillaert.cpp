#include "EngineJorritWillaert.hpp"
#include "PrincipalVariation.hpp"
#include <iostream>

EngineJorritWillaert::EngineJorritWillaert() {}

std::string EngineJorritWillaert::name() const { return "Jorrit Willaert"; }
std::string EngineJorritWillaert::version() const { return "1.0"; }
std::string EngineJorritWillaert::author() const { return "Jorrit Willaert"; }

void EngineJorritWillaert::newGame() {}

int EngineJorritWillaert::alphaBetaMax(int alpha, int beta, int depth,
                                       int maxDepth, const Board &board,
                                       PrincipalVariation &principVar) {
  if (depth == maxDepth) {
    return board.calculateScore();
  }
  if (board.isCheck()) {
    return -50000 - depth;
  }
  Board::MoveVec moves;
  board.pseudoLegalMoves(moves);
  int best_i = 0;
  for (std::vector<int>::size_type i = 0; i < moves.size(); ++i) {
    Move move = moves[i];
    Board newBoard = board;
    newBoard.makeMove(move);
    int score =
        alphaBetaMin(alpha, beta, depth + 1, maxDepth, newBoard, principVar);
    if (score > 50000 || score < -50000) {
      Move *moveHeap =
          new Move(moves[i].from(), moves[i].to(), moves[i].promotion());
      principVar.addFront(*moveHeap);
      return score;
    }
    if (score >= beta) {
      return beta;
    }
    if (score > alpha) {
      alpha = score;
      best_i = i;
    }
  }
  std::cout << "best_i: " << best_i << std::endl;
  Move *moveHeap = new Move(moves[best_i].from(), moves[best_i].to(),
                            moves[best_i].promotion());
  principVar.addFront(*moveHeap);
  return alpha;
}

int EngineJorritWillaert::alphaBetaMin(int alpha, int beta, int depth,
                                       int maxDepth, const Board &board,
                                       PrincipalVariation &principVar) {
  if (depth == maxDepth) {
    return -board.calculateScore();
  }
  if (board.isCheck()) {
    return 50000 + depth;
  }
  Board::MoveVec moves;
  board.pseudoLegalMoves(moves);
  int best_i = 0;
  for (std::vector<int>::size_type i = 0; i < moves.size(); ++i) {
    Move move = moves[i];
    Board newBoard = board;
    newBoard.makeMove(move);
    int score =
        alphaBetaMax(alpha, beta, depth + 1, maxDepth, newBoard, principVar);
    if (score > 50000 || score < -50000) {
      Move *moveHeap =
          new Move(moves[i].from(), moves[i].to(), moves[i].promotion());
      principVar.addFront(*moveHeap);
      return score;
    }
    if (score <= alpha) {
      return alpha;
    }
    if (score < beta) {
      beta = score;
      best_i = i;
    }
  }
  std::cout << "best_i: " << best_i << std::endl;
  Move *moveHeap = new Move(moves[best_i].from(), moves[best_i].to(),
                            moves[best_i].promotion());
  principVar.addFront(*moveHeap);
  return beta;
}

PrincipalVariation
EngineJorritWillaert::pv(const Board &board,
                         const TimeInfo::Optional &timeInfo) {
  auto principVar = PrincipalVariation();
  int finalScore = alphaBetaMax(-100000, 100000, 0, 2, board, principVar);
  std::cout << "Final score" << finalScore << std::endl;
  if (finalScore > 50000 || finalScore < -50000) {
    principVar.setMate(true);
    if (finalScore > 50000) {
      principVar.setScore(finalScore - 50000);
    } else {
      principVar.setScore(-finalScore - 50000);
    }
  }
  principVar.setScore(finalScore);
  std::cout << principVar << std::endl;
  (void)timeInfo;
  return principVar;
}