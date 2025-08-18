#pragma once
#include <array>
#include <memory>
#include <SDL3/SDL_rect.h>

#include "InputManager.h"

class Block;

namespace PieceConsts
{
    static constexpr int numBlocks = 4; // all Pieces use 4 blocks
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

    std::array<SDL_Point, PieceConsts::numBlocks> getBlocksCoord();
    SDL_Point getPieceArea() const;

    // Rotation
    void rotateCW();   // clockwise
    void rotateCCW();  // counter-clockwise

    PieceShapes getShape() const { return _shape; }
    std::shared_ptr<Block> getBlock() const;
private:
    void createI();
    void createJ();
    void createL();
    void createO();
    void createS();
    void createZ();
    void createT();

    std::array<SDL_Point, PieceConsts::numBlocks> _blocksCoord;
    SDL_Point _pivot;   // pivot for rotation
    SDL_Point _area;
    std::shared_ptr<Block> _block;
    PieceShapes _shape;
};