#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);

    //Scene built (game board)
    this->scene = new QGraphicsScene(
                0, 0,
                ui->graphicsView->width()-2,
                ui->graphicsView->height()-2
                );
    this->board_editor = new QGraphicsScene(
                0, 0,
                ui->BoardEditor->width()-2,
                ui->BoardEditor->height()-2
                );
    QPixmap background(":/img/background");         //background pic
    scene->addPixmap(background.scaled(800, 800));  //paste it on
    ui->graphicsView->setScene(scene);              //setscene to grapicView

    board_editor->addPixmap(background.scaled(800, 800));  //paste it on
    ui->BoardEditor->setScene(board_editor);              //setscene to grapicView


    //Objects declarations
    chess_obj = new chesses [32];
    chess_img = new QGraphicsPixmapItem* [32];
    _chess_img = new QGraphicsPixmapItem* [32];
    path_grid = new QGraphicsPixmapItem** [8];
    Board_Editor = new int* [12];
    board = new int* [8];
    buf_board = new int* [8];
    buf2_board = new int* [8];
    show_path = new bool* [8];
    attacked = new bool* [8];
    for(int i=0; i<8; i++){
        board[i] = new int[8];
        buf_board[i] = new int[8];
        buf2_board[i] = new int[8];
        show_path[i] = new bool[8];
        attacked[i] = new bool[8];
        path_grid[i] = new QGraphicsPixmapItem*[8];
    }
    for(int i = 0; i < 12; i++)
        Board_Editor[i] = new int[8];

    situation = SITUATION_MENU;

    QPixmap AGrid(100, 100);
    AGrid.fill(Qt::red);
    selector = board_editor -> addPixmap(AGrid.scaled(100,100));
    selector -> hide();

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++){
            buf_board[i][j] = NONE;
            buf2_board[i][j] = NONE;
            attacked[i][j] = false;
            path_grid[i][j] = scene -> addPixmap(AGrid);
            path_grid[i][j]->setPos(100 * i, 100 * j);
            path_grid[i][j]->hide();
        }

    //Set the icons for promotion
    //White
    ui -> PromotionToKnight_W -> setIcon(QIcon(":/img/Knight_w.png"));
    ui -> PromotionToKnight_W -> setIconSize(QSize(100, 100));
    ui -> PromotionToBishop_W -> setIcon(QIcon(":/img/Bishop_w.png"));
    ui -> PromotionToBishop_W -> setIconSize(QSize(100, 100));
    ui -> PromotionToRook_W -> setIcon(QIcon(":/img/Rook_w.png"));
    ui -> PromotionToRook_W -> setIconSize(QSize(100, 100));
    ui -> PromotionToQueen_W -> setIcon(QIcon(":/img/Queen_w.png"));
    ui -> PromotionToQueen_W -> setIconSize(QSize(100, 100));
    //Black
    ui -> PromotionToKnight_B -> setIcon(QIcon(":/img/Knight_b.png"));
    ui -> PromotionToKnight_B -> setIconSize(QSize(100, 100));
    ui -> PromotionToBishop_B -> setIcon(QIcon(":/img/Bishop_b.png"));
    ui -> PromotionToBishop_B -> setIconSize(QSize(100, 100));
    ui -> PromotionToRook_B -> setIcon(QIcon(":/img/Rook_b.png"));
    ui -> PromotionToRook_B -> setIconSize(QSize(100, 100));
    ui -> PromotionToQueen_B -> setIcon(QIcon(":/img/Queen_b.png"));
    ui -> PromotionToQueen_B -> setIconSize(QSize(100, 100));

    //Initialize
    during_game = false;
    editor_x = NONE;
    editor_y = NONE;
    PicsLoaded();
    //Set UI to menu mode
    UIswitching(situation);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Add the images of the chesses for the first time
void MainWindow::PicsLoaded()
{
    //White chesses
    chess_img[0] = scene -> addPixmap(QPixmap(":/img/King_w.png").scaled(100, 100));
    chess_img[1] = scene -> addPixmap(QPixmap(":/img/Queen_w.png").scaled(100, 100));
    chess_img[2] = scene -> addPixmap(QPixmap(":/img/Rook_w.png").scaled(100, 100));
    chess_img[3] = scene -> addPixmap(QPixmap(":/img/Rook_w.png").scaled(100, 100));
    chess_img[4] = scene -> addPixmap(QPixmap(":/img/Bishop_w.png").scaled(100, 100));
    chess_img[5] = scene -> addPixmap(QPixmap(":/img/Bishop_w.png").scaled(100, 100));
    chess_img[6] = scene -> addPixmap(QPixmap(":/img/Knight_w.png").scaled(100, 100));
    chess_img[7] = scene -> addPixmap(QPixmap(":/img/Knight_w.png").scaled(100, 100));
    for(int i = 0; i < 8; i++)
        chess_img[i + 8] = scene -> addPixmap(QPixmap(":/img/Pawn_w.png").scaled(100, 100));
    //Black chesses
    chess_img[16] = scene -> addPixmap(QPixmap(":/img/King_b.png").scaled(100, 100));
    chess_img[17] = scene -> addPixmap(QPixmap(":/img/Queen_b.png").scaled(100, 100));
    chess_img[18] = scene -> addPixmap(QPixmap(":/img/Rook_b.png").scaled(100, 100));
    chess_img[19] = scene -> addPixmap(QPixmap(":/img/Rook_b.png").scaled(100, 100));
    chess_img[20] = scene -> addPixmap(QPixmap(":/img/Bishop_b.png").scaled(100, 100));
    chess_img[21] = scene -> addPixmap(QPixmap(":/img/Bishop_b.png").scaled(100, 100));
    chess_img[22] = scene -> addPixmap(QPixmap(":/img/Knight_b.png").scaled(100, 100));
    chess_img[23] = scene -> addPixmap(QPixmap(":/img/Knight_b.png").scaled(100, 100));
    for(int i = 0; i < 8; i++)
        chess_img[i + 24] = scene -> addPixmap(QPixmap(":/img/Pawn_b.png").scaled(100, 100));

    //White chesses
    _chess_img[0] = board_editor -> addPixmap(QPixmap(":/img/King_w.png").scaled(100, 100));
    _chess_img[1] = board_editor -> addPixmap(QPixmap(":/img/Queen_w.png").scaled(100, 100));
    _chess_img[2] = board_editor -> addPixmap(QPixmap(":/img/Rook_w.png").scaled(100, 100));
    _chess_img[3] = board_editor -> addPixmap(QPixmap(":/img/Rook_w.png").scaled(100, 100));
    _chess_img[4] = board_editor -> addPixmap(QPixmap(":/img/Bishop_w.png").scaled(100, 100));
    _chess_img[5] = board_editor -> addPixmap(QPixmap(":/img/Bishop_w.png").scaled(100, 100));
    _chess_img[6] = board_editor -> addPixmap(QPixmap(":/img/Knight_w.png").scaled(100, 100));
    _chess_img[7] = board_editor -> addPixmap(QPixmap(":/img/Knight_w.png").scaled(100, 100));
    for(int i = 0; i < 8; i++)
        _chess_img[i + 8] = board_editor -> addPixmap(QPixmap(":/img/Pawn_w.png").scaled(100, 100));
    //Black chesses
    _chess_img[16] = board_editor -> addPixmap(QPixmap(":/img/King_b.png").scaled(100, 100));
    _chess_img[17] = board_editor -> addPixmap(QPixmap(":/img/Queen_b.png").scaled(100, 100));
    _chess_img[18] = board_editor -> addPixmap(QPixmap(":/img/Rook_b.png").scaled(100, 100));
    _chess_img[19] = board_editor -> addPixmap(QPixmap(":/img/Rook_b.png").scaled(100, 100));
    _chess_img[20] = board_editor -> addPixmap(QPixmap(":/img/Bishop_b.png").scaled(100, 100));
    _chess_img[21] = board_editor -> addPixmap(QPixmap(":/img/Bishop_b.png").scaled(100, 100));
    _chess_img[22] = board_editor -> addPixmap(QPixmap(":/img/Knight_b.png").scaled(100, 100));
    _chess_img[23] = board_editor -> addPixmap(QPixmap(":/img/Knight_b.png").scaled(100, 100));
    for(int i = 0; i < 8; i++)
        _chess_img[i + 24] = board_editor -> addPixmap(QPixmap(":/img/Pawn_b.png").scaled(100, 100));

    return;
}

