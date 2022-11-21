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

int EngineJorritWillaert::alphaBetaMax(int alpha, int beta, int depth,
                                       int maxDepth, const Board &board,
                                       PrincipalVariation &principVar) {
  if (depth == maxDepth) {
    return calculateScore(board);
  }
  if (board.isCheck()) {
    return depth;
  }
  Board::MoveVec moves;
  board.pseudoLegalMoves(moves);
  Move *bestMove;
  for (Move move : moves) {
    Board newBoard = board;
    newBoard.makeMove(move);
    int score =
        alphaBetaMin(alpha, beta, depth + 1, maxDepth, newBoard, principVar);
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
    return calculateScore(board);
  }
  if (board.isCheck()) {
    return depth;
  }
  Board::MoveVec moves;
  board.pseudoLegalMoves(moves);
  Move *bestMove;
  for (Move move : moves) {
    Board newBoard = board;
    newBoard.makeMove(move);
    int score =
        alphaBetaMax(alpha, beta, depth + 1, maxDepth, newBoard, principVar);
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
  principVar.setScore(finalScore);
  return principVar;
}