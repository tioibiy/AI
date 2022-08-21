from numpy import *
import matplotlib.pyplot as plt


spt=700
size=1000
k=20

mat=[]
label=[]

def readData():
    filename="KNN/KNN1/data/dataset.txt"
    f=open(filename,"r",encoding='utf-8')
    data=f.readlines()
    lines=len(data)
    returnMat=zeros((lines+1,3))
    label=[0]
    index=1
    for line in data:
        line=line.strip()
        inform=line.split("\t")
        returnMat[index]=inform[:3]
        label+=[inform[-1]]
        index+=1
    f.close()
    return returnMat,label

def dis(x,y):
    return sqrt(sum((x-y)**2))

def test(x):
    tlist=[0.0]
    for i in range(1,spt+1):
        tlist+=[dis(mat[i],mat[x])]
    tlist.sort()
    ans=[0,0,0,0]
    for i in range(1,k+1):
        ans[label[i]]+=1
    return max(ans[1:])


if __name__ == '__main__':
    mat,label=readData()
    label = list(map(int,label))
    fig = plt.figure()
    ax = plt.axes(projection='3d')
    ax.scatter(mat[1:,0],mat[1:,1],mat[1:,2],15.0*array(label[1:]),15.0*array(label[1:]),15.0*array(label[1:]))
    plt.show()
    crt=0
    for i in range(spt-1,1000):
        if test(i)==label[i]:
            crt+=1
    print(crt/(1001-spt))