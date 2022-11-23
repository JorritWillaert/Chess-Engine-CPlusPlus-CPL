#include "Board.hpp"
#include "CastlingRights.hpp"
#include "Piece.hpp"
#include "Square.hpp"

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <optional>
#include <ostream>

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
    masks[i] = (all_north_loc_from_pos(i) | all_south_loc_from_pos(i) |
                all_east_loc_from_pos(i) | all_west_loc_from_pos(i)) &
               ~(RANK_1 | RANK_8 | FILE_A | FILE_H);
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

constexpr std::array<uint64_t, 64> bishop_masks{[]() constexpr {
  std::array<uint64_t, 64> masks{};
  for (int i = 0; i < 64; i++) {
    masks[i] =
        (all_north_east_loc_from_pos(i) | all_south_east_loc_from_pos(i) |
         all_south_west_loc_from_pos(i) | all_north_west_loc_from_pos(i)) &
        ~(RANK_1 | RANK_8 | FILE_A | FILE_H);
  }
  return masks;
}()};
constexpr uint64_t bishop_moves(int square, uint64_t blockers) {
  uint64_t moves = 0;

  uint64_t locs = all_north_east_loc_from_pos(square);
  moves |= locs;
  if (locs & blockers) {
    moves &= ~(all_north_east_loc_from_pos(
        bitscan_forward(all_north_east_loc_from_pos(square) & blockers)));
  }

  locs = all_south_east_loc_from_pos(square);
  moves |= locs;
  if (locs & blockers) {
    moves &= ~(all_south_east_loc_from_pos(
        bitscan_backward(all_south_east_loc_from_pos(square) & blockers)));
  }

  locs = all_south_west_loc_from_pos(square);
  moves |= locs;
  if (locs & blockers) {
    moves &= ~(all_south_west_loc_from_pos(
        bitscan_backward(all_south_west_loc_from_pos(square) & blockers)));
  }

  locs = all_north_west_loc_from_pos(square);
  moves |= locs;
  if (locs & blockers) {
    moves &= ~(all_north_west_loc_from_pos(
        bitscan_forward(all_north_west_loc_from_pos(square) & blockers)));
  }

  return moves;
}
constexpr auto all_bishop_moves{[]() constexpr {
  std::array<std::array<uint64_t, 512>, 64> moves = {};
  for (int i = 0; i < 64; i++) {
    for (int blocker_i = 0; blocker_i < (1 << BISHOPS_INDEX_BITS[i]);
         blocker_i++) {
      uint64_t blockers = getBlockersFromIndex(blocker_i, bishop_masks[i]);
      moves[i][(blockers * BISHOPS_MAGICS[i]) >> (64 - BISHOPS_INDEX_BITS[i])] =
          bishop_moves(i, blockers);
    }
  }
  return moves;
}()};

Board::Board() {
  // all_bitmaps_[0] = RANK_2;                 // White pawns
  // all_bitmaps_[1] = 0x0000000000000042ULL;  // White knights
  // all_bitmaps_[2] = 0x0000000000000024ULL;  // White bishops
  // all_bitmaps_[3] = 0x0000000000000081ULL;  // White rooks
  // all_bitmaps_[4] = 0x0000000000000010ULL;  // White queen
  // all_bitmaps_[5] = 0x0000000000000008ULL;  // White king
  // all_bitmaps_[6] = RANK_7;                 // Black pawns
  // all_bitmaps_[7] = 0x4200000000000000ULL;  // Black knights
  // all_bitmaps_[8] = 0x2400000000000000ULL;  // Black bishops
  // all_bitmaps_[9] = 0x8100000000000000ULL;  // Black rooks
  // all_bitmaps_[10] = 0x1000000000000000ULL; // Black queen
  // all_bitmaps_[11] = 0x0800000000000000ULL; // Black king

  all_bitmaps_[0] = 0;  // White pawns
  all_bitmaps_[1] = 0;  // White knights
  all_bitmaps_[2] = 0;  // White bishops
  all_bitmaps_[3] = 0;  // White rooks
  all_bitmaps_[4] = 0;  // White queen
  all_bitmaps_[5] = 0;  // White king
  all_bitmaps_[6] = 0;  // Black pawns
  all_bitmaps_[7] = 0;  // Black knights
  all_bitmaps_[8] = 0;  // Black bishops
  all_bitmaps_[9] = 0;  // Black rooks
  all_bitmaps_[10] = 0; // Black queen
  all_bitmaps_[11] = 0; // Black king

  castling_rights_ = CastlingRights::None;
  castling_rights_ |= CastlingRights::WhiteKingside;
  castling_rights_ |= CastlingRights::WhiteQueenside;
  castling_rights_ |= CastlingRights::BlackKingside;
  castling_rights_ |= CastlingRights::BlackQueenside;
  en_passant_square_ = std::nullopt;
}

