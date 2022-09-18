#include "food.h"

//Food::Food(QWidget *parent)
//    : QWidget{parent}
//{

//}
Food::Food(int row, int col)
{
    pos_row = row;
    pos_col = col;
    bool ret = pix.load(":/res/food.png");
    if(!ret){
        qDebug()<<"食物加载失败";
        return;
    }
    pix = pix.scaled(pix.width()*0.5, pix.height()*0.5);
    setFixedSize(pix.width(),pix.height());
    setPixmap(pix);
    setStyleSheet("QLabel{border:0px};");
}

