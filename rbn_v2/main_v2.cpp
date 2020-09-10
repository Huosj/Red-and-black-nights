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

inline void setRedOnWhite(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED);
}//白底红字
inline void setBlueOnWhite(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_BLUE);
}//白底蓝字
inline void setGreenOnWhite(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_GREEN);
}//白底绿字
inline void setPurpleOnWhite(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_BLUE |FOREGROUND_RED);
}//白底紫字
inline void setBlackOnWhite(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
}//白底黑字
void showPlayersD(int *player,int n,int ii){
    for(int i=0;i<n;i++){
        if(player[i]&1) setRedOnWhite();
        else setBlackOnWhite();
        if(i!=ii&&player[i]>0) setBlueOnWhite();//活人隐藏身份

        if(player[i]>0) cout<<i+1<<' ';
        else cout<<i+1<<'x';
    }
    cout<<endl;

    setBlueOnWhite();
}
void gameboardD(int round,int*player,int n,int i){
    system("cls");
    system("cls");
    multiprintf("**回合#");
    cout<<round<<":\n";
    showPlayersD(player,n,i);//除当前玩家外隐藏活人身份
    int space;
    for(space=0;space<i;space++) {//确定空间以显示发言人位置
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
void showMsg(vector<string> &MSGS){//输出历史消息，并删除过旧的
    setPurpleOnWhite();
    multiprintf("\n**最近历史记录：\n");
    vector<string>::iterator it=MSGS.begin();
    while(MSGS.size()>23) MSGS.erase(it++);
    for(;it<MSGS.end();it++){
        cout<<' '<<*it<<endl;
    }
    cout<<endl;
    setBlueOnWhite();
}
int main(){
    system("color FA");//改变背景色为白色
    setBlueOnWhite();//默认显示颜色为蓝色
    int n,player[21]={0};//存储玩家人数和玩家代码(奇数为红牌，偶数为黑牌）
    int countRed,countBlack,leader;//存储红色方人数、黑色方人数、村长编号
    int order;//顺序编码,0为从左开始发言，1为从右开始发言。
    multiprintf("*输入玩家人数(3~20人): ");
    cin>>n;
    countRed=(n+1)/2;
    countBlack=n/2;
    
    float vote[21],leader_vote=1.5- float (n&1);//规定投票箱和初始村长票数
    if(n>20){
        multiprintf("**玩家人数过多！");
        return 0;
    }
    if(n<3){
        multiprintf("**玩家人数过少！");
        return 0;
    }
    randDistribute(player,n);//分发身份
    vector<string> msgs;//vector可以方便地删除头尾元素，用vector便于存储新的历史发言发言，且有利于删除旧的历史发言;
    int round=0;
    for(;round<=n&&countRed>0&&countBlack>0;round++){
        if (round>0){//选出村长后
            gameboardD(round,player,n,leader);
            showMsg(msgs);
            multiprintf("*天亮了，轮到 村长#");
            cout<<leader+1;
            multiprintf(" 决定发言顺序(0从左开始，1从右开始)：\n");
            cin>>order;
            while(order<0||order>1){
                gameboardD(round,player,n,leader);
                showMsg(msgs);
                setGreenOnWhite();
                multiprintf("**输入不合法\n*轮到 村长#");
                cout<<leader+1;
                multiprintf(" 重新决定发言顺序(0从左开始，1从右开始)：\n");
                cin.clear();
                cin.sync();
                cin>>order;
                setBlueOnWhite();
            }
            if(order&1){
                for(int i=0;i<n;i++){
                    char nfayan[81],tempfy[111];
                    gameboardD(round,player,n,(i+leader+1)%n);
                    showMsg(msgs);

                    if(player[(i+leader+1)%n]>0){
                        multiprintf("*轮到 玩家#");
                        cout<<(i+leader+1)%n+1;
                        multiprintf(" 发言(限于80字符以内)：\n");
                        cin.clear();
                        cin.sync();
                        cin.getline(nfayan,80);

                        gameboardD(round,player,n,(i+leader+1)%n);
                        if( nfayan!=NULL && strlen(nfayan)>0 ){//防止发生段错误
                            sprintf(tempfy,"*Player#%d(Round %d): ",(i+leader+1)%n+1,round);
                            strcat(tempfy,nfayan);
                            msgs.push_back(tempfy);
                            cout<<'\n'<<tempfy;
                        }
                        else {
                            multiprintf("\n*本回合 玩家#");
                            cout<<(i+leader+1)%n+1;
                            multiprintf(" 跳过了发言.");
                            sprintf(tempfy,"*Player#%d skipped speaking(Round %d).",(i+leader+1)%n+1,round);
                            strcat(tempfy,nfayan);
                            msgs.push_back(tempfy);
                        }
                        multiprintf("\n (按Enter键以继续).");
                        getchar();//执行以读取换行符
                    }
                    else {
                        setGreenOnWhite();
                        multiprintf("**玩家#");
                        cout<<(i+leader+11)%n+1;
                        multiprintf("已死亡\n按Enter键以继续.");
                        cin.sync();
                        getchar();
                        setBlueOnWhite();
                    }
                }
            }
            else{
                for(int i=n;i>0;i--){
                    char nfayan[81],tempfy[121];
                    gameboardD(round,player,n,(i+leader+n-1)%n);
                    showMsg(msgs);

                    if(player[(i+leader+n-1)%n]>0){
                        multiprintf("*轮到 玩家#");
                        cout<<(i+leader+n-1)%n+1;
                        multiprintf(" 发言(100字符以内)：\n");
                        cin.clear();
                        cin.sync();
                        cin.getline(nfayan,80);

                        /*gameboardD(round,player,n,(i+leader+n-1)%n);
                        sprintf(tempfy,"*Player#%d(Day in Round %d): ",(i+leader+n-1)%n+1,round);
                        strcat(tempfy,nfayan);
                        msgs.push_back(tempfy);
                        cout<<'\n'<<tempfy;*/

                        if( nfayan!=NULL && strlen(nfayan)>0 ){//防止发生段错误
                            sprintf(tempfy,"*Player#%d(Round %d): ",(i+leader+n-1)%n+1,round);
                            strcat(tempfy,nfayan);
                            msgs.push_back(tempfy);
                            cout<<'\n'<<tempfy;
                        }
                        else {
                            multiprintf("\n*本回合 玩家#");
                            cout<<(i+leader+n-1)%n+1;
                            multiprintf(" 跳过了发言.");
                            sprintf(tempfy,"*Player#%d skipped speaking(Round %d).",(i+leader+n-1)%n+1,round);
                            strcat(tempfy,nfayan);
                            msgs.push_back(tempfy);
                        }
                        multiprintf("\n按Enter键以继续.");
                        cin.sync();
                        getchar();//执行以读取换行符
                    }
                    else {
                        setGreenOnWhite();
                        multiprintf("**玩家#");
                        cout<<(i+leader+n-1)%n+1;
                        multiprintf("已死亡\n按Enter键以继续.");
                        cin.sync();
                        getchar();
                        setBlueOnWhite();
                    }
                }
            }

            memset(vote,0,sizeof(vote));
            int temp,max=0;
            vector<int>maxindex;
            for(int i=0;i<n;i++){
                //gameboardL(round,player,n,i);
                gameboardD(round,player,n,i);
                showMsg(msgs);

                if(player[i]>0){
                    multiprintf("*轮到 玩家#");
                    cout<<i+1;
                    multiprintf(" 投票(输入编号，输入0弃票)：\n");
                    cin.clear();
                    cin.sync();
                    cin>>temp;
                    while(temp>0&&player[temp-1]<0){
                        gameboardD(round,player,n,i);
                        showMsg(msgs);
                        setGreenOnWhite();
                        multiprintf("**您所选的玩家已死亡\n*轮到 玩家#");
                        cout<<i+1;
                        multiprintf(" 重新投票(输入编号，输入0弃票)：\n");
                        cin.clear();
                        cin.sync();
                        cin>>temp;
                        setBlueOnWhite();
                    }
                    if(temp<=0) continue;
                    else if(i!=leader) vote[temp-1]+=1.0;
                    else vote[temp-1]+=leader_vote;
                }
                else{
                    setGreenOnWhite();
                    multiprintf("**玩家#");
                    cout<<(i+leader+n-1)%n+1;
                    multiprintf("已死亡(按Enter键以继续).");
                    cin.sync();
                    getchar();
                    setBlueOnWhite();
                }
            }
            for(int i=0;i<n;i++){
                if(player[i]<0||(vote[i]<1e-3&&vote[i]>-1e-3)) continue;
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
                gameboardD(round,player,n,*it);
                showMsg(msgs);
                multiprintf("*玩家#");
                cout<<(*it)+1;
                if(player[*it]&1) {
                    sprintf(tempmsg,"*Player#%d is exiled,with red card.",(*it)+1);
                    countRed--;
                    multiprintf("被放逐,身份为红牌(按Enter键以继续).");
                }
                else {
                    sprintf(tempmsg,"*Player#%d is exiled,with black card.",(*it)+1);
                    countBlack--;
                    multiprintf("被放逐,身份为黑牌(按Enter键以继续).");
                }
                player[*it]=-player[*it];
                cin.sync();
                getchar();
                msgs.push_back(tempmsg);
            }
            if(countRed<=0||countBlack<=0) break;
            if(player[leader]<0){
                    gameboardD(round,player,n,leader);
                    showMsg(msgs);
                    multiprintf("*轮到 村长#");
                    cout<<leader+1;
                    multiprintf(" 移交新村长(输入存活玩家编号)：\n");
                    int nleader;
                    cin>>nleader;
                    while(player[nleader-1]<0||nleader<1||nleader>n){
                        setGreenOnWhite();
                        multiprintf("**您所选的玩家已死亡或序号不合法\n*轮到 村长#");
                        cout<<leader+1;
                        multiprintf(" 重新移交新村长(输入存活玩家编号)：\n");
                        cin.clear();
                        cin.sync();
                        cin>>nleader;
                        setBlueOnWhite();
                    }
                    leader=nleader-1;
                    multiprintf("*玩家#");
                    cout<<leader+1;
                    multiprintf(" 当选为村长(按Enter键以继续).");
                    cin.sync();
                    getchar();//执行以读取换行符
                    char tempmsg[50];
                    sprintf(tempmsg,"*Player#%d becomes new village leader.",leader+1);
                    msgs.push_back(tempmsg);
                }
            //进入夜晚;
            srand(time(NULL));
            int night=rand()&1;//夜晚标记,1为红夜，0为黑夜
            int counteyes=0;
            gameboardD(round,player,n,-1);
            showMsg(msgs);
            if(night!=0){
                multiprintf("*进入红夜(按Enter键以继续).");
                msgs.push_back("*Red night!");
            }
            else{
                multiprintf("*进入黑夜(按Enter键以继续).");
                msgs.push_back("*Black night!");
            }
            cin.sync();
            getchar();
            memset(vote,0,sizeof(vote));
            for(int i=0;i<n;i++){
                gameboardD(round,player,n,i);
                showMsg(msgs);
                multiprintf("*轮到 玩家#");
                cout<<i+1;
                multiprintf(" 睁眼(输入1睁眼，输入0闭眼)：\n");
                cin.clear();
                cin.sync();
                cin>>temp;
                if((player[i]&1)!=night||temp==0) continue;
                else counteyes++;
            }
            if(counteyes&1) leader_vote=1.5-float(n&1);//复原
            else{
                leader_vote=1.0;
                gameboardD(round,player,n,-1);
                showMsg(msgs);
                multiprintf("*第二天村长拥有1票(按Enter键以继续).");
                cin.sync();
                getchar();
            }
        }
        else{
            int cvote[21]={0};
            for(int i=0;i<n;i++){
                char nfayan[81],tempfy[111];
                gameboardD(round+1,player,n,i);
                showMsg(msgs);

                multiprintf("*轮到 玩家#");
                cout<<i+1;
                multiprintf(" 发言(80字符以内)：\n");
                cin.clear();
                cin.sync();
                cin.getline(nfayan,80);
                
                gameboardD(round+1,player,n,i);
                if( nfayan!=NULL && strlen(nfayan)>0 ){//防止发生段错误
                    sprintf(tempfy,"*Player#%d(Round %d): ",i+1,round+1);
                    strcat(tempfy,nfayan);
                    msgs.push_back(tempfy);
                    cout<<'\n'<<tempfy;
                }
                else {
                    multiprintf("\n*本回合 玩家#");
                    cout<<i+1;
                    multiprintf(" 跳过了发言.");
                    sprintf(tempfy,"*Player#%d skipped speaking(Round %d).",i+1,round+1);
                    strcat(tempfy,nfayan);
                    msgs.push_back(tempfy);
                }
                multiprintf("\n 按Enter键以继续.");
                cin.sync();
                getchar();//执行以读取换行符
            }
            int temp,cmax=-1,cmaxindex=-1;
            for(int i=0;i<n;i++){
                //gameboardL(round,player,n,i);
                gameboardD(round+1,player,n,i);
                showMsg(msgs);

                multiprintf("*轮到 玩家#");
                cout<<i+1;
                multiprintf(" 票选村长(输入编号，输入0弃票)：\n");
                cin.clear();
                cin.sync();
                cin>>temp;
                while(temp<0||temp>n){
                        gameboardD(round+1,player,n,i);
                        showMsg(msgs);
                        setGreenOnWhite();
                        multiprintf("**序号不合法\n*轮到 玩家#");
                        cout<<i+1;
                        multiprintf(" 票选村长(输入编号，输入0弃票)：\n");
                        cin.clear();
                        cin.sync();
                        cin>>temp;
                        setBlueOnWhite();
                    }
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
                    break;//存在平票，重新开始该轮
                }
            }
            if(round>=0){
                leader=cmaxindex;
                char tempmsg[50];
                multiprintf("*玩家#");
                cout<<leader+1;
                multiprintf(" 当选为村长(按Enter键以继续).");
                cin.sync();
                getchar();//执行以读取换行符
                sprintf(tempmsg,"*Player#%d becomes village leader.",leader+1);
                msgs.push_back(tempmsg);
            }
        }
    }
    gameboardD(round+1,player,n,-1);
    //本来有showMsg(msgs);
    
    if(countRed>0&&countBlack<=0) {
        setRedOnWhite();
        multiprintf("*游戏结束，所有的黑牌被放逐，红方胜利(按Enter键以继续).");
    }
    else if(countBlack>0&&countRed<=0) {
        setBlackOnWhite();
        multiprintf("*游戏结束，所有的红牌被放逐，黑方胜利(按Enter键以继续).");
    }
    else if(countRed<=0&&countBlack<=0) 
        multiprintf("*游戏结束，所有的牌被放逐，平局(按Enter键以继续).");
    else {
        setGreenOnWhite();
        multiprintf("**bug");
    }
    cin.sync();
    getchar();//执行以读取换行符
    return 0;
}