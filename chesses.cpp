#include "chesses.h"
#include "mainwindow.h"

chesses::chesses(QObject *parent) : QObject(parent)
{
    existence = true;
}

void chesses::chesses_ini(int _career, int _x, int _y){
    career = _career;
    x = _x;
    y = _y;
    existence = true;
}

//Compute the availible blocks for white chess
void chesses::white_path_compute(bool **path, int **board){

    if(existence == false) return;
    //Pawn
    if(career == PAWN_W){
        if(board[x][y - 1] == NONE){
            path[x][y - 1] = true;   //normally, could move forword if no chess at front
            if(y == 6 && board[x][4] == NONE)path[x][y - 2] = true;     //if first move, could move forward two block
        }
        if(x < 7) //front-right has black chess
            if(board[x + 1][y - 1] >= 16 && board[x + 1][y - 1] <= 31)
                path[x + 1][y - 1] = true;

        if(x > 0) //front-left has black chess
            if(board[x - 1][y - 1] >= 16 && board[x - 1][y - 1] <= 31)
                path[x - 1][y - 1] = true;
    }

    //Knight
    if(career == KNIGHT_W){
        for(int i = -2; i < 3; i++){
            if(i == 0) continue;
            if(i == -1 || i == 1){
                for(int j = -2; j < 3; j+=4){
                    if(x + i >= 0 && x + i <= 7 && y + j >= 0 && y + j <= 7)
                        if(board[x + i][y + j] < 0 || board[x + i][y + j] > 15) //Not white
                            path[x + i][y + j] = true;
            }}
            else if(i == -2 || i == 2){
                for(int j = -1; j < 2; j+=2){
                    if(x + i >= 0 && x + i <= 7 && y + j >= 0 && y + j <= 7)
                        if(board[x + i][y + j] < 0 || board[x + i][y + j] > 15)
                            path[x + i][y + j] = true;
    }}}
    }

    //Bishop
    if(career == BISHOP_W || career == QUEEN_W){
        for(int i = 1;; i++){
            if(x + i <= 7 && y + i <= 7){
                if(board[x + i][y + i] == NONE) path[x + i][y + i] = true;
                else if(board[x + i][y + i] >= 16 && board[x + i][y + i] <= 31){
                    path[x + i][y + i] = true; break;
                }
                else break;
            }
            else break;
        }
        for(int i = 1;; i++){
            if(x + i <= 7 && y - i >= 0){
                if(board[x + i][y - i] == NONE) path[x + i][y - i] = true;
                else if(board[x + i][y - i] >= 16 && board[x + i][y - i] <= 31){
                    path[x + i][y - i] = true; break;
                }
                else break;
            }
            else break;
        }
        for(int i = 1;; i++){
            if(x - i >= 0 && y + i <= 7){
                if(board[x - i][y + i] == NONE) path[x - i][y + i] = true;
                else if(board[x - i][y + i] >= 16 && board[x - i][y + i] <= 31){
                    path[x - i][y + i] = true; break;
                }
                else break;
            }
            else break;
        }
        for(int i = 1;; i++){
            if(x - i >= 0 && y - i >= 0){
                if(board[x - i][y - i] == NONE) path[x - i][y - i] = true;
                else if(board[x - i][y - i] >= 16 && board[x - i][y - i] <= 31){
                    path[x - i][y - i] = true; break;
                }
                else break;
            }
            else break;
        }
    }

    //Rook
    if(career == ROOK_W || career == QUEEN_W){
        for(int i = 1;; i++){
            if(x + i <= 7){
                if(board[x + i][y] == NONE) path[x + i][y] = true;
                else if(board[x + i][y] >= 16 && board[x + i][y] <= 31){
                    path[x + i][y] = true; break;
                }
                else break;
            }
            else break;
        }
        for(int i = 1;; i++){
            if(x - i >= 0){
                if(board[x - i][y] == NONE) path[x - i][y] = true;
                else if(board[x - i][y] >= 16 && board[x - i][y] <= 31){
                    path[x - i][y] = true; break;
                }
                else break;
            }
            else break;
        }
        for(int i = 1;; i++){
            if(y + i <= 7){
                if(board[x][y + i] == NONE) path[x][y + i] = true;
                else if(board[x][y + i] >= 16 && board[x][y + i] <= 31){
                    path[x][y + i] = true; break;
                }
                else break;
            }
            else break;
        }
        for(int i = 1;; i++){
            if(y - i >= 0){
                if(board[x][y - i] == NONE) path[x][y - i] = true;
                else if(board[x][y - i] >= 16 && board[x][y - i] <= 31){
                    path[x][y - i] = true; break;
                }
                else break;
            }
            else break;
        }
    }

    return;
}

