# Hello World Example
#
# Welcome to the OpenMV IDE! Click on the green run arrow button below to run the script!

import sensor, image, time,pyb #导入必要的模块
from pyb import UART           #串口
from pyb import LED
Blank=0
#green_threshold   = (10,130) #小球灰度的范围
#green_threshold   = (195,255) 
#green_threshold=(18, 255)
green_threshold=(0, 10, -88, -2, -68, 21)
#green_threshold=(2, 25, -64, -1, -4, 32)

#green_threshold   = (180,255) 
#green_threshold   = (150,255) 
#green_threshold=(19, 100, -128, 127, -128, 127)
#green_threshold=(130, 255)
#具体范围可以连接图像再画一个框，从右下角可以看到范围
uart = UART(3,115200,timeout_char = 1000000) #初始化串口 波特率9600和控制器通信
#ROI=(35,10,300,300)            #窗口大小设置为平板大小 和坐标
sensor.reset()
sensor.set_pixformat(sensor.RGB565)#设置像素模式  GRAYSCALE灰度
sensor.set_framesize(sensor.QQVGA)#设置像素帧大小
#sensor.set_framesize(sensor.VGA)#设置像素帧大小
#sensor.set_windowing(ROI)      #开窗
sensor.skip_frames(10)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False) #关闭白平衡
#x=sensor.width()
#y=sensor.height()
#sensor.set_vflip(True)   #将摄像头反转
sensor.height()
clock = time.clock()
ir_led = LED(4)              #点亮补光灯
bool_x=0
bool_y=0
while(True):
       
        clock.tick()
        Blank=0                 #Blank=0说明有球，Blank=1说明找不到球
        #img = sensor.snapshot() #抓取图像
        img = sensor.snapshot().lens_corr(strength = 1.8, zoom = 1.0)
        blobs = img.find_blobs([green_threshold],pixels_threshold =20,merge=True) #找点
        #img.draw_rectangle(area)
        #img.draw_cross()
        if blobs:               #找到了小球
            for b in blobs:
                #if 10<b.pixels()<50:
                    img.draw_cross(b[5],b[6])          #在右上方的监视器画十字叉丝 有多少画多少             
                    img.draw_rectangle(b[0:4])         #rect
                    bool_x=int(b[5])
                    bool_y=int(b[6])   
        else:
            Blank=1
        if Blank:
            output_str=bytearray([0xff,0xfe,0xfd,0xfd,0x5B])    #打印坐标
            #output_str=bytearray([0xff,0xfe,0xfd,0xfd])
        else:
            output_str=bytearray([0xff,0xfe,int(b[5]),int(b[6]),0x5B]) #打印坐标
        uart.write(output_str)
        #print(clock.fps())
        #print(int(b[5]),int(b[6]))
        #print(output_str)
        print(bool_x,bool_y)
        #print(b.pixels())
        #ir_led.on()