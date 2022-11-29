#include "EngineJorritWillaert.hpp"
#include "PrincipalVariation.hpp"
#include <iostream>

#define set_bit(bitmap, index) (bitmap |= (1ULL << index))
#define get_bit(bitmap, index) (bitmap & (1ULL << index))
#define clear_bit(bitmap, index) (bitmap &= ~(1ULL << index))
#define get_LSB(bitmap) (__builtin_ctzll(bitmap))

constexpr int pop_lsb(uint64_t &bitmap) {
  int index = get_LSB(bitmap);
  bitmap &= bitmap - 1;
  return index;
}

/* If you read the bit number from left to right, you move from the upper left
 * of the board to the lower right of the board */
const uint64_t FILE_A = 0x8080808080808080ULL;  // 1000 0000 1000 000 ...
const uint64_t FILE_B = 0x4040404040404040ULL;  // 0100 0000 0100 000 ...
const uint64_t FILE_C = 0x2020202020202020ULL;  // 0010 0000 0010 000 ...
const uint64_t FILE_D = 0x1010101010101010ULL;  // 0001 0000 0001 000 ...
const uint64_t FILE_E = 0x0808080808080808ULL;  // 0000 1000 0000 100 ...
const uint64_t FILE_F = 0x0404040404040404ULL;  // 0000 0100 0000 010 ...
const uint64_t FILE_G = 0x0202020202020202ULL;  // 0000 0010 0000 001 ...
const uint64_t FILE_H = 0x0101010101010101ULL;  // 0000 0001 0000 000 ...
const uint64_t FILE_AB = 0xc0c0c0c0c0c0c0c0ULL; // 1100 0000 1100 000 ...
const uint64_t FILE_GH = 0x0303030303030303ULL; // 0000 0011 0000 001 ...

const uint64_t RANK_1 =
    0x00000000000000FFULL; // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
                           // 0000 0000 0000 0000 1111 1111
const uint64_t RANK_2 =
    0x000000000000FF00ULL; // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
                           // 0000 0000 1111 1111 0000 0000
const uint64_t RANK_3 =
    0x0000000000FF0000ULL; // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
                           // 1111 1111 0000 0000 0000 0000
const uint64_t RANK_4 =
    0x00000000FF000000ULL; // 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111
                           // 0000 0000 0000 0000 0000 0000
const uint64_t RANK_5 =
    0x000000FF00000000ULL; // 0000 0000 0000 0000 0000 0000 1111 1111 0000 0000
                           // 0000 0000 0000 0000 0000 0000
const uint64_t RANK_6 =
    0x0000FF0000000000ULL; // 0000 0000 0000 0000 1111 1111 0000 0000 0000 0000
                           // 0000 0000 0000 0000 0000 0000
const uint64_t RANK_7 =
    0x00FF000000000000ULL; // 0000 0000 1111 1111 0000 0000 0000 0000 0000 0000
                           // 0000 0000 0000 0000 0000 0000
const uint64_t RANK_8 =
    0xFF00000000000000ULL; // 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000
                           // 0000 0000 0000 0000 0000 0000
const uint64_t RANK_12 =
    0x000000000000FFFFULL; // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
                           // 0000 0000 1111 1111 1111 1111
const uint64_t RANK_78 =
    0xFFFF000000000000ULL; // 1111 1111 1111 1111 0000 0000 0000 0000 0000 0000
                           // 0000 0000 0000 0000 0000 0000

const uint64_t DIAGONAL_NE = 0x0102040810204080ULL;
const uint64_t DIAGONAL_NW = 0x8040201008040201ULL;
const uint64_t UPPER_LEFT_TRIANGLE = 0xfefcf8f0e0c08000ULL;
const uint64_t UPPER_RIGHT_TRIANGLE = 0x7f3f1f0f07030100ULL;
const uint64_t LOWER_LEFT_TRIANGLE = 0x0080c0e0f0f8fcfeULL;
const uint64_t LOWER_RIGHT_TRIANGLE = 0x000103070f1f3f7fULL;

