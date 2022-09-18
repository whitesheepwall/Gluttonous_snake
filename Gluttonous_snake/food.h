#ifndef FOOD_H
#define FOOD_H

#include <QLabel>

class Food : public QLabel
{
    Q_OBJECT
public:
    //explicit Food(QWidget *parent = nullptr);
    Food(int row, int col);
    int pos_row;
    int pos_col;
    QPixmap pix;

signals:

};

#endif // FOOD_H
