#include "snake.h"

//Snake::Snake(QWidget *parent)
//    : QWidget{parent}
//{

//}

Snake::Snake(QString path)
{
    bool ret = pix.load(path);
    pix = pix.scaled(pix.width()*0.5, pix.height()*0.5);
    if(!ret){
        qDebug()<<"蛇加载失败";
        return;
    }
    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QLabel{border:0px};");
    this->setPixmap(pix);
}
