#include <./Piece.hh>
#include <vector>

Piece::Piece() : 
  type{PieceEnums::BlockType::EMTPY}, 
  shape{
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  },
  x{0},
  y{0} {};	 

std::vector<std::pair<int, int>> Piece::getPieceRelativeDirectionalCoords(PieceEnums::Direction direction) {
 std::vector<std::pair<int, int>> pieceSideCoords{};
    switch(direction) {
      case PieceEnums::Direction::BOTTOM:
        for(auto i = canvasSize - 1; i > -1; i--) {
          for(auto j = canvasSize - 1; j > -1; j--) {
            if(shape[j][i]) {
              pieceSideCoords.emplace_back(std::pair<int, int>{i, j});
              break;
            }
          }
        }
        break;
      case PieceEnums::Direction::LEFT:
        for(auto i = 0; i < canvasSize; i++) {
          for(auto j = 0; j < canvasSize; j++) {
            if(shape[j][i]) {
              pieceSideCoords.emplace_back(std::pair<int, int>{i, j});
              break;
            }
          }
        }
        break;
      case PieceEnums::Direction::RIGHT:
        for(auto i = 0; i < canvasSize; i++) {
          for(auto j = canvasSize - 1; j > -1; j--) {
            if(shape[j][i]) {
              pieceSideCoords.emplace_back(std::pair<int, int>{i, j});
              break;
            }
          }
        }
        break;
      case PieceEnums::Direction::TOP:
        break;
    }

  return pieceSideCoords;
};

SquarePiece::SquarePiece() {
  type = PieceEnums::BlockType::GREEN;
  shape = {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0}
  };

  x = 0;
  y = 0;
};

LinePiece::LinePiece() {
  type = PieceEnums::BlockType::BLUE;
  shape = {
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0}
  };

  x = 0;
  y = 0;
};

LPiece::LPiece() {
  type = PieceEnums::BlockType::RED;
  shape = {
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0}
  };

  x = 0;
  y = 0;
};

TPiece::TPiece() {
  type = PieceEnums::BlockType::PURPLE;
  shape = {
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 0}
  };

  x = 0;
  y = 0;
};

ZPiece::ZPiece() {
  type = PieceEnums::BlockType::YELLOW;
  shape = {
    {0, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0}
  };

  x = 0;
  y = 0;
}

// TODO: figure this out
// struct PieceFactory {
//   virtual Piece* create() = 0;
// };

// struct SquarePieceFactory : public PieceFactory {
//   Piece* create() {
//     return new SquarePiece{};
//   }
// };

// struct LPieceFactory : public PieceFactory {
//   Piece* create() {
//     return new LPiece{};
//   }
// };

// std::vector<PieceFactory*> pieceFactories {
//   new SquarePieceFactory{},
//   new LPieceFactory{}
// };