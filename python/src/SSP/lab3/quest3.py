import tkinter as tk
from tkinter import messagebox

def add_name():
    full_name = entry.get()
    parts = full_name.split()
    if len(parts) == 3:
        last_name, first_name, middle_name = parts
        last_name_list.insert(tk.END, last_name)
        first_name_list.insert(tk.END, first_name)
        middle_name_list.insert(tk.END, middle_name)
    else:
        messagebox.showerror("Ошибка", "Введите Фамилию Имя Отчество в правильном формате!")

def sort_names():
    last_names = list(last_name_list.get(0, tk.END))
    last_names.sort()
    last_name_list.delete(0, tk.END)
    for last_name in last_names:
        last_name_list.insert(tk.END, last_name)
    update_sorted_list()

def copy_selected():
    selected_last_names = list(last_name_list.curselection())
    for index in selected_last_names:
        last_name = last_name_list.get(index)
        sorted_last_names_list.insert(tk.END, last_name)

def update_sorted_list():
    sorted_last_names_list.delete(0, tk.END)
    last_names = list(last_name_list.get(0, tk.END))
    sorted_last_names_list.insert(tk.END, *sorted(last_names))

root = tk.Tk()
root.title("Управление списками")

frame = tk.Frame(root)
frame.pack(pady=10)

last_name_label = tk.Label(root, text="Фамилия")
last_name_label.pack(side=tk.LEFT)

first_name_label = tk.Label(root, text="Имя")
first_name_label.pack(side=tk.LEFT)

middle_name_label = tk.Label(root, text="Отчество")
middle_name_label.pack(side=tk.LEFT)

entry = tk.Entry(frame, width=80)
entry.pack(side=tk.LEFT)

add_button = tk.Button(frame, text="Добавить", command=add_name)
add_button.pack(side=tk.LEFT)

last_name_list = tk.Listbox(root, selectmode=tk.MULTIPLE)
last_name_list.pack(side=tk.LEFT)

first_name_list = tk.Listbox(root)
first_name_list.pack(side=tk.LEFT)

middle_name_list = tk.Listbox(root)
middle_name_list.pack(side=tk.LEFT)

sorted_last_names_list = tk.Listbox(root)
sorted_last_names_list.pack()

copy_button = tk.Button(root, text="Скопировать", command=copy_selected)
copy_button.pack(side=tk.LEFT)

sort_button = tk.Button(root, text="Сортировать", command=sort_names)
sort_button.pack(side=tk.LEFT)

root.mainloop()


# Разработать приложение управления тремя списками («Фамилия», «Имя», «Отчество») и строкой редактирования (TextField).
# Для отображения строки редактирования вызывается диалоговое окно (Dialog).
# В строку редактирования вводится информация в формате «Фамилия Имя Отчество».
# По завершении ввода диалоговое окно закрывается, фамилия должна появиться в списке «Фамилия», имя - в списке «Имя», отчество - в списке «Отчество».
# Предусмотреть возможность множественного выбора фамилий и записи их в отсортированном порядке в четвертый список