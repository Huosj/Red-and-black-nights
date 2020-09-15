#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <utility>
#include <string>
#include <string.h>
#include <QString>
#include <QInputDialog>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QColor>
using namespace std;
void swap(int &a,int &b){
    int temp=b;
    b=a;
    a=temp;
}
int leader=-1;//存储村长编号
namespace Ui {
    class MainWindow;
}
void randDistribute(int *a,int n){
    for(int i=0;i<n;i++){
        a[i]=i+1;//先编好号
    }
    srand(time(NULL));//改变随机数种子
    for(int i=n-1;i>0;i--){
        swap(a[rand()%i],a[i]);//洗牌算法 创造范围随机数
    }
    return;
}
bool paircmp(pair<int,int> a,pair<int,int> b){
    if(a.first==b.first) return a.second<b.second;
    return a.first>b.first;
}

void MainWindow::gameboardD(int*player,int n,int i){
    ui->gameTable->clearContents();
    for(int ii=0;ii<n;ii++){
        ui->gameTable->setItem(ii,0,new QTableWidgetItem(QString("%1").arg(ii+1)));
        ui->gameTable->setItem(ii,1,new QTableWidgetItem(player[ii]>0?"存活":"死亡"));
        QString identity=(player[ii]>0&&(ii!=i) )?"未知":( (player[ii]%2==0) ?"黑色":"红色");
        if(ii==leader) identity.append("-村长");
        QTableWidgetItem *colorItem=new QTableWidgetItem(identity);
        colorItem->setTextColor( (player[ii]>0&&(ii!=i) ) ? QColor(128,160,192) : ( (player[ii]%2==0)? QColor(0,0,0) : QColor(255,0,0)));
        ui->gameTable->setItem(ii,2,colorItem);
        ui->gameTable->setItem(ii,3,new QTableWidgetItem((ii==i)? ((player[ii]>0)?"<-发言中":"<-确认中") :""));
    }
}
void MainWindow::gameboardD2(int*player,int n,int i){
    ui->gameTable->clearContents();
    for(int ii=0;ii<n;ii++){
        ui->gameTable->setItem(ii,0,new QTableWidgetItem(QString("%1").arg(ii+1)));
        ui->gameTable->setItem(ii,1,new QTableWidgetItem(player[ii]>0?"存活":"死亡"));
        QString identity=(player[ii]>0&&(ii!=i) )?"未知":( (player[ii]%2==0) ?"黑色":"红色");
        if(ii==leader) identity.append("-村长");
        QTableWidgetItem *colorItem=new QTableWidgetItem(identity);
        colorItem->setTextColor( (player[ii]>0&&(ii!=i) ) ? QColor(128,160,192) : ( (player[ii]%2==0)? QColor(0,0,0) : QColor(255,0,0)));
        ui->gameTable->setItem(ii,2,colorItem);
        ui->gameTable->setItem(ii,3,new QTableWidgetItem((ii==i)? ((player[ii]>0)?"<-行动中":"<-确认中") :""));
    }
}
void MainWindow::gameboardL(int*player,int n){
    ui->gameTable->clearContents();
    for(int ii=0;ii<n;ii++){

        ui->gameTable->setItem(ii,0,new QTableWidgetItem(QString::number(ii+1)));
        ui->gameTable->setItem(ii,1,new QTableWidgetItem(player[ii]>0?"存活":"死亡"));
        QString identity= (player[ii]%2==0) ?"黑色":"红色";
        if(ii==leader) identity.append("-村长");
        QTableWidgetItem *colorItem=new QTableWidgetItem(identity);
        colorItem->setTextColor(  (player[ii]%2==0)? QColor(0,0,0) : QColor(255,0,0));
        ui->gameTable->setItem(ii,2,colorItem);
        ui->gameTable->setItem(ii,3,new QTableWidgetItem(""));
    }
}

