import sys

from PyQt5.QtWidgets import *
from PyQt5.QtCore import Qt

import py_common_lib

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setGeometry(300, 300, 1024, 768)
        
        # 메뉴 바 설정
        self.menubar = self.menuBar()
        self.menubar.setNativeMenuBar(False)

        # File 메뉴 생성
        self.log_action = QAction("Log")
        self.quit_action = QAction("Quit")
        self.quit_action.triggered.connect(self.close)

        file_menu = self.menubar.addMenu("File")
        file_menu.addAction(self.log_action)
        file_menu.addSeparator()
        file_menu.addAction(self.quit_action)

        # 메인 위젯 설정
        main_widget = QWidget()
        self.setCentralWidget(main_widget)

        # 레이아웃 설정
        layout = QHBoxLayout(main_widget)

        # 로그 위젯 생성
        self.log_widget = QTextEdit(self)
        self.log_widget.setReadOnly(True)
        self.log_widget.setMaximumWidth(self.width() // 3)  # 1/3 크기로 제한

        # 중앙 내용 영역(샘플 위젯)
        self.main_content = QLabel("Main Content Area", self)
        self.main_content.setAlignment(Qt.AlignCenter)

        # 레이아웃 구성
        layout.addWidget(self.main_content, 2)  # 중앙 콘텐츠 비중을 더 크게 설정
        layout.addWidget(self.log_widget, 1)   # 오른쪽 로그 위젯
        
        # Imu
        self._imu = py_common_lib.Imu()
        
    def connect_imu(self, port, baudrate, mode):
        if self._imu.initialize(port, baudrate, mode):
            self._imu.subscribe(self.__imu_data_callback)
        
    def __imu_data_callback(self, direction):
        print(f"IMU direction: {direction}")

app = QApplication(sys.argv)
window = MainWindow()
window.show()
window.connect_imu("COM5", 38400, 1)
app.exec_()