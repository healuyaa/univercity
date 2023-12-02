import tkinter as tk

class BallAnimation:
    def __init__(self, master):
        self.master = master
        self.master.title("Надувание и взрыв шарика")
        self.width, self.height = 800, 600
        self.canvas = tk.Canvas(self.master, width=self.width, height=self.height, bg="white")
        self.canvas.pack()

        # Параметры шарика
        self.ball_radius = 20
        self.ball_color = "red"
        self.ball_position = [self.width // 2, self.height - self.ball_radius]

        # Переменные для надувания и взрыва
        self.inflation_rate = 1.02
        self.explosion_rate = 2.0
        self.inflating = True

        # Основной цикл анимации
        self.animate()

    def animate(self):
        if self.inflating:
            self.ball_radius *= self.inflation_rate
            self.ball_radius = min(self.ball_radius, self.height // 2)
            self.canvas.delete("all")
            self.canvas.create_oval(
                self.ball_position[0] - self.ball_radius, self.ball_position[1] - self.ball_radius,
                self.ball_position[0] + self.ball_radius, self.ball_position[1] + self.ball_radius,
                fill=self.ball_color
            )
            if self.ball_radius >= self.height // 2:
                self.inflating = False
        else:
            self.ball_radius *= self.explosion_rate
            self.canvas.delete("all")
            if self.ball_radius < max(self.width, self.height):
                self.canvas.create_oval(
                    self.ball_position[0] - self.ball_radius, self.ball_position[1] - self.ball_radius,
                    self.ball_position[0] + self.ball_radius, self.ball_position[1] + self.ball_radius,
                    fill="black"
                )
            else:
                self.master.destroy()
                return

        self.master.after(30, self.animate)

if __name__ == "__main__":
    root = tk.Tk()
    app = BallAnimation(root)
    root.mainloop()
