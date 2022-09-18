#include "playscene.h"
#include <QMenuBar>
#include <QMessageBox>

PlayScene::PlayScene(int x, int y)
{
    QMenuBar *bar = menuBar();
    QMenu *startMenu = bar->addMenu("开始");
    QAction *returnAction = startMenu->addAction("返回");
    QAction *restartAction = startMenu->addAction("重新开始");
    connect(returnAction,&QAction::triggered,[=](){
        emit playSceneBack();
        this->close();
    });
    connect(restartAction,&QAction::triggered,[=](){
        this->restartGame();
    });

    srand((unsigned int)time(NULL));
    row = x;
    col = y;
    bool ret = pix.load(":/res/BoardNode(1).png");
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    if(!ret){
        qDebug()<<"方块加载失败";
        return;
    }

    setWindowIcon(QIcon(":/res/food.png"));
    setWindowTitle("贪吃蛇");
    setFixedSize(col*25+100,row*25+150);

    score = new QLabel;
    QFont ft;
    ft.setPointSize(20);
    score->setParent(this);
    score->setFixedSize(150,50);
    score->setText("得分:0");
    score->setFont(QFont(ft));
    score->move(50,50);

    food = new Food(row-1,col-1);
    food->setParent(this);
    initScene();  //生成场景
    initSnake();  //初始化蛇
    setFood();  //放置食物

    timer.setInterval(300); //每秒移动一次
    connect(&timer, &QTimer::timeout,[=](){
        keyIsListen = true;
        updatePosition();
    });
    timer.start();
}

void PlayScene::initScene()
{
    QVector<int> temp(col,0);
    for(int i=0;i<row;i++)  //初始化全零矩阵，用于计算随机食物
        blocks.push_back(temp);

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            QLabel *label = new QLabel;
            label->setParent(this);
            label->setStyleSheet("QPushButton{border:0px};");
            label->setFixedSize(25,25);
            label->setPixmap(pix);
            label->move(j*25+50,i*25+100);
        }
    }
}

void PlayScene::initSnake()
{
    Snake *head = new Snake(":/res/head.png");
    head->setParent(this);
    head->pos_row = 0;
    head->pos_col = 2;
    head->positon = 4;
    head->move(head->pos_col*25+50, head->pos_row*25+100);
    head->show();
    s.push_back(head);

    blocks[0][2] = 1;

    getBody();
    getBody(); 
}

void PlayScene::getBody()
{
    Snake *body = new Snake;
    body->setParent(this);
    Snake *temp = s.back();
    if(temp->positon==1)
    {
        body->pos_row = temp->pos_row+1;
        body->pos_col = temp->pos_col;
    }
    else if(temp->positon==2)
    {
        body->pos_row = temp->pos_row-1;
        body->pos_col = temp->pos_col;
    }
    else if(temp->positon==3)
    {
        body->pos_row = temp->pos_row;
        body->pos_col = temp->pos_col+1;
    }
    else
    {
        body->pos_row = temp->pos_row;
        body->pos_col = temp->pos_col-1;
    }
    body->positon = temp->positon;
    //qDebug()<<body->pos_row<<body->pos_col;

    if(body->pos_col<0 || body->pos_col>col-1 || body->pos_row<0 || body->pos_row>row-1){
        qDebug()<<"溢出";
        delete body;
        return;
    }
    body->move(body->pos_col*25+50, body->pos_row*25+100);
    body->show();
    s.push_back(body);
    blocks[body->pos_row][body->pos_col] = 1;
    QString str = QString("得分:%1").arg(s.size());
    score->setText(str);
}

