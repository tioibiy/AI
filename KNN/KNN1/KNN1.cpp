#include<bits/stdc++.h>
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;
using namespace std;
const int size_a=3,N=1000,size_train=700,type=3,showId_x=1,showId_y=2;
int k=20;
double maxa[size_a+1],mina[size_a+1];
struct node{
    double a[size_a+1];//属性
    int id;//标签
}d[N+1];
void readData(){
    //输入
    freopen("data\\dataset.txt","r",stdin);
    for(int i=1;i<=N;i++){
        for(int j=1;j<=size_a;j++){
            cin>>d[i].a[j];
            if(d[i].a[j]>maxa[j]){
                maxa[j]=d[i].a[j];
            }
            if(d[i].a[j]<mina[j]){
                mina[j]=d[i].a[j];
            }
        }
        cin>>d[i].id;
    }
    //线性归一化
    vector<double> x,y;
    for(int i=1;i<=N;i++){
        for(int j=1;j<=size_a;j++){
            d[i].a[j]=(d[i].a[j]-mina[j])/(maxa[j]-mina[j]);
        }
        x.push_back(d[i].a[showId_x]);
        y.push_back(d[i].a[showId_y]);
    }
    //绘图
    plt::scatter(x,y);
}
double dis(int x,int y){
    double ans=0;
    for(int i=1;i<=size_a;i++){
        ans+=(d[x].a[i]-d[y].a[i])*(d[x].a[i]-d[y].a[i]);
    }
    return sqrt(ans);
}
int testOne(int x){
    double temp[size_train+1]={2e9};
    int ans[size_train+1]={};
    for(int i=1;i<=size_train;i++){
        temp[i]=dis(x,i);
        for(int j=1;j<=k;j++){
            if(temp[ans[j]]>temp[i]){
                for(int l=k;l>j;l--){
                    ans[l]=ans[l-1];
                }
                ans[j]=i;
                break;
            }
        }
    }
    int cnt[type+1]={},rtn=0,maxnum=-1;
    for(int i=1;i<=k;i++){
        cnt[d[ans[i]].id]++;
    }
    for(int i=1;i<=type;i++){
        if(cnt[i]>maxnum){
            maxnum=cnt[i];
            rtn=i;
        }
    }
    vector<double> u,v;
    for(int i=1;i<=k;i++){
        u.push_back(d[ans[i]].a[showId_x]);
        v.push_back(d[ans[i]].a[showId_y]);
    }
    plt::scatter(u,v);
    return rtn;
}
double testAll(){
    int cnt=0;
    for(int i=size_train+1;i<=N;i++){
        if(testOne(i)==d[i].id){
            cnt++;
        }
    }
    return cnt*1.0/(N-size_train);
}
int main(){
    readData();
    /*//k对成功率的影响
    vector<double> x,ans;
    for(int i=1;i<=100;i++){
        k=i;
        x.push_back(i);
        ans.push_back(testAll());
    }
    plt::plot(x,ans);
    plt::show();*/
    cout<<testAll();
    plt::xlim(0,1);
    plt::ylim(0,1);
    plt::show();
    return 0;
}