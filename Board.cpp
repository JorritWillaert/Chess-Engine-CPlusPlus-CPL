#include "Board.hpp"
#include "CastlingRights.hpp"
#include "Piece.hpp"
#include "Square.hpp"

#include <cstdint>
#include <optional>
#include <ostream>
#include <cassert>
#include <cmath>
#include <array>

#define set_bit(bitmap, index) (bitmap |= (1ULL << index))
#define get_bit(bitmap, index) (bitmap & (1ULL << index))
#define clear_bit(bitmap, index) (bitmap &= ~(1ULL << index))
// #define get_LSB(bitmap) (__builtin_ctzll(bitmap))

/* If you read the bit number from left to right, you move from the upper left of the board to the lower right of the board */
const uint64_t FILE_A = 0x8080808080808080ULL; // 1000 0000 1000 000 ...
const uint64_t FILE_B = 0x4040404040404040ULL; // 0100 0000 0100 000 ...
const uint64_t FILE_C = 0x2020202020202020ULL; // 0010 0000 0010 000 ...
const uint64_t FILE_D = 0x1010101010101010ULL; // 0001 0000 0001 000 ...
const uint64_t FILE_E = 0x0808080808080808ULL; // 0000 1000 0000 100 ...
const uint64_t FILE_F = 0x0404040404040404ULL; // 0000 0100 0000 010 ...
const uint64_t FILE_G = 0x0202020202020202ULL; // 0000 0010 0000 001 ...
const uint64_t FILE_H = 0x0101010101010101ULL; // 0000 0001 0000 000 ...
const uint64_t FILE_AB = 0xc0c0c0c0c0c0c0c0ULL; // 1100 0000 1100 000 ...
const uint64_t FILE_GH = 0x0303030303030303ULL; // 0000 0011 0000 001 ...

const uint64_t RANK_1 = 0x00000000000000FFULL; // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111
const uint64_t RANK_2 = 0x000000000000FF00ULL; // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111 0000 0000
const uint64_t RANK_3 = 0x0000000000FF0000ULL; // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111 0000 0000 0000 0000
const uint64_t RANK_4 = 0x00000000FF000000ULL; // 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111 0000 0000 0000 0000 0000 0000
const uint64_t RANK_5 = 0x000000FF00000000ULL; // 0000 0000 0000 0000 0000 0000 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000
const uint64_t RANK_6 = 0x0000FF0000000000ULL; // 0000 0000 0000 0000 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
const uint64_t RANK_7 = 0x00FF000000000000ULL; // 0000 0000 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
const uint64_t RANK_8 = 0xFF00000000000000ULL; // 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
const uint64_t RANK_12 = 0x000000000000FFFFULL; // 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 1111 1111 1111 1111
const uint64_t RANK_78 = 0xFFFF000000000000ULL; // 1111 1111 1111 1111 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000

const uint64_t ROOKS_MAGICS[64] = {
    0xa8002c000108020ULL, 0x6c00049b0002001ULL, 0x100200010090040ULL, 0x2480041000800801ULL, 0x280028004000800ULL,
    0x900410008040022ULL, 0x280020001001080ULL, 0x2880002041000080ULL, 0xa000800080400034ULL, 0x4808020004000ULL,
    0x2290802004801000ULL, 0x411000d00100020ULL, 0x402800800040080ULL, 0xb000401004208ULL, 0x2409000100040200ULL,
    0x1002100004082ULL, 0x22878001e24000ULL, 0x1090810021004010ULL, 0x801030040200012ULL, 0x500808008001000ULL,
    0xa08018014000880ULL, 0x8000808004000200ULL, 0x201008080010200ULL, 0x801020000441091ULL, 0x800080204005ULL,
    0x1040200040100048ULL, 0x120200402082ULL, 0xd14880480100080ULL, 0x12040280080080ULL, 0x100040080020080ULL,
    0x9020010080800200ULL, 0x813241200148449ULL, 0x491604001800080ULL, 0x100401000402001ULL, 0x4820010021001040ULL,
    0x400402202000812ULL, 0x209009005000802ULL, 0x810800601800400ULL, 0x4301083214000150ULL, 0x204026458e001401ULL,
    0x40204000808000ULL, 0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL,
    0x12000810020004ULL, 0x1000100200040208ULL, 0x430000a044020001ULL, 0x280009023410300ULL, 0xe0100040002240ULL,
    0x200100401700ULL, 0x2244100408008080ULL, 0x8000400801980ULL, 0x2000810040200ULL, 0x8010100228810400ULL,
    0x2000009044210200ULL, 0x4080008040102101ULL, 0x40002080411d01ULL, 0x2005524060000901ULL, 0x502001008400422ULL,
    0x489a000810200402ULL, 0x1004400080a13ULL, 0x4000011008020084ULL, 0x26002114058042ULL
};

