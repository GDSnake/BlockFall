#pragma once
#include <memory>
#include <span>
#include <SDL3/SDL_rect.h>

#include "Input/InputManager.h"

class Block;

namespace PieceConsts
{
    static constexpr int numBlocks = 4; // all Pieces use 4 blocks
    static constexpr int rotationsNumber_I_S_Z = 2;
    static constexpr int rotationsNumber_J_L_T = 4;
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

    std::span<SDL_Point> getBlocksCoord();
    SDL_Point getCurrentDeltaOrigin() const;
    SDL_Point getPieceArea() const;

    // Rotation
    void rotateCW();   // clockwise
    void rotateCCW();  // counter-clockwise
    inline int getRotationIndex() const { return _currentRotationIndex; }
    inline void setRotationIndex(int index) { _currentRotationIndex = index; }

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

    void createRotationI();
    void createRotationJ();
    void createRotationL();
    void createRotationS();
    void createRotationZ();
    void createRotationT();

    std::vector<SDL_Point> _blocksCoord;
    std::vector<SDL_Point> _deltaOriginforEachRotation;

    SDL_Point _area;
    int _currentRotationIndex;
    std::shared_ptr<Block> _block;
    PieceShapes _shape;
};