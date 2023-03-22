# Selective-image-encryption-system-based-on-saliency-target
# 项目名称：基于语义理解的选择性图像加密系统

## 项目源码成分分布：
```
    整个工程较为复杂，其中图片加密算法考虑到速度问题主要由C++实现，整个系统的UI与显著性目标识别网络使用Python编写，具体说明如下：
    0.其中Executablefile中是可执行文件，其中main文件夹下载之后点击其中exe文件便可运行整个软件
    1.U2-Net显著性目标识别网络，在main_work中
    2.界面部分，使用pyqt实现，在main_work中
    3.“一次一密”选择性图像加密，在encryption中
    4.“一次一密”选择性图相关解密，在decryption中
    5.连通域分析（bounding box获取）在Connected_domain中
    6.Testsample中存在提供的测试样例
```
## 使用环境说明：
```
    1.U2-Net为pytorch环境
        numpy==1.15.2
        scikit-image==0.14.0
        torch
        torchvision
        pillow==8.1.1
        opencv-python
        paddlepaddle
        paddlehub
        gradio
    2.主界面运行要求
        PyQt5
        qtawesome
    3.加密解密
        编译环境：vs2019
        大部分采用C语言，少量采用C++进行编写
        在整个系统执行过程中，并不需要运行再去编译该部分，因为Python中直接调用了该部分的exe可执行程序
    4.连通域分析
        opencv-C++
```
