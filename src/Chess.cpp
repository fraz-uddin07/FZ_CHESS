#include <iostream>
#include <string.h>
#include "Chess.hpp"
#include "TextureLoader.hpp"

#define nl '\n'

using namespace std;

Chess::Chess() {

    // White player always starts
   m_CurrentTurn = WHITE_PLAYER;

   // Game on!
   m_bGameFinished = false;

   // Nothing has happend yet
   m_undo.bCapturedLastMove         = false;
   m_undo.bCanUndo                  = false;
   m_undo.bCastlingKingSideAllowed  = false;
   m_undo.bCastlingQueenSideAllowed = false;
   m_undo.en_passant.bApplied       = false;
   m_undo.castling.bApplied         = false;

   // Initial board settings
   memcpy(board, initial_board, sizeof(char) * 8 * 8);

   // Castling is allowed (to each side) until the player moves the king or the rook
   m_bCastlingKingSideAllowed[WHITE_PLAYER]  = true;
   m_bCastlingKingSideAllowed[BLACK_PLAYER]  = true;

   m_bCastlingQueenSideAllowed[WHITE_PLAYER] = true;
   m_bCastlingQueenSideAllowed[BLACK_PLAYER] = true;




};


Chess::~Chess() {

    white_captured.clear();
   black_captured.clear();
   rounds.clear();

};


void Chess::printLogo(void)
{
    cout << "STARTING GAME..." << nl << nl;
    cout << "    ================================================\n";
    cout << "     _______ _________           _____ _    _ ______  _____ _____\n";
    cout << "    |  _____|______   |        / ____| |  | |  ____|/ ____/ ____|\n";
    cout << "    |  |___      _| _|        | |    | |__| | |__  | (___| (___ \n";
    cout << "    |   ___|   _| _|          | |    |  __  |  __|  \\___ \\\\___ \\ \n";
    cout << "    |  |     _|  |____        | |____| |  | | |____ ____) |___) |\n";
    cout << "    |__|    |_________|       \\_____|_|  |_|______|_____/_____/  ...... by FraZ Uddin\n\n";
    cout << "    ================================================\n\n";
}


void Chess::getTextures() {
    whiteRookTex = TextureLoader::generateTexture("res/white_rook.png", renderer);
    whiteKnightTex = TextureLoader::generateTexture("res/white_knight.png", renderer);
    whiteBishopTex = TextureLoader::generateTexture("res/white_bishop.png", renderer);
    whiteQueenTex = TextureLoader::generateTexture("res/white_queen.png", renderer);
    whiteKingTex = TextureLoader::generateTexture("res/white_king.png", renderer);
    whitePawnTex = TextureLoader::generateTexture("res/white_pawn.png", renderer);
    blackRookTex = TextureLoader::generateTexture("res/black_rook.png", renderer);
    blackKnightTex = TextureLoader::generateTexture("res/black_knight.png", renderer);
    blackBishopTex = TextureLoader::generateTexture("res/black_bishop.png", renderer);
    blackQueenTex = TextureLoader::generateTexture("res/black_queen.png", renderer);
    blackKingTex = TextureLoader::generateTexture("res/black_king.png", renderer);
    blackPawnTex = TextureLoader::generateTexture("res/black_pawn.png", renderer);
}


void Chess::renderCellHighlighter(SDL_Rect cellToHighlight) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 30, 144, 255, 1);           //Mildly transparent Blue Cell Highlighter 
    SDL_RenderFillRect(renderer, &cellToHighlight);
    SDL_RenderPresent(renderer);
}


void Chess::undoHighlighter(SDL_Rect cellToUndoHighlight, const char currentBoard[8][8]) {
    int x = cellToUndoHighlight.x / 80;
    int y = cellToUndoHighlight.y / 80;
    if ((x + y) % 2) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);           //Red Color
        SDL_RenderFillRect(renderer, &cellToUndoHighlight);
    }
    else {
        //Draw White Square
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &cellToUndoHighlight);
    }

    cellToUndoHighlight.w = 60;
    cellToUndoHighlight.h = 60;
    cellToUndoHighlight.x += 10;
    cellToUndoHighlight.y += 10;

    switch (currentBoard[y][x]) {
        case 'R':
                SDL_RenderCopy(renderer, whiteRookTex, NULL, &cellToUndoHighlight);
                break;
        case 'N':
                SDL_RenderCopy(renderer, whiteKnightTex, NULL, &cellToUndoHighlight);
                break;
        case 'B':
                SDL_RenderCopy(renderer, whiteBishopTex, NULL, &cellToUndoHighlight);
                break;
        case 'Q':
                SDL_RenderCopy(renderer, whiteQueenTex, NULL, &cellToUndoHighlight);
                break;
        case 'K':
                SDL_RenderCopy(renderer, whiteKingTex, NULL, &cellToUndoHighlight);
                break;
        case 'P':
                SDL_RenderCopy(renderer, whitePawnTex, NULL, &cellToUndoHighlight);
                break;
        case 'r':
                SDL_RenderCopy(renderer, blackRookTex, NULL, &cellToUndoHighlight);
                break;
        case 'n':
                SDL_RenderCopy(renderer, blackKnightTex, NULL, &cellToUndoHighlight);
                break;
        case 'b':
                SDL_RenderCopy(renderer, blackBishopTex, NULL, &cellToUndoHighlight);
                break;
        case 'q':
                SDL_RenderCopy(renderer, blackQueenTex, NULL, &cellToUndoHighlight);
                break;
        case 'k':
                SDL_RenderCopy(renderer, blackKingTex, NULL, &cellToUndoHighlight);
                break;
        case 'p':
                SDL_RenderCopy(renderer, blackPawnTex, NULL, &cellToUndoHighlight);
                break;
        default:
                break;

    }
    SDL_RenderPresent(renderer);
}


void Chess::renderBoard(const char boardToRender[8][8]) {
    SDL_RenderClear(renderer);

    // for (int y = 0; y < 8; y++) {
    //     for (int x = 0; x < 8; x++) {
    //         cout << boardToRender[y][x] << " ";
    //     }
    //     cout << nl;
    // } cout << nl;


    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            SDL_Rect rect;
            rect.h = 80;
            rect.w = 80;
            rect.x = 80 * x;
            rect.y = 80 * y;

            SDL_Rect texRect;
            texRect.h = 60;
            texRect.w = 60;
            texRect.x = 80 * x + 10;
            texRect.y = 80 * y + 10;

            if ((x + y) % 2) {
                //Draw Black Square
                // SDL_SetRenderDrawColor(renderer, 125, 135, 150, 255);    //Pale_Grey Color
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);           //Red Color
                SDL_RenderFillRect(renderer, &rect);
            }
            else {
                //Draw White Square
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            }

            switch (boardToRender[y][x]) {
            case 'R':
                SDL_RenderCopy(renderer, whiteRookTex, NULL, &texRect);
                break;
            case 'N':
                SDL_RenderCopy(renderer, whiteKnightTex, NULL, &texRect);
                break;
            case 'B':
                SDL_RenderCopy(renderer, whiteBishopTex, NULL, &texRect);
                break;
            case 'Q':
                SDL_RenderCopy(renderer, whiteQueenTex, NULL, &texRect);
                break;
            case 'K':
                SDL_RenderCopy(renderer, whiteKingTex, NULL, &texRect);
                break;
            case 'P':
                SDL_RenderCopy(renderer, whitePawnTex, NULL, &texRect);
                break;
            case 'r':
                SDL_RenderCopy(renderer, blackRookTex, NULL, &texRect);
                break;
            case 'n':
                SDL_RenderCopy(renderer, blackKnightTex, NULL, &texRect);
                break;
            case 'b':
                SDL_RenderCopy(renderer, blackBishopTex, NULL, &texRect);
                break;
            case 'q':
                SDL_RenderCopy(renderer, blackQueenTex, NULL, &texRect);
                break;
            case 'k':
                SDL_RenderCopy(renderer, blackKingTex, NULL, &texRect);
                break;
            case 'p':
                SDL_RenderCopy(renderer, blackPawnTex, NULL, &texRect);
                break;
            default:
                break;

            }
        }
    }
    SDL_RenderPresent(renderer);
    cout << "Render Presented" << nl;
}


bool Chess::isKingBeaten( const char currentBoard[8][8]) {
    
    bool whiteKing = false;
    bool blackKing = false;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (currentBoard[y][x] == 'K') whiteKing = true;
            else if (currentBoard[y][x] == 'k') blackKing = true;
        }
    }
    if (whiteKing && blackKing) return false;
    else return true;

}



