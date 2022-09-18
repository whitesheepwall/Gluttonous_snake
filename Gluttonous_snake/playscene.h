#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include <QLabel>
#include "snake.h"
#include <QTimer>
#include <QKeyEvent>
#include "food.h"

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    PlayScene(int x, int y);

    void initScene();
    void initSnake();
    void getBody();
    void updatePosition(); //更新蛇的位置
    void keyPressEvent(QKeyEvent*); //键盘事件
    void setFood();
    void restartGame();

public:
    int row;
    int col;
    QPixmap pix;
    QLabel *score;
    QVector<QVector<int>> blocks;
    QVector<Snake*> s;
    QTimer timer; //更新位置定时器
    bool isPause = false;
    Food *food = NULL;
    bool keyIsListen = true; //按键监听

signals:
    void playSceneBack();
};

#endif // PLAYSCENE_H
