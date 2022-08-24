from cProfile import label
import matplotlib.pyplot as plt
from pylab import *

l1=open("BP\BP1.out").read().splitlines()[0].split( )
l2=open("BP\BP2.out").read().splitlines()[0].split( )
l3=open("BP\BP3.out").read().splitlines()[0].split( )

for i in range(len(l1)):
    l1[i]=float(l1[i])
    l2[i]=float(l2[i])
    l3[i]=float(l3[i])

plt.plot(l1,label='6 nodes')
plt.plot(l2,label='3 nodes')
plt.plot(l3,label='2 nodes')

plt.legend()

plt.xlabel("times")
plt.ylabel("lost-L1")

plt.show()