void PlayScene::updatePosition()
{
    if(s[0]->positon == 1 && s[0]->pos_row<=0)return;
    else if(s[0]->positon == 2 && s[0]->pos_row>=row-1)return;
    else if(s[0]->positon == 3 && s[0]->pos_col<=0)return;
    else if(s[0]->positon == 4 && s[0]->pos_col>=col-1)return;

    blocks[s[s.size()-1]->pos_row][s[s.size()-1]->pos_col] = 0; //尾部置0

    for(int i=s.size()-1;i>0;i--)
    {
        s[i]->move(s[i-1]->pos_col*25+50, s[i-1]->pos_row*25+100);
        s[i]->positon = s[i-1]->positon;
        s[i]->pos_col = s[i-1]->pos_col;
        s[i]->pos_row = s[i-1]->pos_row;
    }
    if(s[0]->positon==1)
        s[0]->pos_row-=1;
    else if(s[0]->positon==2)
        s[0]->pos_row+=1;
    else if(s[0]->positon==3)
        s[0]->pos_col-=1;
    else
        s[0]->pos_col+=1;

    s[0]->move(s[0]->pos_col*25+50,s[0]->pos_row*25+100);
    if(s[0]->pos_row == food->pos_row && s[0]->pos_col == food->pos_col)
    {   //如果头部撞到食物
        setFood(); //改变食物位置
        getBody();  //身体增长
        if(s.size()>=row*col){
            qDebug()<<"胜利";
        }
    }
    if(blocks[s[0]->pos_row][s[0]->pos_col] == 1)
    {
        //如果头部撞到身体
        keyIsListen = false;
        timer.stop();
        QMessageBox::information(this,"提示","撞到自己, Game Over!");
    }

    blocks[s[0]->pos_row][s[0]->pos_col] = 1; //头部置1
}

void PlayScene::keyPressEvent(QKeyEvent *e)
{
    if(!keyIsListen){
        return;
    }
    keyIsListen = false;
    switch(e->key())
    {
        case Qt::Key_Up:
        {
            if(s[0]->positon!=2)
                s[0]->positon = 1;
            break;
        }
        case Qt::Key_Down:
        {
            if(s[0]->positon!=1)
                s[0]->positon = 2;
            break;
        }
        case Qt::Key_Left:
        {
            if(s[0]->positon!=4)
                s[0]->positon = 3;
            break;
        }
        case Qt::Key_Right:
        {
            if(s[0]->positon!=3)
                s[0]->positon = 4;
            break;
        }
        case Qt::Key_Space:  //暂停
        {
            if(!isPause){
                isPause = true;
                timer.stop();
                keyIsListen = true;
            }
            else
            {
                isPause = false;
                timer.start();
            }
        }
        default:
            break;
    }
}

void PlayScene::setFood()
{
    if(s.size()>=4*row*col/5) //如果只剩五分之一的空格子,就不适用完全的随机了
    {
        int n=row*col-s.size();  //剩余的格子数
        if(n==0)return;
        int random = rand()%n+1;  //取第随机数个剩余格子，把食物放在这
        for(int i=0;i<row;i++){
            for(int j=0;j<col;j++){
                if(blocks[i][j]==0){
                    random--;
                    if(random==0){
                        food->move(25*j+50, 25*i+100);
                        food->pos_row = i;
                        food->pos_col = j;
                        return;
                    }
                }
            }
        }
    }
    else{  //完全随机
        int r=0;
        int c=0;
        while(true)
        {
            r = rand()%row;
            c = rand()%col;
            if(blocks[r][c] == 0){
                food->move(25*c+50, 25*r+100);
                food->pos_row = r;
                food->pos_col = c;
                return;
            }
        }
    }
}

void PlayScene::restartGame()//重新开始
{
    blocks.clear();
    for(int i=0;i<s.size();i++)
    {
        if(s[i]!=NULL){
            delete s[i];
            s[i] = NULL;
        }
    }
    s.clear();

    bool isPause = false;
    Food *food = NULL;
    bool keyIsListen = true; //按键监听

    delete food;
    food = NULL;
    food = new Food(row-1,col-1);
    food->setParent(this);
    initScene();  //生成场景
    initSnake();  //初始化蛇
    setFood();  //放置食物
    timer.start();
}

