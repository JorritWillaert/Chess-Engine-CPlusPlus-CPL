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
  Move *bestMove;
  for (Move move : moves) {
    Board newBoard = board;
    newBoard.makeMove(move);
    int score =
        alphaBetaMin(alpha, beta, depth + 1, maxDepth, newBoard, principVar);
    if (score > 50000 || score < -50000) {
      return score;
    }
    if (score >= beta) {
      return beta;
    }
    if (score > alpha) {
      alpha = score;
      bestMove = &move;
    }
  }
  principVar.addFront(*bestMove);
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
  Move *bestMove;
  for (Move move : moves) {
    Board newBoard = board;
    newBoard.makeMove(move);
    int score =
        alphaBetaMax(alpha, beta, depth + 1, maxDepth, newBoard, principVar);
    if (score > 50000 || score < -50000) {
      return score;
    }
    if (score <= alpha) {
      return alpha;
    }
    if (score < beta) {
      beta = score;
      bestMove = &move;
    }
  }
  principVar.addFront(*bestMove);
  return beta;
}

PrincipalVariation
EngineJorritWillaert::pv(const Board &board,
                         const TimeInfo::Optional &timeInfo) {
  auto moves = Board::MoveVec();
  auto principVar = PrincipalVariation();
  int finalScore = alphaBetaMax(-100000, 100000, 0, 3, board, principVar);
  if (finalScore > 50000 || finalScore < -50000) {
    principVar.setMate(true);
    if (finalScore > 50000) {
      principVar.setScore(finalScore - 50000);
    } else {
      principVar.setScore(-finalScore - 50000);
    }
  }
  principVar.setScore(finalScore);
  (void)timeInfo;
  return principVar;
}