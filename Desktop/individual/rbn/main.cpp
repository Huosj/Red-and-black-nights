#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <vector>
#include <string>
#include <string.h>
using namespace std;
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

inline void multiprintf(char const*a){
    SetConsoleOutputCP(65001);
    cout<<a;
    SetConsoleOutputCP(936);
}//用来输出含中文的混合字串multichar

/*inline void colour(int x){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),x);
}*/
inline void setRedOnWhite(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED);
}//白底红字
inline void setBlueOnWhite(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_BLUE);
}//白底蓝字
inline void setPurpleOnWhite(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_BLUE |FOREGROUND_RED);
}//白底紫字
inline void setBlackOnWhite(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
}//白底黑字
void showPlayersL(int *player,int n){
    for(int i=0;i<n;i++){
        if(player[i]%2) setRedOnWhite();
        else setBlackOnWhite();
        if(player[i]>0) cout<<i+1<<' ';
        else cout<<i+1<<'X';
    }
    cout<<endl;

    setBlueOnWhite();
}
void showPlayersD(int *player,int n,int ii){
    for(int i=0;i<n;i++){
        if(player[i]%2) setRedOnWhite();
        else setBlackOnWhite();
        if(i!=ii&&player[i]>0) setBlueOnWhite();
        if(player[i]>0) cout<<i+1<<' ';
        else cout<<i+1<<'x';
    }
    cout<<endl;

    setBlueOnWhite();
}
void gameboardL(int round,int*player,int n,int i){
    system("cls");
    printf("#Round %d\n",round);
    showPlayersL(player,n);//明身份
    int space;
    for(space=0;space<i;space++) {
        if(space<9) printf("__");
        else printf("___");
    }
    multiprintf("^");
    for(space++;space<n;space++) {
        if(space<9) printf("__");
        else printf("___");
    }
}
void gameboardD(int round,int*player,int n,int i){
    system("cls");
    printf("#Round %d:\n",round);
    showPlayersD(player,n,i);//除当前玩家外隐藏活人身份
    int space;
    for(space=0;space<i;space++) {
        if(space<9) printf("__");
        else printf("___");
    }
    if(space<9) printf("^");
    else printf("^^");
    for(space++;space<n;space++) {
        if(space<9) printf("__");
        else printf("___");
    }
}
void showMsg(vector<string> &MSGS){
    setPurpleOnWhite();
    multiprintf("\n#最近历史记录：\n");
    vector<string>::iterator it=MSGS.begin();
    if(MSGS.size()>23) MSGS.erase(it);
    for(;it<MSGS.end();it++){
        cout<<' '<<*it<<endl;
    }
    cout<<endl;
    setBlueOnWhite();
}
int main(){
    system("color FA");//改变背景色
    //system("chcp 65001");//改变字符编码为utf-8
    setBlueOnWhite();
    int n,player[44]={0},countp,leader;
    float vote[44];
    multiprintf("输入玩家人数(3~43人): ");
    cin>>n;
    countp=n;
    if(n>43){
        multiprintf("玩家人数过多！");
        return 0;
    }
    if(n<3){
        multiprintf("玩家人数过少！");
        return 0;
    }
    randDistribute(player,n);
    vector<string> msgs;
    //string fayan;

    for(int round=1;round<=n&&countp;round++){
        if (round>1){//选出村长后
            for(int i=0;i<n;i++){
                char nfayan[81],tempfy[121];
                //gameboardL(round,player,n,i);
                gameboardD(round,player,n,i);
                showMsg(msgs);

                if(player[i]>0){
                    multiprintf("*轮到 玩家#");
                    cout<<i+1;
                    multiprintf(" 发言(100字符以内)：\n");
                    cin.clear();
                    cin.sync();
                    cin.getline(nfayan,80);

                    //gameboardL(round,player,n,i);
                    gameboardD(round,player,n,i);
                    sprintf(tempfy,"*Player #%d(in Round %d): ",i+1,round);
                    strcat(tempfy,nfayan);
                    msgs.push_back(tempfy);
                    cout<<'\n'<<tempfy<<"\n Press Enter to continue.";
                    getchar();//执行以读取换行符
                }
                else {
                    cout<<"*Player #"<<i+1<<"is dead.\n Press Enter to continue.";
                    getchar();
                }
            }
            memset(vote,0,sizeof(vote));
            int temp,max=0;
            vector<int>maxindex;
            for(int i=0;i<n;i++){
                //gameboardL(round,player,n,i);
                gameboardD(round,player,n,i);
                showMsg(msgs);

                //for(int i=0;i<n;i++) cout<<vote[i]<<' ';
                cout<<"*It is Player #"<<i+1<<"'s turn to vote(input order number):\n";
                cin.clear();
                cin.sync();
                cin>>temp;
                while(player[temp]<0){
                    //gameboardL(round,player,n,i);
                    gameboardD(round,player,n,i);
                    showMsg(msgs);

                    cout<<"**The player you vote for is dead.\n*It is Player #"<<i+1<<"'s turn to vote(input order number):\n";
                    cin.clear();
                    cin.sync();
                    cin>>temp;
                }
                if(i!=leader) vote[temp-1]+=1.0;
                else vote[temp-1]+=1.5-float(n%2);
            }
            for(int i=0;i<n;i++){
                if(player[i]<0) continue;
                if(vote[i]-max>1e-3){
                    maxindex.clear();
                    max=vote[i];
                    maxindex.push_back(i);
                }
                else if(vote[i]-max<1e-3&&vote[i]-max>-1e-3){
                    maxindex.push_back(i);
                }
                else continue;
            }
            for(vector<int>::iterator it=maxindex.begin();it<maxindex.end();it++){
                char tempmsg[50];
                if(player[*it]%2) sprintf(tempmsg,"*Player #%d is dead,with red card.",(*it)+1);
                else sprintf(tempmsg,"*Player #%d is dead,with red card.",(*it)+1);
                player[*it]=-player[*it];

                msgs.push_back(tempmsg);
                countp--;
            }
        }
        else{
            int cvote[44]={0};
            for(int i=0;i<n;i++){
                char nfayan[81],tempfy[121];
                //gameboardL(round,player,n,i);
                gameboardD(round,player,n,i);
                showMsg(msgs);

                multiprintf("*轮到 玩家#");
                cout<<i+1;
                multiprintf(" 发言(100字符以内)：\n");
                cin.clear();
                cin.sync();
                cin.getline(nfayan,80);

                //gameboardL(round,player,n,i);
                gameboardD(round,player,n,i);
                sprintf(tempfy,"*Player #%d(in Round %d): ",i+1,round);
                strcat(tempfy,nfayan);
                msgs.push_back(tempfy);
                cout<<'\n'<<tempfy<<"\n Press Enter to continue.";
                getchar();//执行以读取换行符
            }
            int temp,cmax=0,cmaxindex=0;
            for(int i=0;i<n;i++){
                //gameboardL(round,player,n,i);
                gameboardD(round,player,n,i);
                showMsg(msgs);

                //for(int i=0;i<n;i++) cout<<cvote[i]<<' ';
                cout<<"*It is Player #"<<i+1<<"'s turn to vote for village head(input order number):\n";
                cin.clear();
                cin.sync();
                cin>>temp;
                cvote[temp-1]++;
            }
            for(int i=0;i<n;i++){
                if(cvote[i]>cmax){
                    cmax=cvote[i];
                    cmaxindex=i;
                }
            }
            for(int i=0;i<n;i++){
                if(cvote[i]==cmax&&i!=cmaxindex){
                    round--;
                    break;
                }
            }
            if(round){
                leader=cmaxindex;
                char tempmsg[50];
                sprintf(tempmsg,"*Player #%d becomes the village head.",leader+1);
                msgs.push_back(tempmsg);
            }
        }
    }
    cout<<endl;
    return 0;
}
