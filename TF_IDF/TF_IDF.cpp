#include<iostream>
#include<math.h>
#include<algorithm>

using namespace std;
string
dataset[][1005] = { {"quit", "buying", "worthless", "dog", "food", "stupid"},
                    {"maybe", "not", "take", "him", "to", "dog", "park", "stupid"},
                    {"my", "dalmation", "is", "so", "cute", "I", "love", "him"},
                    {"stop", "posting", "stupid", "worthless", "garbage"},
                    {"mr", "licks", "ate", "my", "steak", "how", "to", "stop", "him"} },
test[] =            { "my", "dog", "has", "flea", "problems", "help", "please" };
int data_linesize[] = { 7,8,8,5,9 };
template<class T>
int length(T& arr) {
    return sizeof(arr) / sizeof(arr[0]);
}
double findidf(string s) {
    int sum = 0;
    for (int i = 0; i < length(dataset); i++) {
        for (int j = 0; j < data_linesize[i]; j++) {
            if (s == dataset[i][j]) {
                sum++;
                break;
            }
        }
    }
    return log(double(length(dataset) / (sum + 1.0)));
}
struct word {
    string s;
    int fre_test;
    double tf, idf, tf_idf;
}list[1005];
int num;
void copystr(int num, string str) {
    for (int i = 0; i < str.length(); i++) {
        list[num].s[i] = str[i];
    }
}
void feature_select() {
    for (int i = 0; i < length(test); i++) {
        bool flag = 1;
        for (int j = 1; j <= num; j++) {
            if (list[j].s == test[i]) {
                flag = 0;
                list[j].fre_test++;
                break;
            }
        }
        if (flag) {
            num++;
            list[num].idf = findidf(test[i]);
            list[num].s = test[i];
            list[num].fre_test++;
        }
    }
    for (int i = 1; i <= num; i++) {
        list[i].tf = double(list[i].fre_test * 1.0 / length(test));
        list[i].tf_idf = list[i].tf * list[i].idf;
    }
}
bool cmp(word x, word y) {
    return x.tf_idf > y.tf_idf;
}
int main() {
    feature_select();
    sort(list + 1, list + 1 + num, cmp);
    for (int i = 1; i <= num; i++) {
        cout << list[i].s << " " << list[i].tf_idf << endl;
    }
    return 0;
}
