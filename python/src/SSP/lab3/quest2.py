import tkinter as tk

def select_odd_rows():
    odd_indices = [i for i in range(len(listbox.get(0, tk.END))) if i % 2 != 0]
    listbox.selection_clear(0, tk.END)
    for index in odd_indices:
        listbox.selection_set(index)

def select_even_rows():
    even_indices = [i for i in range(len(listbox.get(0, tk.END))) if i % 2 == 0]
    listbox.selection_clear(0, tk.END)
    for index in even_indices:
        listbox.selection_set(index)

def move_selected():
    selected_indices = listbox.curselection()
    items_to_move = [listbox.get(index) for index in selected_indices]
    
    for item in items_to_move:
        listbox2.insert(tk.END, item)
    
    for index in reversed(selected_indices):
        listbox.delete(index)


def clear_list2():
    listbox2.delete(0, tk.END)

def update_list():
    item = entry.get()
    if item:
        listbox.insert(tk.END, item)
    entry.delete(0, tk.END)

app = tk.Tk()
app.title("Управление списком")

frame = tk.Frame(app)
frame.pack(pady=10)

listbox = tk.Listbox(frame, selectmode=tk.MULTIPLE)
listbox.pack(side=tk.LEFT)

listbox2 = tk.Listbox(frame)
listbox2.pack(side=tk.RIGHT)

select_odd_button = tk.Checkbutton(app, text="Выбрать четные строки", command=select_odd_rows)
select_odd_button.pack()

select_even_button = tk.Checkbutton(app, text="Выбрать нечетные строки", command=select_even_rows)
select_even_button.pack()

entry = tk.Entry(app)
entry.pack()

add_button = tk.Button(app, text="Добавить в список", command=update_list)
add_button.pack()

move_button = tk.Button(app, text="Переместить во второй список", command=move_selected)
move_button.pack()

clear_button = tk.Button(app, text="Очистить второй список", command=clear_list2)
clear_button.pack()

app.mainloop()

# Разработать приложение управления списком.
# Вывести два флажка (Checkbox).
# При первом включенном флажке осуществляется выбор всех нечетных строк и их удаление,
# при втором включенном флажке осуществляется выбор всех четных строк и перенос их во второй список.
# Предусмотреть обновление элементов списка и очистку второго списка
