# 工程说明
## 使用说明
```
    直接运行main.py即可开始运行整个工程
    若想单独测试U2-Net请运行u2net_test.py
    u2net显著性目标检测网络的详细说明可见README-U2Net.md
```
## 文件组织说明
``` 
    choosepic文件中存放运行当前已经选择的图片
    Connected_domain文件中存放连通域分析算法的exe程序与一些依赖项
    coordinates文件中存放已经获取到的ROI区域信息（bounding box）
    d_results文件中存放解密好的图片
    decryption文件中存放加密的可执行程序以及一些依赖项
    e_results文件中存放加密之后的图片
    encryption中存放加密的可执行程序以及一些依赖项
    key中存放可用的密钥以及密钥的添加位置
    save_models中存放预训练的权重文件
```
## 架构说明
``` 
    main.py为可运行的UI界面，在该文件中会调用U2-Net、加密的可执行程序
    解密的可执行程序、以及连通域分析程序（bounding box获取）
```