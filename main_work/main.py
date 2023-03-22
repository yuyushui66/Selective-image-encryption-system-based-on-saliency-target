import shutil
from showpicture import Picture
import sys,os
from PyQt5.QtWidgets import (QWidget, QLabel, QVBoxLayout, QHBoxLayout, QApplication, QPushButton,
                             QFileDialog, QGridLayout, QListWidget, QMessageBox, QListWidgetItem)
from PyQt5 import QtWidgets
import qtawesome
import datetime
import threading
import u2net_test
from PyQt5.QtGui import QPixmap, QIcon
from PyQt5.QtCore import (Qt, QSize, pyqtSignal)

#获取当前文件目录绝对地址
global fileBasePath
fileBasePath = os.getcwd()
# 储存当前正在处理的文件夹名
global now_name

# 储存当前密匙列表
global key_list
key_list = []
for srcfile in os.listdir(fileBasePath + "\\key"):
    key_list.append(srcfile)
# print(key_list)

# 储存当前密匙,默认情况为密匙文件中第一个密匙文件
global now_key
now_key = fileBasePath + "\\key\\" + key_list[0]
# print(now_key)

global tmp_key
tmp_key = now_key

class ListWidget(QListWidget):
    def clicked(self, item):
        global now_key
        global tmp_key
        tmp_key = fileBasePath + "/key/" + item.text()
        print(tmp_key)

