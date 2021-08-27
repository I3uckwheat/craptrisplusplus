#pragma once
#include <./Piece.hh>
#include <memory>

class PieceFactory {
  const PieceEnums::BlockType blockType;
  const std::vector<std::vector<bool>> pieceShape;

public:
  PieceFactory(PieceEnums::BlockType type, std::vector<std::vector<bool>> pieceShape); 
  Piece* create(); 
  static int pieceCount;
  static std::vector<PieceFactory> pieceFactories;
};
