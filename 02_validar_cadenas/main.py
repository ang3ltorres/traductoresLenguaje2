import re
from tkinter import *

# Función para validar y cambiar el color del label
def validar(label: Label, pattern: str, entry: str):
	# Imprime el patrón de validación (solo para depuración)
	print(pattern)

	# Intenta hacer coincidir el patrón con la entrada
	if re.match(pattern, entry):
		label.config(bg="green")  # Cambia el fondo del label a verde si es una coincidencia
	else:
		label.config(bg="red")  # Cambia el fondo del label a rojo si no es una coincidencia

def main():
	# Define patrones de validación para diferentes tipos de entrada
	patterns = [
		("Teléfono de 10 dígitos", r"^\d{10}$"),
		("Correo electrónico", r"^\w+@[a-zA-Z_]+\.[a-zA-Z]{2,3}$"),
		("CURP", r"^[A-Z]{4}\d{6}[HM][A-Z]{5}\w\d$"),
		("RFC", r"^[A-Z]{4}\d{6}[A-Z\d]{3}$"),
		("Dirección IP v4", r"^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$"),
		("Fecha de cumpleaños DD/MM/AA", r"^(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/\d{2}$")
	]

	# Crear la ventana principal
	root = Tk()
	root.title("Validar cadenas")
	root.resizable(False, False)
	
	frame = Frame(root)
	frame.pack()

	component = []
	for i in range(6):
		# Crear un Label con el nombre del patrón
		label = Label(frame, text=patterns[i][0])
		
		# Crear un campo de entrada
		entry = Entry(frame)
		
		# Crear un botón "Validar" que llama a la función validar con parámetros específicos
		button = Button(frame, text="Validar", command=lambda l=label, p=patterns[i][1], e=entry: validar(l, p, e.get()))

		component.append((label, entry, button))

	for i in range(6):
		# Colocar los elementos en la cuadrícula
		component[i][0].grid(row=i, column=0, padx=5, pady=5)
		component[i][1].grid(row=i, column=1, padx=5, pady=5)
		component[i][2].grid(row=i, column=2, padx=5, pady=5)

	root.mainloop()

if __name__ == "__main__":
	main()
