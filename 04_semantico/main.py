import os
import sys
from PyQt6.QtWidgets import *
from PyQt6.QtGui import *
from semantico import parse, ErrorCode

class MainWindow(QMainWindow):
	def __init__(self):
		super().__init__()

		self.setWindowTitle("Analizador")
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

		self.errorsBoxSintactic = QPlainTextEdit(self.w)
		metrics = QFontMetrics(self.fontConsolas)
		self.errorsBoxSintactic.setTabStopDistance(4 * metrics.horizontalAdvance(' '))
		self.errorsBoxSintactic.setFont(self.fontConsolas)
		self.errorsBoxSintactic.setReadOnly(True)

		self.errorsBoxSemantic = QPlainTextEdit(self.w)
		metrics = QFontMetrics(self.fontConsolas)
		self.errorsBoxSemantic.setTabStopDistance(4 * metrics.horizontalAdvance(' '))
		self.errorsBoxSemantic.setFont(self.fontConsolas)
		self.errorsBoxSemantic.setReadOnly(True)

		layout.addWidget(self.codeBox)
		layout.addWidget(self.boton)
		layout.addWidget(self.tabla)
		layout.addWidget(self.errorsBoxSintactic)
		layout.addWidget(self.errorsBoxSemantic)

	def open_file(self):
		file_name, _ = QFileDialog.getOpenFileName(self, 'Abrir archivo', '', 'Archivos de código C (*.c)')
		if file_name:
			file = open(file_name, 'r', encoding='utf-8')
			self.codeBox.clear()
			self.codeBox.setPlainText(file.read())

	def highlight_line(self, line: int, color: QColor):
		cursor = QTextCursor(self.codeBox.document())
		cursor.movePosition(QTextCursor.MoveOperation.Start)
		for _ in range(line - 1):
			cursor.movePosition(QTextCursor.MoveOperation.Down)

		cursor.select(QTextCursor.SelectionType.LineUnderCursor)
		format = QTextCharFormat()
		format.setBackground(color)
		cursor.mergeCharFormat(format)

	def highlight_clear(self):
		cursor = QTextCursor(self.codeBox.document())
		format = QTextCharFormat()
		format.clearBackground()
		cursor.select(QTextCursor.SelectionType.Document)
		cursor.setCharFormat(format)

	def check(self):
		
		tokens, error_sintactic, error_semantic = parse(self.codeBox.toPlainText())

		# Limpiar la tabla
		self.tabla.clearContents()
		self.tabla.setRowCount(0)

		for i in range(len(tokens)):

			row_pos = self.tabla.rowCount()
			self.tabla.insertRow(row_pos)
			self.tabla.setItem(row_pos, 0, QTableWidgetItem(tokens[i].get_type_string()))
			self.tabla.setItem(row_pos, 1, QTableWidgetItem(tokens[i].lexema))
			self.tabla.setItem(row_pos, 2, QTableWidgetItem(str(tokens[i].token.value)))

		# errorsBoxSintactic
		self.errorsBoxSintactic.clear()
		self.highlight_clear()

		if (error_sintactic.line == 0):
			self.errorsBoxSintactic.setPlainText(error_sintactic.error_str)
		else:
			self.errorsBoxSintactic.setPlainText(error_sintactic.what())
			self.highlight_line(error_sintactic.line, QColor("red"))

		# errorsBoxSintactic
		self.errorsBoxSemantic.clear()
		self.highlight_clear()

		if (error_semantic.line == 0):
			self.errorsBoxSemantic.setPlainText(error_semantic.error_str)
		else:
			self.errorsBoxSemantic.setPlainText(error_semantic.what())
			self.highlight_line(error_semantic.line, QColor("red"))

def main():
	app = QApplication(sys.argv)
	ventana = MainWindow()
	ventana.show()
	sys.exit(app.exec())


if __name__ == '__main__':
	main()