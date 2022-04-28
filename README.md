# control_BALL
##从零开始搭建板球系统
1.首先移植自己选定芯片的库函数版本，我选用的是C8T6

2.移植FreeRTOS实时操作系统，我是freertos操作系统理论看的比较多，第一次实际应用碰到一些坑，先转裸机等过段时间有空了在移植到freertos系统中。

3.蓝牙模块控制舵机的位置确定好舵机平衡的PWM值。这步骤之前碰到挺多坑的，什么电源问题，舵机问题，定时器设置问题等等，一步一步排查完成这部。

4.视觉方案确定好。我采用的是OPENMVH7，因为之前关于视觉方面什么都不懂协会有什么就用什么就直接用openmv，但是为了长远考虑建议转k210用opencv更猛，而且外面很多公司都会专门招聘opencv相关岗位。不要为了电赛的方便选择openmv。这边方案是白色的地板，然后选择识别黑色的球。

5.理解视觉中的坐标，并且跟stm32能够进行通讯。这边要注意的就是如果你的画质是QQVAG的话整个图像的坐标系就会超过一个字节256，因为发送数据的时候都是按一个字节放松的，所以stm32接收的时候要么用联合体或者分高低位接收组合成int型，或者改成低画质x和y的坐标系就不会超一个字节。

6.利用摄像头传过来的坐标+PID控制使得小球能够稳定在中心点。如果自己准备从零开始写的话pid的部分可以参考我上传的arduino pid库的写法，不过是用c++写的其实了解面向对象就能够看的懂，并且写成c风格结构体。我PID部分已经写完了过后才发现这个的。我自己之前对arduino使用带有一点”偏见“的，所以说看待任何一个东西都不能带有偏见，要不然你就损失了arduino这个生态活跃并且经过打磨的优秀库可以做为参考。
7.调参技巧：不要两个方向一起调整，让小球只在一个方向运动并在这个方向进行调参。首先调P让小球有震荡的效果，这部其实要在脑子有个“建模”过程，就是能够大概的知道坐标差乘上设置的p对应舵机活动的范围。然后调整i让小球能够有抖动的效果，最后调整d消除抖动。之前pid计算是放在接收中断当中，因为串口中断没有固定的时间，并且线比较长接收过程中难免数据丢失，效果就没有那么好，固放到定时器中断当中。之前在matlab上是自己改pid的参数查看曲线变化，后面有时间尝试用matlab给这个板球建个模然后能够推算出pid的参数做为参考，这样在以后碰到其他的项目就能够更快的调参。
8.球的选择：我第一次是直接去超市买乒乓球然后涂黑，但是乒乓球太轻了，板子倾斜30度的重力的向下的作用力都大不过摩擦力，所以选一个稍微重一点的球能够尽量的减少摩擦力的影响。
9.不仅要把小球控制在中间，还要控制小球从中间到左边或者右边并且不能够出线，如果出板子范围的话说明“刹车”的效果不明显要将d的参数调大来。
10.两个位置都能控制好了那么完成任务就非常的简单了。

机械结构
![IMG_4458](https://user-images.githubusercontent.com/58476906/164168354-d4f074b7-fc1f-41da-83be-a8f9507baa41.JPG)
