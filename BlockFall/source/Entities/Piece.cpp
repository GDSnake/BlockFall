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

std::shared_ptr<Block> Piece::getBlock() const
{
    return _block;
}

void Piece::createI()
{
    _blocksCoord = { SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{2,0}, SDL_Point{3,0} };
    _pivot = { 1,0 }; // 2nd block is pivot
    _area = { 4,1 };
    _shape = PieceShapes::I_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Cyan);
}

//  |X| | |
//  |X|X|X|
void Piece::createJ()
{
    _blocksCoord = { SDL_Point{0,0}, SDL_Point{0,1}, SDL_Point{1,1}, SDL_Point{2,1} };
    _pivot = { 1,1 };
    _area = { 3,2 };
    _shape = PieceShapes::J_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Blue);
}

//  | | |X|
//  |X|X|X|
void Piece::createL()
{
    _blocksCoord = { SDL_Point{2,0}, SDL_Point{0,1}, SDL_Point{1,1}, SDL_Point{2,1} };
    _pivot = { 1,1 };
    _area = { 3,2 };
    _shape = PieceShapes::L_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Orange);
}

//  |X|X|
//  |X|X|
void Piece::createO()
{
    _blocksCoord = { SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{0,1}, SDL_Point{1,1} };
    _pivot = { 0,0 }; // irrelevant for square
    _area = { 2,2 };
    _shape = PieceShapes::O_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Yellow);
}

//  | |X|X|
//  |X|X| |
void Piece::createS()
{
    _blocksCoord = { SDL_Point{1,0}, SDL_Point{2,0}, SDL_Point{0,1}, SDL_Point{1,1} };
    _pivot = { 1,1 };
    _area = { 3,2 };
    _shape = PieceShapes::S_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Green);
}

//  |X|X| |
//  | |X|X|
void Piece::createZ()
{
    _blocksCoord = { SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{1,1}, SDL_Point{2,1} };
    _pivot = { 1,1 };
    _area = { 3,2 };
    _shape = PieceShapes::Z_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Red);
}

//  | |X| |
//  |X|X|X|
void Piece::createT()
{
    _blocksCoord = { SDL_Point{1,0}, SDL_Point{0,1}, SDL_Point{1,1}, SDL_Point{2,1} };
    _pivot = { 1,1 };
    _area = { 3,2 };
    _shape = PieceShapes::T_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Purple);
}

Piece::~Piece()
{
}

std::array<SDL_Point, PieceConsts::numBlocks> Piece::getBlocksCoord()
{
    return _blocksCoord;
}

SDL_Point Piece::getPieceArea() const
{
    return _area;
}

void Piece::rotateCW()
{
    if (_shape == PieceShapes::O_shape) return; // square doesn't rotate

    for (auto& cell : _blocksCoord) {
        int relX = cell.x - _pivot.x;
        int relY = cell.y - _pivot.y;

        // 90o clockwise
        int rotatedX = relY;
        int rotatedY = -relX;

        cell.x = rotatedX + _pivot.x;
        cell.y = rotatedY + _pivot.y;
    }
}

void Piece::rotateCCW()
{
    if (_shape == PieceShapes::O_shape) return;

    for (auto& cell : _blocksCoord) {
        int relX = cell.x - _pivot.x;
        int relY = cell.y - _pivot.y;

        // 90o counter-clockwise
        int rotatedX = -relY;
        int rotatedY = relX;

        cell.x = rotatedX + _pivot.x;
        cell.y = rotatedY + _pivot.y;
    }
}