void MainWindow::showMsg(vector<QString> &MSGS){//输出历史消息，并删除过旧的
    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(QString("**最近历史记录:"));
    vector<QString>::iterator it=MSGS.begin();
    while(MSGS.size()>50){
        MSGS.erase(it++);
    }
    for(;it<MSGS.end();it++){
        ui->plainTextEdit->appendPlainText(*it);
    }

}

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->show();
    setWindowTitle("红黑夜法官 (发言时输入\"/system quit\"以退出)");
    QString quitstr="/system quit";
    ui->gameTable->setEnabled(false);
    ui->plainTextEdit->setReadOnly(true);
    setFixedSize(920,880);
    int n=0,player[21]={0};//存储玩家人数和玩家代码(奇数为红牌，偶数为黑牌）
    int countRed,countBlack;//存储红色方人数、黑色方人数
    int order;//顺序编码,0为从左开始发言，1为从右开始发言。
    bool quitflag=0;//结束标志
    ui->plainTextEdit->appendPlainText("*输入玩家人数(3~20人): ");
    bool isOK;
    n=QInputDialog::getInt(this,QString("整数输入框"),QString("*自定义玩家人数(3~20人,默认8人)："),8,3,20,1,&isOK);
    ui->gameTable->setRowCount(n);
    randDistribute(player,n);//分发身份
    countRed=(n+1)/2;
    countBlack=n/2;
    int vote[21]={0},leader_vote=3-2*(n&1);//规定投票箱和初始村长票数(是实际值两倍)

    vector<QString> msgs;//vector可以方便地删除头尾元素，便于存储新的历史发言发言，且有利于删除旧的历史发言;同时它具有迭代器，遍历元素无须像queue一样逐个pop
    int round=0;
    for(;round<=n&&countRed>0&&countBlack>0;round++){
        if(round>0){//选出村长后
            gameboardD2(player,n,leader);
            showMsg(msgs);
            order=QInputDialog::getInt(NULL,QString("整数输入框"),QString("*天亮了,轮到 村长#%1 决定发言顺序(0从左开始，1从右开始)：").arg(leader+1),1,0,1,1,&isOK);

            if(order&1){
                for(int i=0;i<n;i++){
                    QString nfayan,tempfy;
                    gameboardD(player,n,(i+leader+1)%n);
                    showMsg(msgs);
                    if(player[(i+leader+1)%n]>0){
                        nfayan=QInputDialog::getText(NULL,QString("文本输入框"),QString("*轮到 玩家#%1 发言：").arg((i+leader+1)%n+1));
                        if(nfayan==quitstr){
                            QApplication *app;
                            quitflag=1;
                            app->quit();
                            break;
                        }
                        if(nfayan!=NULL){
                            tempfy=QString("*玩家#%1在第%2回合的发言：").arg((i+leader+1)%n+1,round);
                            tempfy.append(nfayan);
                            msgs.push_back(tempfy);
                        }
                        else{
                            msgs.push_back(QString("*本回合 玩家#%1 跳过了发言").arg((i+leader+1)%n+1));
                            }
                    }
                    else{
                            QMessageBox::about(NULL,QString("死亡提示"),QString("**玩家#%1 已死亡").arg((i+leader+1)%n+1));
                    }
                }
            }

            else{
                    for(int i=n;i>0;i--){
                        QString nfayan,tempfy;
                        gameboardD(player,n,(i+leader+n-1)%n);
                        showMsg(msgs);

                        if(player[(i+leader+n-1)%n]>0){
                            nfayan=QInputDialog::getText(NULL,QString("文本输入框"),QString("*轮到 玩家#%1 发言：").arg((i+leader+n-1)%n+1));
                            if(nfayan==quitstr){
                                QApplication *app;
                                quitflag=1;
                                app->quit();
                                break;
                            }
                            if(nfayan!=NULL){
                                tempfy=QString("*玩家#%1 在第%2回合的发言: ").arg((i+leader+n-1)%n+1,round);
                                tempfy.append(nfayan);
                                msgs.push_back(tempfy);
                            }
                            else{
                                msgs.push_back(QString("*玩家#%1 在第%2回合跳过了发言").arg((i+leader+n-1)%n+1,round));
                            }
                        }
                        else{
                            QMessageBox::about(NULL,QString("死亡提示"),QString("**玩家#%1 已死亡").arg((i+leader+n-1)%n+1));
                        }
                    }
            }
            if(quitflag) break;
            memset(vote,0,sizeof(vote));
            int temp;

            for(int i=0;i<n;i++){
                gameboardD2(player,n,i);
                showMsg(msgs);
                if(player[i]>0){
                    temp=QInputDialog::getInt(NULL,QString("整数输入框"),QString("*轮到 玩家#%1 投票(输入编号，输入0弃票)：").arg(i+1),0,0,n,1,&isOK);
                    while(temp>0&&player[temp-1]<0){
                        temp=QInputDialog::getInt(NULL,QString("整数输入框"),QString("**您所选的玩家已死亡\n轮到 玩家#%1 重新投票(输入编号，输入0弃票)：").arg(i+1),0,0,n,1,&isOK);
                    }
                    if(temp<=0) continue;
                    else if(i!=leader) {
                        vote[temp-1]+=2;}
                    else vote[temp-1]+=leader_vote;
                }
                else{
                    QMessageBox::about(NULL,QString("死亡提示"),QString("**玩家#%1 已死亡").arg(i+1));
                }
            }
            pair<int,int> votemsg[21];
            for(int i=0;i<n;i++){
                votemsg[i].first=vote[i];
                votemsg[i].second=i;
            }
            sort(votemsg,votemsg+n,paircmp);
            int max=votemsg[0].first;
            vector<int>maxindex;
            if(max>0) //投票有了结果
                for(int i=0;i<n;i++){
                    if(votemsg[i].first>0){
                        int b1=votemsg[i].second+1,b2=(votemsg[i].first/2);
                        QString real=QString("*玩家#%1 得票：").arg(b1);
                        real.append(QString::number(b2));
                        if(votemsg[i].first%2==1) real.append(QString(".5"));//double类型不能正常使用占位符，需要间接运算
                        msgs.push_back(real);
                }
                if(votemsg[i].first==max) maxindex.push_back(votemsg[i].second);
            }
            for(vector<int>::iterator it=maxindex.begin();it<maxindex.end();it++){
                gameboardD2(player,n,*it);
                showMsg(msgs);
                player[*it]=-player[*it];
                if(player[*it]%2==0) {
                    QMessageBox::about(NULL,QString("放逐提示"),QString("*玩家#%1 被放逐，身份为黑牌").arg( *it +1));
                    msgs.push_back(QString("*玩家#%1 被放逐，身份为黑牌").arg( *it +1));
                    countBlack--;
                }
                else {
                    QMessageBox::about(NULL,QString("放逐提示"),QString("*玩家#%1 被放逐，身份为红牌").arg( *it +1));
                    msgs.push_back(QString("*玩家#%1 被放逐，身份为红牌").arg( *it +1));
                    countRed--;
                }

            }
            if(countRed<=0||countBlack<=0) break;
            if(player[leader]<0){
                gameboardD2(player,n,leader);
                showMsg(msgs);
                int nleader=QInputDialog::getInt(NULL,QString("整数输入框"),QString("*轮到 村长#%1 移交新村长(输入存活玩家编号)：").arg(leader+1),1,1,n,1,&isOK);
                while(player[nleader-1]<0||nleader<1||nleader>n){
                    nleader=QInputDialog::getInt(NULL,QString("整数输入框"),QString("**您所选的玩家已死亡或序号不合法\n*轮到 村长#%1 重新移交新村长(输入存活玩家编号)：").arg(leader+1),1,1,n,1,&isOK);
                }
                leader=nleader-1;
                gameboardD2(player,n,leader);
                QMessageBox::about(NULL,QString("上任提示"),QString("*玩家#%1 当选为村长").arg(leader+1));
                msgs.push_back(QString("*玩家#%1 当选为村长").arg(leader+1));
            }
            gameboardD(player,n,-1);
            srand(time(NULL));
            int night=rand()&1;
            int counteyes=0;
            memset(vote,0,sizeof(vote));
            showMsg(msgs);
            if(night!=0){
                QMessageBox::about(NULL,QString("夜晚提示"),QString("*进入红夜"));
                msgs.push_back("*进入红夜");
            }
            else{
                QMessageBox::about(NULL,QString("夜晚提示"),QString("*进入黑夜"));
                msgs.push_back("*进入黑夜");
            }
            showMsg(msgs);
            for(int i=0;i<n;i++){
                gameboardD2(player,n,i);
                temp=QInputDialog::getInt(NULL,QString("整数输入框"),QString("*轮到 玩家#%1 睁眼(输入1睁眼，输入0闭眼)：").arg(i+1),0,0,1,1,&isOK);
                if((player[i]&1)!=night||temp==0) continue;
                else counteyes++;
            }
            if(counteyes&1){
                leader_vote=3-2*(n&1);//复原村长票数
                QMessageBox::about(NULL,QString("变更提示"),QString("*第%1回合村长拥有正常票数").arg(round+1));
            }
            else{
                leader_vote=2;
                gameboardD(player,n,-1);
                showMsg(msgs);
                QMessageBox::about(NULL,QString("变更提示"),QString("*第%1回合村长拥有1票").arg(round+1));
            }
            msgs.push_back(QString("**第%1回合开始").arg(round+1));
        }
        else{
            msgs.push_back(QString("**第1回合开始"));
            int cvote[21]={0};
            for(int i=0;i<n;i++){
                QString nfayan,tempfy;
                gameboardD(player,n,i);
                showMsg(msgs);
                nfayan=QInputDialog::getText(NULL,QString("文本输入框"),QString("*轮到 玩家#%1 发言：").arg(i+1));
                if(nfayan==quitstr){
                    QApplication *app;
                    quitflag=1;
                    app->quit();
                    break;
                }
                if(nfayan!=NULL){
                    tempfy=QString("*玩家#%1 在第1回合的发言：").arg(i+1);
                    tempfy.append(nfayan);
                    msgs.push_back(tempfy);
                }
                else{
                    msgs.push_back(QString("*玩家#%1 在第1回合跳过了发言").arg(i+1));
                }
            }
            if(quitflag) break;
            int temp;
            for(int i=0;i<n;i++){
                gameboardD2(player,n,i);
                showMsg(msgs);
                temp=QInputDialog::getInt(NULL,QString("整数输入框"),QString("*轮到 玩家#%1 票选村长(输入编号，输入0弃票)：").arg(i+1),0,0,n,1,&isOK);
                while(temp<0||temp>n){
                    temp=QInputDialog::getInt(NULL,QString("整数输入框"),QString("**序号不合法\n*轮到 玩家#%1 重新票选村长(输入编号，输入0弃票)：").arg(i+1),0,0,n,1,&isOK);
                }
                if(temp>0) cvote[temp-1]++;
            }
            /*for(int i=0;i<n;i++){
                if(cvote[i]>cmax){
                    cmax=cvote[i];
                    cmaxindex=i;
                }
            }执行结束后才能判断是否存在最高票平票*/
            pair<int,int> votemsg[21];
            for(int i=0;i<n;i++){
                votemsg[i].first=cvote[i];
                votemsg[i].second=i;
            }
            sort(votemsg,votemsg+n,paircmp);
            int cmax=votemsg[0].first,cmaxindex=votemsg[0].second;
            vector<int>maxindex;
            if(cmax>0) //投票有了结果
                for(int i=0;i<n;i++){
                    if(votemsg[i].first>0){
                        int b1=votemsg[i].second+1,b2=votemsg[i].first;
                        QString real=QString("*玩家#%1 得票：").arg(b1);
                        real.append(QString::number(b2));
                        msgs.push_back(real);
                }
            }
            if(votemsg[1].first==cmax){
                    round--;
                    msgs.push_back("*无人投票或存在平票，重新选村长");
                    break;
            }

            if(round>=0){
                leader=cmaxindex;
                QMessageBox::about(NULL,QString("上任提示"),QString("*玩家#%1 当选为村长").arg(leader+1));
                msgs.push_back(QString("*玩家#%1 当选为村长").arg(leader+1));
            }
        }
    }
    gameboardL(player,n);//结束游戏：显示所有人身份
    if(countRed>0&&countBlack<=0) {
        QMessageBox::about(NULL,QString("结束提示"),QString("*游戏结束，所有的黑牌被放逐，红方胜利"));
        msgs.push_back(QString("*游戏结束，所有的黑牌被放逐，红方胜利"));
    }
    else if(countBlack>0&&countRed<=0) {
        QMessageBox::about(NULL,QString("结束提示"),QString("*游戏结束，所有的红牌被放逐，黑方胜利"));
        msgs.push_back(QString("*游戏结束，所有的黑牌被放逐，黑方胜利"));
    }
    else if(countRed<=0&&countBlack<=0){
        QMessageBox::about(NULL,QString("结束提示"),QString("*游戏结束，所有的牌被放逐，平局"));
        msgs.push_back(QString("*游戏结束，所有的牌被放逐，平局"));
    }//出现于每人得一票的情况
    else{
        QMessageBox::about(NULL,QString("结束提示"),QString("**退出游戏"));
        msgs.push_back(QString("**退出游戏"));

    }
    showMsg(msgs);
}

MainWindow::~MainWindow()
{
    delete ui;
}

