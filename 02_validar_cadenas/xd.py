import tkinter as tk
import re

def validar_cadena():
    cadena = entrada.get()
    patron = patron_texto.get()
    
    if re.match(patron, cadena):
        resultado.config(text="Válido", fg="green")
    else:
        resultado.config(text="Inválido", fg="red")

# Crear la ventana principal
ventana = tk.Tk()
ventana.title("Validador de Cadenas")

# Crear etiqueta para ingresar la cadena
etiqueta = tk.Label(ventana, text="Ingresa la cadena:")
etiqueta.pack()

# Crear campo de entrada
entrada = tk.Entry(ventana)
entrada.pack()

# Crear etiqueta para seleccionar el tipo de cadena
etiqueta_patron = tk.Label(ventana, text="Selecciona el tipo de cadena:")
etiqueta_patron.pack()

# Opciones de tipos de cadena
opciones_patron = [
    ("Teléfono de 10 dígitos", r"^\d{10}$"),
    ("Correo electrónico", r"^\w+@[a-zA-Z_]+\.[a-zA-Z]{2,3}$"),
    ("CURP", r"^[A-Z]{4}\d{6}[HM][A-Z]{5}\w\d$"),
    ("RFC", r"^[A-Z]{4}\d{6}[A-Z\d]{3}$"),
    ("Dirección IP v4", r"^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$"),
    ("Fecha de cumpleaños DD/MM/AA", r"^(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/\d{2}$")
]

# Variable para almacenar el tipo de cadena seleccionado
patron_texto = tk.StringVar()

# Crear botones de radio para seleccionar el tipo de cadena
for opcion, patron in opciones_patron:
    boton = tk.Radiobutton(ventana, text=opcion, variable=patron_texto, value=patron)
    boton.pack()

# Botón de validación
boton_validar = tk.Button(ventana, text="Validar", command=validar_cadena)
boton_validar.pack()

# Etiqueta para mostrar el resultado de la validación
resultado = tk.Label(ventana, text="", fg="black")
resultado.pack()

ventana.mainloop()
