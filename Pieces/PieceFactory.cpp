#include <./PieceFactory.hh>
#include <./Piece.hh>
#include <memory>

int PieceFactory::pieceCount = 0;

PieceFactory::PieceFactory(PieceEnums::BlockType type, std::vector<std::vector<bool>> pieceShape) 
  : blockType{type}, 
  pieceShape{pieceShape}
  {
	  pieceCount++;
  }

Piece* PieceFactory::create() {
  return new Piece{blockType, pieceShape};
}

std::vector<PieceFactory> PieceFactory::pieceFactories {
  // Square Piece
  PieceFactory{PieceEnums::BlockType::GREEN, {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0}
  }},

  // Line Piece
  PieceFactory{PieceEnums::BlockType::BLUE, {
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0}
  }},

  // T Piece
  PieceFactory{PieceEnums::BlockType::PURPLE, {
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0}
  }},

  // L piece
  PieceFactory{PieceEnums::BlockType::RED, {
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0}
  }},

  // Z Piece
  PieceFactory{PieceEnums::BlockType::YELLOW, {
    {0, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
  }},
};
