#include <./TetrisBoard.hh>

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <exception>
#include <cstdint>
#include <memory>
#include <random>

#include <../Pieces/Piece.hh>
#include <../Pieces/PieceFactory.hh>
#include <../constants.hh>

TetrisBoard::TetrisBoard(SDL_Renderer *grenderer) : gRenderer{grenderer}
{
  std::random_device rseed;
  rgen = std::mt19937(rseed());
  pieceRandomizer = std::uniform_int_distribution<int>(0, PieceFactory::pieceCount - 1);

  for (auto row = 0; row < height; row++)
  {
    board.emplace_back(std::vector<BlockType>{});
    for (auto col = 0; col < width; col++)
    {
      board[row].emplace_back(BlockType::EMPTY);
    }
  }

  setNextPiece();
};

TetrisBoard::~TetrisBoard() {}

bool TetrisBoard::willBottomCollide()
{
  auto pieceCoords = activePiece->getPieceCoords();
  for (auto pieceCoord = pieceCoords.begin(); pieceCoord != pieceCoords.end(); pieceCoord++)
  {
    auto row = pieceCoord->first + activePiece->row;
    auto col = pieceCoord->second + activePiece->col;
    if (row >= height - 1)
    {
      return true;
    }

    if (board.at(row + 1).at(col))
    {
      return true;
    }
  }
  return false;
}

void TetrisBoard::mergePieceToBoard()
{
  for (auto row = activePiece->canvasSize - 1; row > -1; row--)
  {
    for (auto col = activePiece->canvasSize - 1; col > -1; col--)
    {
      if (activePiece->shape[row][col])
      {
        board[row + activePiece->row][col + activePiece->col] = activePiece->type;
      }
    }
  }
}

bool TetrisBoard::willHaveXCollision(Direction direction)
{
  int newColumn{activePiece->col};
  switch (direction)
  {
  case Direction::LEFT:
    newColumn--;
    break;
  case Direction::RIGHT:
    newColumn++;
    break;
  }

  auto pieceCoords = activePiece->getPieceCoords();
  for (auto i = pieceCoords.begin(); i != pieceCoords.end(); i++)
  {
    auto row = i->first + activePiece->row;
    auto col = i->second + newColumn;

    if (
        col < 0                  // left bounds
        || col >= width          // right bounds
        || board.at(row).at(col) // piece bounds
    )
    {
      return true;
    }
  }

  return false;
}

void TetrisBoard::move(SDL_Keycode key)
{
  switch (key)
  {
  case SDLK_LEFT:
    if (!willHaveXCollision(Direction::LEFT))
    {
      activePiece->col--;
    }
    break;
  case SDLK_RIGHT:
    if (!willHaveXCollision(Direction::RIGHT))
    {
      activePiece->col++;
    }
    break;
  case SDLK_UP:
    // TODO Rotate
    break;
  case SDLK_DOWN:
    break;
  }
}

void TetrisBoard::setNextPiece()
{
  activePiece.reset(PieceFactory::pieceFactories[pieceRandomizer(rgen)].create());
}

// bool isLineComplete(const std::vector<BlockType>& line) {
//   std::cout<<"lineSize in isCOmplete: "<<line.size()<<std::endl;
//   for(auto i = line.begin(); i < line.end(); i++) {
//     std::cout<<*i<<" | "<<static_cast<BlockType>(*i)<<std::endl;
//     if(static_cast<BlockType>(*i) == BlockType::EMTPY) {
//       return false;
//     }
//   }

//   return true;
// }

// void clearCompletedLines() {
//   auto result = isLineComplete(board[0]);
//   std::cout<<result<<std::endl;
//   result = isLineComplete(board[12]);
//   std::cout<<result<<std::endl;
//   // auto i = height - 1;
//   // while(i > 0) {
//   //   auto lineIsCompleted = isLineComplete(board[i]);
//   //   if(lineIsCompleted) std::cout<<"is complete"<<std::endl;
//   //   i--;
//   // }
// }

void TetrisBoard::update(float dt)
{
  currentTime += dt;
  if (nextUpdate < currentTime)
  {
    activePiece->row++;
    nextUpdate = currentTime + 20;
  }

  if (willBottomCollide())
  {
    mergePieceToBoard();
    setNextPiece();
    // clearCompletedLines();
  }
}

void TetrisBoard::setBlockColor(BlockType type)
{
  switch (type)
  {
  case BlockType::RED:
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    break;
  case BlockType::GREEN:
    SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
    break;
  case BlockType::BLUE:
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
    break;
  case BlockType::PURPLE:
    SDL_SetRenderDrawColor(gRenderer, 128, 0, 128, 255);
    break;
  case BlockType::YELLOW:
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
    break;
  default:
    SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 100);
  }
}

void TetrisBoard::renderFallingPiece()
{
  setBlockColor(activePiece->type);
  for (auto row = 0; row < Piece::canvasSize; ++row)
  {
    for (auto col = 0; col < Piece::canvasSize; ++col)
    {
      if (activePiece->shape[row][col])
      {
        SDL_Rect rect{};
        rect.x = (squareSize * (col + activePiece->col)) + playFieldX;
        rect.y = squareSize * (row + activePiece->row);
        rect.w = squareSize;
        rect.h = squareSize;

        SDL_RenderFillRect(gRenderer, &rect);
      }
    }
  }
}

void TetrisBoard::renderSquares()
{
  for (auto row = 0; row < height; row++)
  {
    for (auto col = 0; col < width; col++)
    {
      SDL_Rect rect{};
      rect.y = squareSize * row;
      rect.x = (squareSize * col) + playFieldX;
      rect.w = squareSize;
      rect.h = squareSize;

      auto block = board[row][col];
      setBlockColor(block);
      if (block == BlockType::EMPTY)
      {
        SDL_RenderDrawRect(gRenderer, &rect);
      }
      else
      {
        SDL_RenderFillRect(gRenderer, &rect);
      }
    }
  }
}

void TetrisBoard::render()
{
  // Render playfield
  SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
  SDL_RenderClear(gRenderer);
  renderSquares();
  renderFallingPiece();

  SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(gRenderer, &playField);
}