constexpr uint64_t ROOKS_MAGICS[64] = {
    0xa8002c000108020ULL,  0x6c00049b0002001ULL,  0x100200010090040ULL,
    0x2480041000800801ULL, 0x280028004000800ULL,  0x900410008040022ULL,
    0x280020001001080ULL,  0x2880002041000080ULL, 0xa000800080400034ULL,
    0x4808020004000ULL,    0x2290802004801000ULL, 0x411000d00100020ULL,
    0x402800800040080ULL,  0xb000401004208ULL,    0x2409000100040200ULL,
    0x1002100004082ULL,    0x22878001e24000ULL,   0x1090810021004010ULL,
    0x801030040200012ULL,  0x500808008001000ULL,  0xa08018014000880ULL,
    0x8000808004000200ULL, 0x201008080010200ULL,  0x801020000441091ULL,
    0x800080204005ULL,     0x1040200040100048ULL, 0x120200402082ULL,
    0xd14880480100080ULL,  0x12040280080080ULL,   0x100040080020080ULL,
    0x9020010080800200ULL, 0x813241200148449ULL,  0x491604001800080ULL,
    0x100401000402001ULL,  0x4820010021001040ULL, 0x400402202000812ULL,
    0x209009005000802ULL,  0x810800601800400ULL,  0x4301083214000150ULL,
    0x204026458e001401ULL, 0x40204000808000ULL,   0x8001008040010020ULL,
    0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL,
    0x12000810020004ULL,   0x1000100200040208ULL, 0x430000a044020001ULL,
    0x280009023410300ULL,  0xe0100040002240ULL,   0x200100401700ULL,
    0x2244100408008080ULL, 0x8000400801980ULL,    0x2000810040200ULL,
    0x8010100228810400ULL, 0x2000009044210200ULL, 0x4080008040102101ULL,
    0x40002080411d01ULL,   0x2005524060000901ULL, 0x502001008400422ULL,
    0x489a000810200402ULL, 0x1004400080a13ULL,    0x4000011008020084ULL,
    0x26002114058042ULL};

constexpr uint64_t BISHOPS_MAGICS[64] = {
    0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL,
    0x62880a0220200808ULL, 0x4042004000000ULL,    0x100822020200011ULL,
    0xc00444222012000aULL, 0x28808801216001ULL,   0x400492088408100ULL,
    0x201c401040c0084ULL,  0x840800910a0010ULL,   0x82080240060ULL,
    0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL,
    0x8144042209100900ULL, 0x208081020014400ULL,  0x4800201208ca00ULL,
    0xf18140408012008ULL,  0x1004002802102001ULL, 0x841000820080811ULL,
    0x40200200a42008ULL,   0x800054042000ULL,     0x88010400410c9000ULL,
    0x520040470104290ULL,  0x1004040051500081ULL, 0x2002081833080021ULL,
    0x400c00c010142ULL,    0x941408200c002000ULL, 0x658810000806011ULL,
    0x188071040440a00ULL,  0x4800404002011c00ULL, 0x104442040404200ULL,
    0x511080202091021ULL,  0x4022401120400ULL,    0x80c0040400080120ULL,
    0x8040010040820802ULL, 0x480810700020090ULL,  0x102008e00040242ULL,
    0x809005202050100ULL,  0x8002024220104080ULL, 0x431008804142000ULL,
    0x19001802081400ULL,   0x200014208040080ULL,  0x3308082008200100ULL,
    0x41010500040c020ULL,  0x4012020c04210308ULL, 0x208220a202004080ULL,
    0x111040120082000ULL,  0x6803040141280a00ULL, 0x2101004202410000ULL,
    0x8200000041108022ULL, 0x21082088000ULL,      0x2410204010040ULL,
    0x40100400809000ULL,   0x822088220820214ULL,  0x40808090012004ULL,
    0x910224040218c9ULL,   0x402814422015008ULL,  0x90014004842410ULL,
    0x1000042304105ULL,    0x10008830412a00ULL,   0x2520081090008908ULL,
    0x40102000a0a60140ULL,
};

constexpr int ROOKS_INDEX_BITS[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11, 11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11, 12, 11, 11, 11, 11, 11, 11, 12};

constexpr int BISHOPS_INDEX_BITS[64] = {
    6, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 7, 7, 7,
    5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 9, 9, 7, 5, 5, 5, 5, 7, 7,
    7, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 6};

constexpr uint64_t north(uint64_t bitmap) { return (bitmap & ~RANK_8) << 8; }
constexpr uint64_t east(uint64_t bitmap) { return (bitmap & ~FILE_H) >> 1; }
constexpr uint64_t south(uint64_t bitmap) { return (bitmap & ~RANK_1) >> 8; }
constexpr uint64_t west(uint64_t bitmap) { return (bitmap & ~FILE_A) << 1; }