void Chess::startGameLoop() {

    isGameRunning = true;

    Chess::getTextures();
    cout << "Initial Board Configuration..." << nl;
    Chess::renderBoard(board);

    int fromXcor = 0;
    int fromYcor = 0;
    int toXcor = 0;
    int toYcor = 0;
    bool clicked = false;

    SDL_Rect cellClicked = {0, 0, 0, 0};


    while (isGameRunning) {

        SDL_PollEvent(&event);
        
        switch (event.type) {
        
            case SDL_QUIT:
            {
                isGameRunning = false;
                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            {
                if (!clicked) {
                    fromXcor = event.button.x;
                    fromYcor = event.button.y;
                    clicked = true;
                }
                else {
                    toXcor = event.button.x;
                    toYcor = event.button.y;
                    clicked = false;
                }
            }

        }


        if (clicked) {
            cellClicked.h = 80;
            cellClicked.w = 80;
            cellClicked.x = fromXcor / 80 * 80;
            cellClicked.y = fromYcor / 80 * 80;
            Chess::renderCellHighlighter(cellClicked);

        }
        
        if (!clicked && fromXcor > 0 && fromXcor < 640 &&  fromYcor > 0 && fromYcor < 640 && toXcor > 0 && toXcor < 640 && toYcor > 0 && toYcor < 640) {
            // cout << "From " << fromXcor << "," << fromYcor << " to " << toXcor << "," << toYcor << nl;
            checkPieceMovement(cellClicked, toXcor, toYcor);
            fromXcor = 0;
            fromYcor = 0;
            toXcor = 0;
            toYcor = 0;
            Chess::undoHighlighter(cellClicked, board);
            Chess::renderBoard(board);
            if (isKingBeaten(board)) {
                if (this->getCurrentTurn() == Chess::WHITE_PIECE) {
                    cout << "BLACK WINS" << nl;
                } 
                else {
                    cout << "WHITE WINS" << nl;
                }
                //Render Winning Banner
                SDL_Delay(1500);
                isGameRunning = false;
            }
        }

        else if (!clicked && fromXcor && fromYcor && toXcor && toYcor) {
            cout << "Invalid Move" << nl;
            fromXcor = 0;
            fromYcor = 0;
            toXcor = 0;
            toYcor = 0;
            Chess::undoHighlighter(cellClicked, initial_board);
        }   
    }
}






//Imported Functions


void Chess::checkPieceMovement(SDL_Rect pieceRect, int toXcor, int toYcor)
{


    //Comsidering rows as Letters and Columns as numbers
   std::string to_record;

   // Get user input for the piece they want to move
//    cout << "Choose piece to be moved. (example: A1 or b2): ";

   std::string move_from = "00";
//    getline(cin, move_from);

    // cout << "length " << move_from.length() << nl;
    // cout << move_from << nl;

   if ( move_from.length() > 2 )
   {
      createNextMessage("You should type only two characters (column and row)\n");
      return;
   }

    move_from[0] = char(pieceRect.x / 80) + 'A';
    move_from[1] = char(pieceRect.y / 80) + '1';

   Chess::Position present;
   present.iColumn = move_from[0];
   present.iRow    = move_from[1];

   // ---------------------------------------------------
   // Did the user pick a valid piece?
   // Must check if:
   // - It's inside the board (A1-H8)
   // - There is a piece in the square
   // - The piece is consistent with the player's turn
   // ---------------------------------------------------
   present.iColumn = toupper(present.iColumn);

   if ( present.iColumn < 'A' || present.iColumn > 'H' )
   {
      createNextMessage("Invalid column.\n");
      return;
   }

   if ( present.iRow < '0' || present.iRow > '8' )
   {
      createNextMessage("Invalid row.\n");
      return;
   }

   // Put in the string to be logged
   to_record += present.iColumn;
   to_record += present.iRow;
   to_record += "-";

   // Convert column from ['A'-'H'] to [0x00-0x07]
   present.iColumn = present.iColumn - 'A';

   // Convert row from ['1'-'8'] to [0x00-0x07]
   present.iRow  = present.iRow  - '1';

   char chPiece = Chess::getPieceAtPosition(present.iRow, present.iColumn);
   cout << "Piece is " << char(chPiece) << "\n";

   if ( 0x20 == chPiece )
   {
      createNextMessage("You picked an EMPTY square.\n");
      return;
   }

   if ( Chess::WHITE_PIECE == this->getCurrentTurn() )
   {
      if ( false == Chess::isWhitePiece(chPiece) )
      {
         createNextMessage("It is WHITE's turn and you picked a BLACK piece\n");
         return;
      }
   }
   else
   {
      if ( false == Chess::isBlackPiece(chPiece) )
      {
         createNextMessage("It is BLACK's turn and you picked a WHITE piece\n");
         return;
      }
   }

   // ---------------------------------------------------
   // Get user input for the square to move to
   // ---------------------------------------------------
   cout << "Move to: ";
   std::string move_to = "00";

   move_to[0] = char(toXcor / 80) + 'A';
    move_to[1] = char(toYcor / 80) + '1';

    cout << "From " << move_from[0] << move_from[1] << " to " << move_to[0] << move_to[1] << nl;


//    getline(cin, move_to);

   if ( move_to.length() > 2 )
   {
      createNextMessage("You should type only two characters (column and row) error in move_to\n");
      return;
   }

   // ---------------------------------------------------
   // Did the user pick a valid house to move?
   // Must check if:
   // - It's inside the board (A1-H8)
   // - The move is valid
   // ---------------------------------------------------


   Chess::Position future;
   future.iColumn = move_to[0];
   future.iRow    = move_to[1];

   future.iColumn = toupper(future.iColumn);

   if ( future.iColumn < 'A' || future.iColumn > 'H' )
   {
      createNextMessage("Invalid column.\n");
      return;
   }

   if ( future.iRow < '0' || future.iRow > '8' )
   {
      createNextMessage("Invalid row.\n");
      return;
   }

   // Put in the string to be logged
   to_record += future.iColumn;
   to_record += future.iRow;

   // Convert columns from ['A'-'H'] to [0x00-0x07]
   future.iColumn = future.iColumn - 'A';

   // Convert row from ['1'-'8'] to [0x00-0x07]
   future.iRow = future.iRow - '1';

   // Check if it is not the exact same square
   if ( future.iRow == present.iRow && future.iColumn == present.iColumn )
   {
      createNextMessage("[Invalid] You picked the same square!\n");
      return;
   }

   // Is that move allowed?
   Chess::EnPassant  S_enPassant  = { 0 };
   Chess::Castling   S_castling   = { 0 };
   Chess::Promotion  S_promotion  = { 0 };

   if ( false == isMoveValid(present, future, &S_enPassant, &S_castling, &S_promotion) )
   {
      createNextMessage("[Invalid] Piece can not move to that square!\n");
      return;
   }
   
   // ---------------------------------------------------
   // Promotion: user most choose a piece to
   // replace the pawn
   // ---------------------------------------------------
   if ( S_promotion.bApplied == true )
   {
      cout << "Promote to (Q, R, N, B): ";
      std::string piece;
      getline(cin, piece);

      if ( piece.length() > 1 )
      {
         createNextMessage("You should type only one character (Q, R, N or B)\n");
         return;
      }

      char chPromoted = toupper(piece[0]);

      if ( chPromoted != 'Q' && chPromoted != 'R' && chPromoted != 'N' && chPromoted != 'B' )
      {
         createNextMessage("Invalid character.\n");
         return;
      }

      S_promotion.chBefore = this->getPieceAtPosition(present.iRow, present.iColumn);

      if (Chess::WHITE_PLAYER == this->getCurrentTurn())
      {
         S_promotion.chAfter = toupper(chPromoted);
      }
      else
      {
         S_promotion.chAfter = tolower(chPromoted);
      }

      to_record += '=';
      to_record += toupper(chPromoted); // always log with a capital letter
   }

   // ---------------------------------------------------
   // Log the move: do it prior to making the move
   // because we need the getCurrentTurn()
   // ---------------------------------------------------
   this->logMove( to_record );

   // ---------------------------------------------------
   // Make the move
   // ---------------------------------------------------
   makeTheMove(present, future, &S_enPassant, &S_castling, &S_promotion);

   // ---------------------------------------------------------------
   // Check if this move we just did put the oponent's king in check
   // Keep in mind that player turn has already changed
   // ---------------------------------------------------------------
   if ( true == this->playerKingInCheck() )
   {
      if (true == this->isCheckMate() )
      {
         if (Chess::WHITE_PLAYER == this->getCurrentTurn())
         {
            appendToNextMessage("Checkmate! Black wins the game!\n");
         }
         else
         {
            appendToNextMessage("Checkmate! White wins the game!\n");
         }
      }
      else
      { 
         // Add to the string with '+=' because it's possible that
         // there is already one message (e.g., piece captured)
         if (Chess::WHITE_PLAYER == this->getCurrentTurn())
         {
            appendToNextMessage("White king is in check!\n");
         }
         else
         {
            appendToNextMessage("Black king is in check!\n");
         }
      }
   }

   return;
}

int Chess::getPieceColor(char chPiece)
{
   if (isupper(chPiece))
   {
      return WHITE_PIECE;
   }
   else
   {
      return BLACK_PIECE;
   }
}

bool Chess::isWhitePiece(char chPiece)
{
   return getPieceColor(chPiece) == Chess::WHITE_PIECE ? true : false;
}

bool Chess::isBlackPiece(char chPiece)
{
   return getPieceColor(chPiece) == Chess::BLACK_PIECE ? true : false;
}

std::string Chess::describePiece(char chPiece)
   {
      std::string description;

      if (isWhitePiece(chPiece))
      {
         description += "White ";
      }
      else
      {
         description += "Black ";
      }

      switch (toupper(chPiece))
      {
         case 'P':
         {
            description += "pawn";
         }
         break;

         case 'N':
         {
            description += "knight";
         }
         break;

         case 'B':
         {
            description += "bishop";
         }
         break;

         case 'R':
         {
            description += "rook";
         }
         break;

         case 'Q':
         {
            description += "queen";
         }
         break;

         default:
         {
            description += "unknow piece";
         }
         break;
      }

      return description;
   }


void Chess::movePiece(Position present, Position future, Chess::EnPassant* S_enPassant, Chess::Castling* S_castling, Chess::Promotion* S_promotion)
{
   // Get the piece to be moved
   char chPiece = getPieceAtPosition(present);

   // Is the destination square occupied?
   char chCapturedPiece = getPieceAtPosition(future);

   // So, was a piece captured in this move?
   if (0x20 != chCapturedPiece)
   {
      if (WHITE_PIECE == getPieceColor(chCapturedPiece))
      {
         // A white piece was captured
         white_captured.push_back(chCapturedPiece);
      }
      else
      {
         // A black piece was captured
         black_captured.push_back(chCapturedPiece);
      }

      // Set Undo structure. If a piece was captured, then no "en passant" move performed
      m_undo.bCapturedLastMove = true;
      
      // Reset m_undo.castling
      memset( &m_undo.en_passant, 0, sizeof( Chess::EnPassant ));
   }
   else if (true == S_enPassant->bApplied)
   {
      char chCapturedEP = getPieceAtPosition(S_enPassant->PawnCaptured.iRow, S_enPassant->PawnCaptured.iColumn);

      if (WHITE_PIECE == getPieceColor(chCapturedEP))
      {
         // A white piece was captured
         white_captured.push_back(chCapturedEP);
      }
      else
      {
         // A black piece was captured
         black_captured.push_back(chCapturedEP);
      }

      // Now, remove the captured pawn
      cout << "Board Changed" << nl;
      board[S_enPassant->PawnCaptured.iRow][S_enPassant->PawnCaptured.iColumn] = EMPTY_SQUARE;

      // Set Undo structure as piece was captured and "en passant" move was performed
      m_undo.bCapturedLastMove = true;
      memcpy(&m_undo.en_passant, S_enPassant, sizeof(Chess::EnPassant));
   }
   else
   {
      m_undo.bCapturedLastMove   = false;
      
      // Reset m_undo.castling
      memset( &m_undo.en_passant, 0, sizeof( Chess::EnPassant ));
   }

   // Remove piece from present position
   cout << "Board Changed" << nl;
   board[present.iRow][present.iColumn] = EMPTY_SQUARE;

   // Move piece to new position
   if ( true == S_promotion->bApplied )
   {
      board[future.iRow][future.iColumn] = S_promotion->chAfter;

      // Set Undo structure as a promotion occured
      memcpy(&m_undo.promotion, S_promotion, sizeof(Chess::Promotion));
   }
   else
   {
      board[future.iRow][future.iColumn] = chPiece;

      // Reset m_undo.promotion
      memset( &m_undo.promotion, 0, sizeof( Chess::Promotion ));
   }  

   // Was it a castling move?
   if ( S_castling->bApplied == true  )
   {
      // The king was already move, but we still have to move the rook to 'jump' the king
      char chPiece = getPieceAtPosition(S_castling->rook_before.iRow, S_castling->rook_before.iColumn);

      // Remove the rook from present position
      board[S_castling->rook_before.iRow][S_castling->rook_before.iColumn] = EMPTY_SQUARE;

      // 'Jump' into to new position
      board[S_castling->rook_after.iRow][S_castling->rook_after.iColumn] = chPiece;

      // Write this information to the m_undo struct
      memcpy(&m_undo.castling, S_castling, sizeof(Chess::Castling));

      // Save the 'CastlingAllowed' information in case the move is undone
      m_undo.bCastlingKingSideAllowed  = m_bCastlingKingSideAllowed[getCurrentTurn()] ;
      m_undo.bCastlingQueenSideAllowed = m_bCastlingQueenSideAllowed[getCurrentTurn()];
   }
   else
   {
      // Reset m_undo.castling
      memset( &m_undo.castling, 0, sizeof( Chess::Castling ));
   }

   // Castling requirements
   if ( 'K' == toupper(chPiece) )
   {
      // After the king has moved once, no more castling allowed
      m_bCastlingKingSideAllowed[getCurrentTurn()]  = false;
      m_bCastlingQueenSideAllowed[getCurrentTurn()] = false;
   }
   else if ('R' == toupper(chPiece) )
   {
      // If the rook moved from column 'A', no more castling allowed on the queen side
      if ( 0 == present.iColumn )
      {
         m_bCastlingQueenSideAllowed[getCurrentTurn()] = false;
      }

      // If the rook moved from column 'A', no more castling allowed on the queen side
      else if ( 7 == present.iColumn )
      {
         m_bCastlingKingSideAllowed[getCurrentTurn()] = false;
      }
   }

   // Change turns
   changeTurns();

   // This move can be undone
   m_undo.bCanUndo = true;
}

void Chess::undoLastMove()
{
   string last_move = getLastMove();

   // Parse the line
   Chess::Position from;
   Chess::Position to;
   parseMove(last_move, &from, &to);

   // Since we want to undo a move, we will be moving the piece from (iToRow, iToColumn) to (iFromRow, iFromColumn)
   char chPiece = getPieceAtPosition(to.iRow, to.iColumn);

   // Moving it back
   // If there was a castling
   if ( true == m_undo.promotion.bApplied )
   {
      board[from.iRow][from.iColumn] = m_undo.promotion.chBefore;
   }
   else
   {
      board[from.iRow][from.iColumn] = chPiece;
   }

   // Change turns
   changeTurns();

   // If a piece was captured, move it back to the board
   if (m_undo.bCapturedLastMove)
   {
      // Let's retrieve the last captured piece
      char chCaptured;

      // Since we already changed turns back, it means we should we pop a piece from the oponents vector
      if (WHITE_PLAYER == m_CurrentTurn)
      {
         chCaptured = black_captured.back();
         black_captured.pop_back();
      }
      else
      {
         chCaptured = white_captured.back();
         white_captured.pop_back();
      }

      // Move the captured piece back. Was this an "en passant" move?
      if (m_undo.en_passant.bApplied)
      {
         // Move the captured piece back
         board[m_undo.en_passant.PawnCaptured.iRow][m_undo.en_passant.PawnCaptured.iColumn] = chCaptured;

         // Remove the attacker
         board[to.iRow][to.iColumn] = EMPTY_SQUARE;
      }
      else
      {
         board[to.iRow][to.iColumn] = chCaptured;
      }
   }
   else
   {
      board[to.iRow][to.iColumn] = EMPTY_SQUARE;
   }

   // If there was a castling
   if ( m_undo.castling.bApplied )
   {
      char chRook = getPieceAtPosition(m_undo.castling.rook_after.iRow, m_undo.castling.rook_after.iColumn);

      // Remove the rook from present position
      board[m_undo.castling.rook_after.iRow][m_undo.castling.rook_after.iColumn] = EMPTY_SQUARE;

      // 'Jump' into to new position
      board[m_undo.castling.rook_before.iRow][m_undo.castling.rook_before.iColumn] = chRook;

      // Restore the values of castling allowed or not
      m_bCastlingKingSideAllowed[getCurrentTurn()]  = m_undo.bCastlingKingSideAllowed;
      m_bCastlingQueenSideAllowed[getCurrentTurn()] = m_undo.bCastlingQueenSideAllowed;
   }

   // Clean m_undo struct
   m_undo.bCanUndo             = false;
   m_undo.bCapturedLastMove    = false;
   m_undo.en_passant.bApplied  = false;
   m_undo.castling.bApplied    = false;
   m_undo.promotion.bApplied   = false;

   // If it was a checkmate, toggle back to game not finished
   m_bGameFinished = false;

   // Finally, remove the last move from the list
   deleteLastMove();
}

bool Chess::undoIsPossible()
{
   return m_undo.bCanUndo;
}

bool Chess::castlingAllowed(Side iSide, int iColor)
{
   if ( QUEEN_SIDE == iSide )
   {
      return m_bCastlingQueenSideAllowed[iColor];
   }
   else //if ( KING_SIDE == iSide )
   {
      return m_bCastlingKingSideAllowed[iColor];
   }
}

char Chess::getPieceAtPosition(int iRow, int iColumn)
{
   return board[iRow][iColumn];
}

char Chess::getPieceAtPosition(Position pos)
{
   return board[pos.iRow][pos.iColumn];
}

char Chess::getPiece_considerMove(int iRow, int iColumn, IntendedMove* intended_move)
{
   char chPiece;

   // If there is no intended move, just return the current position of the board
   if (nullptr == intended_move)
   {
      chPiece = getPieceAtPosition(iRow, iColumn);
   }
   else
   {
      // In this case, we are trying to understand what WOULD happed if the move was made,
      // so we consider a move that has not been made yet
      if (intended_move->from.iRow == iRow && intended_move->from.iColumn == iColumn)
      {
         // The piece wants to move from that square, so it would be empty
         chPiece = EMPTY_SQUARE;
      }
      else if (intended_move->to.iRow == iRow && intended_move->to.iColumn == iColumn)
      {
         // The piece wants to move to that square, so return the piece
         chPiece = intended_move->chPiece;
      }
      else
      {
         chPiece = getPieceAtPosition(iRow, iColumn);
      }
   }

   return chPiece;
}

Chess::UnderAttack Chess::isUnderAttack(int iRow, int iColumn, int iColor, IntendedMove* pintended_move)
{
   UnderAttack attack = { 0 };

   // a) Direction: HORIZONTAL
   {
      // Check all the way to the right
      for (int i = iColumn + 1; i < 8; i++)
      {
         char chPieceFound = getPiece_considerMove(iRow, i, pintended_move);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if ( iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color, so no problem
            break;
         }
         else if ( (toupper(chPieceFound) == 'Q') ||
                   (toupper(chPieceFound) == 'R') )
         {
            // There is a queen or a rook to the right, so the piece is in jeopardy
            attack.bUnderAttack = true;
            attack.iNumAttackers += 1;

            attack.attacker[attack.iNumAttackers-1].pos.iRow = iRow;
            attack.attacker[attack.iNumAttackers-1].pos.iColumn = i;
            attack.attacker[attack.iNumAttackers-1].dir = HORIZONTAL;
            break;
         }
         else
         {
            // There is a piece that does not attack horizontally, so no problem
            break;
         }
      }

      // Check all the way to the left
      for (int i = iColumn - 1; i >= 0; i--)
      {
         char chPieceFound = getPiece_considerMove(iRow, i, pintended_move);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if  (iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color, so no problem
            break;
         }
         else if ( (toupper(chPieceFound) == 'Q') ||
                   (toupper(chPieceFound) == 'R') )
         {
            // There is a queen or a rook to the right, so the piece is in jeopardy
            attack.bUnderAttack = true;
            attack.iNumAttackers += 1;

            attack.attacker[attack.iNumAttackers-1].pos.iRow    = iRow;
            attack.attacker[attack.iNumAttackers-1].pos.iColumn = i;
            attack.attacker[attack.iNumAttackers-1].dir = HORIZONTAL;
            break;
         }
         else
         {
            // There is a piece that does not attack horizontally, so no problem
            break;
         }
      }
   }

   // b) Direction: VERTICAL
   {
      // Check all the way up
      for (int i = iRow + 1; i < 8; i++)
      {
         char chPieceFound = getPiece_considerMove(i, iColumn, pintended_move);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if ( iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color, so no problem
            break;
         }
         else if ( (toupper(chPieceFound) == 'Q') ||
                   (toupper(chPieceFound) == 'R')  )
         {
            // There is a queen or a rook to the right, so the piece is in jeopardy
            attack.bUnderAttack = true;
            attack.iNumAttackers += 1;

            attack.attacker[attack.iNumAttackers-1].pos.iRow    = i;
            attack.attacker[attack.iNumAttackers-1].pos.iColumn = iColumn;
            attack.attacker[attack.iNumAttackers-1].dir = VERTICAL;
            break;
         }
         else
         {
            // There is a piece that does not attack vertically, so no problem
            break;
         }
      }

      // Check all the way down
      for (int i = iRow - 1; i >= 0; i--)
      {
         char chPieceFound = getPiece_considerMove(i, iColumn, pintended_move);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if ( iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color, so no problem
            break;
         }
         else if ((toupper(chPieceFound) == 'Q') ||
                  (toupper(chPieceFound) == 'R') )
         {
            // There is a queen or a rook to the right, so the piece is in jeopardy
            attack.bUnderAttack = true;
            attack.iNumAttackers += 1;

            attack.attacker[attack.iNumAttackers-1].pos.iRow    = i;
            attack.attacker[attack.iNumAttackers-1].pos.iColumn = iColumn;
            attack.attacker[attack.iNumAttackers-1].dir = VERTICAL;
            break;
         }
         else
         {
            // There is a piece that does not attack vertically, so no problem
            break;
         }
      }
   }

   // c) Direction: DIAGONAL
   {
      // Check the diagonal up-right
      for (int i = iRow + 1, j = iColumn + 1; i < 8 && j < 8; i++, j++)
      {
         char chPieceFound = getPiece_considerMove(i, j, pintended_move);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if ( iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color, so no problem
            break;
         }
         else if ( (toupper(chPieceFound) == 'P' ) &&
                   (   i   == iRow    + 1        ) &&
                   (   j   == iColumn + 1        ) &&
                   (iColor == WHITE_PIECE ) )
         {
            // A pawn only puts another piece in jeopardy if it's (diagonally) right next to it
            attack.bUnderAttack = true;
            attack.iNumAttackers += 1;

            attack.attacker[attack.iNumAttackers-1].pos.iRow    = i;
            attack.attacker[attack.iNumAttackers-1].pos.iColumn = j;
            attack.attacker[attack.iNumAttackers-1].dir = DIAGONAL;
            break;
         }
         else if ( (toupper(chPieceFound) == 'Q') ||
                   (toupper(chPieceFound) == 'B'))
         {
            // There is a queen or a bishop in that direction, so the piece is in jeopardy
            attack.bUnderAttack = true;
            attack.iNumAttackers += 1;

            attack.attacker[attack.iNumAttackers-1].pos.iRow    = i;
            attack.attacker[attack.iNumAttackers-1].pos.iColumn = j;
            attack.attacker[attack.iNumAttackers-1].dir = DIAGONAL;
            break;
         }
         else
         {
            // There is a piece that does not attack diagonally, so no problem
            break;
         }
      }

      // Check the diagonal up-left
      for (int i = iRow + 1, j = iColumn - 1; i < 8 && j > 0; i++, j--)
      {
         char chPieceFound = getPiece_considerMove(i, j, pintended_move);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if ( iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color, so no problem
            break;
         }
         else if ( (toupper(chPieceFound) == 'P' ) &&
                   (   i   == iRow    + 1        ) &&
                   (   j   == iColumn - 1        ) &&
                   (iColor == WHITE_PIECE ) )
         {
            // A pawn only puts another piece in jeopardy if it's (diagonally) right next to it
            attack.bUnderAttack = true;
            attack.iNumAttackers += 1;

            attack.attacker[attack.iNumAttackers-1].pos.iRow    = i;
            attack.attacker[attack.iNumAttackers-1].pos.iColumn = j;
            attack.attacker[attack.iNumAttackers-1].dir = DIAGONAL;
            break;
         }
         else if ( (toupper(chPieceFound) == 'Q') ||
                   (toupper(chPieceFound) == 'B'))
         {
            // There is a queen or a bishop in that direction, so the piece is in jeopardy
            attack.bUnderAttack = true;
            attack.iNumAttackers += 1;

            attack.attacker[attack.iNumAttackers-1].pos.iRow    = i;
            attack.attacker[attack.iNumAttackers-1].pos.iColumn = j;
            attack.attacker[attack.iNumAttackers-1].dir = DIAGONAL;
            break;
         }
         else
         {
            // There is a piece that does not attack diagonally, so no problem
            break;
         }
      }

      // Check the diagonal down-right
      for (int i = iRow - 1, j = iColumn + 1; i > 0 && j < 8; i--, j++)
      {
         char chPieceFound = getPiece_considerMove(i, j, pintended_move);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if ( iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color, so no problem
            break;
         }
         else if ( (toupper(chPieceFound) == 'P' ) &&
                   (   i   == iRow    - 1        ) &&
                   (   j   == iColumn + 1        ) &&
                   (iColor == BLACK_PIECE ) )
         {
            // A pawn only puts another piece in jeopardy if it's (diagonally) right next to it
            attack.bUnderAttack = true;
            attack.iNumAttackers += 1;

            attack.attacker[attack.iNumAttackers-1].pos.iRow    = i;
            attack.attacker[attack.iNumAttackers-1].pos.iColumn = j;
            attack.attacker[attack.iNumAttackers-1].dir = DIAGONAL;
            break;
         }
         else if ( (toupper(chPieceFound) == 'Q') ||
                   (toupper(chPieceFound) == 'B') )
         {
            // There is a queen or a bishop in that direction, so the piece is in jeopardy
            attack.bUnderAttack = true;
            attack.iNumAttackers += 1;

            attack.attacker[attack.iNumAttackers-1].pos.iRow    = i;
            attack.attacker[attack.iNumAttackers-1].pos.iColumn = j;
            attack.attacker[attack.iNumAttackers-1].dir = DIAGONAL;
            break;
         }
         else
         {
            // There is a piece that does not attack diagonally, so no problem
            break;
         }
      }

      // Check the diagonal down-left
      for (int i = iRow - 1, j = iColumn - 1; i > 0 && j > 0; i--, j--)
      {
         char chPieceFound = getPiece_considerMove(i, j, pintended_move);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if ( iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color, so no problem
            break;
         }
         else if ( (toupper(chPieceFound) == 'P' ) &&
                   (   i   == iRow    - 1        ) &&
                   (   j   == iColumn - 1        ) &&
                   (iColor == BLACK_PIECE ) )
         {
            // A pawn only puts another piece in jeopardy if it's (diagonally) right next to it
            attack.bUnderAttack = true;
            attack.iNumAttackers += 1;

            attack.attacker[attack.iNumAttackers-1].pos.iRow    = i;
            attack.attacker[attack.iNumAttackers-1].pos.iColumn = j;
            attack.attacker[attack.iNumAttackers-1].dir = DIAGONAL;
            break;
         }
         else if ( (toupper(chPieceFound) == 'Q') ||
                     (toupper(chPieceFound) == 'B') )
         {
            // There is a queen or a bishop in that direction, so the piece is in jeopardy
            attack.bUnderAttack = true;
            attack.iNumAttackers += 1;

            attack.attacker[attack.iNumAttackers-1].pos.iRow    = i;
            attack.attacker[attack.iNumAttackers-1].pos.iColumn = j;
            attack.attacker[attack.iNumAttackers-1].dir = DIAGONAL;
            break;
         }
         else
         {
            // There is a piece that does not attack diagonally, so no problem
            break;
         }
      }
   }

   // d) Direction: L_SHAPED
   {
      // Check if the piece is put in jeopardy by a knigh

      Position knight_moves[8] = { {  1, -2 }, {  2, -1 }, {  2, 1 }, {  1, 2 },
                                   { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };
      for (int i = 0; i < 8; i++)
      {
         int iRowToTest    = iRow    + knight_moves[i].iRow;
         int iColumnToTest = iColumn + knight_moves[i].iColumn;

         if (iRowToTest < 0 || iRowToTest > 7 || iColumnToTest < 0 || iColumnToTest > 7)
         {
            // This square does not even exist, so no need to test
            continue;
         }

         char chPieceFound = getPiece_considerMove(iRowToTest, iColumnToTest, pintended_move);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if (iColor == getPieceColor(chPieceFound))
         {
            // This is a piece of the same color, so no problem
            continue;
         }
         else if ( (toupper(chPieceFound) == 'N') )
         {
            attack.bUnderAttack = true;
            attack.iNumAttackers += 1;

            attack.attacker[attack.iNumAttackers-1].pos.iRow    = iRowToTest;
            attack.attacker[attack.iNumAttackers-1].pos.iColumn = iColumnToTest;
            break;
         }
      }
   }

   return attack;
}

bool Chess::isReachable( int iRow, int iColumn, int iColor )
{
   bool bReachable = false;

   // a) Direction: HORIZONTAL
   {
      // Check all the way to the right
      for (int i = iColumn + 1; i < 8; i++)
      {
         char chPieceFound = getPieceAtPosition(iRow, i);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if ( iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color
            break;
         }
         else if ( (toupper(chPieceFound) == 'Q') ||
                   (toupper(chPieceFound) == 'R') )
         {
            // There is a queen or a rook to the right, so the square is reachable
            bReachable = true;
            break;
         }
         else
         {
            // There is a piece that does not move horizontally
            break;
         }
      }

      // Check all the way to the left
      for (int i = iColumn - 1; i >= 0; i--)
      {
         char chPieceFound = getPieceAtPosition(iRow, i);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if  (iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color
            break;
         }
         else if ( (toupper(chPieceFound) == 'Q') ||
                   (toupper(chPieceFound) == 'R') )
         {
            // There is a queen or a rook to the left, so the square is reachable
            bReachable = true;
            break;
         }
         else
         {
            // There is a piece that does not move horizontally
            break;
         }
      }
   }

   // b) Direction: VERTICAL
   {
      // Check all the way up
      for (int i = iRow + 1; i < 8; i++)
      {
         char chPieceFound = getPieceAtPosition(i, iColumn);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if ( iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color
            break;
         }
         else if ( (toupper(chPieceFound)       == 'P' )         &&
                   (getPieceColor(chPieceFound) == BLACK_PIECE ) &&
                   ( i  == iRow + 1 )                            )  
         {
            // There is a pawn one square up, so the square is reachable
            bReachable = true;
            break;
         }
         else if ( (toupper(chPieceFound) == 'Q') ||
                   (toupper(chPieceFound) == 'R')  )
         {
            // There is a queen or a rook on the way up, so the square is reachable
            bReachable = true;
            break;
         }
         else
         {
            // There is a piece that does not move vertically
            break;
         }
      }

      // Check all the way down
      for (int i = iRow - 1; i >= 0; i--)
      {
         char chPieceFound = getPieceAtPosition(i, iColumn);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if ( iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color
            break;
         }
         else if ( (toupper(chPieceFound)       == 'P' )         &&
                   (getPieceColor(chPieceFound) == WHITE_PIECE ) &&
                   ( i  == iRow - 1 )                            )  
         {
            // There is a pawn one square down, so the square is reachable
            bReachable = true;
            break;
         }
         else if ((toupper(chPieceFound) == 'Q') ||
                  (toupper(chPieceFound) == 'R') )
         {
            // There is a queen or a rook on the way down, so the square is reachable
            bReachable = true;
            break;
         }
         else
         {
            // There is a piece that does not move vertically
            break;
         }
      }
   }

   // c) Direction: DIAGONAL
   {
      // Check the diagonal up-right
      for (int i = iRow + 1, j = iColumn + 1; i < 8 && j < 8; i++, j++)
      {
         char chPieceFound = getPieceAtPosition(i, j);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if ( iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color
            break;
         }
         else if ( (toupper(chPieceFound) == 'Q') ||
                   (toupper(chPieceFound) == 'B'))
         {
            // There is a queen or a bishop in that direction, so the square is reachable
            bReachable = true;
            break;
         }
         else
         {
            // There is a piece that does not move diagonally
            break;
         }
      }

      // Check the diagonal up-left
      for (int i = iRow + 1, j = iColumn - 1; i < 8 && j > 0; i++, j--)
      {
         char chPieceFound = getPieceAtPosition(i, j);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if ( iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color
            break;
         }
         else if ( (toupper(chPieceFound) == 'Q') ||
                   (toupper(chPieceFound) == 'B'))
         {
            // There is a queen or a bishop in that direction, so the square is reachable
            bReachable = true;
            break;
         }
         else
         {
            // There is a piece that does not move diagonally
            break;
         }
      }

      // Check the diagonal down-right
      for (int i = iRow - 1, j = iColumn + 1; i > 0 && j < 8; i--, j++)
      {
         char chPieceFound = getPieceAtPosition(i, j);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if ( iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color
            break;
         }
         else if ( (toupper(chPieceFound) == 'Q') ||
                   (toupper(chPieceFound) == 'B') )
         {
            // There is a queen or a bishop in that direction, so the square is reachable
            bReachable = true;
            break;
         }
         else
         {
            // There is a piece that does not move diagonally
            break;
         }
      }

      // Check the diagonal down-left
      for (int i = iRow - 1, j = iColumn - 1; i > 0 && j > 0; i--, j--)
      {
         char chPieceFound = getPieceAtPosition(i, j);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if ( iColor == getPieceColor(chPieceFound) )
         {
            // This is a piece of the same color
            break;
         }
         else if ( (toupper(chPieceFound) == 'Q') ||
                   (toupper(chPieceFound) == 'B') )
         {
            // There is a queen or a bishop in that direction, so the square is reachable
            bReachable = true;
            break;
         }
         else
         {
            // There is a piece that does not move diagonally
            break;
         }
      }
   }

   // d) Direction: L_SHAPED
   {
      // Check if the piece is put in jeopardy by a knigh

      Position knight_moves[8] = { {  1, -2 }, {  2, -1 }, {  2, 1 }, {  1, 2 },
                                   { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };
      for (int i = 0; i < 8; i++)
      {
         int iRowToTest    = iRow    + knight_moves[i].iRow;
         int iColumnToTest = iColumn + knight_moves[i].iColumn;

         if (iRowToTest < 0 || iRowToTest > 7 || iColumnToTest < 0 || iColumnToTest > 7)
         {
            // This square does not even exist, so no need to test
            continue;
         }

         char chPieceFound = getPieceAtPosition(iRowToTest, iColumnToTest);
         if (EMPTY_SQUARE == chPieceFound)
         {
            // This square is empty, move on
            continue;
         }

         if (iColor == getPieceColor(chPieceFound))
         {
            // This is a piece of the same color
            continue;
         }
         else if ( (toupper(chPieceFound) == 'N') )
         {
            bReachable = true;
            break;
         }
      }
   }

   return bReachable;
}

bool Chess::isSquareOccupied(int iRow, int iColumn)
{
   bool bOccupied = false;

   if ( 0x20 != getPieceAtPosition(iRow,iColumn) )
   {
      bOccupied = true;
   }

   return bOccupied;
}

bool Chess::isPathFree(Position startingPos, Position finishingPos, int iDirection)
{
   bool bFree = false;

   switch(iDirection)
   {
      case Chess::HORIZONTAL:
      {
         // If it is a horizontal move, we can assume the startingPos.iRow == finishingPos.iRow
         // If the piece wants to move from column 0 to column 7, we must check if columns 1-6 are free
         if (startingPos.iColumn == finishingPos.iColumn)
         {
            cout << "Error. Movement is horizontal but column is the same\n";
         }

         // Moving to the right
         else if (startingPos.iColumn < finishingPos.iColumn)
         {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;
            
            for (int i = startingPos.iColumn + 1; i < finishingPos.iColumn; i++)
            {
               if (isSquareOccupied(startingPos.iRow, i))
               {
                  bFree = false;
                  cout << "Horizontal path to the right is not clear!\n";
               }
            }
         }

         // Moving to the left
         else //if (startingPos.iColumn > finishingPos.iColumn)
         {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;
            
            for (int i = startingPos.iColumn - 1; i > finishingPos.iColumn; i--)
            {
               if (isSquareOccupied(startingPos.iRow, i))
               {
                  bFree = false;
                  cout << "Horizontal path to the left is not clear!\n";
               }
            }
         }
      }
      break;

      case Chess::VERTICAL:
      {
         // If it is a vertical move, we can assume the startingPos.iColumn == finishingPos.iColumn
         // If the piece wants to move from column 0 to column 7, we must check if columns 1-6 are free
         if (startingPos.iRow == finishingPos.iRow)
         {
            cout << "Error. Movement is vertical but row is the same\n";
           throw("Error. Movement is vertical but row is the same");
         }

         // Moving up
         else if (startingPos.iRow < finishingPos.iRow)
         {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;
            
            for (int i = startingPos.iRow + 1; i < finishingPos.iRow; i++)
            {
               if ( isSquareOccupied(i, startingPos.iColumn) )
               {
                  bFree = false;
                  cout << "Vertical path up is not clear!\n";
               }
            }
         }

         // Moving down
         else //if (startingPos.iColumn > finishingPos.iRow)
         {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;
            
            for (int i = startingPos.iRow - 1; i > finishingPos.iRow; i--)
            {
               if ( isSquareOccupied(i, startingPos.iColumn) )
               {
                  bFree = false;
                  cout << "Vertical path down is not clear!\n";
               }
            }
         }
      }
      break;

      case Chess::DIAGONAL:
      {
         // Moving up and right
         if ( (finishingPos.iRow > startingPos.iRow) && (finishingPos.iColumn > startingPos.iColumn) )
         {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;

            for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
            {
               if (isSquareOccupied(startingPos.iRow + i, startingPos.iColumn + i))
               {
                  bFree = false;
                  cout << "Diagonal path up-right is not clear!\n";
               }
            }
         }

         // Moving up and left
         else if ( (finishingPos.iRow > startingPos.iRow) && (finishingPos.iColumn < startingPos.iColumn) )
         {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;
            
            for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
            {
               if (isSquareOccupied(startingPos.iRow+i, startingPos.iColumn-i))
               {
                  bFree = false;
                  cout << "Diagonal path up-left is not clear!\n";
               }
            }
         }

         // Moving down and right
         else if ( (finishingPos.iRow < startingPos.iRow) && (finishingPos.iColumn > startingPos.iColumn) )
         {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;
            
            for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
            {
               if (isSquareOccupied(startingPos.iRow - i, startingPos.iColumn + i))
               {
                  bFree = false;
                  cout << "Diagonal path down-right is not clear!\n";
               }
            }
         }

         // Moving down and left
         else if ( (finishingPos.iRow < startingPos.iRow) && (finishingPos.iColumn < startingPos.iColumn) )
         {
            // Settting bFree as initially true, only inside the cases, guarantees that the path is checked
            bFree = true;
            
            for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
            {
               if (isSquareOccupied(startingPos.iRow - i, startingPos.iColumn - i))
               {
                  bFree = false;
                  cout << "Diagonal path down-left is not clear!\n";
               }
            }
         }

         else
         {
            throw("Error. Diagonal move not allowed");
         }
      }
      break;
   }

   return bFree;
}

bool Chess::canBeBlocked(Position startingPos, Position finishingPos, int iDirection)
{
   bool bBlocked = false;
   
   Chess::UnderAttack blocker = {0};

   switch(iDirection)
   {
      case Chess::HORIZONTAL:
      {
         // If it is a horizontal move, we can assume the startingPos.iRow == finishingPos.iRow
         // If the piece wants to move from column 0 to column 7, we must check if columns 1-6 are free
         if (startingPos.iColumn == finishingPos.iColumn)
         {
            cout << "Error. Movement is horizontal but column is the same\n";
         }

         // Moving to the right
         else if (startingPos.iColumn < finishingPos.iColumn)
         {
            for (int i = startingPos.iColumn + 1; i < finishingPos.iColumn; i++)
            {
               if ( true == isReachable( startingPos.iRow, i, getOpponentColor() ) )
               {
                  // Some piece can block the way
                  bBlocked = true;
               }
            }
         }

         // Moving to the left
         else //if (startingPos.iColumn > finishingPos.iColumn)
         {
            for (int i = startingPos.iColumn - 1; i > finishingPos.iColumn; i--)
            {
               if ( true == isReachable( startingPos.iRow, i, getOpponentColor() ) )
               {
                  // Some piece can block the way
                  bBlocked = true;
               }
            }
         }
      }
      break;

      case Chess::VERTICAL:
      {
         // If it is a vertical move, we can assume the startingPos.iColumn == finishingPos.iColumn
         // If the piece wants to move from column 0 to column 7, we must check if columns 1-6 are free
         if (startingPos.iRow == finishingPos.iRow)
         {
            cout << "Error. Movement is vertical but row is the same\n";
           throw("Error. Movement is vertical but row is the same");
         }

         // Moving up
         else if (startingPos.iRow < finishingPos.iRow)
         {
            for (int i = startingPos.iRow + 1; i < finishingPos.iRow; i++)
            {
               if ( true == isReachable( i, startingPos.iColumn, getOpponentColor() ) )
               {
                  // Some piece can block the way
                  bBlocked = true;
               }
            }
         }

         // Moving down
         else //if (startingPos.iColumn > finishingPos.iRow)
         {
            for (int i = startingPos.iRow - 1; i > finishingPos.iRow; i--)
            {
               if ( true == isReachable( i, startingPos.iColumn, getOpponentColor() ) )
               {
                  // Some piece can block the way
                  bBlocked = true;
               }
            }
         }
      }
      break;

      case Chess::DIAGONAL:
      {
         // Moving up and right
         if ( (finishingPos.iRow > startingPos.iRow) && (finishingPos.iColumn > startingPos.iColumn) )
         {
            for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
            {
               if ( true == isReachable( startingPos.iRow + i, startingPos.iColumn + i, getOpponentColor() ) )
               {
                  // Some piece can block the way
                  bBlocked = true;
               }
            }
         }

         // Moving up and left
         else if ( (finishingPos.iRow > startingPos.iRow) && (finishingPos.iColumn < startingPos.iColumn) )
         {
            for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
            {
               if ( true == isReachable( startingPos.iRow + i, startingPos.iColumn - i, getOpponentColor() ) )
               {
                  // Some piece can block the way
                  bBlocked = true;
               }
            }
         }

         // Moving down and right
         else if ( (finishingPos.iRow < startingPos.iRow) && (finishingPos.iColumn > startingPos.iColumn) )
         {
            for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
            {
               if ( true == isReachable( startingPos.iRow - i, startingPos.iColumn + i, getOpponentColor() ) )
               {
                  // Some piece can block the way
                  bBlocked = true;
               }
            }
         }

         // Moving down and left
         else if ( (finishingPos.iRow < startingPos.iRow) && (finishingPos.iColumn < startingPos.iColumn) )
         {
            for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
            {
               if ( true == isReachable( startingPos.iRow - i, startingPos.iColumn - i, getOpponentColor() ) )
               {
                  // Some piece can block the way
                  bBlocked = true;
               }
            }
         }

         else
         {
            cout << "Error. Diagonal move not allowed\n";
            throw("Error. Diagonal move not allowed");
         }
      }
      break;
   }

   return bBlocked;
}

bool Chess::isCheckMate()
{
   bool bCheckmate = false;

   // 1. First of all, it the king in check?
   if ( false == playerKingInCheck() )
   {
      return false;
   }

   // 2. Can the king move the other square?
   Chess::Position king_moves[8]  = { {  1, -1 },{  1, 0 },{  1,  1 }, { 0,  1 },
                                      { -1,  1 },{ -1, 0 },{ -1, -1 }, { 0, -1 } };

   Chess::Position king = findKing(getCurrentTurn() );

   for (int i = 0; i < 8; i++)
   {
      int iRowToTest    = king.iRow    + king_moves[i].iRow;
      int iColumnToTest = king.iColumn + king_moves[i].iColumn;

      if ( iRowToTest < 0 || iRowToTest > 7 || iColumnToTest < 0 || iColumnToTest > 7 )
      {
         // This square does not even exist, so no need to test
         continue;
      }

      if ( EMPTY_SQUARE != getPieceAtPosition(iRowToTest, iColumnToTest) )
      {
         // That square is not empty, so no need to test
         continue;
      }

      Chess::IntendedMove intended_move;
      intended_move.chPiece      = getPieceAtPosition(king.iRow, king.iColumn);
      intended_move.from.iRow    = king.iRow;
      intended_move.from.iColumn = king.iColumn;
      intended_move.to.iRow      = iRowToTest;
      intended_move.to.iColumn   = iColumnToTest;

      // Now, for every possible move of the king, check if it would be in jeopardy
      // Since the move has already been made, this->getCurrentTurn() now will return 
      // the next player's color. And it is in fact this king that we want to check for jeopardy
      Chess::UnderAttack king_moved = isUnderAttack( iRowToTest, iColumnToTest, getCurrentTurn(), &intended_move );

      if ( false == king_moved.bUnderAttack )
      {
         // This means there is at least one position when the king would not be in jeopardy, so that's not a checkmate
         return false;
      }
   }

   // 3. Can the attacker be taken or another piece get into the way? 
   Chess::UnderAttack king_attacked = isUnderAttack( king.iRow, king.iColumn, getCurrentTurn() );
   if ( 1 == king_attacked.iNumAttackers )
   {
      // Can the attacker be taken?
      Chess::UnderAttack king_attacker = { 0 };
      king_attacker = isUnderAttack( king_attacked.attacker[0].pos.iRow, king_attacked.attacker[0].pos.iColumn, getOpponentColor() );

      if ( true == king_attacker.bUnderAttack )
      {
         // This means that the attacker can be taken, so it's not a checkmate
         return false;
      }
      else
      {
         // Last resort: can any piece get in between the attacker and the king?
         char chAttacker = getPieceAtPosition( king_attacked.attacker[0].pos.iRow, king_attacked.attacker[0].pos.iColumn );

         switch( toupper(chAttacker) )
         {
            case 'P':
            case 'N':
            {
               // If it's a pawn, there's no space in between the attacker and the king
               // If it's a knight, it doesn't matter because the knight can 'jump'
               // So, this is checkmate
               bCheckmate = true;
            }
            break;

            case 'B':
            {
               if ( false == canBeBlocked(king_attacked.attacker[0].pos, king, Chess::DIAGONAL ) )
               {
                  // If no piece can get in the way, it's a checkmate
                  bCheckmate = true;
               }
            }
            break;

            case 'R':
            {
               if ( false == canBeBlocked(king_attacked.attacker[0].pos, king, king_attacked.attacker[0].dir ) )
               {
                  // If no piece can get in the way, it's a checkmate
                  bCheckmate = true;
               }
            }
            break;

            case 'Q':
            {
               if ( false == canBeBlocked(king_attacked.attacker[0].pos, king, king_attacked.attacker[0].dir ) )
               {
                  // If no piece can get in the way, it's a checkmate
                  bCheckmate = true;
               }
            }
            break;


            default:
            {
               throw("!!!!Should not reach here. Invalid piece");
            }
            break;
         }
      }
   }
   else
   {
      // If there is more than one attacker and we reached this point, it's a checkmate
      bCheckmate      = true;
   }

   // If the game has ended, store in the class variable
   m_bGameFinished = bCheckmate;

   return bCheckmate;
}

bool Chess::isKingInCheck(int iColor, IntendedMove* pintended_move)
{
   bool bCheck = false;

   Position king = { 0 };
   
   // Must check if the intended move is to move the king itself
   if ( nullptr != pintended_move && 'K' == toupper( pintended_move->chPiece) )
   {
      king.iRow    = pintended_move->to.iRow;
      king.iColumn = pintended_move->to.iColumn;
   }
   else
   {
      king = findKing( iColor );
   }
   
   UnderAttack king_attacked = isUnderAttack( king.iRow, king.iColumn, iColor, pintended_move );

   if ( true == king_attacked.bUnderAttack )
   {
      bCheck = true;
   }

   return bCheck;
}

bool Chess::playerKingInCheck(IntendedMove* intended_move)
{
   return isKingInCheck( getCurrentTurn(), intended_move);
}

bool Chess::wouldKingBeInCheck(char chPiece, Position present, Position future, EnPassant* S_enPassant)
{
   IntendedMove intended_move;

   intended_move.chPiece      = chPiece;
   intended_move.from.iRow    = present.iRow;
   intended_move.from.iColumn = present.iColumn;
   intended_move.to.iRow      = future.iRow;
   intended_move.to.iColumn   = future.iColumn;

   return playerKingInCheck(&intended_move);
}

Chess::Position Chess::findKing(int iColor)
{
   char chToLook = (WHITE_PIECE == iColor) ? 'K': 'k';
   Position king = { 0 };

   for (int i = 0; i < 8; i++)
   {
      for (int j = 0; j < 8; j++)
      {
         if ( chToLook == getPieceAtPosition(i, j) )
         {
            king.iRow    = i;
            king.iColumn = j;
         }
      }
   }

   return king;
}

void Chess::changeTurns(void)
{
   if (WHITE_PLAYER == m_CurrentTurn)
   {
      m_CurrentTurn = BLACK_PLAYER;
   }
   else
   {
      m_CurrentTurn = WHITE_PLAYER;
   }
}

bool Chess::isFinished( void )
{
   return m_bGameFinished;
}

int Chess::getCurrentTurn(void)
{
   return m_CurrentTurn;
}

int Chess::getOpponentColor(void)
{
   int iColor;

   if (Chess::WHITE_PLAYER == getCurrentTurn())
   {
      iColor = Chess::BLACK_PLAYER;
   }
   else
   {
      iColor = Chess::WHITE_PLAYER;
   }

   return iColor;
}

void Chess::parseMove(string move, Position* pFrom, Position* pTo, char* chPromoted)
{
   pFrom->iColumn = move[0];
   pFrom->iRow    = move[1];
   pTo->iColumn   = move[3];
   pTo->iRow      = move[4];

   // Convert columns from ['A'-'H'] to [0x00-0x07]
   pFrom->iColumn = pFrom->iColumn - 'A';
   pTo->iColumn   = pTo->iColumn   - 'A';

   // Convert row from ['1'-'8'] to [0x00-0x07]
   pFrom->iRow  = pFrom->iRow  - '1';
   pTo->iRow    = pTo->iRow    - '1';

   if ( chPromoted != nullptr )
   {
      if ( move[5] == '=' )
      {
         *chPromoted = move[6];
      }
      else
      {
         *chPromoted = EMPTY_SQUARE;
      }
   }
}

void Chess::logMove(std::string &to_record)
{
   // If record contains only 5 chracters, add spaces
   // Because when 
   if ( to_record.length() == 5 )
   {
      to_record += "  ";
   }

   if ( WHITE_PLAYER == getCurrentTurn() )
   {
      // If this was a white player move, create a new round and leave the black_move empty
      Round round;
      round.white_move = to_record;
      round.black_move = "";

      rounds.push_back(round);
   }
   else
   {
      // If this was a black_move, just update the last Round
      Round round = rounds[rounds.size() - 1];
      round.black_move = to_record;

      // Remove the last round and put it back, now with the black move
      rounds.pop_back();
      rounds.push_back(round);
   }
}

string Chess::getLastMove(void)
{
   string last_move;

   // Who did the last move?
   if (BLACK_PLAYER == getCurrentTurn())
   {
      // If it's black's turn now, white had the last move
      last_move = rounds[rounds.size() - 1].white_move;
   }
   else
   {
      // Last move was black's
      last_move = rounds[rounds.size() - 1].black_move;
   }

   return last_move;
}

void Chess::deleteLastMove( void )
{
   // Notice we already changed turns back
   if (WHITE_PLAYER == getCurrentTurn())
   {
      // Last move was white's turn, so simply pop from the back
      rounds.pop_back();
   }
   else
   {
      // Last move was black's, so let's 
      Round round = rounds[rounds.size() - 1];
      round.black_move = "";

      // Pop last round and put it back, now without the black move
      rounds.pop_back();
      rounds.push_back(round);
   }
}




//Auxiliary Helper Functions



bool Chess::isMoveValid(Chess::Position present, Chess::Position future, Chess::EnPassant* S_enPassant, Chess::Castling* S_castling, Chess::Promotion* S_promotion)
{
   bool bValid = false;

   char chPiece = this->getPieceAtPosition(present.iRow, present.iColumn);

   // ----------------------------------------------------
   // 1. Is the piece  allowed to move in that direction?
   // ----------------------------------------------------
   switch( toupper(chPiece) )
   {
      case 'P':
      {
         // Wants to move forward
         if ( future.iColumn == present.iColumn )
         {
            // Simple move forward
            if ( (Chess::isWhitePiece(chPiece) && future.iRow == present.iRow + 1) ||
                 (Chess::isBlackPiece(chPiece) && future.iRow == present.iRow - 1) )
            {
               if ( EMPTY_SQUARE == this->getPieceAtPosition(future.iRow, future.iColumn) )
               {
                  bValid = true;
               }
            }

            // Double move forward
            else if ( (Chess::isWhitePiece(chPiece) && future.iRow == present.iRow + 2) ||
                      (Chess::isBlackPiece(chPiece) && future.iRow == present.iRow - 2) )
            {
               // This is only allowed if the pawn is in its original place
               if ( Chess::isWhitePiece(chPiece) )
               {
                  if ( EMPTY_SQUARE == this->getPieceAtPosition(future.iRow-1, future.iColumn) &&
                       EMPTY_SQUARE == this->getPieceAtPosition(future.iRow, future.iColumn)   &&
                                1   == present.iRow )
                  {
                     bValid = true;
                  }
               }
               else // if ( isBlackPiece(chPiece) )
               {
                  if ( EMPTY_SQUARE == this->getPieceAtPosition(future.iRow + 1, future.iColumn) &&
                       EMPTY_SQUARE == this->getPieceAtPosition(future.iRow, future.iColumn)     &&
                                6   == present.iRow)
                  {
                     bValid = true;
                  }
               }
            }
            else
            {
               // This is invalid
               return false;
            }
         }
         
         // The "en passant" move
         else if ( (Chess::isWhitePiece(chPiece) && 4 == present.iRow && 5 == future.iRow && 1 == abs(future.iColumn - present.iColumn) ) ||
                   (Chess::isBlackPiece(chPiece) && 3 == present.iRow && 2 == future.iRow && 1 == abs(future.iColumn - present.iColumn) ) )
         {
            // It is only valid if last move of the opponent was a double move forward by a pawn on a adjacent column
            string last_move = this->getLastMove();

            // Parse the line
            Chess::Position LastMoveFrom;
            Chess::Position LastMoveTo;
            this->parseMove(last_move, &LastMoveFrom, &LastMoveTo);

            // First of all, was it a pawn?
            char chLstMvPiece = this->getPieceAtPosition(LastMoveTo.iRow, LastMoveTo.iColumn);

            if (toupper(chLstMvPiece) != 'P')
            {
               return false;
            }

            // Did the pawn have a double move forward and was it an adjacent column?
            if ( 2 == abs(LastMoveTo.iRow - LastMoveFrom.iRow) && 1 == abs(LastMoveFrom.iColumn - present.iColumn) )
            {
               cout << "En passant move!\n";
               bValid = true;

               S_enPassant->bApplied = true;
               S_enPassant->PawnCaptured.iRow    = LastMoveTo.iRow;
               S_enPassant->PawnCaptured.iColumn = LastMoveTo.iColumn;
            }
         }

         // Wants to capture a piece
         else if (1 == abs(future.iColumn - present.iColumn))
         {
            if ( (Chess::isWhitePiece(chPiece) && future.iRow == present.iRow + 1) || (Chess::isBlackPiece(chPiece) && future.iRow == present.iRow - 1))
            {
               // Only allowed if there is something to be captured in the square
               if (EMPTY_SQUARE != this->getPieceAtPosition(future.iRow, future.iColumn))
               {
                  bValid = true;
                  cout << "Pawn captured a piece!\n";
               }
            }
         }
         else
         {
            // This is invalid
            return false;
         }

         // If a pawn reaches its eight rank, it must be promoted to another piece
         if ( (Chess::isWhitePiece( chPiece ) && 7 == future.iRow) ||
              (Chess::isBlackPiece( chPiece ) && 0 == future.iRow) )
         {
            cout << "Pawn must be promoted!\n";
            S_promotion->bApplied = true;
         }
      }
      break;

      case 'R':
      {
         // Horizontal move
         if ( (future.iRow == present.iRow) && (future.iColumn != present.iColumn) )
         {
            // Check if there are no pieces on the way
            if ( this->isPathFree(present, future, Chess::HORIZONTAL) )
            {
               bValid = true;
            }
         }
         // Vertical move
         else if ( (future.iRow != present.iRow) && (future.iColumn == present.iColumn) )
         {
            // Check if there are no pieces on the way
            if ( this->isPathFree(present, future, Chess::VERTICAL) )
            {
               bValid = true;
            }
         }
      }
      break;

      case 'N':
      {
         if ( (2 == abs(future.iRow - present.iRow)) && (1 == abs(future.iColumn - present.iColumn)) )
         {
            bValid = true;
         }

         else if (( 1 == abs(future.iRow - present.iRow)) && (2 == abs(future.iColumn - present.iColumn)) )
         {
            bValid = true;
         }
      }
      break;

      case 'B':
      {
         // Diagonal move
         if ( abs(future.iRow - present.iRow) == abs(future.iColumn - present.iColumn) )
         {
            // Check if there are no pieces on the way
            if ( this->isPathFree(present, future, Chess::DIAGONAL) )
            {
               bValid = true;
            }
         }
      }
      break;

      case 'Q':
      {
         // Horizontal move
         if ( (future.iRow == present.iRow) && (future.iColumn != present.iColumn) )
         {
            // Check if there are no pieces on the way
            if ( this->isPathFree(present, future, Chess::HORIZONTAL))
            {
               bValid = true;
            }
         }
         // Vertical move
         else if ( (future.iRow != present.iRow) && (future.iColumn == present.iColumn) )
         {
            // Check if there are no pieces on the way
            if ( this->isPathFree(present, future, Chess::VERTICAL))
            {
               bValid = true;
            }
         }

         // Diagonal move
         else if ( abs(future.iRow - present.iRow) == abs(future.iColumn - present.iColumn) )
         {
            // Check if there are no pieces on the way
            if ( this->isPathFree(present, future, Chess::DIAGONAL))
            {
               bValid = true;
            }
         }
      }
      break;

      case 'K':
      {
         // Horizontal move by 1
         if ( (future.iRow == present.iRow) && (1 == abs(future.iColumn - present.iColumn) ) )
         {
            bValid = true;
         }

         // Vertical move by 1
         else if ( (future.iColumn == present.iColumn) && (1 == abs(future.iRow - present.iRow) ) )
         {
            bValid = true;
         }

         // Diagonal move by 1
         else if ( (1 == abs(future.iRow - present.iRow) ) && (1 == abs(future.iColumn - present.iColumn) ) )
         {
            bValid = true;
         }

         // Castling
         else if ( (future.iRow == present.iRow) && (2 == abs(future.iColumn - present.iColumn) ) )
         {
            // Castling is only allowed in these circunstances:

            // 1. King is not in check
            if ( true == this->playerKingInCheck() )
            {
               return false;
            }

            // 2. No pieces in between the king and the rook
            if ( false == this->isPathFree( present, future, Chess::HORIZONTAL ) )
            {
               return false;
            }

            // 3. King and rook must not have moved yet;
            // 4. King must not pass through a square that is attacked by an enemy piece
            if ( future.iColumn > present.iColumn )
            {
               // if future.iColumn is greather, it means king side
               if ( false == this->castlingAllowed(Chess::Side::KING_SIDE, Chess::getPieceColor(chPiece) ) )
               {
                  createNextMessage("Castling to the king side is not allowed.\n");
                  return false;
               }
               else
               {
                  // Check if the square that the king skips is not under attack
                  Chess::UnderAttack square_skipped = this->isUnderAttack( present.iRow, present.iColumn + 1, this->getCurrentTurn() );
                  if ( false == square_skipped.bUnderAttack )
                  {
                     // Fill the S_castling structure
                     S_castling->bApplied = true;

                     // Present position of the rook
                     S_castling->rook_before.iRow    = present.iRow;
                     S_castling->rook_before.iColumn = present.iColumn + 3;

                     // Future position of the rook
                     S_castling->rook_after.iRow    = future.iRow;
                     S_castling->rook_after.iColumn = present.iColumn + 1; // future.iColumn -1

                     bValid = true;
                  }
               }
            }
            else //if (future.iColumn < present.iColumn)
            {
               // if present.iColumn is greather, it means queen side
               if (false == this->castlingAllowed(Chess::Side::QUEEN_SIDE, Chess::getPieceColor(chPiece)))
               {
                  createNextMessage("Castling to the queen side is not allowed.\n");
                  return false;
               }
               else
               {
                  // Check if the square that the king skips is not attacked
                  Chess::UnderAttack square_skipped = this->isUnderAttack( present.iRow, present.iColumn - 1, this->getCurrentTurn() );
                  if ( false == square_skipped.bUnderAttack )
                  {
                     // Fill the S_castling structure
                     S_castling->bApplied = true;

                     // Present position of the rook
                     S_castling->rook_before.iRow    = present.iRow;
                     S_castling->rook_before.iColumn = present.iColumn - 4;

                     // Future position of the rook
                     S_castling->rook_after.iRow    = future.iRow;
                     S_castling->rook_after.iColumn = present.iColumn - 1; // future.iColumn +1

                     bValid = true;
                  }
               }
            }
         }
      }
      break;

      default:
      {
         cout << "!!!!Should not reach here. Invalid piece: " << char(chPiece) << "\n\n\n";
      }
      break;
   }

   // If it is a move in an invalid direction, do not even bother to check the rest
   if ( false == bValid )
   {
      cout << "Piece is not allowed to move to that square\n";
      return false;
   }


   // -------------------------------------------------------------------------
   // 2. Is there another piece of the same color on the destination square?
   // -------------------------------------------------------------------------
   if (this->isSquareOccupied(future.iRow, future.iColumn))
   {
      char chAuxPiece = this->getPieceAtPosition(future.iRow, future.iColumn);
      if ( Chess::getPieceColor(chPiece) == Chess::getPieceColor(chAuxPiece) )
      {
         cout << "Position is already taken by a piece of the same color\n";
         return false;
      }
   }

   // ----------------------------------------------
   // 3. Would the king be in check after the move?
   // ----------------------------------------------
   if ( true == this->wouldKingBeInCheck(chPiece, present, future, S_enPassant) )
   {
      cout << "Move would put player's king in check\n";
      return false;
   }

   return bValid;
}

void Chess::makeTheMove(Chess::Position present, Chess::Position future, Chess::EnPassant* S_enPassant, Chess::Castling* S_castling, Chess::Promotion* S_promotion)
{
   char chPiece = this->getPieceAtPosition(present.iRow, present.iColumn);

   // -----------------------
   // Captured a piece?
   // -----------------------
   if ( this->isSquareOccupied(future.iRow, future.iColumn) )
   {
      char chAuxPiece = this->getPieceAtPosition(future.iRow, future.iColumn);

      if ( Chess::getPieceColor(chPiece) != Chess::getPieceColor(chAuxPiece))
      {
         createNextMessage(Chess::describePiece(chAuxPiece) + " captured!\n");
      }
      else
      {
         cout << "Error. We should not be making this move\n";
         throw("Error. We should not be making this move");
      }
   }
   else if (true == S_enPassant->bApplied)
   {
      createNextMessage("Pawn captured by \"en passant\" move!\n");
   }

   if ( (true == S_castling->bApplied) )
   {
      createNextMessage("Castling applied!\n");
   }

   this->movePiece(present, future, S_enPassant, S_castling, S_promotion);
}


// void newGame(void)
// {
//    if (NULL != current_game)
//    {
//       delete current_game;
//    }

//    current_game = new Game();
// }

void Chess::undoMove(void)
{
   if ( false == this->undoIsPossible() )
   {
      createNextMessage("Undo is not possible now!\n");
      return;
   }

   this->undoLastMove();
   createNextMessage("Last move was undone\n");
}


void Chess::appendToNextMessage( string msg )
{
   next_message += msg;
}

void Chess::createNextMessage( string msg )
{
    cout << msg << nl;
   next_message = msg;
}