//Compute the availible blocks for black chess
void chesses::black_path_compute(bool **path, int **board){

    if(existence == false) return;
    //Pawn
    if(career == PAWN_B){
        if(board[x][y + 1] == NONE){
            path[x][y + 1] = true;   //normally, could move forword if no chess at front
            if(y == 1 && board[x][3] == NONE)path[x][y + 2] = true;     //if first move, could move forward two block
        }
        if(x < 7) //front-right has white chess
            if(board[x + 1][y + 1] >= 0 && board[x + 1][y + 1] <= 15)
                path[x + 1][y + 1] = true;

        if(x > 0) //front-left has white chess
            if(board[x - 1][y + 1] >= 0 && board[x - 1][y + 1] <= 15)
                path[x - 1][y + 1] = true;
    }

    //Knight
    if(career == KNIGHT_B){
        for(int i = -2; i < 3; i++){
            if(i == 0) continue;
            if(i == -1 || i == 1){
                for(int j = -2; j < 3; j+=4){
                    if(x + i >= 0 && x + i <= 7 && y + j >= 0 && y + j <= 7)
                        if(board[x + i][y + j] < 16 || board[x + i][y + j] > 31) //Not black
                            path[x + i][y + j] = true;
            }}
            else if(i == -2 || i == 2){
                for(int j = -1; j < 2; j+=2){
                    if(x + i >= 0 && x + i <= 7 && y + j >= 0 && y + j <= 7)
                        if(board[x + i][y + j] < 16 || board[x + i][y + j] > 31)
                            path[x + i][y + j] = true;
    }}}
    }

    //Bishop
    if(career == BISHOP_B || career == QUEEN_B){
        for(int i = 1;; i++){
            if(x + i <= 7 && y + i <= 7){
                if(board[x + i][y + i] == NONE) path[x + i][y + i] = true;
                else if(board[x + i][y + i] >= 0 && board[x + i][y + i] <= 15){
                    path[x + i][y + i] = true; break;
                }
                else break;
            }
            else break;
        }
        for(int i = 1;; i++){
            if(x + i <= 7 && y - i >= 0){
                if(board[x + i][y - i] == NONE) path[x + i][y - i] = true;
                else if(board[x + i][y - i] >= 0 && board[x + i][y - i] <= 15){
                    path[x + i][y - i] = true; break;
                }
                else break;
            }
            else break;
        }
        for(int i = 1;; i++){
            if(x - i >= 0 && y + i <= 7){
                if(board[x - i][y + i] == NONE) path[x - i][y + i] = true;
                else if(board[x - i][y + i] >= 0 && board[x - i][y + i] <= 15){
                    path[x - i][y + i] = true; break;
                }
                else break;
            }
            else break;
        }
        for(int i = 1;; i++){
            if(x - i >= 0 && y - i >= 0){
                if(board[x - i][y - i] == NONE) path[x - i][y - i] = true;
                else if(board[x - i][y - i] >= 0 && board[x - i][y - i] <= 15){
                    path[x - i][y - i] = true; break;
                }
                else break;
            }
            else break;
        }
    }

    //Rook
    if(career == ROOK_B || career == QUEEN_B){
        for(int i = 1;; i++){
            if(x + i <= 7){
                if(board[x + i][y] == NONE) path[x + i][y] = true;
                else if(board[x + i][y] >= 0 && board[x + i][y] <= 15){
                    path[x + i][y] = true; break;
                }
                else break;
            }
            else break;
        }
        for(int i = 1;; i++){
            if(x - i >= 0){
                if(board[x - i][y] == NONE) path[x - i][y] = true;
                else if(board[x - i][y] >= 0 && board[x - i][y] <= 15){
                    path[x - i][y] = true; break;
                }
                else break;
            }
            else break;
        }
        for(int i = 1;; i++){
            if(y + i <= 7){
                if(board[x][y + i] == NONE) path[x][y + i] = true;
                else if(board[x][y + i] >= 0 && board[x][y + i] <= 15){
                    path[x][y + i] = true; break;
                }
                else break;
            }
            else break;
        }
        for(int i = 1;; i++){
            if(y - i >= 0){
                if(board[x][y - i] == NONE) path[x][y - i] = true;
                else if(board[x][y - i] >= 0 && board[x][y - i] <= 15){
                    path[x][y - i] = true; break;
                }
                else break;
            }
            else break;
        }
    }

    return;
}