//Updates the pics of the 32 chesses accroding to its career
void MainWindow::PicsUpdating()
{
    for(int i = 0; i <= 31; i++){
        if(chess_obj[i].career == KING_W)
            chess_img[i]->setPixmap(QPixmap(":/img/King_w.png").scaled(100, 100));
        else if(chess_obj[i].career == KING_B)
            chess_img[i]->setPixmap(QPixmap(":/img/King_b.png").scaled(100, 100));
        else if(chess_obj[i].career == QUEEN_W)
            chess_img[i]->setPixmap(QPixmap(":/img/Queen_w.png").scaled(100, 100));
        else if(chess_obj[i].career == QUEEN_B)
            chess_img[i]->setPixmap(QPixmap(":/img/Queen_b.png").scaled(100, 100));
        else if(chess_obj[i].career == ROOK_W)
            chess_img[i]->setPixmap(QPixmap(":/img/Rook_w.png").scaled(100, 100));
        else if(chess_obj[i].career == ROOK_B)
            chess_img[i]->setPixmap(QPixmap(":/img/Rook_b.png").scaled(100, 100));
        else if(chess_obj[i].career == BISHOP_W)
            chess_img[i]->setPixmap(QPixmap(":/img/Bishop_w.png").scaled(100, 100));
        else if(chess_obj[i].career == BISHOP_B)
            chess_img[i]->setPixmap(QPixmap(":/img/Bishop_b.png").scaled(100, 100));
        else if(chess_obj[i].career == KNIGHT_W)
            chess_img[i]->setPixmap(QPixmap(":/img/Knight_w.png").scaled(100, 100));
        else if(chess_obj[i].career == KNIGHT_B)
            chess_img[i]->setPixmap(QPixmap(":/img/Knight_b.png").scaled(100, 100));
        else if(chess_obj[i].career == PAWN_W)
            chess_img[i]->setPixmap(QPixmap(":/img/Pawn_w.png").scaled(100, 100));
        else if(chess_obj[i].career == PAWN_B)
            chess_img[i]->setPixmap(QPixmap(":/img/Pawn_b.png").scaled(100, 100));
    }
    return;
}

//Place the chesses in default position, and update the board
void MainWindow::Default_set()
{
    for(int i = 0; i < 32; i++)
        chess_obj[i].moved = false;
    //White chesses
    chess_obj[0].chesses_ini(KING_W, 4, 7);
    chess_obj[1].chesses_ini(QUEEN_W, 3, 7);
    chess_obj[2].chesses_ini(ROOK_W, 0, 7);
    chess_obj[3].chesses_ini(ROOK_W, 7, 7);
    chess_obj[4].chesses_ini(BISHOP_W, 2, 7);
    chess_obj[5].chesses_ini(BISHOP_W, 5, 7);
    chess_obj[6].chesses_ini(KNIGHT_W, 6, 7);
    chess_obj[7].chesses_ini(KNIGHT_W, 1, 7);
    for(int i = 0; i < 8; i++){
        chess_obj[i + 8].chesses_ini(PAWN_W, i, 6);
    }
    //Black chesses
    chess_obj[16].chesses_ini(KING_B, 4, 0);
    chess_obj[17].chesses_ini(QUEEN_B, 3, 0);
    chess_obj[18].chesses_ini(ROOK_B, 0, 0);
    chess_obj[19].chesses_ini(ROOK_B, 7, 0);
    chess_obj[20].chesses_ini(BISHOP_B, 2, 0);
    chess_obj[21].chesses_ini(BISHOP_B, 5, 0);
    chess_obj[22].chesses_ini(KNIGHT_B, 6, 0);
    chess_obj[23].chesses_ini(KNIGHT_B, 1, 0);
    for(int i = 0; i < 8; i++){
        chess_obj[i + 24].chesses_ini(PAWN_B, i, 1);
    }

    //update the board
    for(int i = 0; i < 32; i++){
        chess_obj[i].existence = true;
        board[chess_obj[i].x][chess_obj[i].y] = i;  //If the chess exist, write its number onto the board
    }

    return;
}

//Reset the board when enter the edit mode
void MainWindow::Editor_set(){

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            Board_Editor[i][j] = NONE;
    int n = 0;
    for(int i = 8; i < 12; i++)
        for(int j = 0; j < 8; j++){
            Board_Editor[i][j] = n;
            _chess_img[n] -> setPos(i * 100, j * 100);
            n++;
        }
}

//Draw the scene when editting the board
void MainWindow::Editor_draw(){
    for(int i = 0; i < 12; i++)
        for(int j = 0; j < 8; j++){
            if(Board_Editor[i][j] != NONE){
                _chess_img[Board_Editor[i][j]] -> setPos(i * 100, j * 100);
            }
        }
}

