#include <./Piece.hh>
#include <vector>

Piece::Piece(
    PieceEnums::BlockType type = PieceEnums::BlockType::EMPTY, 
    std::vector<std::vector<bool>> shape = {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    }
  ) : 
  type{type}, 
  shape{shape},
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
