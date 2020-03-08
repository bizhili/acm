 18402218@smail.cczu.edu.com
 感谢博文：
 [https://blog.csdn.net/zhangyue_lala/article/details/64437547?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task](https://blog.csdn.net/zhangyue_lala/article/details/64437547?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)
 ## 基于牛顿插值的实时概率预测方法
  前言：实时预测作用于采样频率较高的场景（大于10Hz），实时预测的作用大多是为了提高采样频率，类似于提前插值。从实际上说，未来时刻的位置信息是未知的，只能基于概率预测。
 ### 注意！！！预测前的数据必须平滑化处理
 ### 1.实时预测的原则
 1.尽可能多地利用确定的坐标信息(Newton法)
 2.更多地相信时间近的坐标信息（定最小确性度）
 3.具有安全性（长时间没有得到实际坐标点的更新时的安全处理，概率监测）
 ### 2.算法
 ### 2.1对于n个已知点的概率预测
 已知n个x1,x2,x3...xn
 下标数字越大时间离得越近
 #### 只有1个点xn时：
 当$\Delta$T-->0时，x在xn位置的概率趋近于1，即：
 P(xn)-->1,其它地方的概率均为0。
 在$\Delta$T时间上我们可以知道x的位置概率服从高斯分布：
 $N$(xn,$p^2$)
 当$\Delta$T-->0时，p-->$0$,为了计算p取0.1
 随着时间的增长，x位置的不确定性也随之增加，对于高斯分布，即p值会慢慢变大，直到平坦。（图形如下）
![输入图片描述](3_1_newton_predict_md_files/Figure_1_20200307152247.png?v=1&type=image&token=V1:2gOdpEB-hCAk439B9MkcT3RchTmApiGvrFCoTZFIk84)
 这里
 $z$=$N$(0,$p^2$)
 $p$=y
 这里我们让方差$p$随着时间线性变化，随着时间的变化，虽然0点的位置概率始终最大，但概率值在不断缩小，当$p$增长到一定程度，0点的可行度是很低的，我们不得不放弃这个点，所以在模型中我们必须给出最小确信度$P$min用来舍弃一些时间较长的点。
 如果最相近的一点低于最小确信度，进入报错程序
 #### 只有2个点x(n-1),xn时：
 当$\Delta$T-->0时，x在xn位置的概率趋近于1，即：
 P(xn)-->1,其它地方的概率均为0。
 如果x(n-1)点在我们上述的最小确性度内：
 我们可以计算出
 $V$=[$f$(xn)-$f$(x(n-1))]/xn-x(n-1)
 在$\Delta$T时间上我们可以知道x的位置概率服从高斯分布：
 $N$(xn+$V$*$\Delta$T,$p^2$)
 这时图像如下：
 ![输入图片描述](3_1_newton_predict_md_files/Figure_1_20200307155452.png?v=1&type=image&token=V1:NlqqtRWxmi0ab3XoIKMiCDg8YdtYoqpDmRqKLmXwOm0)
  这里
 $z$=$N$($p/2$,$p^2$)
 $p$=y
 这里我们让方差$p$随着时间线性变化，随着时间的变化，虽然p/2点的位置概率始终最大，但概率值在不断缩小，当$p$增长到一定程度，当x(n-1)点已经小于我们的最小确性度$P$min，我们就要使用上面的只有一个点的预测方法。
 ### 同理有n个已知点可以用上述方法预测
 以上方法满足了我们的2，3条原则：
 2.更多地相信时间近的坐标信息（定义最小确性度）
 3.具有安全性（长时间没有得到实际坐标点的更新时的安全处理，概率监测）
 ### 2.2定义差商
 ![输入图片描述](3_1_newton_predict_md_files/20170326210211096_20200308153205.png?v=1&type=image&token=V1:37CZy5nLjRVXnp8fAjoTVNTyhKDnTenI5qUiFX8PPDs)
可以很好地得到这些性质：
![](https://img-blog.csdn.net/20170326210230409?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmd5dWVfbGFsYQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
定义差商对我们的下面表示方便了不少。
可以看到：
对于物体运动，一阶差商得到速度，二阶差商得到加速度，三阶差商是加速度的微分...对于n阶差商我们会用到n+1个状态的坐标。对于下一个$\Delta$T时刻的位置信息的预测，我们可以从n阶差商逐渐下降到0阶差商得到下一个$\Delta$T时刻的坐标：
$\Delta Pt$
=$f$[x(n-1,xn)]+$f$(xn)
={$f$[x(n-2),x(n-1),xn]*$\Delta$T+$f$[x(n-1,xn)]}*$\Delta$T+$f$(xn)
...
通过牛顿法，我们可以达到我们的第一条原则：
 1.尽可能多地利用确定的坐标信息
 ### 2.3算法流程
 ![输入图片描述](3_1_newton_predict_md_files/gauss_predic_20200307182655.png?v=1&type=image&token=V1:op4-pZaoo86GXFHPUMIkTMkiCLGdn4C0BfuwAwn9lME)
### 3.python实现
### 3.1加上噪音的模拟数据
```
import math

import json

import numpy

import threading

import time

import matplotlib.pylab as plt

#store

pool=[]

#store predicted position

predict_1=[]

#store predicted time

time_1=[]

#threading protected lock

lock=threading.Lock()

#to get Difference quotient of a list

def  get_DQ(list1=[]):

answers=[]

next=list1

new_next=[]

while  len(next)>1:

new_next=[]

for i in  range(len(next)-1):

temp=[]

val=(next[i+1][0]-next[i][0])/(next[i+1][1]-next[i][1])

temp.append(val)

time1=(next[i+1][1]+next[i][1])/2

temp.append(time1)

new_next.append(temp)

if i==len(next)-2:

answers.append(round(val,4))

next=new_next

return answers

#class a class of predict ,with inherited class of threading,Thread

class  predict(threading.Thread):

#unit of od trust:microseconds,of freq:Hz,

def  __init__(self,trust,dimension,freq):

threading.Thread.__init__(self)

self.trust=trust

self.dimension=dimension

self.freq=freq

self.piece=1000/freq

self.thisTime=0

self.last_time=0.0

self.DQ=[]

self.next_predict=0

self.last_position=0

def  run(self):

global pool

global predict_1

global lock

while  True:

lock.acquire()

for i in pool:

if i[1]<self.thisTime-self.trust:

pool.pop(0)

else :

break

lock.release()

length=len(pool)

if  len(pool)==0:

self.go_error()

elif pool[length-1][1]!=self.last_time:

predict_1.append(pool[length-1][0])

self.last_time=pool[length-1][1]

self.last_position=pool[length-1][0]

self.thisTime=self.last_time

time_1.append(self.thisTime)

else :

self.DQ=get_DQ(pool)

value=self.calculate()

self.last_position+=value

predict_1.append(self.last_position)

time_1.append(self.thisTime)

time.sleep(self.piece/1000)

self.thisTime+=self.piece

def  go_error(self):

print("out of time!")

if  len(predict_1)>10:

exit(0)

def  calculate(self):

predict_2=0

size=len(self.DQ)

for i in  range(len(self.DQ)):

predict_2=(predict_2+self.DQ[size-1-i])*self.piece

return predict_2

  

def  main():

#list1=[[1,2.3],[2.1,3],[2.9,4],[4,5.3],[5.1,6],[6.2,7.3],[7.2,9],[8,10],[8.8,11],[9.7,12]]

#list2=[[1,2],[2,3],[3,4],[4,5],[5,6],[6,7],[7,8],[7.3,9],[7.4,10]]

#to use this class with trust =200,dimension=1,expect freq=100

task1=predict(200,1,100)

global pool

global lock

#read json file

with  open("myData3.json",'r') as load_f:

load_dict = json.load(load_f)

#threading start

task1.start()

last_time=0

#store original data

or_p=[]

or_t=[]

for i in load_dict:

time.sleep((load_dict[str(i)][1]-last_time)/1000)

lock.acquire()

pool.append(load_dict[str(i)])

lock.release()

or_p.append(load_dict[str(i)][0])

or_t.append(load_dict[str(i)][1])

last_time=load_dict[str(i)][1]

task1.join()

#draw the graph

plt.plot(time_1,predict_1,'.')

plt.plot(or_t,or_p,'.')

plt.plot()

plt.show()

  

if  __name__=="__main__":

main()

```
实验中用到的数据可以从这里下载
https://www.jianguoyun.com/p/DVC73JYQ7L2UCBiHh-4C
代码实时地预测，从25帧达到了100帧，图像如下：
![输入图片描述](3_1_newton_predict_md_files/Figure_5_20200308144623.png?v=1&type=image&token=V1:9G_WUvHJfa5iu5xeaagpNFg-P472QVdUQ2Zt5B0Z154)
图像中蓝色点为插值插入的点，橙色为采样数据点。
我们可以得出：
1.该预测可以很好地预测出足够平滑采样点的数据
（要求采样帧率至少10Hz）
2.该预测是非线性的，从最后的一段蓝色的预测中可以得出
3.如果采样数据不够平滑，该预测结果离散度将会很大
### 无噪音的函数图像数据
我们再对正弦函数做一次预测,以计算一次偏差率
可以得到这样的模拟图像：
![输入图片描述](3_1_newton_predict_md_files/Figure_7_20200308150804.png?v=1&type=image&token=V1:hdaUUmC9cVzS8alXBpekKKE6b2yotHh7BfjVlihU9rY)
在这次模拟中，采样频率为12.5Hz，期待帧率为100帧，可以发现出现了明显的滞后现象。
采样频率设置为25Hz后，滞后现象有明显改进：
![输入图片描述](3_1_newton_predict_md_files/Figure_6_20200308151156.png?v=1&type=image&token=V1:BUTrE2GY2qNzKqdWJf1HNUzo05uZAqIpJQXQwSCCrC8)
最后我们来求出12.5Hz时的累积误差，公式如下：
$D$sum=$\sum$abs($f$(x)-$Predict$(x))
![输入图片描述](3_1_newton_predict_md_files/Figure_8_20200308152449.png?v=1&type=image&token=V1:pKmQe8NsQYKOIoBV6P-wD4Lyf3ZE6iB9z6rkPprLfsc)
最后累积误差小于4.0
本人不擅数理，欢迎批评指正





