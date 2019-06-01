#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "chesses.h"
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QDebug>
#include <QPixmap>
#include <QPainter>


#define SITUATION_TURN          401
#define SITUATION_SELECTED      402
#define SITUATION_MENU          403
#define SITUATION_BLACKWIN      404
#define SITUATION_WHITEWIN      405
#define SITUATION_PROMOTION_W   406
#define SITUATION_PROMOTION_B   407
#define SITUATION_EDITOR        408

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void DrawChesses();
    void DrawPath();
    void PicsLoaded();
    void PicsUpdating();
    void Board_and_Path_Ini();
    void Default_set();
    void Editor_set();
    void Editor_draw();
    void Editor_load();
    void RefreshPath();
    bool Move(int, int, int);
    void UIswitching(int);
    void mousePressEvent(QMouseEvent *event);
    bool black_lose();
    bool white_lose();

private slots:
    void on_StartByDefaultWhite_clicked();
    void on_StartByDefaultBlack_clicked();
    void on_BackToMenu_clicked();
    void on_Continue_clicked();
    void on_PromotionToBishop_B_clicked();
    void on_PromotionToKnight_B_clicked();
    void on_PromotionToQueen_B_clicked();
    void on_PromotionToRook_B_clicked();
    void on_PromotionToBishop_W_clicked();
    void on_PromotionToKnight_W_clicked();
    void on_PromotionToQueen_W_clicked();
    void on_PromotionToRook_W_clicked();

    void on_StartByEdited_clicked();

    void on_EditorWhiteFirst_clicked();

    void on_EditorBlackFirst_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsScene *board_editor;
    QGraphicsPixmapItem **chess_img;
    QGraphicsPixmapItem **_chess_img;
    QGraphicsPixmapItem ***path_grid;
    QGraphicsPixmapItem *selector;
    chesses *chess_obj;
    int **Board_Editor;
    bool **show_path;
    bool **attacked;
    int **board;
    int **buf_board;
    int **buf2_board;
    int situation;
    bool turn;
    int selected_chess;
    bool during_game;
    int editor_x;
    int editor_y;

};

#endif // MAINWINDOW_H
