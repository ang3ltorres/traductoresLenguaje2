import re
from tkinter import *

def saludar(target_label):
	target_label.config(text="Hola mundo!")

def main():
	root = Tk()
	root.title("Validar cadenas")
	root.resizable(False, False)
	root.geometry("400x300")

	frame1 = Frame(root, highlightthickness=8, highlightbackground="black", relief="solid", width=200, height=1000)
	frame1.pack(side="top")

	label = Label(frame1, text="...", font=("Helvetica", 18))
	label.pack(pady=20)

	boton = Button(frame1, text="Saludar", command=lambda: saludar(label))
	boton.pack(pady=20)

	frame2 = Frame(root)
	frame2.pack()
	for i in range(4):
		lab = Label(frame2, text="LABEL")
		input_entry = Entry(frame2)
		checkbox = Checkbutton(frame2, text="Checkbox")

		lab.grid(row=i, column=0, padx=5, pady=5, sticky="e")
		input_entry.grid(row=i, column=1, padx=5, pady=5)
		checkbox.grid(row=i, column=2, padx=5, pady=5, sticky="w")

	root.mainloop()

if __name__ == "__main__":
	main()