void Board::setPiece(const Square &square, const Piece::Optional &piece) {
  if (!piece.has_value()) {
    return;
  }
  Piece p = piece.value();
  int piece_index =
      static_cast<int>(p.type()) + static_cast<int>(p.color()) * 6;
  set_bit(all_bitmaps_[piece_index], square.index());
  for (int i = 0; i < 12; i++) {
    if (i != piece_index) {
      clear_bit(all_bitmaps_[i],
                square.index()); // Because we might have captured a piece
    }
  }
}

void Board::removePiece(const Square &square, const Piece::Optional &piece) {
  if (!piece.has_value()) {
    return;
  }
  Piece p = piece.value();
  int piece_index =
      static_cast<int>(p.type()) + static_cast<int>(p.color()) * 6;
  clear_bit(all_bitmaps_[piece_index], square.index());
}

Piece::Optional Board::piece(const Square &square) const {
  for (int i = 0; i < 12; i++) {
    if (get_bit(all_bitmaps_[i], square.index())) {
      int piece_color = i / 6; // 0 = white, 1 = black
      int piece_type =
          i %
          6; // 0 = Pawn, 1 = Knight, 2 = Bishop, 3 = Rook, 4 = Queen, 5 = King
      return Piece(static_cast<PieceColor>(piece_color),
                   static_cast<PieceType>(piece_type));
    }
  }
  return std::nullopt;
}

void Board::setTurn(PieceColor turn) { turn_ = turn; }

PieceColor Board::turn() const { return turn_; }

void Board::setCastlingRights(CastlingRights cr) { castling_rights_ = cr; }

CastlingRights Board::castlingRights() const { return castling_rights_; }

void Board::setEnPassantSquare(const Square::Optional &square) {
  en_passant_square_ = square;
}

Square::Optional Board::enPassantSquare() const { return en_passant_square_; }

void Board::makeMove(const Move &move) {
  Square from = move.from();
  Square to = move.to();
  Piece::Optional piece = Board::piece(from);
  setPiece(to, piece);
  removePiece(from, piece);

  // Castling updates for king
  if (piece.has_value() && piece.value().type() == PieceType::King) {
    if (piece.value().color() == PieceColor::White) {
      castling_rights_ &= ~CastlingRights::WhiteKingside;
      castling_rights_ &= ~CastlingRights::WhiteQueenside;
      if (from == Square::E1 && to == Square::G1) {
        setPiece(Square::F1, Board::piece(Square::H1));
        removePiece(Square::H1, Board::piece(Square::H1));
      } else if (from == Square::E1 && to == Square::C1) {
        setPiece(Square::D1, Board::piece(Square::A1));
        removePiece(Square::A1, Board::piece(Square::A1));
      }
    } else {
      castling_rights_ &= ~CastlingRights::BlackKingside;
      castling_rights_ &= ~CastlingRights::BlackQueenside;
      if (from == Square::E8 && to == Square::G8) {
        setPiece(Square::F8, Board::piece(Square::H8));
        removePiece(Square::H8, Board::piece(Square::H8));
      } else if (from == Square::E8 && to == Square::C8) {
        setPiece(Square::D8, Board::piece(Square::A8));
        removePiece(Square::A8, Board::piece(Square::A8));
      }
    }
  }

  // Castling updates for rook
  if (piece.has_value() && piece.value().type() == PieceType::Rook) {
    if (piece.value().color() == PieceColor::White) {
      if (from == Square::A1) {
        castling_rights_ &= ~CastlingRights::WhiteQueenside;
      } else if (from == Square::H1) {
        castling_rights_ &= ~CastlingRights::WhiteKingside;
      }
    } else {
      if (from == Square::A8) {
        castling_rights_ &= ~CastlingRights::BlackQueenside;
      } else if (from == Square::H8) {
        castling_rights_ &= ~CastlingRights::BlackKingside;
      }
    }
  }

  // Updates for capturing rooks and kings
  if (move.to() == Square::A1) {
    castling_rights_ &= ~CastlingRights::WhiteQueenside;
  } else if (move.to() == Square::H1) {
    castling_rights_ &= ~CastlingRights::WhiteKingside;
  } else if (move.to() == Square::A8) {
    castling_rights_ &= ~CastlingRights::BlackQueenside;
  } else if (move.to() == Square::H8) {
    castling_rights_ &= ~CastlingRights::BlackKingside;
  } else if (move.to() == Square::E1) {
    castling_rights_ &= ~CastlingRights::WhiteKingside;
    castling_rights_ &= ~CastlingRights::WhiteQueenside;
  } else if (move.to() == Square::E8) {
    castling_rights_ &= ~CastlingRights::BlackKingside;
    castling_rights_ &= ~CastlingRights::BlackQueenside;
  }

  // En passant updates
  if (piece.has_value() && piece.value().type() == PieceType::Pawn) {
    if (enPassantSquare().has_value() && enPassantSquare().value() == to) {
      if (piece.value().color() == PieceColor::White) {
        Square squareToRemove = Square::fromIndex(to.index() - 8).value();
        removePiece(squareToRemove, Board::piece(squareToRemove));
      } else {
        Square squareToRemove = Square::fromIndex(to.index() + 8).value();
        removePiece(squareToRemove, Board::piece(squareToRemove));
      }
    }
    if (from.rank() == 1 && to.rank() == 3) {
      setEnPassantSquare(Square::fromIndex(to.index() - 8));
    } else if (from.rank() == 6 && to.rank() == 4) {
      setEnPassantSquare(Square::fromIndex(to.index() + 8));
    } else {
      setEnPassantSquare(std::nullopt);
    }
  } else {
    setEnPassantSquare(std::nullopt);
  }

  std::optional<PieceType> promotion = move.promotion();
  if (piece.has_value() && promotion.has_value()) {
    PieceType promotion_type = promotion.value();
    PieceColor color = piece.value().color();
    Piece promoted_piece = Piece(color, promotion_type);
    removePiece(to, piece);
    setPiece(to, promoted_piece);
  }
  setTurn(!turn());
}

