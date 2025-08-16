#include "Piece.h"
#include "SpriteManager.h"

Piece::Piece(PieceShapes shape)
{
    switch (shape)
    {
    case PieceShapes::I_shape:
        createI();
        break;    
    case PieceShapes::J_shape:
        createJ();
        break;
    case PieceShapes::L_shape:
        createL();
        break;
    case PieceShapes::O_shape:
        createO();
        break;
    case PieceShapes::S_shape:
        createS();
        break;
    case PieceShapes::Z_shape:
        createZ();
        break;
    case PieceShapes::T_shape:
        createT();
        break;
    case PieceShapes::Total:
        break;
    }
}

//  |X|X|X|X|
//  | | | | |
void Piece::createI()
{
    for (int i = 0; i < PieceConsts::maxPieceArea * 0.5f; ++i)
    {
        _pieceBlocks[i] = SpriteManager::getInstance().getBlock(BlockTypes::Cyan);
    }
    _area = { 4, 1};
}

//  |X| | | |
//  |X|X|X| |
void Piece::createJ()
{
    _pieceBlocks[0] = SpriteManager::getInstance().getBlock(BlockTypes::Blue);
    for (int i = 4; i < PieceConsts::maxPieceArea - 1; ++i)
    {
        _pieceBlocks[i] = SpriteManager::getInstance().getBlock(BlockTypes::Blue);
    }
    _area = {3, 2};
}

//  | | |X| |
//  |X|X|X| |
void Piece::createL()
{
    _pieceBlocks[2] = SpriteManager::getInstance().getBlock(BlockTypes::Orange);
    for (int i = 4; i < PieceConsts::maxPieceArea - 1; ++i)
    {
        _pieceBlocks[i] = SpriteManager::getInstance().getBlock(BlockTypes::Orange);
    }
    _area = { 3, 2 };
}

//  |X|X| | |
//  |X|X| | |
void Piece::createO()
{
    for (int i = 0; i < 2; ++i)
    {
        _pieceBlocks[i] = SpriteManager::getInstance().getBlock(BlockTypes::Yellow);
    }
    for (int i = 4; i < 6; ++i)
    {
        _pieceBlocks[i] = SpriteManager::getInstance().getBlock(BlockTypes::Yellow);
    }
    _area = { 2, 2 };
}

//  | |X|X| |
//  |X|X| | |
void Piece::createS()
{
    for (int i = 1; i < 3; ++i)
    {
        _pieceBlocks[i] = SpriteManager::getInstance().getBlock(BlockTypes::Green);
    }
    for (int i = 4; i < 6; ++i)
    {
        _pieceBlocks[i] = SpriteManager::getInstance().getBlock(BlockTypes::Green);
    }
    _area = { 3, 2 };
}

//  |X|X| | |
//  | |X|X| |
void Piece::createZ()
{
    for (int i = 0; i < 2; ++i)
    {
        _pieceBlocks[i] = SpriteManager::getInstance().getBlock(BlockTypes::Red);
    }
    for (int i = 5; i < PieceConsts::maxPieceArea - 1; ++i)
    {
        _pieceBlocks[i] = SpriteManager::getInstance().getBlock(BlockTypes::Red);
    }
    _area = { 3, 2 };
}

//  | |X| | |
//  |X|X|X| |
void Piece::createT()
{
    _pieceBlocks[1] = SpriteManager::getInstance().getBlock(BlockTypes::Purple);
    for (int i = 4; i < PieceConsts::maxPieceArea - 1; ++i)
    {
        _pieceBlocks[i] = SpriteManager::getInstance().getBlock(BlockTypes::Purple);
    }
    _area = { 3, 2 };
}

Piece::~Piece()
{
}

std::array<std::shared_ptr<Block>, PieceConsts::maxPieceArea> Piece::getPiece()
{
    return _pieceBlocks;
}

SDL_Point Piece::getPieceArea() const
{
    return _area;
}
