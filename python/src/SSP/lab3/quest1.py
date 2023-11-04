import tkinter as tk
from tkinter import messagebox

def on_select(event):
    selected_items = listbox.curselection()
    selected_values = [listbox.get(item) for item in selected_items]
    text_var.set(", ".join(selected_values))
    check_length()

def check_length():
    text = text_var.get()
    if len(text) > 100:
        messagebox.showinfo("Длинная строка", text)

root = tk.Tk()
root.title("Множественный выбор элементов")

text_var = tk.StringVar()
text_field = tk.Entry(root, width=50, textvariable=text_var)
text_field.pack()

elements = ["Элемент 1", "Элемент 2", "Элемент 3", "Элемент 4", "Элемент 5", "afiuosdhjf9a708syfa890s7fhja90sdfjhas987fas", "as9807fhas7890ghas8g7ahdf9g87ahdg987adfhga80d7fhga8079dfgh0a8d97fhga87d9hga"]
listbox = tk.Listbox(root, selectmode=tk.MULTIPLE)
for element in elements:
    listbox.insert(tk.END, element)
listbox.pack()

listbox.bind('<<ListboxSelect>>', on_select)

root.mainloop()


# Разработать приложение, обеспечивающее возможность множественного выбора элементов из списка.
# Выбранные элементы должны образовывать строку текста и помещаться в текстовое поле.
# Предусмотреть возможность вывода сообщения в диалоговое окно (Dialog) в случае, если суммарное количество символов будет превышать 100.