//Read the position we set when start
void MainWindow::Editor_load(){

    for(int i = 0; i < 32; i++){
        chess_obj[i].existence = false;
        chess_obj[i].moved = false;
    }

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++){
            board[i][j] = NONE;
            //White chesses
            if(Board_Editor[i][j] == 0)
                chess_obj[0].chesses_ini(KING_W, i, j);
            else if(Board_Editor[i][j] == 1)
                chess_obj[1].chesses_ini(QUEEN_W, i, j);
            else if(Board_Editor[i][j] == 2)
                chess_obj[2].chesses_ini(ROOK_W, i, j);
            else if(Board_Editor[i][j] == 3)
                chess_obj[3].chesses_ini(ROOK_W, i, j);
            else if(Board_Editor[i][j] == 4)
                chess_obj[4].chesses_ini(BISHOP_W, i, j);
            else if(Board_Editor[i][j] == 5)
                chess_obj[5].chesses_ini(BISHOP_W, i, j);
            else if(Board_Editor[i][j] == 6)
                chess_obj[6].chesses_ini(KNIGHT_W, i, j);
            else if(Board_Editor[i][j] == 7)
                chess_obj[7].chesses_ini(KNIGHT_W, i, j);
            else if(Board_Editor[i][j] >= 8 && Board_Editor[i][j] <= 15)
                chess_obj[Board_Editor[i][j]].chesses_ini(PAWN_W, i, j);
            //Black chesses
            else if(Board_Editor[i][j] == 16)
                chess_obj[16].chesses_ini(KING_B, i, j);
            else if(Board_Editor[i][j] == 17)
                chess_obj[17].chesses_ini(QUEEN_B, i, j);
            else if(Board_Editor[i][j] == 18)
                chess_obj[18].chesses_ini(ROOK_B, i, j);
            else if(Board_Editor[i][j] == 19)
                chess_obj[19].chesses_ini(ROOK_B, i, j);
            else if(Board_Editor[i][j] == 20)
                chess_obj[20].chesses_ini(BISHOP_B, i, j);
            else if(Board_Editor[i][j] == 21)
                chess_obj[21].chesses_ini(BISHOP_B, i, j);
            else if(Board_Editor[i][j] == 22)
                chess_obj[22].chesses_ini(KNIGHT_B, i, j);
            else if(Board_Editor[i][j] == 23)
                chess_obj[23].chesses_ini(KNIGHT_B, i, j);
            else if(Board_Editor[i][j] >= 24 && Board_Editor[i][j] <= 31)
                chess_obj[Board_Editor[i][j]].chesses_ini(PAWN_B, i, j);
    }

    //update the board
    for(int i = 0; i < 32; i++)
        if(chess_obj[i].existence == true)
            board[chess_obj[i].x][chess_obj[i].y] = i;  //If the chess exist, write its number onto the board

    return;
}

//Set board and path to empty (used while initialize and restart)
void MainWindow::Board_and_Path_Ini(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            board[i][j] = NONE;
            show_path[i][j] = false;
        }
    }
}

//Update the latest position of the chesses
void MainWindow::DrawChesses()
{
    //Draw the chesses according to its existence and position
    for(int i = 0; i < 32; i++)
        if(chess_obj[i].existence == true){
            chess_img[i] -> show();
            chess_img[i] -> setPos(100 * chess_obj[i].x , 100 * chess_obj[i].y);
        }
        else
            chess_img[i] -> hide();
}

//Show the allowable block for the chess choosed to go
void MainWindow::DrawPath(){

    //Show the two color according to
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++){
            if(show_path[i][j] == true)
                path_grid[i][j] -> show();//If path is marked, show the red block.
            else
                path_grid[i][j] -> hide();//If not, hide it.
        }
    return;
}

//Reinitialize the path
void MainWindow::RefreshPath(){
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            show_path[i][j] = false;
}

//Move the chess
bool MainWindow::Move(int chess_num, int new_x, int new_y){
    int eat = false;
    int buf_x;
    int buf_y;
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            buf_board[i][j] = board[i][j];
    buf_x = chess_obj[chess_num].x;
    buf_y = chess_obj[chess_num].y;

    if(board[new_x][new_y] != NONE){
        chess_obj[board[new_x][new_y]].existence = false;
        eat = true;
    }

    buf_board[buf_x][buf_y] = NONE;
    chess_obj[chess_num].x = new_x;
    chess_obj[chess_num].y = new_y;
    buf_board[new_x][new_y] = chess_num;

    if(turn == BLACK){
        if(chess_obj[16].King_being_controlled(show_path, buf_board, chess_obj, attacked) == true){
            chess_obj[chess_num].x = buf_x;
            chess_obj[chess_num].y = buf_y;
            chess_obj[board[new_x][new_y]].existence = true;
            turn = !turn;
            return false;
        }
    }
    else if(turn == WHITE){
        if(chess_obj[0].King_being_controlled(show_path, buf_board, chess_obj, attacked) == true){
            chess_obj[chess_num].x = buf_x;
            chess_obj[chess_num].y = buf_y;
            chess_obj[board[new_x][new_y]].existence = true;
            turn = !turn;
            return false;
        }
    }

    if(board[new_x][new_y] != NONE){
        chess_obj[board[new_x][new_y]].existence = false;
        chess_obj[board[new_x][new_y]].x = NONE;
        chess_obj[board[new_x][new_y]].y = NONE;
    }
    board[buf_x][buf_y] = NONE;
    chess_obj[chess_num].moved = true;
    board[new_x][new_y] = chess_num;
    return true;
}