class MainUi(QWidget):
    # 储存选择的图片的路径
    path_list = []
    # 界面设计与信号连接

    def __init__(self):
        super().__init__()

        logo = fileBasePath + "\\logo.ico"
        icon = QIcon(logo)
        self.setWindowIcon(icon)
        self.setWindowTitle("基于语义理解的选择性图像加密系统")
        # 隐藏主窗口边界
        # self.setWindowFlags(Qt.FramelessWindowHint)
        # 整个界面分为左右，设置大小为1200*800
        self.left_area = QWidget(self)
        self.right_area = QWidget(self)
        self.right_area.setStyleSheet(
            '''
            QWidget{
                border:1px groove gray;
                border-radius:20px;
                background:white;
            }
            '''
        )
        # self.left_area.setStyleSheet()
        self.setFixedSize(1200, 800)
        # 栅格布局，8行12列
        grid = QGridLayout(self)
        self.setLayout(grid)
        grid.addWidget(self.left_area, 0, 0, 8, 2)
        grid.addWidget(self.right_area, 0, 2, 8, 10)

        # 分别初始化左右功能区，左边为按钮区，右边为图像展示区
        # 左边为按钮功能区，按盒布局
        left_grid = QVBoxLayout()
        self.left_area.setLayout(left_grid)
        # 增加按钮
        #self.start_button = QPushButton("开始", self.left_area)
        self.start_button = QtWidgets.QPushButton(qtawesome.icon('fa.home', color='#2c3a45'), "开始")
        self.start_button.setStyleSheet(
            '''
            QPushButton{
            border:1px groove gray;
            border-radius:20px;
            border-style：outset;
            background:Qcolor(250,250,250);
            font-size:20px;
            text-align:centre;
            height:70px;
            font-family:'微软雅黑';
             }
            QPushButton:hover{background:grey;}
            ''')
        self.detection_button = QtWidgets.QPushButton(qtawesome.icon('fa.eye', color='#2c3a45'), "识别")
        self.detection_button.setStyleSheet(
            '''
            QPushButton{
            border:1px groove gray;
            border-radius:20px;
            border-style：outset;
            background:Qcolor(250,250,250);
            font-size:20px;
            text-align:centre;
            height:70px;
            font-family:'微软雅黑';
             }
            QPushButton:hover{background:grey;}
            ''')
        self.encry_button = QtWidgets.QPushButton(qtawesome.icon('fa.bars',color='#2c3a45'),"加密")
        self.encry_button.setStyleSheet(
            '''
            
            QPushButton{
            border:1px groove gray;
            border-radius:20px;
            border-style：outset;
            background:Qcolor(250,250,250);
            font-size:20px;
            text-align:centre;
            height:70px;
            font-family:'微软雅黑';
             }
            QPushButton:hover{background:grey;}
            ''')
        self.decry_button = QtWidgets.QPushButton(qtawesome.icon('fa.file-text',color='#2c3a45'),"解密")
        self.decry_button.setStyleSheet(
            '''
            
            QPushButton{
            border:1px groove gray;
            border-radius:20px;
            border-style：outset;
            background:Qcolor(250,250,250);
            font-size:20px;
            text-align:centre;
            height:70px;
            font-family:'微软雅黑';
             }
            QPushButton:hover{background:grey;}
            ''')
        self.key_button = QtWidgets.QPushButton(qtawesome.icon('fa.search',color='#2c3a45'),"密钥")
        self.key_button.setStyleSheet(
            '''
          
            QPushButton{
            border:1px groove gray;
            border-radius:20px;
            border-style：outset;
            background:Qcolor(250,250,250);
            font-size:20px;
            text-align:centre;
            height:70px;
            font-family:'微软雅黑';
             }
            QPushButton:hover{background:grey;}
            ''')
        # 信号连接
        self.start_button.clicked.connect(self.start)
        self.detection_button.clicked.connect(self.show_u2net)
        self.encry_button.clicked.connect(self.encryption)
        self.decry_button.clicked.connect(self.decryption)
        self.key_button.clicked.connect(self.key)
        # 插入到盒布局中
        left_grid.addWidget(self.start_button)
        left_grid.addWidget(self.detection_button)
        left_grid.addWidget(self.encry_button)
        left_grid.addWidget(self.decry_button)
        left_grid.addWidget(self.key_button)

        # 右边为图像展示区，按盒布局
        right_grid = QVBoxLayout()
        self.right_area.setLayout(right_grid)
        # 两个图像展示区
        self.origin_pic = Picture(self.right_area)
        self.modify_pic = Picture(self.right_area)
        right_grid.addWidget(self.origin_pic)
        right_grid.addWidget(self.modify_pic)

        #
        self.key_widget = QWidget()

    # 开始，选择图片并展示，且后台调用显著性区域识别函数
    def start(self):
        global now_name
        # 流程 #
        # 选择图片，展示
        # 调用U-2net算法，获得处理后图片，展示
        self.origin_pic.change_path_list()
        self.path_list = self.origin_pic.path_list
        t2 = threading.Thread(target=self.u2net)
        t2.start()
        pass

    # 显著性区域识别连接函数
    def u2net(self):
        global now_name
        now = datetime.datetime.now().strftime('%Y%m%d%H%M%S')
        now_name = now
        # print(now)
        dstpath = fileBasePath + "\\choosepic\\" + now_name
        for srcfile in self.path_list:
            if not os.path.exists(dstpath):
                os.makedirs(dstpath)
            shutil.copy(srcfile, dstpath)
        # cmd_1 = sys.executable + " " + fileBasePath + "/u2net_test.py" + " " + dstpath + " " + now_name
        # os.system(cmd_1)
        u2net_test.main(dstpath, now_name)
        pass

    # 展示图片
    def show_u2net(self):
        # 展示处理过后的图片
        u2net_path_list = []
        dstpath3 = fileBasePath + "\\test_data\\" + now_name + "_results"
        for srcfile in os.listdir(dstpath3):
            u2net_path_list.append(dstpath3 + "\\" + srcfile)
        self.modify_pic.change_path_list(u2net_path_list)

        dstpath2 = fileBasePath + "\\coordinates\\" + now_name
        if not os.path.exists(dstpath2):
            os.makedirs(dstpath2)
        cmd_2 = fileBasePath + "\\Connected_domain\\Connected_domain.exe" + " " + \
                fileBasePath + "\\test_data\\" + now_name + "_results" + " " + dstpath2
        # print(cmd_2)
        os.system(cmd_2)

    # 加密连接函数
    def encryption(self):
        # 流程 #
        global now_name
        global now_key
        # 展示原图片
        u2net_path_list = []
        dstpath = fileBasePath + "\\choosepic\\" + now_name
        # print(os.listdir(dstpath))
        for srcfile in os.listdir(dstpath):
            u2net_path_list.append(dstpath + "\\" + srcfile)
        # print(u2net_path_list)
        self.origin_pic.change_path_list(u2net_path_list)
        # 调用加密算法，展示加密图片
        encry = fileBasePath + "\\encryption\\batch_encry.exe"
        coordinates = fileBasePath + "\\coordinates\\" + now_name
        dstpath2 = fileBasePath + "\\e_results\\" + now_name
        if not os.path.exists(dstpath2):
            os.makedirs(dstpath2)
        cmd = encry + " " + dstpath + " " + coordinates + " " + now_key + " " + dstpath2
        # print(cmd)
        os.system(cmd)

        encryption_path_list = []
        for srcfile in os.listdir(dstpath2):
            encryption_path_list.append(dstpath2 + "\\" + srcfile)
        # print(encryption_path_list)
        self.modify_pic.change_path_list(encryption_path_list)
        pass

    # 解密连接函数
    def decryption(self):
        # 流程 #
        # 展示加密图片
        encryption_path_list = []
        dstpath = fileBasePath + "\\e_results\\" + now_name
        for srcfile in os.listdir(dstpath):
            encryption_path_list.append(dstpath + "\\" + srcfile)
        self.origin_pic.change_path_list(encryption_path_list)
        # 调用解密算法，展示解密后图片
        decry = fileBasePath + "\\decryption\\encry.exe"
        coordinates = fileBasePath + "\\coordinates\\" + now_name
        dstpath2 = fileBasePath + "\\d_results\\" + now_name
        if not os.path.exists(dstpath2):
            os.makedirs(dstpath2)
        print(dstpath)
        print(decry)
        print(coordinates)
        print(dstpath2)
        cmd = decry + " " + dstpath + " " + coordinates + " " + now_key + " " + dstpath2
        os.system(cmd)

        decryption_path_list = []
        for srcfile in os.listdir(dstpath2):
            decryption_path_list.append(dstpath2 + "\\" + srcfile)
        self.modify_pic.change_path_list(decryption_path_list)
        pass

    # 密匙函数，弹出新窗口，美化也在这边
    def key(self):
        global fileBasePath
        global key_list
        global now_key
        global tmp_key
        # 流程 #
        # 弹出新窗口，新窗口为垂直盒布局
        self.key_widget = QWidget()
        logo = fileBasePath + "\\logo.ico"
        icon = QIcon(logo)
        self.key_widget.setWindowTitle("密钥")
        self.key_widget.setWindowIcon(icon)
        layout = QVBoxLayout()
        self.key_widget.setLayout(layout)
        # label用于展示当前密匙信息
        self.key_show = QLabel(self.key_widget)
        self.key_show.setFixedSize(200, 70)
        now_key_file = open(now_key)
        now_key_content = now_key_file.readlines()
        now_key_string = ''
        for z in now_key_content:
            now_key_string += z
        self.key_show.setText(now_key_string)
        self.key_show.setStyleSheet(
            '''
            QLabel{
            border:1px groove gray;
            background:white;
            }
            '''
        )
        # listWidget显示密匙文件清单，按钮用于选择密匙文件、增加密匙文件
        self.key_list_widget = ListWidget(self.key_widget)
        self.key_list_widget.setFixedSize(200, 300)
        choose_button = QPushButton(self.key_widget)
        add_button = QPushButton(self.key_widget)
        choose_button.setText("Choose")
        add_button.setText("Add")
        layout_2 = QHBoxLayout()  # 按钮为水平盒布局
        layout_2.addWidget(choose_button)
        layout_2.addWidget(add_button)
        layout.addWidget(self.key_show)
        layout.addWidget(self.key_list_widget)
        layout.addLayout(layout_2)
        # 可以根据列表中已有的key选择密匙文件
        for x in key_list:
            tmp = QListWidgetItem()
            tmp.setText(x)
            # 提取密匙文件内容
            file = open(fileBasePath + "/key/" + tmp.text())
            content = file.readlines()
            tmp_string = ''
            for y in content:
                tmp_string += y
            # print(tmp_string)
            tmp.setToolTip(tmp_string)
            file.close()
            self.key_list_widget.addItem(tmp)

        # 绑定信号
        self.key_list_widget.itemClicked.connect(self.key_list_widget.clicked)
        choose_button.clicked.connect(self.choose_key)
        add_button.clicked.connect(self.add_key)
        self.key_widget.show()
        pass

    def choose_key(self):
        global fileBasePath
        global key_list
        global now_key
        global tmp_key
        now_key = tmp_key
        now_key_file = open(now_key)
        now_key_content = now_key_file.readlines()
        now_key_string = ''
        for z in now_key_content:
            now_key_string += z
        self.key_show.setText(now_key_string)
        # print(now_key)
        pass

    def add_key(self):
        global fileBasePath
        global key_list
        global now_key
        global tmp_key
        tmp_list, file_type = QFileDialog.getOpenFileNames(self)
        dstpath = fileBasePath + "\\key"
        for srcfile in tmp_list:
            if not os.path.exists(dstpath):
                os.makedirs(dstpath)
            shutil.copy(srcfile, dstpath)
        self.key_widget.close()
        # for z in key_list:
            # tmp = self.key_list_widget.findItems(z,)
            # self.key_list_widget.removeItemWidget()
        key_list.clear()
        self.key_list_widget.clear()
        for srcfile_2 in os.listdir(fileBasePath + "\\key"):
            key_list.append(srcfile_2)
        for x in key_list:
            tmp = QListWidgetItem()
            tmp.setText(x)
            file = open(fileBasePath + "\\key\\" + tmp.text())
            content = file.readlines()
            tmp_string = ''
            for y in content:
                tmp_string += y
            # print(tmp_string)
            tmp.setToolTip(tmp_string)
            file.close()
            self.key_list_widget.addItem(tmp)
        self.key_widget.close()
        self.key_widget.show()
        print(key_list)
        pass


if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = MainUi()
    # w = CandyWindow.createWindow(w, title='hh', theme='blueDeep')
    w.show()
    sys.exit(app.exec_())