uint64_t Board::get_all_friendly_pieces() const {
  if (turn_ == PieceColor::White) {
    return all_bitmaps_[0] | all_bitmaps_[1] | all_bitmaps_[2] |
           all_bitmaps_[3] | all_bitmaps_[4] | all_bitmaps_[5];
  } else {
    return all_bitmaps_[6] | all_bitmaps_[7] | all_bitmaps_[8] |
           all_bitmaps_[9] | all_bitmaps_[10] | all_bitmaps_[11];
  }
}

uint64_t Board::get_all_opponent_pieces() const {
  if (turn_ == PieceColor::White) {
    return all_bitmaps_[6] | all_bitmaps_[7] | all_bitmaps_[8] |
           all_bitmaps_[9] | all_bitmaps_[10] | all_bitmaps_[11];
  } else {
    return all_bitmaps_[0] | all_bitmaps_[1] | all_bitmaps_[2] |
           all_bitmaps_[3] | all_bitmaps_[4] | all_bitmaps_[5];
  }
}

uint64_t Board::get_pawns(const PieceColor color) const {
  if (color == PieceColor::White) {
    return all_bitmaps_[0];
  } else {
    return all_bitmaps_[6];
  }
}

uint64_t Board::get_knights(const PieceColor color) const {
  if (color == PieceColor::White) {
    return all_bitmaps_[1];
  } else {
    return all_bitmaps_[7];
  }
}

uint64_t Board::get_bishops_and_queens(const PieceColor color) const {
  if (color == PieceColor::White) {
    return all_bitmaps_[2] | all_bitmaps_[4];
  } else {
    return all_bitmaps_[8] | all_bitmaps_[10];
  }
}

uint64_t Board::get_rooks_and_queens(const PieceColor color) const {
  if (color == PieceColor::White) {
    return all_bitmaps_[3] | all_bitmaps_[4];
  } else {
    return all_bitmaps_[9] | all_bitmaps_[10];
  }
}

uint64_t Board::get_king(const PieceColor color) const {
  if (color == PieceColor::White) {
    return all_bitmaps_[5];
  } else {
    return all_bitmaps_[11];
  }
}

