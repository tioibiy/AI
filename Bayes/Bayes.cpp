#include<iostream>
#include<math.h>
#include<algorithm>
#include<vector>

using namespace std;
const int max_FctSize=105;
const double Lsmooth=0.01;
int dataFctSize;
//最小单元，关于正反例与总数据在int上的结构体
struct node_int{
    int pst,ngt,dataset;
};
//最小单元，关于正反例与总数据在double上的结构体
struct node_double{
    double pst,ngt,dataset;
};
//一个int类型单元和一个double类型单元组合而成的结构体
struct dataNode{
    node_int count;
    node_double ratio;
}alldata;
//对于一个属性，记录其不同取值的结构体
struct fctNode{
    vector<string> fctId;
    vector<node_int> count;
    vector<node_double> ratio;
};
fctNode fct[max_FctSize];
double smooth(double x,double y){
    return (x+Lsmooth)/(y+Lsmooth);
}
void trainData(){
    cin.clear();
    freopen("train\\play_golf_train_dataInformation.txt","r",stdin);
    cin>>alldata.count.dataset>>dataFctSize;
    cin.clear();
    freopen("train\\play_golf_train_data.txt","r",stdin);
    for(int i=1;i<=alldata.count.dataset;i++){
        string s[max_FctSize];
        for(int j=1;j<=dataFctSize;j++){
            cin>>s[j];
        }
        string lable=s[dataFctSize];
        if(lable=="yes"){
            alldata.count.pst++;
        }else{
            alldata.count.ngt++;
        }
        for(int j=1;j<dataFctSize;j++){
            bool flag=1;
            for(int k=0;k<fct[j].fctId.size();k++){
                if(fct[j].fctId[k]==s[j]){
                    fct[j].count[k].dataset++;
                    if(lable=="yes"){
                        fct[j].count[k].pst++;
                    }else{
                        fct[j].count[k].ngt++;
                    }
                    flag=0;
                    break;
                }
            }
            if(flag){
                fct[j].fctId.push_back(s[j]);
                if(lable=="yes"){
                    fct[j].count.push_back({1,0,1});
                }else{
                    fct[j].count.push_back({0,1,1});
                }
            }
        }
    }
    alldata.ratio.pst=alldata.count.pst*1.0/alldata.count.dataset;
    alldata.ratio.ngt=alldata.count.ngt*1.0/alldata.count.dataset;
    for(int i=1;i<=dataFctSize;i++){
        for(int j=0;j<fct[i].fctId.size();j++){
            node_int temp=fct[i].count[j];
            fct[i].ratio.push_back({smooth(temp.pst*1.0,temp.dataset*1.0),smooth(temp.ngt*1.0,temp.dataset*1.0),1});
        }
    }
    // for(int i=1;i<=dataFctSize;i++){
    //     for(int j=0;j<fct[i].fctId.size();j++){
    //         cout<<fct[i].fctId[j]<<endl<<"\tpst:"<<fct[i].count[j].pst<<" ngt:"<<fct[i].count[j].ngt<<" all:"<<fct[i].count[j].dataset<<endl;
    //         //<<"\tpst ratio:"<<fct[i].ratio[j].pst<<" ngt ratio:"<<fct[i].ratio[j].ngt<<endl;
    //     }
    // }
    cin.clear();
    return;
}
double test(){
    cin.clear();
    freopen("test\\play_golf_test_information.txt","r",stdin);
    int testSum;
    cin>>testSum;
    cin.clear();
    freopen("test\\play_golf_test.txt","r",stdin);
    int success=0;
    double
    ans_pst_const=pow(alldata.count.dataset/alldata.count.pst,dataFctSize-1),
    ans_ngt_const=pow(alldata.count.dataset/alldata.count.ngt,dataFctSize-1);
    for(int i=1;i<=testSum;i++){
        string s[max_FctSize];
        for(int j=1;j<=dataFctSize;j++){
            cin>>s[j];
        }
        string label=s[dataFctSize];
        /*
        P(B|A)=P(A|B)*P(B)/P(A)
        */
        double
        ans_pst=ans_pst_const,
        ans_ngt=ans_ngt_const;
        for(int j=1;j<dataFctSize;j++){
            int k=0;
            for(k=0;k<fct[j].fctId.size();k++){
                if(fct[j].fctId[k]==s[j]){
                    break;
                }
            }
            ans_pst*=fct[j].ratio[k].pst;
            ans_ngt*=fct[j].ratio[k].ngt;
        }
        printf("%.7lf\t%.7lf\t",ans_pst,ans_ngt);
        cout<<label<<"\t";
        if((label=="yes"&&ans_pst>=ans_ngt)||(label=="no"&&ans_pst<=ans_ngt)){
            success++;
            printf("success");
        }
        printf("\n");
    }
    return success*1.0/testSum;
}
int main(){
    trainData();
    cout<<test();
    return 0;
}