constexpr uint64_t north_east(uint64_t bitmap) {
  return (bitmap & ~RANK_8 & ~FILE_H) << 7;
}
constexpr uint64_t south_east(uint64_t bitmap) {
  return (bitmap & ~RANK_1 & ~FILE_H) >> 9;
}
constexpr uint64_t south_west(uint64_t bitmap) {
  return (bitmap & ~RANK_1 & ~FILE_A) >> 7;
}
constexpr uint64_t north_west(uint64_t bitmap) {
  return (bitmap & ~RANK_8 & ~FILE_A) << 9;
}

constexpr uint64_t all_north_loc_from_pos(int pos) {
  uint64_t locations = north(1ULL << pos);
  for (int i = 0; i < 6; i++) {
    locations |= north(locations);
  }
  return locations;
}
constexpr uint64_t all_east_loc_from_pos(int pos) {
  uint64_t locations = east(1ULL << pos);
  for (int i = 0; i < 6; i++) {
    locations |= east(locations);
  }
  return locations;
}
constexpr uint64_t all_west_loc_from_pos(int pos) {
  uint64_t locations = west(1ULL << pos);
  for (int i = 0; i < 6; i++) {
    locations |= west(locations);
  }
  return locations;
}
constexpr uint64_t all_south_loc_from_pos(int pos) {
  uint64_t locations = south(1ULL << pos);
  for (int i = 0; i < 6; i++) {
    locations |= south(locations);
  }
  return locations;
}

constexpr uint64_t all_north_east_loc_from_pos(int pos) {
  uint64_t locations = north_east(1ULL << pos);
  for (int i = 0; i < 6; i++) {
    locations |= north_east(locations);
  }
  return locations;
}
constexpr uint64_t all_south_east_loc_from_pos(int pos) {
  uint64_t locations = south_east(1ULL << pos);
  for (int i = 0; i < 6; i++) {
    locations |= south_east(locations);
  }
  return locations;
}
constexpr uint64_t all_south_west_loc_from_pos(int pos) {
  uint64_t locations = south_west(1ULL << pos);
  for (int i = 0; i < 6; i++) {
    locations |= south_west(locations);
  }
  return locations;
}
constexpr uint64_t all_north_west_loc_from_pos(int pos) {
  uint64_t locations = north_west(1ULL << pos);
  for (int i = 0; i < 6; i++) {
    locations |= north_west(locations);
  }
  return locations;
}

constexpr uint64_t getBlockersFromIndex(int i, uint64_t mask) {
  uint64_t blockers = 0;
  int bits = __builtin_popcountll(mask);
  for (int j = 0; j < bits; j++) {
    int bitPos = pop_lsb(mask);
    if (i & (1 << j)) {
      blockers |= (1ULL << bitPos);
    }
  }
  return blockers;
}

constexpr uint64_t knight_moves(uint64_t bitmap) {
  return (((bitmap >> 6) | (bitmap << 10)) & ~FILE_GH) |
         (((bitmap >> 10) | (bitmap << 6)) & ~FILE_AB) |
         (((bitmap >> 15) | (bitmap << 17)) & ~FILE_H) |
         (((bitmap >> 17) | (bitmap << 15)) & ~FILE_A);
}
constexpr auto all_knight_moves{[]() constexpr {
  std::array<uint64_t, 64> moves{};
  for (int i = 0; i < 64; i++) {
    moves[i] = knight_moves(1ULL << i);
  }
  return moves;
}()};

constexpr uint64_t king_moves(uint64_t bitmap) {
  return north(bitmap) | east(bitmap) | south(bitmap) | west(bitmap) |
         north_east(bitmap) | south_east(bitmap) | south_west(bitmap) |
         north_west(bitmap);
}
constexpr auto all_king_moves{[]() constexpr {
  std::array<uint64_t, 64> moves{};
  for (int i = 0; i < 64; i++) {
    moves[i] = king_moves(1ULL << i);
  }
  return moves;
}()};

constexpr int bitscan_forward(uint64_t bitmap) {
  if (bitmap == 0) {
    return -1;
  }
  return __builtin_ffsll(bitmap) - 1;
}
constexpr int bitscan_backward(uint64_t bitmap) {
  return 63 - __builtin_clzll(bitmap);
}

