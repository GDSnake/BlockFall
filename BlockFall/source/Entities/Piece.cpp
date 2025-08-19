#include "Piece.h"
#include "SpriteManager.h"

Piece::Piece(PieceShapes shape)
{
    _currentRotationIndex = 0;
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
    createRotationI();
    _pivot = { 1,0 }; // 2nd block is pivot
    _area = { 4,1 };
    _shape = PieceShapes::I_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Cyan);
}

//  |X| | |
//  |X|X|X|
void Piece::createJ()
{
    createRotationJ();
    _pivot = { 1,1 };
    _area = { 3,2 };
    _shape = PieceShapes::J_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Blue);
}

//  | | |X|
//  |X|X|X|
void Piece::createL()
{
    createRotationL();
    _pivot = { 1,1 };
    _area = { 3,2 };
    _shape = PieceShapes::L_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Orange);
}

//  |X|X|
//  |X|X|
void Piece::createO()
{
    _blocksCoord.reserve(PieceConsts::numBlocks);
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
    createRotationS();
    _pivot = { 1,1 };
    _area = { 3,2 };
    _shape = PieceShapes::S_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Green);
}

//  |X|X| |
//  | |X|X|
void Piece::createZ()
{
    createRotationZ();
    _pivot = { 1,1 };
    _area = { 3,2 };
    _shape = PieceShapes::Z_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Red);
}

//  | |X| |
//  |X|X|X|
void Piece::createT()
{
    createRotationT();
    _pivot = { 1,1 };
    _area = { 3,2 };
    _shape = PieceShapes::T_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Purple);
}

Piece::~Piece()
{
}

std::span<SDL_Point> Piece::getBlocksCoord()
{
    int start = _currentRotationIndex * 4;
    return std::span<SDL_Point>(_blocksCoord).subspan(start, PieceConsts::numBlocks);
}

SDL_Point Piece::getPieceArea() const
{
    return _area;
}

void Piece::rotateCW()
{
    int numberOfRotations = 1;
    switch (_shape)
    {
    case PieceShapes::I_shape:
    case PieceShapes::S_shape:
    case PieceShapes::Z_shape:
        numberOfRotations = PieceConsts::rotationsNumber_I_S_Z;
        break;

    case PieceShapes::J_shape:
    case PieceShapes::L_shape:
    case PieceShapes::T_shape:
        numberOfRotations = PieceConsts::rotationsNumber_J_L_T;
        break;

    case PieceShapes::O_shape:
        numberOfRotations = 1; // no rotation
        break;

    default:
        break;
    }
    std::swap(_area.x, _area.y);
    _currentRotationIndex = (_currentRotationIndex + 1) % numberOfRotations;
}

void Piece::rotateCCW()
{
    int numberOfRotations = 1;
    switch (_shape)
    {
    case PieceShapes::I_shape:
    case PieceShapes::S_shape:
    case PieceShapes::Z_shape:
        numberOfRotations = PieceConsts::rotationsNumber_I_S_Z;
        break;

    case PieceShapes::J_shape:
    case PieceShapes::L_shape:
    case PieceShapes::T_shape:
        numberOfRotations = PieceConsts::rotationsNumber_J_L_T;
        break;

    case PieceShapes::O_shape:
        numberOfRotations = 1; // no rotation
        break;

    default:
        break;
    }
    std::swap(_area.x, _area.y); 
    _currentRotationIndex = (_currentRotationIndex + (numberOfRotations - 1)) % numberOfRotations;
}

void Piece::createRotationI()
{
    _blocksCoord.reserve(PieceConsts::numBlocks * PieceConsts::rotationsNumber_I_S_Z);
    _blocksCoord = {
        // horizontal
        SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{2,0}, SDL_Point{3,0},
        // vertical
        SDL_Point{1,0}, SDL_Point{1,1}, SDL_Point{1,2}, SDL_Point{1,3}
    };
}

void Piece::createRotationJ()
{
    _blocksCoord.reserve(PieceConsts::numBlocks * PieceConsts::rotationsNumber_J_L_T);

    _blocksCoord = {
        // down
        SDL_Point{0,1}, SDL_Point{1,1}, SDL_Point{2,1}, SDL_Point{2,2},
        // left
        SDL_Point{1,0}, SDL_Point{1,1}, SDL_Point{0,2}, SDL_Point{1,2},
        // up
         SDL_Point{0,0}, SDL_Point{0,1}, SDL_Point{1,1}, SDL_Point{2,1},
        // right
        SDL_Point{1,0}, SDL_Point{2,0}, SDL_Point{1,1}, SDL_Point{1,2}
    };
}

void Piece::createRotationL()
{
    _blocksCoord.reserve(PieceConsts::numBlocks * PieceConsts::rotationsNumber_J_L_T);

    _blocksCoord = {
        // up
        SDL_Point{2,0}, SDL_Point{0,1}, SDL_Point{1,1}, SDL_Point{2,1},
        // right
        SDL_Point{1,0}, SDL_Point{1,1}, SDL_Point{1,2}, SDL_Point{2,2},
        // down
        SDL_Point{0,1}, SDL_Point{1,1}, SDL_Point{2,1}, SDL_Point{0,2},
        // left
        SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{1,1}, SDL_Point{1,2}
    };
}

void Piece::createRotationS()
{
    _blocksCoord.reserve(PieceConsts::numBlocks * PieceConsts::rotationsNumber_I_S_Z);

    _blocksCoord = {
        // horizontal
        SDL_Point{1,0}, SDL_Point{2,0}, SDL_Point{0,1}, SDL_Point{1,1},
        // vertical
        SDL_Point{1,0}, SDL_Point{1,1}, SDL_Point{2,1}, SDL_Point{2,2}
    };
}

void Piece::createRotationZ()
{
    _blocksCoord.reserve(PieceConsts::numBlocks * PieceConsts::rotationsNumber_I_S_Z);

    _blocksCoord = {
        // horizontal
        SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{1,1}, SDL_Point{2,1},
        // vertical
        SDL_Point{2,0}, SDL_Point{1,1}, SDL_Point{2,1}, SDL_Point{1,2}
    };
}

void Piece::createRotationT()
{
    _blocksCoord.reserve(PieceConsts::numBlocks * PieceConsts::rotationsNumber_J_L_T);
    _blocksCoord = {
        // down
        SDL_Point{0,1}, SDL_Point{1,1}, SDL_Point{2,1}, SDL_Point{1,2},
        // left
        SDL_Point{1,0}, SDL_Point{0,1}, SDL_Point{1,1}, SDL_Point{1,2},
        // up
        SDL_Point{1,0}, SDL_Point{0,1}, SDL_Point{1,1}, SDL_Point{2,1},
        // right
        SDL_Point{1,0}, SDL_Point{1,1}, SDL_Point{2,1}, SDL_Point{1,2}
    };
}
