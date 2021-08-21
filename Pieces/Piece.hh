#include <vector>
#include <memory>

namespace PieceEnums {
  enum Direction {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
  };

  enum BlockType {
    EMTPY,
    RED,
    GREEN,
    BLUE,
    PURPLE,
    YELLOW
  };
} 

struct Piece {
  Piece();
  int x;
  int y;
  static const int canvasSize{4};
  static const int totalPieces{5};

  // todo make const
  PieceEnums::BlockType type;

  std::vector<std::vector<bool>> shape;
  std::vector<std::pair<int, int>> getPieceRelativeDirectionalCoords(PieceEnums::Direction direction);
};

struct SquarePiece : public Piece {
  SquarePiece();
};

struct LinePiece : public Piece {
  LinePiece();
};

struct LPiece : public Piece {
  LPiece();
};

struct TPiece : public Piece {
  TPiece();
};

struct ZPiece : public Piece {
  ZPiece();
};