constexpr std::array<uint64_t, 64> rook_masks{[]() constexpr {
  std::array<uint64_t, 64> masks{};
  for (int i = 0; i < 64; i++) {
    masks[i] = (((all_north_loc_from_pos(i) | all_south_loc_from_pos(i)) &
                 ~(RANK_1 | RANK_8)) |
                ((all_east_loc_from_pos(i) | all_west_loc_from_pos(i)) &
                 ~(FILE_A | FILE_H)));
  }
  return masks;
}()};
constexpr uint64_t rook_moves(int square, uint64_t blockers) {
  uint64_t moves = 0;

  uint64_t locs = all_north_loc_from_pos(square);
  moves |= locs;
  if (locs & blockers) {
    moves &= ~(all_north_loc_from_pos(
        bitscan_forward(all_north_loc_from_pos(square) & blockers)));
  }

  locs = all_east_loc_from_pos(square);
  moves |= locs;
  if (locs & blockers) {
    moves &= ~(all_east_loc_from_pos(
        bitscan_forward(all_east_loc_from_pos(square) & blockers)));
  }

  locs = all_south_loc_from_pos(square);
  moves |= locs;
  if (locs & blockers) {
    moves &= ~(all_south_loc_from_pos(
        bitscan_backward(all_south_loc_from_pos(square) & blockers)));
  }

  locs = all_west_loc_from_pos(square);
  moves |= locs;
  if (locs & blockers) {
    moves &= ~(all_west_loc_from_pos(
        bitscan_backward(all_west_loc_from_pos(square) & blockers)));
  }

  return moves;
}
constexpr auto all_rook_moves{[]() constexpr {
  std::array<std::array<uint64_t, 4096>, 64> moves = {};
  for (int i = 0; i < 64; i++) {
    for (int blocker_i = 0; blocker_i < (1 << ROOKS_INDEX_BITS[i]);
         blocker_i++) {
      uint64_t blockers = getBlockersFromIndex(blocker_i, rook_masks[i]);
      moves[i][(blockers * ROOKS_MAGICS[i]) >> (64 - ROOKS_INDEX_BITS[i])] =
          rook_moves(i, blockers);
    }
  }
  return moves;
}()};

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
    result.score = -50000 - depth;
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
    std::cout << "Min king dead" << std::endl;
    result.score = 50000 + depth;
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
    if (board.piece(Square::G3) == Piece::fromSymbol('q')) {
      std::cout << "Moves are: " << move << std::endl;
    }
    Board newBoard = board;
    newBoard.makeMove(move);
    ResultWrapper prevResult =
        alphaBetaMax(alpha, beta, depth + 1, maxDepth, newBoard);
    if (prevResult.score < -50000) {
      result.score = prevResult.score;
      result.pv = prevResult.pv;
      if (board.piece(Square::G3) == Piece::fromSymbol('q')) {
        std::cout << "Move me check (not possible): " << move << std::endl;
        Board::MoveVec newMoves;
        uint64_t rook_moves = board.generate_pseudo_rook_moves(Square::B7);
        std::cout << "Rook moves: " << rook_moves << std::endl;
        const uint64_t friendly = board.get_all_friendly_pieces();
        const uint64_t opponent = board.get_all_opponent_pieces();
        uint64_t blockers = friendly | opponent;
        blockers &= ~(1ULL << Square::B7.index());
        std::cout << "Blockers: " << blockers << std::endl;
        blockers &= rook_masks[Square::B7.index()];
        std::cout << "Rook blockers" << blockers << std::endl;
        uint64_t key = (blockers * ROOKS_MAGICS[Square::B7.index()]) >>
                       (64 - ROOKS_INDEX_BITS[Square::B7.index()]);
        uint64_t all_moves = all_rook_moves[Square::B7.index()][key];
        std::cout << "All moves: " << all_moves << std::endl;
      }
      Move *newMove = new Move(move.from(), move.to(), move.promotion());
      result.pv.addFront(*newMove);
      return result;
    }
    if (prevResult.score <= alpha) {
      if (board.piece(Square::G3) == Piece::fromSymbol('q')) {
        std::cout << "Move here " << move << std::endl;
      }
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
  if (board.piece(Square::G3) == Piece::fromSymbol('q')) {
    std::cout << "Move here " << moves[best_i] << std::endl;
    std::cout << "i is" << best_i << std::endl;
  }
  Move *newMove = new Move(moves[best_i].from(), moves[best_i].to(),
                           moves[best_i].promotion());
  result.pv.addFront(*newMove);
  return result;
}

PrincipalVariation
EngineJorritWillaert::pv(const Board &board,
                         const TimeInfo::Optional &timeInfo) {
  ResultWrapper result = alphaBetaMax(-100000, 100000, 0, 2, board);
  std::cout << "Final pv" << result.pv << std::endl;
  PrincipalVariation principVar = result.pv;
  std::cout << "Final score" << result.score << std::endl;
  if (result.score > 50000 || result.score < -50000) {
    principVar.setMate(true);
  }
  principVar.setScore(result.score);
  (void)timeInfo;
  return principVar;
}