void Board::add_pseudo_pawn_moves(const Square &from, Board::MoveVec &moves,
                                  const PieceColor color) const {
  const uint64_t friendly = get_all_friendly_pieces();
  const uint64_t opponent = get_all_opponent_pieces();
  const uint64_t all = friendly | opponent;
  uint64_t en_passant_moves = 0;
  uint64_t all_moves;

  if (color == PieceColor::White) {
    uint64_t single_push = north(1ULL << from.index()) & ~all;
    uint64_t double_push = north(single_push & RANK_3) & ~all;
    uint64_t left_capture = north_west(1ULL << from.index()) & opponent;
    uint64_t right_capture = north_east(1ULL << from.index()) & opponent;
    if (enPassantSquare().has_value()) {
      en_passant_moves |= (north_west(1ULL << from.index()) &
                           (1ULL << enPassantSquare().value().index()));
      en_passant_moves |= (north_east(1ULL << from.index()) &
                           (1ULL << enPassantSquare().value().index()));
    }
    all_moves = single_push | double_push | left_capture | right_capture |
                en_passant_moves;
  } else {
    uint64_t single_push = south(1ULL << from.index()) & ~all;
    uint64_t double_push = south(single_push & RANK_6) & ~all;
    uint64_t left_capture = south_west(1ULL << from.index()) & opponent;
    uint64_t right_capture = south_east(1ULL << from.index()) & opponent;
    if (enPassantSquare().has_value()) {
      en_passant_moves |= (south_west(1ULL << from.index()) &
                           (1ULL << enPassantSquare().value().index()));
      en_passant_moves |= (south_east(1ULL << from.index()) &
                           (1ULL << enPassantSquare().value().index()));
    }
    all_moves = single_push | double_push | left_capture | right_capture |
                en_passant_moves;
  }
  while (all_moves) {
    Square to = Square::fromIndex(pop_lsb(all_moves)).value();
    if (to.rank() == 7 && color == PieceColor::White) {
      moves.push_back(Move(from, to, PieceType::Queen));
      moves.push_back(Move(from, to, PieceType::Rook));
      moves.push_back(Move(from, to, PieceType::Bishop));
      moves.push_back(Move(from, to, PieceType::Knight));
    } else if (to.rank() == 0 && color == PieceColor::Black) {
      moves.push_back(Move(from, to, PieceType::Queen));
      moves.push_back(Move(from, to, PieceType::Rook));
      moves.push_back(Move(from, to, PieceType::Bishop));
      moves.push_back(Move(from, to, PieceType::Knight));
    } else {
      moves.push_back(Move(from, to));
    }
  }
}

void Board::add_pseudo_knight_moves(const Square &from,
                                    Board::MoveVec &moves) const {
  const uint64_t friendly = get_all_friendly_pieces();
  uint64_t all_moves = all_knight_moves[from.index()];
  all_moves &= ~friendly;
  while (all_moves) {
    Square to = Square::fromIndex(pop_lsb(all_moves)).value();
    moves.push_back(Move(from, to));
  }
}

uint64_t Board::generate_pseudo_bishop_moves(const Square &from) const {
  const uint64_t friendly = get_all_friendly_pieces();
  const uint64_t opponent = get_all_opponent_pieces();
  uint64_t blockers = friendly | opponent;
  clear_bit(blockers, from.index());

  blockers &= bishop_masks[from.index()];
  uint64_t key = (blockers * BISHOPS_MAGICS[from.index()]) >>
                 (64 - BISHOPS_INDEX_BITS[from.index()]);
  uint64_t all_moves = all_bishop_moves[from.index()][key];
  all_moves &= ~friendly; // You can't attack your own pieces
  return all_moves;
}

void Board::add_pseudo_bishop_moves(const Square &from,
                                    Board::MoveVec &moves) const {
  uint64_t all_moves = generate_pseudo_bishop_moves(from);
  while (all_moves) {
    Square to = Square::fromIndex(pop_lsb(all_moves)).value();
    moves.push_back(Move(from, to));
  }
}

uint64_t Board::generate_pseudo_rook_moves(const Square &from) const {
  const uint64_t friendly = get_all_friendly_pieces();
  const uint64_t opponent = get_all_opponent_pieces();
  uint64_t blockers = friendly | opponent;
  clear_bit(blockers, from.index());

  blockers &= rook_masks[from.index()];
  uint64_t key = (blockers * ROOKS_MAGICS[from.index()]) >>
                 (64 - ROOKS_INDEX_BITS[from.index()]);
  uint64_t all_moves = all_rook_moves[from.index()][key];
  all_moves &= ~friendly; // You can't attack your own pieces
  return all_moves;
}

void Board::add_pseudo_rook_moves(const Square &from,
                                  Board::MoveVec &moves) const {
  uint64_t all_moves = generate_pseudo_rook_moves(from);
  while (all_moves) {
    Square to = Square::fromIndex(pop_lsb(all_moves)).value();
    moves.push_back(Move(from, to));
  }
}

