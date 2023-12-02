import tkinter as tk

x_position = 50

def create_and_move_shape():
    global x_position
    canvas.delete("shape")
    canvas.create_rectangle(x_position, 25, x_position + 50, 75, outline="black", fill="white", tags="shape")

def move_left():
    global x_position
    x_position -= 10
    create_and_move_shape()

def move_right():
    global x_position
    x_position += 10
    create_and_move_shape()

root = tk.Tk()

canvas = tk.Canvas(root, width=200, height=100)
canvas.pack()

left_button = tk.Button(root, text="<---", command=move_left)
right_button = tk.Button(root, text="--->", command=move_right)


left_button.pack(side=tk.LEFT)
right_button.pack(side=tk.RIGHT)

create_and_move_shape()
root.mainloop()