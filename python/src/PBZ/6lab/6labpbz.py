import tkinter as tk
from tkinter import ttk, simpledialog, messagebox
import mysql.connector
from datetime import datetime

class DatabaseApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Database App")

        # Подключение к базе данных MySQL
        self.connection_params = {
            "host": "localhost",
            "user": "root",
            "password": "1234",
            "database": "lab567"
        }
        self.connection = mysql.connector.connect(**self.connection_params)
        self.cursor = self.connection.cursor()

        # Создание GUI
        self.create_widgets()
        
        # Фрейм для поиска и сортировки
        search_sort_frame = tk.Frame(self.root)
        search_sort_frame.pack(side=tk.BOTTOM)

        # Поле для ввода текста поиска
        self.search_entry = tk.Entry(search_sort_frame)
        self.search_entry.pack(side=tk.LEFT, padx=5)

        # Кнопка для выполнения поиска
        search_button = tk.Button(search_sort_frame, text="Поиск", command=self.search_data)
        search_button.pack(side=tk.LEFT, padx=5)

        # Кнопка для выбора сортировки ASC
        asc_button = tk.Button(search_sort_frame, text="Сортировать ASC", command=lambda: self.sort_data("ASC"))
        asc_button.pack(side=tk.LEFT, padx=5)

        # Кнопка для выбора сортировки DESC
        desc_button = tk.Button(search_sort_frame, text="Сортировать DESC", command=lambda: self.sort_data("DESC"))
        desc_button.pack(side=tk.LEFT, padx=5)
        
        self.current_time = None
        self.last_search = None

    def create_widgets(self):
        # Создание вкладок
        self.tab_control = ttk.Notebook(self.root)
        tab1 = ttk.Frame(self.tab_control)
        tab2 = ttk.Frame(self.tab_control)
        tab3 = ttk.Frame(self.tab_control)
        self.tab_control.add(tab1, text='Таблица 1')
        self.tab_control.add(tab2, text='Таблица 2')
        self.tab_control.add(tab3, text='Таблица 3')
        self.tab_control.pack(expand=1, fill="both")

        # Таблица 1
        self.create_table(tab1, "table1", ["code_model", "model", "car_model", "car_manufacturer", "id"])

        # Таблица 2
        self.create_table(tab2, "table2", ["code_model", "number_model", "color", "mileage", "price", "id"])

        # Таблица 3
        self.create_table(tab3, "table3", ["number_model", "year_of_issue", "engine_capacity", "id"])

        # Фрейм с кнопками
        buttons_frame = tk.Frame(self.root)
        buttons_frame.pack(side=tk.BOTTOM)

        # Кнопки для каждой таблицы
        update_button = tk.Button(buttons_frame, text="Обновить", command=self.update_table)
        update_button.pack(side=tk.LEFT, padx=5)
        add_button = tk.Button(buttons_frame, text="Добавить запись", command=self.add_record)
        add_button.pack(side=tk.LEFT, padx=5)
        edit_button = tk.Button(buttons_frame, text="Редактировать запись", command=self.edit_record)
        edit_button.pack(side=tk.LEFT, padx=5)
        delete_button = tk.Button(buttons_frame, text="Удалить запись", command=self.delete_record)
        delete_button.pack(side=tk.LEFT, padx=5)
        report_button = tk.Button(buttons_frame, text="Сгенерировать отчет", command=self.generate_report)
        report_button.pack(side=tk.LEFT, padx=5)

    def create_table(self, tab, table_name, columns):
        tree = ttk.Treeview(tab, columns=columns, show="headings", selectmode="browse")
        for col in columns:
            tree.heading(col, text=col)
            tree.column(col, width=100)

        # Получение данных из базы данных
        query = f"SELECT {', '.join(columns)} FROM {table_name}"
        self.cursor.execute(query)
        rows = self.cursor.fetchall()

        # Вставка данных в таблицу
        for row in rows:
            tree.insert("", "end", values=row)

        # Добавление treeview в окно
        tree.pack(fill="both", expand=True)

    def update_table(self):
        # Получение текущей вкладки
        current_tab = self.tab_control.select()
        current_tab_index = self.tab_control.index(current_tab)

        # Получение названия таблицы для текущей вкладки
        table_names = ["table1", "table2", "table3"]
        table_name = table_names[current_tab_index]

        # Очистка существующих данных в Treeview
        tree = self.tab_control.winfo_children()[current_tab_index].winfo_children()[0]
        tree.delete(*tree.get_children())

        # Получение данных из базы данных
        query = f"SELECT * FROM {table_name}"
        self.cursor.execute(query)
        rows = self.cursor.fetchall()

        # Вставка данных в таблицу
        for row in rows:
            tree.insert("", "end", values=row)

    def add_record(self):
        # Диалог для выбора таблицы
        table_name = simpledialog.askstring("Выбор таблицы", "Введите название таблицы (table1, table2, table3):")
        if table_name not in ["table1", "table2", "table3"]:
            messagebox.showerror("Ошибка", "Неверное название таблицы")
            return

        # Диалог для ввода данных
        input_data = simpledialog.askstring("Ввод записи", "Введите значения записи через запятую:")
        if not input_data:
            return

        # Разделение введенных данных на список значений
        values = input_data.split(",")

        # Вставка данных в выбранную таблицу
        columns = ["code_model", "model", "car_model", "car_manufacturer", "id"]
        if table_name == "table2":
            columns = ["code_model", "number_model", "color", "mileage", "price", "id"]
        elif table_name == "table3":
            columns = ["number_model", "year_of_issue", "engine_capacity", "id"]

        # Подготовка SQL-запроса
        query = f"INSERT INTO {table_name} ({', '.join(columns)}) VALUES ({', '.join(['%s']*len(columns))})"
        try:
            self.cursor.execute(query, values)
            self.connection.commit()

            # Обновление представления таблицы
            self.update_table_view(table_name)
            messagebox.showinfo("Успех", "Запись успешно добавлена")
        except mysql.connector.Error as e:
            messagebox.showerror("Ошибка", f"Не удалось добавить запись. Ошибка: {e}")

    def update_table_view(self, table_name):
        # Обновление представления таблицы после добавления, редактирования или удаления записей
        # Очистка существующих данных в Treeview
        for child in self.tab_control.winfo_children():
            if child.winfo_children():
                child.winfo_children()[0].delete(*child.winfo_children()[0].get_children())

        # Получение данных из базы данных и обновление Treeview
        query = f"SELECT * FROM {table_name}"
        self.cursor.execute(query)
        rows = self.cursor.fetchall()
        tree = self.tab_control.winfo_children()[self.tab_control.index("current")].winfo_children()[0]
        for row in rows:
            tree.insert("", "end", values=row)

    def edit_record(self):
        # Диалог для выбора таблицы
        table_name = simpledialog.askstring("Выбор таблицы", "Введите название таблицы (table1, table2, table3):")
        if table_name not in ["table1", "table2", "table3"]:
            messagebox.showerror("Ошибка", "Неверное название таблицы")
            return
        
        # Получение данных из выбранной таблицы для отображения
        query = f"SELECT * FROM {table_name}"
        self.cursor.execute(query)
        rows = self.cursor.fetchall()

        # Создание диалогового окна для выбора записи
        record_selection_dialog = simpledialog.askstring("Выбор записи", "Введите ID записи, которую вы хотите редактировать:")
        if not record_selection_dialog:
            return

        try:
            record_id = int(record_selection_dialog)
        except ValueError:
            messagebox.showerror("Ошибка", "Неверный формат ID записи")
            return

        # Проверка наличия записи с указанным ID
        if any(record_id in row for row in rows):
            # Определение имен столбцов для каждой таблицы
            if table_name == "table1":
                columns = ["code_model", "model", "car_model", "car_manufacturer", "id"]
            elif table_name == "table2":
                columns = ["code_model", "number_model", "color", "mileage", "price", "id"]
            elif table_name == "table3":
                columns = ["number_model", "year_of_issue", "engine_capacity", "id"]

            # Диалог для ввода новых данных
            new_data = simpledialog.askstring("Редактирование записи", "Введите новые значения записи через запятую:")
            if not new_data:
                return

            # Разделение введенных данных на список значений
            new_values = new_data.split(",")

            # Подготовка SQL-запроса для обновления записи
            update_query = f"UPDATE {table_name} SET "
            update_query += ", ".join([f"{col} = %s" for col in columns])
            update_query += f" WHERE id = {record_id}"

            try:
                self.cursor.execute(update_query, new_values)
                self.connection.commit()

                # Обновление представления таблицы
                self.update_table_view(table_name)
                messagebox.showinfo("Успех", "Запись успешно отредактирована")
            except mysql.connector.Error as e:
                messagebox.showerror("Ошибка", f"Не удалось отредактировать запись. Ошибка: {e}")
        else:
            messagebox.showerror("Ошибка", f"Запись с ID {record_id} не найдена в таблице {table_name}")
            
    def delete_record(self):
        # Диалог для выбора таблицы
        table_name = simpledialog.askstring("Выбор таблицы", "Введите название таблицы (table1, table2, table3):")
        if table_name not in ["table1", "table2", "table3"]:
            messagebox.showerror("Ошибка", "Неверное название таблицы")
            return

        # Получение данных из выбранной таблицы для отображения
        query = f"SELECT * FROM {table_name}"
        self.cursor.execute(query)
        rows = self.cursor.fetchall()

        # Создание диалогового окна для выбора записи
        record_selection_dialog = simpledialog.askstring("Выбор записи", "Введите ID записи, которую вы хотите удалить:")
        if not record_selection_dialog:
            return

        try:
            record_id = int(record_selection_dialog)
        except ValueError:
            messagebox.showerror("Ошибка", "Неверный формат ID записи")
            return

        # Проверка наличия записи с указанным ID
        if any(record_id in row for row in rows):
            # Подготовка SQL-запроса для удаления записи
            delete_query = f"DELETE FROM {table_name} WHERE id = %s"

            try:
                self.cursor.execute(delete_query, (record_id,))
                self.connection.commit()

                # Обновление представления таблицы
                self.update_table_view(table_name)
                messagebox.showinfo("Успех", "Запись успешно удалена")
            except mysql.connector.Error as e:
                messagebox.showerror("Ошибка", f"Не удалось удалить запись. Ошибка: {e}")
        else:
            messagebox.showerror("Ошибка", f"Запись с ID {record_id} не найдена в таблице {table_name}")

    def search_data(self):
        # Получение текущей вкладки и названия таблицы
        current_tab = self.tab_control.select()
        current_tab_index = self.tab_control.index(current_tab)
        table_names = ["table1", "table2", "table3"]
        table_name = table_names[current_tab_index]

        # Получение текста поиска
        search_text = self.search_entry.get()

        # Подготовка SQL-запроса с использованием LIKE для поиска
        search_query = f"SELECT * FROM {table_name} WHERE CONCAT({', '.join(self.get_columns(table_name))}) LIKE %s"
        self.cursor.execute(search_query, ('%' + search_text + '%',))
        rows = self.cursor.fetchall()
        
        self.last_search = search_text
        # Обновление представления таблицы после выполнения поиска
        self.update_table_view(table_name, rows)

    def sort_data(self, order):
        # Получение текущей вкладки и названия таблицы
        current_tab = self.tab_control.select()
        current_tab_index = self.tab_control.index(current_tab)
        table_names = ["table1", "table2", "table3"]
        table_name = table_names[current_tab_index]

        # Получение данных из базы данных и сортировка
        query = f"SELECT * FROM {table_name} ORDER BY {table_name}.id {order}"
        self.cursor.execute(query)
        rows = self.cursor.fetchall()

        # Обновление представления таблицы после выполнения сортировки
        self.update_table_view(table_name, rows)

    def update_table_view(self, table_name, rows):
        # Очистка существующих данных в Treeview
        tree = self.tab_control.winfo_children()[self.tab_control.index("current")].winfo_children()[0]
        tree.delete(*tree.get_children())

        # Вставка данных в таблицу
        for row in rows:
            tree.insert("", "end", values=row)

    def get_columns(self, table_name):
        # Возвращает список столбцов для указанной таблицы
        if table_name == "table1":
            return ["code_model", "model", "car_model", "car_manufacturer", "id"]
        elif table_name == "table2":
            return ["code_model", "number_model", "color", "mileage", "price", "id"]
        elif table_name == "table3":
            return ["number_model", "year_of_issue", "engine_capacity", "id"]

    def generate_report(self):
        # Получаем текущее время
        self.current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        # Формируем отчет
        report_text = f"Отчет:\nВремя: {self.current_time}\n"

        # Получаем информацию о структуре всех таблиц
        table_info = self.get_table_info()

        # Вставляем информацию о структуре и содержании каждой таблицы в отчет
        for table_name, data in table_info.items():
            report_text += f"\nТаблица: {table_name}\n"
            columns = data['columns']
            rows = data['rows']

            # Вставляем заголовки столбцов
            for col in columns:
                report_text += f"{col:<20}"

            report_text += "\n"

            # Вставляем содержимое таблицы
            for row in rows:
                for cell in row:
                    report_text += f"{str(cell):<20}"
                report_text += "\n"

        # Если есть результат последнего поиска, добавляем его в отчет
        if self.last_search:
            report_text += f"\nПоследний поиск: {self.last_search}\n"

        # Создаем новое диалоговое окно для отчета
        report_dialog = tk.Toplevel(self.root)
        report_dialog.title("Отчет")

        # Создаем Text для отображения отчета
        report_textbox = tk.Text(report_dialog, wrap="none", width=80, height=20)
        report_textbox.insert("1.0", report_text)

        # Размещаем Text в окне
        report_textbox.pack(fill="both", expand=True)

        # Добавляем кнопку для закрытия окна отчета
        close_button = tk.Button(report_dialog, text="Закрыть", command=report_dialog.destroy)
        close_button.pack()

    def get_table_info(self):
        # Получаем информацию о структуре и содержании всех таблиц
        table_info = {}
        query = "SHOW TABLES"
        self.cursor.execute(query)
        table_names = [row[0] for row in self.cursor.fetchall()]

        for table_name in table_names:
            # Получаем структуру таблицы
            columns_query = f"SHOW COLUMNS FROM {table_name}"
            self.cursor.execute(columns_query)
            columns = [row[0] for row in self.cursor.fetchall()]

            # Получаем содержимое таблицы
            data_query = f"SELECT * FROM {table_name}"
            self.cursor.execute(data_query)
            rows = self.cursor.fetchall()

            table_info[table_name] = {'columns': columns, 'rows': rows}

        return table_info

if __name__ == "__main__":
    root = tk.Tk()
    app = DatabaseApp(root)
    root.mainloop()