void Board::add_pseudo_queen_moves(const Square &from,
                                   Board::MoveVec &moves) const {
  add_pseudo_bishop_moves(from, moves);
  add_pseudo_rook_moves(from, moves);
}

bool Board::check_pawn_to_square(const Square &to,
                                 const PieceColor color) const {
  const uint64_t opponent_pawns = get_pawns(color);
  uint64_t all_moves;

  if (color == PieceColor::White) {
    uint64_t made_single_push = south(1ULL << to.index()) & opponent_pawns;
    uint64_t made_left_capture =
        south_west(1ULL << to.index()) & opponent_pawns;
    uint64_t made_right_capture =
        south_east(1ULL << to.index()) & opponent_pawns;
    all_moves = made_single_push | made_left_capture | made_right_capture;
  } else {
    uint64_t made_single_push = north(1ULL << to.index()) & opponent_pawns;
    uint64_t made_left_capture = north(1ULL << to.index()) & opponent_pawns;
    uint64_t made_right_capture = north(1ULL << to.index()) & opponent_pawns;
    all_moves = made_single_push | made_left_capture | made_right_capture;
  }
  if (all_moves) {
    return true;
  }
  return false;
}

bool Board::check_knight_to_square(const Square &to,
                                   const PieceColor color) const {
  const uint64_t opponent = get_knights(color);
  uint64_t all_moves = all_knight_moves[to.index()];
  all_moves &= opponent;
  if (all_moves) {
    return true;
  }
  return false;
}

bool Board::check_bishop_and_queen_to_square(const Square &to,
                                             const PieceColor color) const {
  uint64_t all_moves = generate_pseudo_bishop_moves(to);
  all_moves &= get_bishops_and_queens(color);
  if (all_moves) {
    return true;
  }
  return false;
}

bool Board::check_rook_and_queen_to_square(const Square &to,
                                           const PieceColor color) const {
  uint64_t all_moves = generate_pseudo_rook_moves(to);
  all_moves &= get_rooks_and_queens(color);
  if (all_moves) {
    return true;
  }
  return false;
}

bool Board::check_king_to_square(const Square &to,
                                 const PieceColor color) const {
  const uint64_t opponent = get_king(color);
  uint64_t all_moves = all_king_moves[to.index()];
  all_moves &= opponent;
  if (all_moves) {
    return true;
  }
  return false;
}

bool Board::square_under_attack_by_color(const Square &to,
                                         const PieceColor color) const {
  if (check_pawn_to_square(to, color) || check_knight_to_square(to, color) ||
      check_bishop_and_queen_to_square(to, color) ||
      check_rook_and_queen_to_square(to, color) ||
      check_king_to_square(to, color)) {
    return true;
  }
  return false;
}

uint64_t Board::get_castle_moves(const Square &from) const {
  const uint64_t friendly = get_all_friendly_pieces();
  const uint64_t opponent = get_all_opponent_pieces();
  const uint64_t all = friendly | opponent;
  uint64_t castle_moves = 0;

  if (from == Square::E1) {
    if ((castlingRights() & CastlingRights::WhiteKingside) !=
        CastlingRights::None) {
      if (!(square_under_attack_by_color(Square::E1, !turn_)) &&
          !(all & (1ULL << Square::F1.index())) &&
          !(square_under_attack_by_color(Square::F1, !turn_)) &&
          !(all & (1ULL << Square::G1.index())) &&
          !(square_under_attack_by_color(Square::G1, !turn_))) {
        castle_moves |= 1ULL << Square::G1.index();
      }
    }
    if ((castlingRights() & CastlingRights::WhiteQueenside) !=
        CastlingRights::None) {
      if (!(square_under_attack_by_color(Square::E1, !turn_)) &&
          !(all & (1ULL << Square::D1.index())) &&
          !(square_under_attack_by_color(Square::D1, !turn_)) &&
          !(all & (1ULL << Square::C1.index())) &&
          !(square_under_attack_by_color(Square::C1, !turn_)) &&
          !(all & (1ULL << Square::B1.index()))) {
        castle_moves |= 1ULL << Square::C1.index();
      }
    }
  } else if (from == Square::E8) {
    if ((castlingRights() & CastlingRights::BlackKingside) !=
        CastlingRights::None) {
      if (!(square_under_attack_by_color(Square::E8, !turn_)) &&
          !(all & (1ULL << Square::F8.index())) &&
          !(square_under_attack_by_color(Square::F8, !turn_)) &&
          !(all & (1ULL << Square::G8.index())) &&
          !(square_under_attack_by_color(Square::G8, !turn_))) {
        castle_moves |= 1ULL << Square::G8.index();
      }
    }
    if ((castlingRights() & CastlingRights::BlackQueenside) !=
        CastlingRights::None) {
      if (!(square_under_attack_by_color(Square::E8, !turn_)) &&
          !(all & (1ULL << Square::D8.index())) &&
          !(square_under_attack_by_color(Square::D8, !turn_)) &&
          !(all & (1ULL << Square::C8.index())) &&
          !(square_under_attack_by_color(Square::C8, !turn_)) &&
          !(all & (1ULL << Square::B8.index()))) {
        castle_moves |= 1ULL << Square::C8.index();
      }
    }
  }
  return castle_moves;
}

