#include "mainscene.h"
#include "ui_mainscene.h"
#include "playscene.h"
#include <QMessageBox>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/res/food.png"));
    setWindowTitle("贪吃蛇");

    connect(ui->startBtn,&QPushButton::clicked,[=](){
        int row = 0;
        int col = 0;
        if(ui->radioButton->isChecked()){
            row = 10;
            col = 10;
        }
        else if(ui->radioButton_2->isChecked()){
            row = 20;
            col = 20;
        }
        else if(ui->radioButton_3->isChecked()){
            row = 30;
            col = 30;
        }
        else if(ui->radioButton_4->isChecked()){
            if(ui->rowEdit->text()!="" && ui->colEdit->text()!="")
            {
                row = ui->rowEdit->text().toInt();
                col = ui->colEdit->text().toInt();
            }
            else{
                QMessageBox::information(this,"提示","请输入行列数");
                return;
            }
        }
        if(row<3 || col<3){
            QMessageBox::information(this,"提示","地图太小了");
            return;
        }
        PlayScene *play = new PlayScene(row,col);
        connect(play,&PlayScene::playSceneBack,[=](){
            this->show();
        });
        play->show();
        this->hide();
    });
}

MainScene::~MainScene()
{
    delete ui;
}

