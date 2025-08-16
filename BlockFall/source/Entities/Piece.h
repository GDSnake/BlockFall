#pragma once
#include <array>
#include <memory>
#include <SDL3/SDL_rect.h>

class Block;

namespace PieceConsts
{
    static constexpr int maxPieceArea = 8; // Biggest area is the I shape since spawns flat
                                           //  |X|X|X|X|
                                           //  | | | | |
}


enum class PieceShapes : uint8_t
{
    I_shape = 0,
    J_shape,
    L_shape,
    O_shape,
    S_shape,
    Z_shape,
    T_shape,
    Total
};

class Piece
{
public:
    Piece() = delete;
    Piece(PieceShapes shape);
    ~Piece();

    std::array<std::shared_ptr<Block>, PieceConsts::maxPieceArea> getPiece();
    SDL_Point getPieceArea() const;
private:
    void createI();
    void createJ();
    void createL();
    void createO();
    void createS();
    void createZ();
    void createT();

    std::array<std::shared_ptr<Block>, PieceConsts::maxPieceArea> _pieceBlocks;
    SDL_Point _area;
};