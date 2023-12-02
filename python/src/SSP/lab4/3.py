import tkinter as tk
from tkinter import Canvas, Button

class ShapeChangerApp:
    def __init__(self, master):
        self.master = master
        self.master.title("Смена формы фигуры")

        self.canvas = Canvas(master, width=200, height=200)
        self.canvas.pack()

        self.draw_rectangle()

        self.button_square = Button(master, text="Квадрат", command=self.draw_rectangle)
        self.button_circle = Button(master, text="Круг", command=self.draw_circle)
        self.button_triangle = Button(master, text="Треугольник", command=self.draw_triangle)

        self.button_square.pack(side=tk.LEFT)
        self.button_circle.pack(side=tk.LEFT)
        self.button_triangle.pack(side=tk.LEFT)

    def draw_rectangle(self):
        self.canvas.delete("all")
        self.canvas.create_rectangle(50, 50, 150, 150, fill="blue")

    def draw_circle(self):
        self.canvas.delete("all")
        self.canvas.create_oval(50, 50, 150, 150, fill="green")

    def draw_triangle(self):
        self.canvas.delete("all")
        self.canvas.create_polygon(50, 150, 100, 50, 150, 150, fill="red")

if __name__ == "__main__":
    root = tk.Tk()
    app = ShapeChangerApp(root)
    root.mainloop()