void chesses::whiteKING_path_compute(bool **path, int **board, chesses *chess_obj, bool **attacked){

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            attacked[i][j] = false;

    //Compute the blocks controlled by black chesses

    //compute
    for(int i = -1; i <= 1; i++)                                               //the blocks around the king
        for(int j = -1; j <= 1; j++)
            if(chess_obj[16].x + i <= 7 && chess_obj[16].x + i >= 0 && chess_obj[16].y + j <= 7 && chess_obj[16].y + j >= 0)           //inside the board
                attacked[chess_obj[16].x + i][chess_obj[16].y + j] = true;
    for(int i = 17; i <= 23; i++)
        chess_obj[i].black_path_compute(attacked, board);
    for(int i = 24; i <= 31; i++){
        if(chess_obj[i].x <= 6 && chess_obj[i].existence == true)
            attacked[chess_obj[i].x + 1][chess_obj[i].y + 1] = true;
        if(chess_obj[i].x >= 1 && chess_obj[i].existence == true)
            attacked[chess_obj[i].x - 1][chess_obj[i].y + 1] = true;
    }
    //Compute the availible blocks for the black king
    for(int i = -1; i <= 1; i++)                                                //the blocks around the king
        for(int j = -1; j <= 1; j++)
            if(x + i <= 7 && x + i >= 0 && y + j <= 7 && y + j >= 0){           //inside the board
                if(board[x + i][y + j] < 0 || board[x + i][y + j] > 15)       //Not white chess
                    path[x + i][y + j] = true;
            }
    //Castling
    if(chess_obj[0].moved == false && chess_obj[0].existence == true){
        if(chess_obj[2].moved == false)                                           //king and the rook never moved
            if(attacked[x - 1][y] == false && attacked[x - 2][y] == false)          //the path doesn't controlled by white chesses
                if(board[x - 1][y] == NONE && board[x - 2][y] == NONE && board[x - 3][y] == NONE)
                    path[x - 2][y] = true;

        if(chess_obj[3].moved == false && chess_obj[3].existence == true)                                           //king and the rook never moved
            if(attacked[x + 1][y] == false && attacked[x + 2][y] == false)         //the path doesn't controlled by white chesses
                if(board[x + 1][y] == NONE && board[x + 2][y] == NONE)
                    path[x + 2][y] = true;
    }
    return;
}

