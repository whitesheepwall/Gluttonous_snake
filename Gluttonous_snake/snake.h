#ifndef SNAKE_H
#define SNAKE_H

#include <QLabel>

class Snake : public QLabel
{
    Q_OBJECT
public:
    //explicit Snake(QWidget *parent = nullptr);
    Snake(QString path = QString(":/res/body.png"));

public:
    int pos_row;
    int pos_col;
    int positon; //移动方向 1->上, 2->下, 3->左, 4->右
    QPixmap pix;

signals:

};

#endif // SNAKE_H
