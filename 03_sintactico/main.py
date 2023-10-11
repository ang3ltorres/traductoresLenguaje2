import os
import sys
from PyQt6.QtWidgets import *
from PyQt6.QtGui import *
from sintactico import get_tokens, parse_tokens

class MainWindow(QMainWindow):
	def __init__(self):
		super().__init__()

		self.setWindowTitle("Analizador")
		self.setGeometry(0, 0, 800, 600)

		self.initUI()

	def initUI(self):
		self.w = QWidget(self)
		self.setCentralWidget(self.w)

		layout = QVBoxLayout(self.w)

		self.fontConsolas = QFont()
		self.fontConsolas.setFamily("Consolas")
		self.fontConsolas.setPointSize(11)
		self.fontConsolas.setFixedPitch(True)

		self.codeBox = QPlainTextEdit(self.w)
		metrics = QFontMetrics(self.fontConsolas)
		self.codeBox.setTabStopDistance(4 * metrics.horizontalAdvance(' '))
		self.codeBox.setFont(self.fontConsolas)

		self.boton = QPushButton("Analizar", self.w)
		self.boton.clicked.connect(self.check)

		self.tabla = QTableWidget(self.w)
		self.tabla.setColumnCount(3)
		self.tabla.setHorizontalHeaderLabels(["Lexema", "Token", "#"])
		self.tabla.verticalHeader().setVisible(False)
		for column in range(3):
			self.tabla.horizontalHeader().setSectionResizeMode(column, QHeaderView.ResizeMode.Stretch)

		self.errorsBox = QPlainTextEdit(self.w)
		metrics = QFontMetrics(self.fontConsolas)
		self.errorsBox.setTabStopDistance(4 * metrics.horizontalAdvance(' '))
		self.errorsBox.setFont(self.fontConsolas)
		self.errorsBox.setReadOnly(True)

		layout.addWidget(self.codeBox)
		layout.addWidget(self.boton)
		layout.addWidget(self.tabla)
		layout.addWidget(self.errorsBox)


	def check(self):
		
		tokens = get_tokens(self.codeBox.toPlainText())

		# Limpiar la tabla
		self.tabla.clearContents()
		self.tabla.setRowCount(0)

		for i in range(len(tokens)):

			row_pos = self.tabla.rowCount()
			self.tabla.insertRow(row_pos)
			self.tabla.setItem(row_pos, 0, QTableWidgetItem(tokens[i].get_type_string()))
			self.tabla.setItem(row_pos, 1, QTableWidgetItem(tokens[i].lexema))
			self.tabla.setItem(row_pos, 2, QTableWidgetItem(str(tokens[i].token.value)))

		# errorBox
		self.errorsBox.clear()
		self.errorsBox.setPlainText(parse_tokens(tokens))

def main():
	app = QApplication(sys.argv)
	ventana = MainWindow()
	ventana.show()
	sys.exit(app.exec())


if __name__ == '__main__':
	main()