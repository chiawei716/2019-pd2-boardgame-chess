#ifndef CHESSES_H
#define CHESSES_H

#define KING_W      301
#define QUEEN_W     302
#define ROOK_W      303
#define BISHOP_W    304
#define KNIGHT_W    305
#define PAWN_W      306
#define KING_B      307
#define QUEEN_B     308
#define ROOK_B      309
#define BISHOP_B    310
#define KNIGHT_B    311
#define PAWN_B      312
#define WHITE       true
#define BLACK       false
#define NONE        -1

#include <QObject>

class chesses : public QObject
{
    Q_OBJECT
public:
    explicit chesses(QObject *parent = nullptr);
    void chesses_ini(int, int, int);
    void white_path_compute(bool **, int **);
    void whiteKING_path_compute(bool **, int **,  chesses *, bool **);
    void black_path_compute(bool **, int **);
    void blackKING_path_compute(bool **, int **,  chesses *, bool **);
    int career; //Number of the chess ( DEFINED )
    bool existence = false;
    bool moved = false;
    int x, y;
    bool King_being_controlled(bool **, int **, chesses *, bool **);
signals:

public slots:

private:


};

#endif // CHESSES_H