//MousePressEvents
void MainWindow::mousePressEvent(QMouseEvent *event)
{    int chess_num;
    int grid_x = (event -> x() - 10) / 100;
    int grid_y = (event -> y() - 29) / 100;

    /*If at the menu, no need to click the board.*/
    if(situation == SITUATION_MENU) return;

    /*If have chosen, show_path is built, could shoose which block to move.*/
    else if(situation == SITUATION_SELECTED){

        //If out of board, there'll be error.
        if(grid_x > 7 || grid_x < 0 || grid_y > 7 || grid_y < 0) return;

        //If chosen block is legal
        if(show_path[grid_x][grid_y] == true){
            if(chess_obj[selected_chess].x != grid_x || chess_obj[selected_chess].y != grid_y){

                //Castling
                if(selected_chess == 0 || selected_chess == 16){
                    if(chess_obj[0].moved == false && chess_obj[2].moved == false && grid_x == 2 && grid_y == 7){
                        Move(0, grid_x, grid_y);
                        Move(2, grid_x + 1, grid_y);
                    }
                    else if(chess_obj[0].moved == false && chess_obj[3].moved == false && grid_x == 6 && grid_y == 7){
                        Move(0, grid_x, grid_y);
                        Move(3, grid_x - 1, grid_y);
                    }
                    else if(chess_obj[16].moved == false && chess_obj[18].moved == false && grid_x == 2 && grid_y == 0){
                        Move(16, grid_x, grid_y);
                        Move(18, grid_x + 1, grid_y);
                    }
                    else if(chess_obj[16].moved == false && chess_obj[19].moved == false && grid_x == 6 && grid_y == 0){
                        Move(16, grid_x, grid_y);
                        Move(19, grid_x - 1, grid_y);
                    }
                    else{
                        Move(selected_chess, grid_x, grid_y);
                    }
                }
                //Normal movement
                else{
                    Move(selected_chess, grid_x, grid_y);
                }
                turn = !turn;   //change turn

                if(chess_obj[selected_chess].career == PAWN_W && grid_y == 0){
                    RefreshPath();
                    DrawPath();
                    DrawChesses();
                    situation = SITUATION_PROMOTION_W;
                    UIswitching(situation);
                    return;
                }
                else if(chess_obj[selected_chess].career == PAWN_B && grid_y == 7){
                    RefreshPath();
                    DrawPath();
                    DrawChesses();
                    situation = SITUATION_PROMOTION_B;
                    UIswitching(situation);
                    return;
                }
            }
        }

        //No matter if there's movement excuted, cancel the showing of the path.
        RefreshPath();
        ui -> IllegalMove -> hide();
        if(turn == WHITE){
                if(white_lose() == true) {
                    qDebug() << "white lose";
                    situation = SITUATION_BLACKWIN;
                    DrawChesses();
                    RefreshPath();
                    DrawPath();
                    during_game = false;
                    return;
                }
            }
        if(turn == BLACK){
                if(black_lose() == true){
                    qDebug() << "black lose";
                    situation = SITUATION_WHITEWIN;
                    DrawChesses();
                    RefreshPath();
                    DrawPath();
                    during_game = false;
                    return;
                }
       }
        if(chess_obj[0].King_being_controlled(show_path, buf_board, chess_obj, attacked) == true || chess_obj[16].King_being_controlled(show_path, buf_board, chess_obj, attacked) == true){
            ui -> IllegalMove -> show();
        }
        situation = SITUATION_TURN;
    }


    /*State for any color to choose chess to move.*/
    else if(situation == SITUATION_TURN){
        //If out of board, there'll be error.
        if(grid_x > 7 || grid_x < 0 || grid_y > 7 || grid_y < 0) return;

        //Compute the point choosed.
        chess_num = board[grid_x][grid_y];

        if(turn == WHITE){

            //Queen~Pawn
            if(board[grid_x][grid_y] >= 1 && board[grid_x][grid_y] <= 15){
                chess_obj[chess_num].white_path_compute(show_path, board);
                selected_chess = chess_num;                                 //Remember the chess choosed
                situation = SITUATION_SELECTED;                             //Change the state
            }
            //King
            else if(board[grid_x][grid_y] == 0){
                chess_obj[0].whiteKING_path_compute(show_path, board, chess_obj, attacked);
                selected_chess = chess_num;
                situation = SITUATION_SELECTED;
            }
        }

        //Turn of black
        else{
            //Queen~Pawn
            if(board[grid_x][grid_y] >= 17 && board[grid_x][grid_y] <= 31){
                chess_obj[chess_num].black_path_compute(show_path, board);  //Contain black chess
                selected_chess = chess_num;                                 //Remember the chess choosed
                situation = SITUATION_SELECTED;                             //Change the state
            }
            //King
            else if(board[grid_x][grid_y] == 16){
                chess_obj[16].blackKING_path_compute(show_path, board, chess_obj, attacked);
                selected_chess = chess_num;
                situation = SITUATION_SELECTED;
            }
         }
    }

    else if(situation == SITUATION_EDITOR){
        if(grid_x >= 0 && grid_x <= 11 && grid_y >= 0 && grid_y <= 7){
            if(editor_x == NONE){
                editor_x = grid_x;
                editor_y = grid_y;
                selector -> setPos(100 * grid_x, 100 * grid_y);
                selector -> show();
            }
            else{
                int buf = Board_Editor[editor_x][editor_y];
                Board_Editor[editor_x][editor_y] = Board_Editor[grid_x][grid_y];
                Board_Editor[grid_x][grid_y] = buf;
                editor_x = NONE;
                editor_y = NONE;
                selector -> hide();
            }
        }
        Editor_draw();
    }
    UIswitching(situation);
    DrawPath();
    DrawChesses();
}

//Switching UI elements when different condition
void MainWindow::UIswitching(int state){
    //Set it to Menu state
    if(state == SITUATION_MENU){
        if(during_game == true)
            ui -> Continue -> show();
        else ui -> Continue -> hide();
        //Menu elements
        ui -> StartByEdited -> show();
        ui -> StartByDefaultWhite -> show();
        ui -> StartByDefaultBlack -> show();
        ui -> TITLE -> show();
        ui -> TITLE_2 -> show();
        ui -> BlackWin -> hide();
        ui -> WhiteWin -> hide();
        //GameBoard elements
        ui -> IllegalMove -> hide();
        ui -> graphicsView -> hide();
        ui -> black_turn -> hide();
        ui -> white_turn -> hide();
        ui -> BackToMenu -> hide();
        //Promotion elements
        ui -> Promotion -> hide();
        ui -> PromotionToRook_W -> hide();
        ui -> PromotionToQueen_W -> hide();
        ui -> PromotionToBishop_W -> hide();
        ui -> PromotionToKnight_W -> hide();
        //Promotion elements
        ui -> Promotion -> hide();
        ui -> PromotionToRook_W -> hide();
        ui -> PromotionToQueen_W -> hide();
        ui -> PromotionToBishop_W -> hide();
        ui -> PromotionToKnight_W -> hide();
        ui -> PromotionToRook_B -> hide();
        ui -> PromotionToQueen_B -> hide();
        ui -> PromotionToBishop_B -> hide();
        ui -> PromotionToKnight_B -> hide();
        //Editor elements
        ui -> BoardEditor -> hide();
        ui -> EditorStart -> hide();
        ui -> EditorBlackFirst -> hide();
        ui -> EditorWhiteFirst -> hide();
    }
    else if(situation == SITUATION_TURN){

        if(turn == BLACK){
            ui ->white_turn -> hide();
            ui ->black_turn -> show();
        }
        else{
            ui ->black_turn -> hide();
            ui ->white_turn -> show();
        }
        //Menu elements
        ui -> StartByEdited -> hide();
        ui -> StartByDefaultWhite -> hide();
        ui -> StartByDefaultBlack -> hide();
        ui -> TITLE -> hide();
        ui -> TITLE_2 -> hide();
        ui -> Continue -> hide();
        //GameBoard elements
        ui -> graphicsView ->show();
        ui -> BackToMenu -> show();
        //Promotion elements
        ui -> Promotion -> hide();
        ui -> PromotionToRook_W -> hide();
        ui -> PromotionToQueen_W -> hide();
        ui -> PromotionToBishop_W -> hide();
        ui -> PromotionToKnight_W -> hide();
        ui -> PromotionToRook_B -> hide();
        ui -> PromotionToQueen_B -> hide();
        ui -> PromotionToBishop_B -> hide();
        ui -> PromotionToKnight_B -> hide();
        //Editor elements
        ui -> BoardEditor -> hide();
        ui -> EditorStart -> hide();
        ui -> EditorBlackFirst -> hide();
        ui -> EditorWhiteFirst -> hide();
    }
    else if(situation == SITUATION_PROMOTION_W){

        //GameBoard elements
        ui -> IllegalMove -> hide();
        ui -> BackToMenu -> hide();
        //Promotion elements
        ui -> Promotion -> show();
        ui -> PromotionToRook_W -> show();
        ui -> PromotionToQueen_W -> show();
        ui -> PromotionToBishop_W -> show();
        ui -> PromotionToKnight_W -> show();
    }
    else if(situation == SITUATION_PROMOTION_B){

        //GameBoard elements
        ui -> IllegalMove -> hide();
        ui -> BackToMenu -> hide();
        //Promotion elements
        ui -> Promotion -> show();
        ui -> PromotionToRook_B -> show();
        ui -> PromotionToQueen_B -> show();
        ui -> PromotionToBishop_B -> show();
        ui -> PromotionToKnight_B -> show();
    }
    else if(situation == SITUATION_BLACKWIN){
        ui -> BlackWin -> show();
    }
    else if(situation == SITUATION_WHITEWIN){
        ui -> WhiteWin -> show();
    }
    else if(situation == SITUATION_EDITOR){
        //Editor elements
        ui -> BoardEditor -> show();
        ui -> EditorStart -> show();
        ui -> EditorBlackFirst -> show();
        ui -> EditorWhiteFirst -> show();
    }
}