void chesses::blackKING_path_compute(bool **path, int **board, chesses *chess_obj, bool **attacked){  //1:normal situation 2:lose(can't move)

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            attacked[i][j] = false;

    //Compute the blocks controlled by white chesses
    //compute
    for(int i = -1; i <= 1; i++)                                               //the blocks around the king
        for(int j = -1; j <= 1; j++)
            if(chess_obj[0].x + i <= 7 && chess_obj[0].x + i >= 0 && chess_obj[0].y + j <= 7 && chess_obj[0].y + j >= 0)           //inside the board
                attacked[chess_obj[0].x + i][chess_obj[0].y + j] = true;
    for(int i = 1; i <= 7; i++)
        chess_obj[i].white_path_compute(attacked, board);
    for(int i = 8; i <= 15; i++){
        if(chess_obj[i].x <= 6 && chess_obj[i].existence == true)
            attacked[chess_obj[i].x + 1][chess_obj[i].y - 1] = true;
        if(chess_obj[i].x >= 1 && chess_obj[i].existence == true)
            attacked[chess_obj[i].x - 1][chess_obj[i].y - 1] = true;
    }

    //Compute the availible blocks for the black king
    for(int i = -1; i <= 1; i++)                                               //the blocks around the king
        for(int j = -1; j <= 1; j++)
            if(x + i <= 7 && x + i >= 0 && y + j <= 7 && y + j >= 0)           //inside the board
                if(board[x + i][y + j] < 16 || board[x + i][y + j] > 31)       //Not black chess
                    path[x + i][y + j] = true;
    //Castling
    if(chess_obj[16].moved == false && chess_obj[16].existence == true){
        if(chess_obj[18].moved == false && chess_obj[18].existence == true)                                           //king and the rook never moved
            if(attacked[x - 1][y] == false && attacked[x - 2][y] == false)          //the path doesn't controlled by white chesses
                if(board[x - 1][y] == NONE && board[x - 2][y] == NONE && board[x - 3][y] == NONE)
                    path[x - 2][y] = true;

        if(chess_obj[19].moved == false && chess_obj[19].existence == true)                                           //king and the rook never moved
            if(attacked[x + 1][y] == false && attacked[x + 2][y] == false)         //the path doesn't controlled by white chesses
                if(board[x + 1][y] == NONE && board[x + 2][y] == NONE )
                    path[x + 2][y] = true;
    }

    return;
}

bool chesses::King_being_controlled(bool **path, int **board, chesses *chess_obj, bool **attacked){

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            attacked[i][j] = false;
    //Set the number to use according to the player right now.
    int op;
    if(career == KING_W)
    {
        op = 16;
    }
    else if(career == KING_B)
    {
        op = 0;
    }

    //Compute the blocks controlled by opposite chesses
    //method : Check if the king is on the block controlled by the opposite color

    //board generating
    //King
    for(int i = -1; i <= 1; i++)
        for(int j = -1; j <= 1; j++)
            if(chess_obj[op].x + i <= 7 && chess_obj[op].x + i >= 0 && chess_obj[op].y + j <= 7 && chess_obj[op].y + j >= 0)           //inside the board
                attacked[chess_obj[op].x + i][chess_obj[op].y + j] = true;
    //Pawn~Queen
    if(career == KING_W)
        for(int i = op + 1; i <= op + 15; i++){
            if(chess_obj[i].career != PAWN_B)
                chess_obj[i].black_path_compute(attacked, board);
            else{
                if(chess_obj[i].x <= 6)
                    attacked[chess_obj[i].x + 1][chess_obj[i].y + 1] = true;
                if(chess_obj[i].x >= 1)
                    attacked[chess_obj[i].x - 1][chess_obj[i].y + 1] = true;
            }
        }
    else if(career == KING_B)
        for(int i = op + 1; i <= op + 15; i++){
            if(chess_obj[i].career != PAWN_W)
                chess_obj[i].white_path_compute(attacked, board);
            else{
                if(chess_obj[i].x <= 6)
                    attacked[chess_obj[i].x + 1][chess_obj[i].y - 1] = true;
                if(chess_obj[i].x >= 1)
                    attacked[chess_obj[i].x - 1][chess_obj[i].y - 1] = true;
            }
        }
    //If king is being controlled return true (should move), or not.
    if(attacked[x][y] == true)
    {
        path[x][y] = true;
        return true;
    }
    else{
        return false;
    }
}
