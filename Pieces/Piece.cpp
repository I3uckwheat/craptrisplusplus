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

void Piece::rotatePiece() {
  int layerCount = canvasSize / 2;

  for(auto layer = 0; layer < layerCount; layer++) {
    int first = layer;
    int last = canvasSize - first - 1;

    for(auto element = first; element < last; element++) {
      int offset = element - first;

      bool top = shape[first][element];
      bool right = shape[element][last];
      bool bottom = shape[last][last-offset];
      bool left = shape[last-offset][first];

      // top
      shape[first][element] = left;

      // right
      shape[element][last] = top;

      // bottom
      shape[last][last-offset] = right;

      // left
      shape[last-offset][first] = bottom;
    }
  }
}