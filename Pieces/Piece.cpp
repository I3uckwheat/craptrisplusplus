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
  row{0},
  col{0} {};	 

std::vector<std::pair<int, int>> Piece::getPieceRelativeDirectionalCoords(PieceEnums::Direction direction) {
 std::vector<std::pair<int, int>> pieceSideCoords{};
    switch(direction) {
      case PieceEnums::Direction::BOTTOM:
        for(auto row = canvasSize - 1; row > -1; row--) {
          for(auto col = canvasSize - 1; col > -1; col--) {
            if(shape[row][col]) {
              pieceSideCoords.emplace_back(std::pair<int, int>{row, col});
              break;
            }
          }
        }
        break;
      case PieceEnums::Direction::LEFT:
        for(auto row = 0; row < canvasSize; row++) {
          for(auto col = 0; col < canvasSize; col++) {
            if(shape[row][col]) {
              pieceSideCoords.emplace_back(std::pair<int, int>{row, col});
              break;
            }
          }
        }
        break;
      case PieceEnums::Direction::RIGHT:
        for(auto row = 0; row < canvasSize; row++) {
          for(auto col = canvasSize - 1; col > -1; col--) {
            if(shape[row][col]) {
              pieceSideCoords.emplace_back(std::pair<int, int>{row, col});
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
