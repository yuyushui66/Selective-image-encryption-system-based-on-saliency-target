import sys
from PyQt5.QtWidgets import (QWidget, QLabel, QVBoxLayout, QHBoxLayout, QApplication, QPushButton,
                             QFileDialog)
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import (Qt, QSize)
import qtawesome

class Picture(QWidget):
    # 储存图片路径
    path_list = []
    # 创建图片迭代器，用于展示图片
    current_pic = 0

    # 批量展示图片
    def __init__(self, parent=None, p_width=1000, p_height=400):
        super().__init__()
        # 设置总体样式
        self.setFixedSize(p_width, p_height)
        # 初始化展示标签列表
        self.label = []
        for i in range(2):
            new_label = QLabel(self)
            new_label.setFixedSize(400, 300)
            new_label.setStyleSheet("border:0px;border-radius:10px;")
            new_label.setScaledContents(True)
            self.label.append(new_label)
        # 初始化按钮,两个按钮垂直布局
        btn_layout = QVBoxLayout()
        self.up_button = QPushButton(qtawesome.icon('fa.angle-double-up', color='#2c3a45'), "U", self)
        self.up_button.setStyleSheet(
            '''
           
            QPushButton{
            border:1px groove gray;
            border-radius:10px;
            font-size:20px;
            text-align:centre;
            background:Qcolor(240,240,240);
            font-family:'宋体';
             }
            QPushButton:hover{background:grey;}
            ''')
        self.down_button = QPushButton(qtawesome.icon('fa.angle-double-down', color='#2c3a45'), "D", self)
        self.down_button.setStyleSheet(
            '''
           
            QPushButton{
            border:1px groove gray;
            border-radius:10px;
            font-size:20px;
            text-align:centre;
            background:Qcolor(240,240,240);
            font-family:'宋体';
             }
            QPushButton:hover{background:grey;}
            ''')
        self.up_button.setFixedSize(40, 30)
        self.down_button.setFixedSize(40, 30)
        btn_layout.addWidget(self.up_button)
        btn_layout.addWidget(self.down_button)
        # 总体水平布局
        main_layout = QHBoxLayout()
        self.setLayout(main_layout)
        for x in self.label:
            main_layout.addWidget(x)
        main_layout.addLayout(btn_layout)
        # 信号连接
        self.up_button.clicked.connect(self.up)
        self.down_button.clicked.connect(self.down)
        # 测试
        # test_btn = QPushButton(self)
        # test_btn.clicked.connect(self.change_path_list)
        # 在此美化所有控件
        # 控件清单：一个总的窗口控件,两个label控件,两个按钮控件

    # 修改图片路径
    def change_path_list(self, tmp_list=None):
        if tmp_list:
            self.path_list = tmp_list
        else:
            self.path_list, file_type = QFileDialog.getOpenFileNames(self)
        # print(self.path_list)
        # self.path_list = new_path
        self.current_pic = 0
        self.show_image()
        pass

    # 展示图片
    def show_image(self):
        tmp_list = []
        if self.current_pic < len(self.path_list):
            tmp_list.append(self.path_list[self.current_pic])
        else:
            tmp_list.append(0)
        if self.current_pic + 1 < len(self.path_list):
            tmp_list.append(self.path_list[self.current_pic + 1])
        else:
            tmp_list.append(0)
        # 选择图片
        for i in range(2):
            if tmp_list[i] == 0:
                self.label[i].setText(" ")
            else:
                photo = QPixmap(tmp_list[i])
                width = photo.width()
                height = photo.height()
                tmp_image = photo.toImage()
                size = QSize(width, height)
                photo.convertFromImage(tmp_image.scaled(size, Qt.IgnoreAspectRatio))
                self.label[i].setPixmap(photo)
        pass

    # 向上翻页
    def up(self):
        if self.current_pic == 0:
            self.current_pic = self.current_pic
        else:
            self.current_pic = self.current_pic - 2
        self.show_image()
        pass

    # 向下翻页
    def down(self):
        if self.current_pic == len(self.path_list) - 2 or self.current_pic == len(self.path_list) - 1:
            self.current_pic = self.current_pic
        else:
            self.current_pic = self.current_pic + 2
        self.show_image()
        pass


if __name__ == '__main__':
    app = QApplication(sys.argv)
    pic = Picture()
    pic.show()
    sys.exit(app.exec_())