const uint64_t BISHOPS_MAGICS[64] = {
    0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL,
    0x100822020200011ULL, 0xc00444222012000aULL, 0x28808801216001ULL, 0x400492088408100ULL, 0x201c401040c0084ULL,
    0x840800910a0010ULL, 0x82080240060ULL, 0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL,
    0x8144042209100900ULL, 0x208081020014400ULL, 0x4800201208ca00ULL, 0xf18140408012008ULL, 0x1004002802102001ULL,
    0x841000820080811ULL, 0x40200200a42008ULL, 0x800054042000ULL, 0x88010400410c9000ULL, 0x520040470104290ULL,
    0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL, 0x941408200c002000ULL, 0x658810000806011ULL,
    0x188071040440a00ULL, 0x4800404002011c00ULL, 0x104442040404200ULL, 0x511080202091021ULL, 0x4022401120400ULL,
    0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL, 0x102008e00040242ULL, 0x809005202050100ULL,
    0x8002024220104080ULL, 0x431008804142000ULL, 0x19001802081400ULL, 0x200014208040080ULL, 0x3308082008200100ULL,
    0x41010500040c020ULL, 0x4012020c04210308ULL, 0x208220a202004080ULL, 0x111040120082000ULL, 0x6803040141280a00ULL,
    0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL, 0x2410204010040ULL, 0x40100400809000ULL,
    0x822088220820214ULL, 0x40808090012004ULL, 0x910224040218c9ULL, 0x402814422015008ULL, 0x90014004842410ULL,
    0x1000042304105ULL, 0x10008830412a00ULL, 0x2520081090008908ULL, 0x40102000a0a60140ULL,
};

