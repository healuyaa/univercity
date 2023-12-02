import tkinter as tk

def toggle_led():
    global is_led_on
    is_led_on = not is_led_on
    if is_led_on:
        canvas.itemconfig(led, fill="green")
    else:
        canvas.itemconfig(led, fill="gray")

root = tk.Tk()

canvas = tk.Canvas(root, width=100, height=100)
canvas.pack()

led = canvas.create_oval(25, 25, 75, 75, fill="gray")

toggle_button = tk.Button(root, text="Включить/Выключить", command=toggle_led)
toggle_button.pack()

is_led_on = False
root.mainloop()