#ifndef CHESS_ENGINE_SQUARE_HPP
#define CHESS_ENGINE_SQUARE_HPP

#include <optional>
#include <iosfwd>
#include <string>

class Square {
public:

    using Coordinate = unsigned;
    using Index = unsigned;
    using Optional = std::optional<Square>;

    static Optional fromCoordinates(Coordinate file, Coordinate rank);
    static Optional fromIndex(Index index);
    static Optional fromName(const std::string& name);

    Coordinate file() const;
    Coordinate rank() const;
    Index index() const;

    static const Square A1, B1, C1, D1, E1, F1, G1, H1;
    static const Square A2, B2, C2, D2, E2, F2, G2, H2;
    static const Square A3, B3, C3, D3, E3, F3, G3, H3;
    static const Square A4, B4, C4, D4, E4, F4, G4, H4;
    static const Square A5, B5, C5, D5, E5, F5, G5, H5;
    static const Square A6, B6, C6, D6, E6, F6, G6, H6;
    static const Square A7, B7, C7, D7, E7, F7, G7, H7;
    static const Square A8, B8, C8, D8, E8, F8, G8, H8;

private:

    Square(Index index);
    unsigned index_;
};

std::ostream& operator<<(std::ostream& os, const Square& square);

// Necessary to support Square as the key in std::map.
bool operator<(const Square& lhs, const Square& rhs);
bool operator>(const Square& lhs, const Square& rhs);
bool operator==(const Square& lhs, const Square& rhs);
bool operator!=(const Square& lhs, const Square& rhs);

#endif
