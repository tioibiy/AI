#include<bits/stdc++.h>
#include<time.h>
#include<random>
#include<matplotlibcpp.h>

using namespace std;
const int N=(5e3);//最大值  
const double step=0.1;//学习步长s
void init(){//初始化
    srand(time(NULL));//随机数初始化
}
class Activation{
public:
    virtual double forward(double x){
        return x;
    }
    virtual double backward(double x){
        return 1;
    }
}a;
class Sigmoid:public Activation{
public:
    double forward(double x){
        return 1.0/(1+exp(-x));
    }
    double backward(double x){
        return x*(1-x);
    }
}sigmoid;
class ReLu:public Activation{
public:
    double forward(double x){
        return max(0.0,x);
    }
    double backward(double x){
        if(x>=0) return 1;
        return 0;
    }
}relu;
class neuron{//神经元
private:
public:
    int num=0;//输入的x的数量
    Activation (*activation)=&sigmoid;
    double x[N+5]={},k[N+5]={},b=0,y=0;//y=sigmoid(sum(kx)+b)
    double d=0,dx[N+5]={};//权重(求导) y对输出的权重，x对输出的权重
    neuron(){
        b=rand()/1e5;//将b初始化为随机
    }
    void init(Activation (*atv)=&sigmoid){
        activation=atv;
    }
    void addInput(){//增加一条入边，将k初始化为随机
        k[++num]=rand()/1e5;
    }
    void spread(int side){//两个方向的传播，side=0向前，side=1向后
        if(side==0){//向前传播
            if(num==0) return;//若没有x，则说明这是一个输入节点，其y已经被提前赋值，直接返回
            y=0;
            for(int i=1;i<=num;i++){
                y+=x[i]*k[i];
            }
            y+=b;
            y=activation->forward(y);
        }else if(side==1){//已知y对下一个节点的贡献d时，计算x,k,b的贡献并对k,b梯度下降
            d*=activation->backward(y);
            b-=step*d;
            for(int i=1;i<=num;i++){
                dx[i]=d*k[i];//计算x贡献
                k[i]-=step*d*x[i];//计算k贡献并梯度下降
            }
            d=0;
        }
    }
};
neuron net[N+5];//声明所有可能用到的节点
int Index[N+5];//第i条边为其指向的节点第index[i]个输入
int n=0;//节点数
int m=0;//边数
void addNode(Activation *activation){//添加节点
    net[++n].init(activation);
}
void addNode(int num,Activation *activation){//批量添加节点
    for(int i=1;i<=num;i++){
        addNode(activation);
    }
}
struct edge{//链式前向星（无边权）
    int to=0,nxt=0;
};
class traverse{
private:
    int side=0;//向前还是向后
    int head[N]={};//链式前向星
    edge e[N];
    void update(int x,int y,int index){//更新节点x->y的数值
        if(side==0){
            net[y].x[index]=net[x].y;
        }else if(side==1){
            net[y].d+=net[x].dx[index];
        }
    }
public:
    int ind[N]={};//第i个节点的入度数量
    traverse(int SIDE):side(SIDE){}
    void add(int x,int y,bool visible){//建立x->y的一条边
        if(visible){
            ind[y]++;
        }
        e[m]=edge{y,head[x]};
        head[x]=m;
    }
    void topo(){//拓扑排序
        int temp[N]={};
        for(int i=1;i<=n;i++){
            temp[i]=ind[i];
        }
        queue<int> q;
        for(int i=1;i<=n;i++){
            if(temp[i]==0){
                q.push(i);
            }
        }
        while(!q.empty()){
            int u=q.front();
            q.pop();
            net[u].spread(side);
            for(int i=head[u];i;i=e[i].nxt){
                int v=e[i].to;
                update(u,v,Index[i]);
                temp[v]--;
                if(temp[v]==0){
                    q.push(v);
                }
            }
        }
    }
};
traverse Forward(0);
traverse Backward(1);
void addEdge(int x,int y,bool visible=1){
    net[y].addInput();
    m++;
    Index[m]=net[y].num;
    Forward.add(x,y,visible);
    Backward.add(y,x,visible);
}
void train(double *x,double *y){
    int cnt=0;
    for(int i=1;i<=n;i++){
        if(Forward.ind[i]==0){
            net[i].y=x[++cnt];
        }
    }
    Forward.topo();
    cnt=0;
    for(int i=1;i<=n;i++){
        if(Backward.ind[i]==0){
            net[i].d=net[i].y-y[++cnt];
        }
    }
    Backward.topo();
}
int main(){
    init();
    addNode(4,&sigmoid);
    addEdge(1,2);
    addEdge(1,3);
    addEdge(2,3);
    addEdge(3,4);
    double x[10]={0,0.5},y[10]={0,0.2};
    while(1){
        train(x,y);
        cout<<net[4].y<<endl;
    }
    return 0;
}