//Judge if black lose
bool MainWindow::black_lose(){
    bool pass = false;
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            buf2_board[i][j] = board[i][j];

    //Check for the king
    for(int i = chess_obj[16].x - 1; i <= chess_obj[16].x + 1; i++){
        if(pass == true) break;
        for(int j = chess_obj[16].y - 1; j <= chess_obj[16].y + 1; j++){
            if(i != chess_obj[16].x || j != chess_obj[16].y)
                if(i <= 7 && i >= 0 && j <= 7 && j >= 0)    //Legal block
                    if(buf2_board[i][j] < 16){   //Not black
                        turn = BLACK;
                        if(Move(16, i, j) == true){
                            pass = true;
                            break;
                        }
                    }
        }
    }

    //Check for Queen
    if(chess_obj[17].existence == true){
        //Row and Column
        if(pass == false)
            for(int i = chess_obj[17].x + 1;; i++){
                if(i <= 7){
                    if(buf2_board[i][chess_obj[17].y] < 16){
                        turn = BLACK;
                        if(Move(17, i, chess_obj[17].y) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[i][chess_obj[17].y] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
        if(pass == false)
            for(int i = chess_obj[17].x - 1;; i--){
                if(i >= 0){
                    if(buf2_board[i][chess_obj[17].y] < 16){
                        turn = BLACK;
                        if(Move(17, i, chess_obj[17].y) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[i][chess_obj[17].y] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
        if(pass == false)
            for(int i = chess_obj[17].y + 1;; i++){
                if(i <= 7){
                    if(buf2_board[chess_obj[17].x][i] < 16){
                        turn = BLACK;
                        if(Move(17, chess_obj[17].x, i) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[chess_obj[17].x][i] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
        if(pass == false)
            for(int i = chess_obj[17].y - 1;; i--){
                if(i >= 0){
                    if(buf2_board[chess_obj[17].x][i] < 16){
                        turn = BLACK;
                        if(Move(17, chess_obj[17].x, i) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[chess_obj[17].x][i] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
        //Diagonal
        if(pass == false)
            for(int i = 1;; i++){
                if(chess_obj[17].x + i <= 7 && chess_obj[17].x + i >= 0 && chess_obj[17].y + i <= 7 && chess_obj[17].y + i >= 0){
                    if(buf2_board[chess_obj[17].x + i][chess_obj[17].y + i] < 16){
                        turn = BLACK;
                        if(Move(17, chess_obj[17].x + i, chess_obj[17].y + i) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[chess_obj[17].x + i][chess_obj[17].y + i] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
        if(pass == false)
            for(int i = 1;; i++){
                if(chess_obj[17].x + i <= 7 && chess_obj[17].x + i >= 0 && chess_obj[17].y - i <= 7 && chess_obj[17].y - i >= 0){
                    if(buf2_board[chess_obj[17].x + i][chess_obj[17].y - i] < 16){
                        turn = BLACK;
                        if(Move(17, chess_obj[17].x + i, chess_obj[17].y - i) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[chess_obj[17].x + i][chess_obj[17].y - i] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
        if(pass == false)
            for(int i = 1;; i++){
                if(chess_obj[17].x - i <= 7 && chess_obj[17].x - i >= 0 && chess_obj[17].y + i <= 7 && chess_obj[17].y + i >= 0){
                    if(buf2_board[chess_obj[17].x - i][chess_obj[17].y + i] < 16){
                        turn = BLACK;
                        if(Move(17, chess_obj[17].x - i, chess_obj[17].y + i) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[chess_obj[17].x - i][chess_obj[17].y + i] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
        if(pass == false)
            for(int i = 1;; i++){
                if(chess_obj[17].x - i <= 7 && chess_obj[17].x - i >= 0 && chess_obj[17].y - i <= 7 && chess_obj[17].y - i >= 0){
                    if(buf2_board[chess_obj[17].x - i][chess_obj[17].y - i] < 16){
                        turn = BLACK;
                        if(Move(17, chess_obj[17].x - i, chess_obj[17].y - i) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[chess_obj[17].x - i][chess_obj[17].y - i] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
    }

    //Rook
    for(int n = 18; n <= 19; n++){
        if(chess_obj[n].existence == true){
            //Row and Column
            if(pass == false)
                for(int i = chess_obj[n].x + 1;; i++){
                    if(i <= 7){
                        if(buf2_board[i][chess_obj[n].y] < 16){
                            turn = BLACK;
                            if(Move(n, i, chess_obj[n].y) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[i][chess_obj[n].y] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
            if(pass == false)
                for(int i = chess_obj[n].x - 1;; i--){
                    if(i >= 0){
                        if(buf2_board[i][chess_obj[n].y] < 16){
                            turn = BLACK;
                            if(Move(n, i, chess_obj[n].y) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[i][chess_obj[n].y] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
            if(pass == false)
                for(int i = chess_obj[n].y + 1;; i++){
                    if(i <= 7){
                        if(buf2_board[chess_obj[n].x][i] < 16){
                            turn = BLACK;
                            if(Move(n, chess_obj[n].x, i) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[chess_obj[n].x][i] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
            if(pass == false)
                for(int i = chess_obj[n].y - 1;; i--){
                    if(i >= 0){
                        if(buf2_board[chess_obj[n].x][i] < 16){
                            turn = BLACK;
                            if(Move(n, chess_obj[n].x, i) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[chess_obj[n].x][i] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
        }
        if(pass == true) break;
    }

    //Bishop
    for(int n = 20; n <= 21; n++){
        if(chess_obj[n].existence == true){
            //Diagonal
            if(pass == false)
                for(int i = 1;; i++){
                    if(chess_obj[n].x + i <= 7 && chess_obj[n].x + i >= 0 && chess_obj[n].y + i <= 7 && chess_obj[n].y + i >= 0){
                        if(buf2_board[chess_obj[n].x + i][chess_obj[n].y + i] < 16){
                            turn = BLACK;
                            if(Move(n, chess_obj[n].x + i, chess_obj[n].y + i) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[chess_obj[n].x + i][chess_obj[n].y + i] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
            if(pass == false)
                for(int i = 1;; i++){
                    if(chess_obj[n].x + i <= 7 && chess_obj[n].x + i >= 0 && chess_obj[n].y - i <= 7 && chess_obj[n].y - i >= 0){
                        if(buf2_board[chess_obj[n].x + i][chess_obj[n].y - i] < 16){
                            turn = BLACK;
                            if(Move(n, chess_obj[n].x + i, chess_obj[n].y - i) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[chess_obj[n].x + i][chess_obj[n].y - i] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
            if(pass == false)
                for(int i = 1;; i++){
                    if(chess_obj[n].x - i <= 7 && chess_obj[n].x - i >= 0 && chess_obj[n].y + i <= 7 && chess_obj[n].y + i >= 0){
                        if(buf2_board[chess_obj[n].x - i][chess_obj[n].y + i] < 16){
                            turn = BLACK;
                            if(Move(n, chess_obj[n].x - i, chess_obj[n].y + i) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[chess_obj[n].x - i][chess_obj[n].y + i] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
            if(pass == false)
                for(int i = 1;; i++){
                    if(chess_obj[n].x - i <= 7 && chess_obj[n].x - i >= 0 && chess_obj[n].y - i <= 7 && chess_obj[n].y - i >= 0){
                        if(buf2_board[chess_obj[n].x - i][chess_obj[n].y - i] < 16){
                            turn = BLACK;
                            if(Move(n, chess_obj[n].x - i, chess_obj[n].y - i) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[chess_obj[n].x - i][chess_obj[n].y - i] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
        }
        if(pass == true) break;
    }

    //Knight
    for(int n = 22; n <= 23; n++){
        if(chess_obj[n].existence == true){
            if(pass == false){
                for(int i = chess_obj[n].x - 2; i <= chess_obj[n].x + 2; i++){
                    if(i == chess_obj[n].x) continue;
                    if(i == chess_obj[n].x - 1 || i == chess_obj[n].x + 1){
                        for(int j = chess_obj[n].y - 2; j <= chess_obj[n].y + 2; j += 4){
                            if(i >= 0 && i <= 7 && j >= 0 && j <= 7)
                                if(buf2_board[i][j] < 16){ //Not black
                                    turn = BLACK;
                                    if(Move(n, i, j) == true){
                                        pass = true;
                                        break;
                                    }
                                }
                    }}
                    else if(i == chess_obj[n].x - 2 || i == chess_obj[n].x + 2){
                        for(int j = chess_obj[n].y - 1; j <= chess_obj[n].y + 1; j += 2){
                            if(i >= 0 && i <= 7 && j >= 0 && j <= 7)
                                if(buf2_board[i][j] < 16){ //Not black
                                    turn = BLACK;
                                    if(Move(n, i, j) == true){
                                        pass = true;
                                        break;
                                    }
                                }
                        }
                    }
                    if(pass == true) break;
                }
            }
        }
        if(pass == true) break;
    }

    //Pawn
    for(int n = 24; n <= 31; n++){
        if(chess_obj[n].existence == true){
            if(pass == false){
                if(chess_obj[n].x <= 6 && chess_obj[n].y <= 6)
                    if(buf2_board[chess_obj[n].x + 1][chess_obj[n].y + 1] != NONE && buf2_board[chess_obj[n].x + 1][chess_obj[n].y + 1] < 16){
                        turn = BLACK;
                        if(Move(n, chess_obj[n].x + 1, chess_obj[n].y + 1) == true){
                            pass = true;
                            break;
                        }
                    }
            }
            if(pass == false){
                if(chess_obj[n].x >= 1 && chess_obj[n].y <= 6)
                    if(buf2_board[chess_obj[n].x - 1][chess_obj[n].y + 1] != NONE && buf2_board[chess_obj[n].x - 1][chess_obj[n].y + 1] < 16){
                        turn = BLACK;
                        if(Move(n, chess_obj[n].x - 1, chess_obj[n].y + 1) == true){
                            pass = true;
                            break;
                        }
                    }
            }
            if(pass == false){
                if(chess_obj[n].y <= 6)
                    if(buf2_board[chess_obj[n].x][chess_obj[n].y + 1] == NONE){
                        turn = BLACK;
                        if(Move(n, chess_obj[n].x, chess_obj[n].y + 1) == true){
                            pass = true;
                            break;
                        }
                    }
            }
        }
        if(pass == true) break;
    }

    if(pass == true){
        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++){
                board[i][j] = buf2_board[i][j];
                if(buf2_board[i][j] != NONE){
                    chess_obj[buf2_board[i][j]].existence = true;
                    chess_obj[buf2_board[i][j]].x = i;
                    chess_obj[buf2_board[i][j]].y = j;
                }
            }
        RefreshPath();
        turn = BLACK;
        return false;
    }

    RefreshPath();
    return true;
}

//Judge if white lose
bool MainWindow::white_lose(){
    bool pass = false;
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
            buf2_board[i][j] = board[i][j];

    //Check for the king
    for(int i = chess_obj[0].x - 1; i <= chess_obj[0].x + 1; i++){
        if(pass == true) break;
        for(int j = chess_obj[0].y - 1; j <= chess_obj[0].y + 1; j++){
            if(i != chess_obj[0].x || j != chess_obj[0].y)
                if(i <= 7 && i >= 0 && j <= 7 && j >= 0)    //Legal block
                    if(buf2_board[i][j] > 15 || buf2_board[i][j] == NONE){   //Not white
                        turn = WHITE;
                        if(Move(0, i, j) == true){
                            pass = true;
                            break;
                        }
                    }
        }
    }

    //Check for Queen
    if(chess_obj[1].existence == true){
        //Row and Column
        if(pass == false)
            for(int i = chess_obj[1].x + 1;; i++){
                if(i <= 7){
                    if(buf2_board[i][chess_obj[1].y] > 15 || buf2_board[i][chess_obj[1].y] == NONE){
                        turn = WHITE;
                        if(Move(1, i, chess_obj[1].y) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[i][chess_obj[1].y] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
        if(pass == false)
            for(int i = chess_obj[1].x - 1;; i--){
                if(i >= 0){
                    if(buf2_board[i][chess_obj[1].y] > 15 || buf2_board[i][chess_obj[1].y] == NONE){
                        turn = WHITE;
                        if(Move(1, i, chess_obj[1].y) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[i][chess_obj[1].y] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
        if(pass == false)
            for(int i = chess_obj[1].y + 1;; i++){
                if(i <= 7){
                    if(buf2_board[chess_obj[1].x][i] > 15 || buf2_board[chess_obj[1].x][i] == NONE){
                        turn = WHITE;
                        if(Move(1, chess_obj[1].x, i) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[chess_obj[1].x][i] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
        if(pass == false)
            for(int i = chess_obj[1].y - 1;; i--){
                if(i >= 0){
                    if(buf2_board[chess_obj[1].x][i] > 15 || buf2_board[chess_obj[1].x][i] == NONE){
                        turn = WHITE;
                        if(Move(1, chess_obj[1].x, i) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[chess_obj[1].x][i] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
        //Diagonal
        if(pass == false)
            for(int i = 1;; i++){
                if(chess_obj[1].x + i <= 7 && chess_obj[1].x + i >= 0 && chess_obj[1].y + i <= 7 && chess_obj[1].y + i >= 0){
                    if(buf2_board[chess_obj[1].x + i][chess_obj[1].y + i] > 15 || buf2_board[chess_obj[1].x + i][chess_obj[1].y + i] == NONE){
                        turn = WHITE;
                        if(Move(1, chess_obj[1].x + i, chess_obj[1].y + i) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[chess_obj[1].x + i][chess_obj[1].y + i] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
        if(pass == false)
            for(int i = 1;; i++){
                if(chess_obj[1].x + i <= 7 && chess_obj[1].x + i >= 0 && chess_obj[1].y - i <= 7 && chess_obj[1].y - i >= 0){
                    if(buf2_board[chess_obj[1].x + i][chess_obj[1].y - i] > 15 || buf2_board[chess_obj[1].x + i][chess_obj[1].y - i] == NONE){
                        turn = WHITE;
                        if(Move(1, chess_obj[1].x + i, chess_obj[1].y - i) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[chess_obj[1].x + i][chess_obj[1].y - i] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
        if(pass == false)
            for(int i = 1;; i++){
                if(chess_obj[1].x - i <= 7 && chess_obj[1].x - i >= 0 && chess_obj[1].y + i <= 7 && chess_obj[1].y + i >= 0){
                    if(buf2_board[chess_obj[1].x - i][chess_obj[1].y + i] > 15 || buf2_board[chess_obj[1].x - i][chess_obj[1].y + i] == NONE){
                        turn = WHITE;
                        if(Move(1, chess_obj[1].x - i, chess_obj[1].y + i) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[chess_obj[1].x - i][chess_obj[1].y + i] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
        if(pass == false)
            for(int i = 1;; i++){
                if(chess_obj[1].x - i <= 7 && chess_obj[1].x - i >= 0 && chess_obj[1].y - i <= 7 && chess_obj[1].y - i >= 0){
                    if(buf2_board[chess_obj[1].x - i][chess_obj[1].y - i] > 15 || buf2_board[chess_obj[1].x - i][chess_obj[1].y - i] == NONE){
                        turn = WHITE;
                        if(Move(1, chess_obj[1].x - i, chess_obj[1].y - i) == true){
                            pass = true;
                            break;
                        }
                        if(buf2_board[chess_obj[1].x - i][chess_obj[1].y - i] != NONE) break;
                    }
                    else break;
                }
                else break;
            }
    }

    //Rook
    for(int n = 2; n <= 3; n++){
        if(chess_obj[n].existence == true){
            //Row and Column
            if(pass == false)
                for(int i = chess_obj[n].x + 1;; i++){
                    if(i <= 7){
                        if(buf2_board[i][chess_obj[n].y] > 15 || buf2_board[i][chess_obj[n].y] == NONE){
                            turn = WHITE;
                            if(Move(n, i, chess_obj[n].y) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[i][chess_obj[n].y] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
            if(pass == false)
                for(int i = chess_obj[n].x - 1;; i--){
                    if(i >= 0){
                        if(buf2_board[i][chess_obj[n].y] > 15 || buf2_board[i][chess_obj[n].y] == NONE){
                            turn = WHITE;
                            if(Move(n, i, chess_obj[n].y) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[i][chess_obj[n].y] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
            if(pass == false)
                for(int i = chess_obj[n].y + 1;; i++){
                    if(i <= 7){
                        if(buf2_board[chess_obj[n].x][i] > 15 || buf2_board[chess_obj[n].x][i] == NONE){
                            turn = WHITE;
                            if(Move(n, chess_obj[n].x, i) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[chess_obj[n].x][i] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
            if(pass == false)
                for(int i = chess_obj[n].y - 1;; i--){
                    if(i >= 0){
                        if(buf2_board[chess_obj[n].x][i] > 15 || buf2_board[chess_obj[n].x][i] == NONE){
                            turn = WHITE;
                            if(Move(n, chess_obj[n].x, i) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[chess_obj[n].x][i] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
        }
        if(pass == true) break;
    }

    //Bishop
    for(int n = 4; n <= 5; n++){
        if(chess_obj[n].existence == true){
            //Diagonal
            if(pass == false)
                for(int i = 1;; i++){
                    if(chess_obj[n].x + i <= 7 && chess_obj[n].x + i >= 0 && chess_obj[n].y + i <= 7 && chess_obj[n].y + i >= 0){
                        if(buf2_board[chess_obj[n].x + i][chess_obj[n].y + i] > 15 || buf2_board[chess_obj[n].x + i][chess_obj[n].y + i] == NONE){
                            turn = WHITE;
                            if(Move(n, chess_obj[n].x + i, chess_obj[n].y + i) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[chess_obj[n].x + i][chess_obj[n].y + i] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
            if(pass == false)
                for(int i = 1;; i++){
                    if(chess_obj[n].x + i <= 7 && chess_obj[n].x + i >= 0 && chess_obj[n].y - i <= 7 && chess_obj[n].y - i >= 0){
                        if(buf2_board[chess_obj[n].x + i][chess_obj[n].y - i] > 15 || buf2_board[chess_obj[n].x + i][chess_obj[n].y - i] == NONE){
                            turn = WHITE;
                            if(Move(n, chess_obj[n].x + i, chess_obj[n].y - i) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[chess_obj[n].x + i][chess_obj[n].y - i] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
            if(pass == false)
                for(int i = 1;; i++){
                    if(chess_obj[n].x - i <= 7 && chess_obj[n].x - i >= 0 && chess_obj[n].y + i <= 7 && chess_obj[n].y + i >= 0){
                        if(buf2_board[chess_obj[n].x - i][chess_obj[n].y + i] > 15 || buf2_board[chess_obj[n].x - i][chess_obj[n].y + i] == NONE){
                            turn = WHITE;
                            if(Move(n, chess_obj[n].x - i, chess_obj[n].y + i) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[chess_obj[n].x - i][chess_obj[n].y + i] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
            if(pass == false)
                for(int i = 1;; i++){
                    if(chess_obj[n].x - i <= 7 && chess_obj[n].x - i >= 0 && chess_obj[n].y - i <= 7 && chess_obj[n].y - i >= 0){
                        if(buf2_board[chess_obj[n].x - i][chess_obj[n].y - i] > 15 || buf2_board[chess_obj[n].x - i][chess_obj[n].y - i] == NONE){
                            turn = WHITE;
                            if(Move(n, chess_obj[n].x - i, chess_obj[n].y - i) == true){
                                pass = true;
                                break;
                            }
                            if(buf2_board[chess_obj[n].x - i][chess_obj[n].y - i] != NONE) break;
                        }
                        else break;
                    }
                    else break;
                }
        }
        if(pass == true) break;
    }

    //Knight
    for(int n = 6; n <= 7; n++){
        if(chess_obj[n].existence == true){
            if(pass == false){
                for(int i = chess_obj[n].x - 2; i <= chess_obj[n].x + 2; i++){
                    if(i == chess_obj[n].x) continue;
                    if(i == chess_obj[n].x - 1 || i == chess_obj[n].x + 1){
                        for(int j = chess_obj[n].y - 2; j <= chess_obj[n].y + 2; j += 4){
                            if(i >= 0 && i <= 7 && j >= 0 && j <= 7)
                                if(buf2_board[i][j] > 15 || buf2_board[i][j] == NONE){ //Not black
                                    turn = WHITE;
                                    if(Move(n, i, j) == true){
                                        pass = true;
                                        break;
                                    }
                                }
                    }}
                    else if(i == chess_obj[n].x - 2 || i == chess_obj[n].x + 2){
                        for(int j = chess_obj[n].y - 1; j <= chess_obj[n].y + 1; j += 2){
                            if(i >= 0 && i <= 7 && j >= 0 && j <= 7)
                                if(buf2_board[i][j] > 15 || buf2_board[i][j] == NONE){ //Not black
                                    turn = WHITE;
                                    if(Move(n, i, j) == true){
                                        pass = true;
                                        break;
                                    }
                                }
                        }
                    }
                    if(pass == true) break;
                }
            }
        }
        if(pass == true) break;
    }

    //Pawn
    for(int n = 8; n <= 15; n++){
        if(chess_obj[n].existence == true){
            if(pass == false){
                if(chess_obj[n].x <= 6 && chess_obj[n].y >= 1)
                    if(buf2_board[chess_obj[n].x + 1][chess_obj[n].y - 1] > 15){
                        turn = WHITE;
                        if(Move(n, chess_obj[n].x + 1, chess_obj[n].y - 1) == true){
                            pass = true;
                            break;
                        }
                    }
            }
            if(pass == false){
                if(chess_obj[n].x >= 1 && chess_obj[n].y >= 1)
                    if(buf2_board[chess_obj[n].x - 1][chess_obj[n].y - 1] > 15){
                        turn = WHITE;
                        if(Move(n, chess_obj[n].x - 1, chess_obj[n].y - 1) == true){
                            pass = true;
                            break;
                        }
                    }
            }
            if(pass == false){
                if(chess_obj[n].y >= 1)
                    if(buf2_board[chess_obj[n].x][chess_obj[n].y - 1] == NONE){
                        turn = WHITE;
                        if(Move(n, chess_obj[n].x, chess_obj[n].y - 1) == true){
                            pass = true;
                            break;
                        }
                    }
            }
        }
        if(pass == true) break;
    }

    if(pass == true){
        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++){
                board[i][j] = buf2_board[i][j];
                if(buf2_board[i][j] != NONE){
                    chess_obj[buf2_board[i][j]].existence = true;
                    chess_obj[buf2_board[i][j]].x = i;
                    chess_obj[buf2_board[i][j]].y = j;
                }
            }
        turn = WHITE;
        RefreshPath();
        return false;
    }

    RefreshPath();
    return true;
}

//Start by default set of the board, white first
void MainWindow::on_StartByDefaultWhite_clicked()
{
    Board_and_Path_Ini();
    Default_set();
    PicsUpdating();
    turn = WHITE;
    situation = SITUATION_TURN;
    during_game = true;
    DrawPath();
    DrawChesses();
    UIswitching(situation);
}

//Start by default set of the board, black first
void MainWindow::on_StartByDefaultBlack_clicked()
{
    Board_and_Path_Ini();
    Default_set();
    PicsUpdating();
    turn = BLACK;
    during_game = true;
    DrawChesses();
    DrawPath();
    situation = SITUATION_TURN;
    UIswitching(situation);
}

//Back to menu
void MainWindow::on_BackToMenu_clicked()
{
    situation = SITUATION_MENU;
    UIswitching(situation);
}

//Back to game
void MainWindow::on_Continue_clicked()
{
    situation = SITUATION_TURN;
    UIswitching(situation);
}

//Promotion buttons
void MainWindow::on_PromotionToBishop_B_clicked()
{
    chess_obj[selected_chess].career = BISHOP_B;
    chess_img[selected_chess] -> setPixmap(QPixmap(":/img/Bishop_b.png").scaled(100, 100));
    situation = SITUATION_TURN;
    UIswitching(situation);
    chess_obj[0].King_being_controlled(show_path, buf_board, chess_obj, attacked);
    DrawPath();
}

void MainWindow::on_PromotionToKnight_B_clicked()
{
    chess_obj[selected_chess].career = KNIGHT_B;
    chess_img[selected_chess] -> setPixmap(QPixmap(":/img/Knight_b.png").scaled(100, 100));
    situation = SITUATION_TURN;
    UIswitching(situation);
    chess_obj[0].King_being_controlled(show_path, buf_board, chess_obj, attacked);
    DrawPath();
}

void MainWindow::on_PromotionToQueen_B_clicked()
{
    chess_obj[selected_chess].career = QUEEN_B;
    chess_img[selected_chess] -> setPixmap(QPixmap(":/img/Queen_b.png").scaled(100, 100));
    situation = SITUATION_TURN;
    UIswitching(situation);
    chess_obj[0].King_being_controlled(show_path, buf_board, chess_obj, attacked);
    DrawPath();
}

void MainWindow::on_PromotionToRook_B_clicked()
{
    chess_obj[selected_chess].career = ROOK_B;
    chess_img[selected_chess] -> setPixmap(QPixmap(":/img/Rook_b.png").scaled(100, 100));
    situation = SITUATION_TURN;
    UIswitching(situation);
    chess_obj[0].King_being_controlled(show_path, buf_board, chess_obj, attacked);
    DrawPath();
}

void MainWindow::on_PromotionToBishop_W_clicked()
{
    chess_obj[selected_chess].career = BISHOP_W;
    chess_img[selected_chess] -> setPixmap(QPixmap(":/img/Bishop_w.png").scaled(100, 100));
    situation = SITUATION_TURN;
    UIswitching(situation);
    chess_obj[16].King_being_controlled(show_path, buf_board, chess_obj, attacked);
    DrawPath();
}

void MainWindow::on_PromotionToKnight_W_clicked()
{
    chess_obj[selected_chess].career = KNIGHT_W;
    chess_img[selected_chess] -> setPixmap(QPixmap(":/img/Knight_w.png").scaled(100, 100));
    situation = SITUATION_TURN;
    UIswitching(situation);
    chess_obj[16].King_being_controlled(show_path, buf_board, chess_obj, attacked);
    DrawPath();
}

void MainWindow::on_PromotionToQueen_W_clicked()
{
    chess_obj[selected_chess].career = QUEEN_W;
    chess_img[selected_chess] -> setPixmap(QPixmap(":/img/Queen_w.png").scaled(100, 100));
    situation = SITUATION_TURN;
    UIswitching(situation);
    chess_obj[16].King_being_controlled(show_path, buf_board, chess_obj, attacked);
    DrawPath();
}

void MainWindow::on_PromotionToRook_W_clicked()
{
    chess_obj[selected_chess].career = ROOK_W;
    chess_img[selected_chess] -> setPixmap(QPixmap(":/img/Rook_w.png").scaled(100, 100));
    situation = SITUATION_TURN;
    UIswitching(situation);
    chess_obj[16].King_being_controlled(show_path, buf_board, chess_obj, attacked);
    DrawPath();
}

void MainWindow::on_StartByEdited_clicked()
{
    Editor_set();
    situation = SITUATION_EDITOR;
    UIswitching(situation);
}

void MainWindow::on_EditorWhiteFirst_clicked()
{
    Board_and_Path_Ini();
    Editor_load();
    PicsUpdating();
    turn = WHITE;
    during_game = true;
    DrawChesses();
    DrawPath();
    situation = SITUATION_TURN;
    UIswitching(situation);
}

void MainWindow::on_EditorBlackFirst_clicked()
{
    Board_and_Path_Ini();
    Editor_load();
    PicsUpdating();
    turn = BLACK;
    during_game = true;
    DrawChesses();
    DrawPath();
    situation = SITUATION_TURN;
    UIswitching(situation);
}
