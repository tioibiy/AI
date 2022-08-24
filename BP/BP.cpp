#include<bits/stdc++.h>
#include<time.h>
#include<random>
#include<matplotlibcpp.h>

namespace plt = matplotlibcpp;
using namespace std;
double temp;
vector<double> draw;
const int N=(5e3);//最大值
double step=0.1;//学习步长s
void randInit(){//初始化
    srand(time(NULL));//随机数初始化
}
class Activation{//激活函数基类
public:
    virtual void forward(int num,double *x,double *k,double &y){
        for(int i=1;i<=num;i++){
            y+=x[i]*k[i];
        }
        y+=k[num+1];
    }
    virtual void backward(int num,double *x,double *k,double &y,double &d,double *dx){
        k[num+1]-=step*d;
        for(int i=1;i<=num;i++){
            dx[i]=d*k[i];//计算x贡献
            k[i]-=step*d*x[i];//计算k贡献并梯度下降
        }
    }
}a;
class Sigmoid:public Activation{
public:
    void forward(int num,double *x,double *k,double &y){
        for(int i=1;i<=num;i++){
            y+=x[i]*k[i];
        }
        y+=k[num+1];
        y=1.0/(1+exp(-y));
    }
    void backward(int num,double *x,double *k,double &y,double &d,double *dx){
        d*=y*(1-y);
        k[num+1]-=step*d;
        for(int i=1;i<=num;i++){
            dx[i]=d*k[i];
            k[i]-=step*d*x[i];
        }
    }
}sigmoid;
class ReLu:public Activation{//ReLu激活函数
public:
    void forward(int num,double *x,double *k,double &y){
        for(int i=1;i<=num;i++){
            y+=x[i]*k[i];
        }
        y+=k[num+1];
        y=max(0.0,y);
    }
    void backward(int num,double *x,double *k,double &y,double &d,double *dx){
        if(y<0){
            d=0;
        }
        k[num+1]-=step*d;
        for(int i=1;i<=num;i++){
            dx[i]=d*k[i];
            k[i]-=step*d*x[i];
        }
    }
}relu;
class Binary_Crossentropy:public Activation{//交叉熵
public:
    void forward(int num,double *x,double *k,double &y){
        for(int i=1;i<=num;i++){
            y-=x[i]*log(k[i]);
        }
        y/=num;
    }
    void backward(int num,double *x,double *k,double &y,double &d,double *dx){
        d=1;
        for(int i=1;i<=num;i++){
            dx[i]=-log(k[i])/num;
        }
    }
}binary_crossentropy;
class L1:public Activation{//L1范数
public:
    void forward(int num,double *x,double *k,double &y){
        for(int i=1;i<=num;i++){
            y+=abs(x[i]-k[i]);
        }
    }
    void backward(int num,double *x,double *k,double &y,double &d,double *dx){
        d=1;
        for(int i=1;i<=num;i++){
            if(x[i]>k[i]){
                dx[i]=1;
            }else{
                dx[i]=-1;
            }
            //dx[i]=x[i]-k[i];
            
        }
    }
}l1;
class neuron{//神经元
private:
public:
    int num=0;//输入的x的数量
    Activation (*activation)=&sigmoid;
    double x[N+5]={},k[N+5]={},y=0;//y=sigmoid(sum(kx)+b)
    double d=0,dx[N+5]={};//权重(求导) y对输出的权重，x对输出的权重
    neuron(){}
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
            activation->forward(num,x,k,y);
        }else if(side==1){//已知y对下一个节点的贡献d时，计算x,k,b的贡献并对k,b梯度下降
            activation->backward(num,x,k,y,d,dx);
            d=0;
        }
    }
};
neuron net[N+5];//声明所有可能用到的节点
int Index[N+5];//第i条边为其指向的节点第index[i]个输入
int n=0;//节点数
int m=0;//边数
void addNode(Activation *activation,bool vis=1){//添加节点
    if(vis==1)
        net[++n].init(activation);
    else
        net[n+1].init(activation);
}
void addNode(int num,Activation *activation){//批量添加节点
    for(int i=1;i<=num;i++){
        addNode(activation);
    }
}
struct edge{//链式前向星（无边权）
    int to=0,nxt=0;
};
class traverse{//遍历
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
    friend void train(double *x,double *y);
    void add(int x,int y,bool visible){//建立x->y的一条边
        if(visible){
            ind[y]++;
        }
        e[m]=edge{y,head[x]};
        head[x]=m;
    }
    void topo(){//拓扑排序
        int temp[N]={};
        for(int i=1;i<=n+1;i++){
            temp[i]=ind[i];
        }
        queue<int> q;
        if(side==0){
            for(int i=1;i<=n;i++){
                if(temp[i]==0){
                    q.push(i);
                }
            }
        }else if(side==1){
            q.push(n+1);
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
void addEdge(int x,int y,bool vis=1){
    net[y].addInput();
    m++;
    Index[m]=net[y].num;
    Forward.add(x,y,vis);
    Backward.add(y,x,vis);
}
void init(Activation *activation){
    addNode(activation,0);
    for(int i=1;i<=n;i++){
        if(Backward.ind[i]==0){
            addEdge(i,n+1);
        }
    }
}
void train(double *x,double *y){
    int cnt=0;
    for(int i=1;i<=n;i++){
        if(Forward.ind[i]==0){
            net[i].y=x[++cnt];
        }
    }
    for(int i=1;i<=net[n+1].num;i++){
        net[n+1].k[i]=y[i];
    }
    Forward.topo();
    step=net[n+1].y*10;
    temp=net[n+1].y;
    //printf("%.6lf\t%.6lf\t%.6lf\t%.6lf\n",x[1],y[1],net[n].y,net[n+1].y);
    Backward.topo();
}
void clean(){
    for(int i=1;i<=n+1;i++){
        net[i].d=net[i].y=0;
        for(int j=1;j<=net[i].num;j++){
            net[i].dx[j]=net[i].x[j]=0;
        }
    }
}
int main(){
    freopen("BP3.out","w",stdout);
    randInit();
    addNode(2,&sigmoid);
    addEdge(1,2);
    // addEdge(2,3);
    // addEdge(1,3);
    // addEdge(2,4);
    // addEdge(3,4);
    // addEdge(2,5);
    // addEdge(3,5);
    // addEdge(4,6);
    // addEdge(5,6);
    init(&l1);
    for(int j=1;j<=2e3;j++){
    //while(1){
        double ans=0;
        for(double i=0.2;i<=0.9;i+=0.1){
            double x[10]={},y[10]={};
            x[1]=i;
            y[1]=i;
            train(x,y);
            clean();
            ans=max(ans,temp);
        }
        cout<<ans<<" ";
        //draw.push_back(ans);
    }
    cout<<endl;
    return 0;
}