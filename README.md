# AI

## 我的人工智能学习日志！

---

## 目录

### Bayes

贝叶斯算法

### BP

图论+神经网络反向传播算法

### KNN

K均值聚拢算法

### TF-IDF

$Term Frequency$词频

$Inverse Document Frequency$逆文本频率指数

---

## Bayes

公式：

$$
P(A|B)=\frac{P(B|A)P(A)}{P(B)}
$$

$$
P(A_i|B)=\frac{P(B|A_i)P(A_i)}{\sum_{j}P(B|A_j)P(A_j)}
$$

其中$A_1,A_2\cdots ,A_n$为完备事件组，即$\bigcup_{i=1}^nA_i=\Omega,A_iA_j=\varnothing,P(A_i)>0$

~~之后再补充概率论吧~~

## BP

~~等我写完PPT~~

## KNN

## TF-IDF

对于某个词汇$i$在第$j$篇文章中的权重$w_{i,j}$

$$
tf_{i,j}=\frac{n_{i,j}}{N_j}
$$

其中$n_{i,j}$表示某词$i$在文档$j$中出现的次数，$N$表示文档$j$的词数

$$
idf_i=lg(\frac{D}{1+d_i})
$$