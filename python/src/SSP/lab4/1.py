import tkinter as tk

class TrafficLightSimulator:
    def __init__(self, master):
        self.master = master
        self.master.title("Traffic Light Simulator")

        self.current_state = 0  # 0: Red, 1: Yellow, 2: Green
        self.colors = ["red", "yellow", "green"]

        self.create_widgets()

    def create_widgets(self):
        self.canvas = tk.Canvas(self.master, width=100, height=300, bg="white")
        self.canvas.grid(row=0, column=0, rowspan=3, padx=10)

        self.red_button = tk.Button(self.master, text="Red", command=self.set_red)
        self.red_button.grid(row=0, column=1, pady=10)

        self.yellow_button = tk.Button(self.master, text="Yellow", command=self.set_yellow)
        self.yellow_button.grid(row=1, column=1, pady=10)

        self.green_button = tk.Button(self.master, text="Green", command=self.set_green)
        self.green_button.grid(row=2, column=1, pady=10)

        self.update_light()

    def set_red(self):
        self.current_state = 0
        self.update_light()

    def set_yellow(self):
        self.current_state = 1
        self.update_light()

    def set_green(self):
        self.current_state = 2
        self.update_light()

    def update_light(self):
        self.canvas.delete("light")
        radius = 20
        x = 50
        y = 50 + self.current_state * 100  # Adjust y-coordinate based on the current state
        self.canvas.create_oval(x - radius, y - radius, x + radius, y + radius, fill=self.colors[self.current_state], tags="light")

if __name__ == "__main__":
    root = tk.Tk()
    app = TrafficLightSimulator(root)
    root.mainloop()