const int ROOKS_INDEX_BITS[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

const int BISHOPS_INDEX_BITS[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

constexpr uint64_t north(uint64_t bitmap) {
    return (bitmap & ~RANK_8) << 8; 
}
constexpr uint64_t east(uint64_t bitmap) {
    return (bitmap & ~FILE_H) >> 1;
}
constexpr uint64_t south(uint64_t bitmap) {
    return (bitmap & ~RANK_1) >> 8;
}
constexpr uint64_t west(uint64_t bitmap) {
    return (bitmap & ~FILE_A) << 1;
}

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

/* TODO: keep in mind, here are you returning all possible moves, including the ones where you HAVE to attack someone. Take care of this */
constexpr uint64_t pawn_moves(uint64_t bitmap, bool is_white) {
    if (is_white) {
        uint64_t moves_from_start = north(north(bitmap & RANK_2));
        uint64_t moves_from_not_start = north(bitmap);
        uint64_t moves_up_right = north_east(bitmap);
        uint64_t moves_up_left = north_west(bitmap);
        return moves_from_start | moves_from_not_start | moves_up_right | moves_up_left;
    } else {
        uint64_t moves_from_start = south(south(bitmap & RANK_7));
        uint64_t moves_from_not_start = south(bitmap);
        uint64_t moves_up_right = south_east(bitmap);
        uint64_t moves_up_left = south_west(bitmap);
        return moves_from_start | moves_from_not_start | moves_up_right | moves_up_left;
    }
}
constexpr auto all_pawn_moves_white {[] () constexpr {
    std::array<uint64_t, 64> moves{};
    for (int i = 0; i < 8; i++) {
        moves[i] = 0; // Not possible that a pawn is located here
    }
    for (int i = 8; i < 64; i++) {
        moves[i] = pawn_moves(1ULL << i, true);
    }
    return moves; 
}()};
constexpr auto all_pawn_moves_black {[] () constexpr {
    std::array<uint64_t, 64> moves{};
    for (int i = 0; i < 56; i++) {
        moves[i] = pawn_moves(1ULL << i, true);
    }
    for (int i = 56; i < 64; i++) {
        moves[i] = 0; // Not possible that a pawn is located here
    }
    return moves; 
}()};

constexpr uint64_t knight_moves(uint64_t bitmap) {
    return (((bitmap >> 6 & ~RANK_1) | (bitmap << 10 & ~RANK_8)) & ~FILE_GH) |
           (((bitmap >> 10 & ~RANK_1) | (bitmap << 6 & ~RANK_8)) & ~FILE_AB) |
           (((bitmap >> 15 & ~RANK_12) | (bitmap << 17 & ~RANK_78)) & ~FILE_H) |
           (((bitmap >> 17 & ~RANK_12) | (bitmap << 15 & ~RANK_78)) & ~FILE_A);
}
constexpr auto all_knight_moves {[]() constexpr {
    std::array<uint64_t, 64> moves{};
    for (int i = 0; i < 64; i++) {
        moves[i] = knight_moves(1ULL << i);
    }
    return moves; 
}()};

constexpr uint64_t king_moves(uint64_t bitmap) {
    return north(bitmap) | east(bitmap) | south(bitmap) | west(bitmap) | north_east(bitmap) | south_east(bitmap) | south_west(bitmap) | north_west(bitmap);
}
constexpr auto all_king_moves {[]() constexpr {
    std::array<uint64_t, 64> moves{};
    for (int i = 0; i < 64; i++) {
        moves[i] = king_moves(1ULL << i);
    }
    return moves; 
}()};

constexpr uint64_t mask_occupancy_bishop(uint64_t bitmap, int index) {
    int rank = index / 8;
    int file = index % 8;
    clear_bit(bitmap, index); // You can't move to the same place
    uint64_t mask = bitmap & (RANK_1 << rank * 8) & (FILE_A << file);
    return mask;
}

Board::Board()
{
    all_bitmaps_[0] = RANK_2; // White pawns
    all_bitmaps_[1] = 0x0000000000000042ULL; // White knights
    all_bitmaps_[2] = 0x0000000000000024ULL; // White bishops
    all_bitmaps_[3] = 0x0000000000000081ULL; // White rooks
    all_bitmaps_[4] = 0x0000000000000010ULL; // White queen
    all_bitmaps_[5] = 0x0000000000000008ULL; // White king
    all_bitmaps_[6] = RANK_7; // Black pawns
    all_bitmaps_[7] = 0x4200000000000000ULL; // Black knights
    all_bitmaps_[8] = 0x2400000000000000ULL; // Black bishops
    all_bitmaps_[9] = 0x8100000000000000ULL; // Black rooks
    all_bitmaps_[10] = 0x1000000000000000ULL; // Black queen
    all_bitmaps_[11] = 0x0800000000000000ULL; // Black king

    castling_rights_ = CastlingRights::None;
    castling_rights_ |= CastlingRights::WhiteKingside;
    castling_rights_ |= CastlingRights::WhiteQueenside;
    castling_rights_ |= CastlingRights::BlackKingside;
    castling_rights_ |= CastlingRights::BlackQueenside;
    en_passant_square_ = std::nullopt;
}

void Board::setPiece(const Square& square, const Piece::Optional& piece) {
    if (!piece.has_value()) {
        return;
    }
    Piece p = piece.value();
    int piece_index = static_cast<int>(p.type()) + static_cast<int>(p.color()) * 6;
    set_bit(all_bitmaps_[piece_index], square.index());
    for (int i = 0; i < 12; i++) {
        if (i != piece_index) {
            clear_bit(all_bitmaps_[i], square.index()); // Because we might have captured a piece
        }
    }
}

void Board::removePiece(const Square& square, const Piece::Optional& piece) {
    if (!piece.has_value()) {
        return;
    }
    Piece p = piece.value();
    int piece_index = static_cast<int>(p.type()) + static_cast<int>(p.color()) * 6;
    clear_bit(all_bitmaps_[piece_index], square.index());
}

Piece::Optional Board::piece(const Square& square) const {
    for (int i = 0; i < 12; i++) {
        if (get_bit(all_bitmaps_[i], square.index())) {
            int piece_color = i / 6; // 0 = white, 1 = black
            int piece_type = i % 6; // 0 = Pawn, 1 = Knight, 2 = Bishop, 3 = Rook, 4 = Queen, 5 = King
            return Piece(static_cast<PieceColor>(piece_color), static_cast<PieceType>(piece_type));
        }
    }
    return std::nullopt;
}

void Board::setTurn(PieceColor turn) {
    turn_ = turn;
}

PieceColor Board::turn() const {
    return turn_;
}

void Board::setCastlingRights(CastlingRights cr) {
    castling_rights_ = cr;
}

CastlingRights Board::castlingRights() const {
    return castling_rights_;
}

void Board::setEnPassantSquare(const Square::Optional& square) {
    en_passant_square_ = square;
}

Square::Optional Board::enPassantSquare() const {
    return en_passant_square_;
}

void Board::makeMove(const Move& move) {
    Square from = move.from();
    Square to = move.to();
    Piece::Optional piece = Board::piece(from);
    setPiece(to, piece);
    removePiece(from, piece);
    
    std::optional<PieceType> promotion = move.promotion();
    if (piece.has_value() && promotion.has_value()) {
        PieceType promotion_type = promotion.value();
        PieceColor color = piece.value().color();
        Piece promoted_piece = Piece(color, promotion_type);
        setPiece(to, promoted_piece);
    }
}

void Board::pseudoLegalMoves(MoveVec& moves) const {
    (void)moves;
}

void Board::pseudoLegalMovesFrom(const Square& from,
                                 Board::MoveVec& moves) const {
    (void)from;
    (void)moves;
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
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
