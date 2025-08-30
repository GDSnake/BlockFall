#include "Piece.h"

#include "AssetManagers/SpriteManager.h"

static std::vector<SDL_Point> calculateMinimumValues(const std::vector<SDL_Point>& coordVector)
{
    std::vector<SDL_Point> result;
    result.reserve(coordVector.size()/4);
    for (int i = 0; i < coordVector.size(); i += 4)
    {
        SDL_Point tempPoint = { INT_MAX, INT_MAX };
        for (int j = i; j < i + 4; ++j)
        {
            tempPoint.x = std::min(tempPoint.x, coordVector[j].x);
            tempPoint.y = std::min(tempPoint.y, coordVector[j].y);
        }
        result.emplace_back(tempPoint);
    }
    return result;
}

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
    _area = { 4,1 };
    _shape = PieceShapes::I_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Cyan);
}

//  |X| | |
//  |X|X|X|
void Piece::createJ()
{
    createRotationJ();
    _area = { 3,2 };
    _shape = PieceShapes::J_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Blue);
}

//  | | |X|
//  |X|X|X|
void Piece::createL()
{
    createRotationL();
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
    _deltaOriginforEachRotation = { SDL_Point{0,0} }; // no rotation
    _area = { 2,2 };
    _shape = PieceShapes::O_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Yellow);
}

//  | |X|X|
//  |X|X| |
void Piece::createS()
{
    createRotationS();
    _area = { 3,2 };
    _shape = PieceShapes::S_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Green);
}

//  |X|X| |
//  | |X|X|
void Piece::createZ()
{
    createRotationZ();
    _area = { 3,2 };
    _shape = PieceShapes::Z_shape;
    _block = SpriteManager::getInstance().getBlock(BlockTypes::Red);
}

//  | |X| |
//  |X|X|X|
void Piece::createT()
{
    createRotationT();
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

SDL_Point Piece::getCurrentDeltaOrigin() const
{
    return _deltaOriginforEachRotation[_currentRotationIndex];
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
        SDL_Point{1,-2}, SDL_Point{1,-1}, SDL_Point{1,0}, SDL_Point{1,1}
    };
    _deltaOriginforEachRotation = calculateMinimumValues(_blocksCoord);
}

void Piece::createRotationJ() // Working good
{
    _blocksCoord.reserve(PieceConsts::numBlocks * PieceConsts::rotationsNumber_J_L_T);

    _blocksCoord = {
        // down
        SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{2,0}, SDL_Point{2,1},
        // left
        SDL_Point{1,-1}, SDL_Point{1,0}, SDL_Point{0,1}, SDL_Point{1,1},
        // up
        SDL_Point{0,-1}, SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{2,0},
        // right
        SDL_Point{1,-1}, SDL_Point{2,-1}, SDL_Point{1,0}, SDL_Point{1,1}
    };
    _deltaOriginforEachRotation = calculateMinimumValues(_blocksCoord);
}

void Piece::createRotationL() // Working Good
{
    _blocksCoord.reserve(PieceConsts::numBlocks * PieceConsts::rotationsNumber_J_L_T);

    _blocksCoord = {
        // down
        SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{2,0}, SDL_Point{0,1},
        // left
        SDL_Point{0,-1}, SDL_Point{1,-1}, SDL_Point{1,0},SDL_Point{1,1},
        // up
        SDL_Point{2,-1}, SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{2,0},
        // right
        SDL_Point{1,-1}, SDL_Point{1,0}, SDL_Point{1,1}, SDL_Point{2,1}
    };
    _deltaOriginforEachRotation = calculateMinimumValues(_blocksCoord);


}

void Piece::createRotationS() // Working good
{
    _blocksCoord.reserve(PieceConsts::numBlocks * PieceConsts::rotationsNumber_I_S_Z);

    _blocksCoord = {
        // horizontal
         SDL_Point{0,1}, SDL_Point{1,1}, SDL_Point{1,0}, SDL_Point{2,0},
        // vertical
        SDL_Point{0,-1}, SDL_Point{0,0}, SDL_Point{1,0} , SDL_Point{1,1},
    };
    _deltaOriginforEachRotation = calculateMinimumValues(_blocksCoord);
}

void Piece::createRotationZ()// Working good
{
    _blocksCoord.reserve(PieceConsts::numBlocks * PieceConsts::rotationsNumber_I_S_Z);

    _blocksCoord = {
        // horizontal
        SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{1,1}, SDL_Point{2,1},
        // vertical
        SDL_Point{1,-1}, SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{0,1}
    };
    _deltaOriginforEachRotation = calculateMinimumValues(_blocksCoord);
}

void Piece::createRotationT() // Working good
{
    _blocksCoord.reserve(PieceConsts::numBlocks * PieceConsts::rotationsNumber_J_L_T);
    _blocksCoord = {
        // down
        SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{2,0}, SDL_Point{1,1},
        // left
        SDL_Point{1,-1}, SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{1,1},
        // up
        SDL_Point{1,-1}, SDL_Point{0,0}, SDL_Point{1,0}, SDL_Point{2,0},
        // right
        SDL_Point{1,-1}, SDL_Point{1,0}, SDL_Point{2,0}, SDL_Point{1,1}
    };

    _deltaOriginforEachRotation = calculateMinimumValues(_blocksCoord);
}
