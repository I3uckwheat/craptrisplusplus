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

std::vector<std::pair<int, int>> Piece::getPieceCoords() {
 std::vector<std::pair<int, int>> pieceCoords{};
  for(auto row = canvasSize - 1; row > -1; row--) {
    for(auto col = canvasSize - 1; col > -1; col--) {
      if(shape[row][col]) {
        pieceCoords.emplace_back(std::pair<int, int>{row, col});
      }
    }
  }

  return pieceCoords;
};
