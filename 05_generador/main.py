from generador import compile_code

import os
import sys
from PyQt6.QtWidgets import *
from PyQt6.QtGui import *

class MainWindow(QMainWindow):
	def __init__(self):
		super().__init__()

		self.setWindowTitle("Analizador lexico")
		self.setGeometry(0, 0, 800, 600)

		self.initUI()

	def initUI(self):
		self.w = QWidget(self)
		self.setCentralWidget(self.w)

		# Menu file
		open_action_file = QAction('Abrir archivo de código', self)
		open_action_file.triggered.connect(self.open_file)

		file_menu = self.menuBar().addMenu('Archivo')
		file_menu.addAction(open_action_file)
		########

		layout = QVBoxLayout(self.w)

		self.fontConsolas = QFont()
		self.fontConsolas.setFamily("Consolas")
		self.fontConsolas.setPointSize(11)
		self.fontConsolas.setFixedPitch(True);
		metrics = QFontMetrics(self.fontConsolas)

		####
		self.codeBox = QPlainTextEdit(self.w)
		self.codeBox.setTabStopDistance(4 * metrics.horizontalAdvance(' '))
		self.codeBox.setFont(self.fontConsolas)

		self.boton = QPushButton("Compilar", self.w)
		self.boton.clicked.connect(self.compile_code)

		self.codeBoxRes = QPlainTextEdit(self.w)
		self.codeBoxRes.setTabStopDistance(4 * metrics.horizontalAdvance(' '))
		self.codeBoxRes.setFont(self.fontConsolas)
		self.codeBoxRes.setReadOnly(True)

		layout.addWidget(self.codeBox)
		layout.addWidget(self.boton)
		layout.addWidget(self.codeBoxRes)

	def compile_code(self):
		temp_file = open('TEMP', 'w')
		temp_file.write(self.codeBox.toPlainText())
		temp_file.close()

		compile_code('TEMP')

		xd = open('salida.txt', 'r')

		self.codeBoxRes.clear()
		self.codeBoxRes.setPlainText(xd.read())

	def open_file(self):
		file_name, _ = QFileDialog.getOpenFileName(self, 'Abrir archivo')
		if file_name:
			file = open(file_name, 'r', encoding='utf-8')
			self.codeBox.clear()
			self.codeBox.setPlainText(file.read())

def main():
	app = QApplication(sys.argv)
	ventana = MainWindow()
	ventana.show()
	sys.exit(app.exec())


if __name__ == '__main__':
	main()