void Board::add_pseudo_king_moves(const Square &from,
                                  Board::MoveVec &moves) const {
  const uint64_t friendly = get_all_friendly_pieces();
  uint64_t all_moves = all_king_moves[from.index()];
  all_moves &= ~friendly;

  all_moves |= get_castle_moves(from);

  while (all_moves) {
    Square to = Square::fromIndex(pop_lsb(all_moves)).value();
    moves.push_back(Move(from, to));
  }
}

void Board::pseudoLegalMoves(MoveVec &moves) const {
  for (int i = 0; i < 64; i++) {
    pseudoLegalMovesFrom(Square::fromIndex(i).value(), moves);
  }
}

void Board::pseudoLegalMovesFrom(const Square &from,
                                 Board::MoveVec &moves) const {
  Piece::Optional piece = Board::piece(from);
  if (!piece.has_value() || piece.value().color() != turn_) {
    return;
  }

  Piece p = piece.value();
  switch (p.type()) {
  case PieceType::Pawn:
    add_pseudo_pawn_moves(from, moves, p.color());
    break;
  case PieceType::Knight:
    add_pseudo_knight_moves(from, moves);
    break;
  case PieceType::Bishop:
    add_pseudo_bishop_moves(from, moves);
    break;
  case PieceType::Rook:
    add_pseudo_rook_moves(from, moves);
    break;
  case PieceType::Queen:
    add_pseudo_queen_moves(from, moves);
    break;
  case PieceType::King:
    add_pseudo_king_moves(from, moves);
    break;
  }
}

bool Board::isCheck() const {
  uint64_t king = get_king(turn_);
  Square king_square = Square::fromIndex(pop_lsb(king)).value();
  return square_under_attack_by_color(king_square, !turn_);
}

bool Board::isMate() const {
  if (!isCheck()) {
    return false;
  }
  MoveVec moves;
  pseudoLegalMoves(moves);
  std::cout << "Possible moves: " << moves.size() << std::endl;
  for (const Move &move : moves) {
    Board copyBoard = *this;
    copyBoard.makeMove(move);
    if (!copyBoard.isCheck()) {
      return false;
    }
  }
  return true;
}

int Board::calculateScore() const {
  int score = 0;
  score += __builtin_popcount(all_bitmaps_[0]);
  score += __builtin_popcount(all_bitmaps_[1]) * 3;
  score += __builtin_popcount(all_bitmaps_[2]) * 3;
  score += __builtin_popcount(all_bitmaps_[3]) * 5;
  score += __builtin_popcount(all_bitmaps_[4]) * 9;
  score += __builtin_popcount(all_bitmaps_[5]) * 100;
  score -= __builtin_popcount(all_bitmaps_[6]);
  score -= __builtin_popcount(all_bitmaps_[7]) * 3;
  score -= __builtin_popcount(all_bitmaps_[8]) * 3;
  score -= __builtin_popcount(all_bitmaps_[9]) * 5;
  score -= __builtin_popcount(all_bitmaps_[10]) * 9;
  score -= __builtin_popcount(all_bitmaps_[11]) * 100;
  return score;
}

std::ostream &operator<<(std::ostream &os, const Board &board) {
  for (int rank = 7; rank >= 0; rank--) {
    for (int file = 0; file < 8; file++) {
      std::optional<Square> square = Square::fromCoordinates(file, rank);
      Piece::Optional piece = board.piece(square.value());
      if (piece.has_value()) {
        os << piece.value();
      } else {
        os << '.';
      }
    }
    os << std::endl;
  }
  return os;
}
