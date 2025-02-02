from PyQt5.QtCore import Qt, pyqtSignal, QThread
from PyQt5.QtGui import QPixmap, QPainter
from PyQt5.QtWidgets import QApplication, QGraphicsView, QGraphicsScene, QGraphicsPixmapItem, QVBoxLayout, QLineEdit, QPushButton, QWidget

import os
import py_common_lib

class RotatingImage(QGraphicsPixmapItem):
    def __init__(self, image_path: str):
        super().__init__()
        
        # 이미지 로드
        self.pixmap = QPixmap(image_path)
        if self.pixmap.isNull():
            print("이미지를 불러올 수 없습니다.")
            return
        
        # 이미지 크기 조정 (원하는 크기로 설정 가능)
        self.setPixmap(self.pixmap.scaled(100, 100, Qt.KeepAspectRatio, Qt.SmoothTransformation))

        # 중심점을 이미지 중앙으로 설정하여 회전 시 중심을 기준으로 회전
        self.setTransformOriginPoint(self.pixmap.width() / 2, self.pixmap.height() / 2)

    def set_angle(self, angle: float):
        self.setRotation(angle)

class ImuThread(QThread):
    update_direction_signal = pyqtSignal(float)

    def __init__(self, imu):
        super().__init__()
        self._imu = imu

    def run(self):
        self._imu.subscribe(self._imu_data_callback)

    def _imu_data_callback(self, direction: float):
        self.update_direction_signal.emit(direction)

class TriangleApp(QWidget):
    def __init__(self):
        super().__init__()

        # UI 설정
        self.layout = QVBoxLayout()
        self.input_field = QLineEdit(self)
        self.input_field.setPlaceholderText("Port")
        self.layout.addWidget(self.input_field)

        self.button = QPushButton("Connect", self)
        self.button.clicked.connect(self.connect_imu)
        self.layout.addWidget(self.button)

        # 그래픽 씬 및 뷰 설정
        self.scene = QGraphicsScene()
        self.scene.setSceneRect(-200, -200, 400, 400)

        # 사용자 이미지를 로드하여 추가
        path = os.path.dirname(os.path.abspath(__file__))
        self.image_item = RotatingImage(path + "/image/red_arrow.png")
        
        self.scene.addItem(self.image_item)
        self.image_item.setPos(-50, -50)  # 이미지 위치 중앙 정렬

        self.view = QGraphicsView(self.scene)
        self.view.setRenderHint(QPainter.Antialiasing)
        self.view.fitInView(self.scene.sceneRect(), Qt.KeepAspectRatio)
        self.layout.addWidget(self.view)

        self.setLayout(self.layout)
        self.setWindowTitle("Heading")

        self._count = 0
        self._imu_thread = None

    def connect_imu(self):
        self._imu = py_common_lib.Imu()
        if self._imu.initialize(self.input_field.text(), 38400, 1):
            self._imu_thread = ImuThread(self._imu)
            self._imu_thread.update_direction_signal.connect(self.__imu_data_callback)
            self._imu_thread.start()
        else:
            print("IMU connection failed")

    def __imu_data_callback(self, direction: float):
        print(f"Received IMU direction: {direction}")

        if self._count > 10:
            if -180 <= direction <= 180:
                self.image_item.set_angle(direction)
            self._count = 0
        else:
            self._count += 1

if __name__ == '__main__':
    app = QApplication([])
    window = TriangleApp()
    window.show()
    app.exec_()
