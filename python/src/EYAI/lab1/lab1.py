import tkinter as tk
from tkinter import ttk
import pymorphy2

class WordViewerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Word Viewer")

        self.label = ttk.Label(root, text="Выберите слово:")
        self.label.grid(row=0, column=0, padx=10, pady=10)

        self.word_combobox = ttk.Combobox(root, state="readonly", width=30)
        self.word_combobox.grid(row=0, column=1, padx=10, pady=10)

        self.word_combobox.bind("<<ComboboxSelected>>", self.display_word_info)

        self.text_info = tk.Text(root, height=10, width=50)
        self.text_info.grid(row=1, column=0, columnspan=2, padx=10, pady=10)

        self.search_entry = tk.Entry(root, width=30)
        self.search_entry.grid(row=2, column=0, padx=10, pady=10)

        self.search_button = tk.Button(root, text="Найти", command=self.search_word_info)
        self.search_button.grid(row=2, column=1, padx=10, pady=10)

        self.search_result_text = tk.Text(root, height=10, width=50)
        self.search_result_text.grid(row=3, column=0, columnspan=2, padx=10, pady=10)

        self.words = self.load_data_from_file("lab1/test.txt")
        self.word_combobox["values"] = self.words

        self.morph_analyzer = pymorphy2.MorphAnalyzer()

    def load_data_from_file(self, filename):
        with open(filename, 'r', encoding='utf-8') as file:
            words = file.read().split()
        return words

    def display_word_info(self, event=None):
        selected_word = self.word_combobox.get()
        forms = self.get_word_forms(selected_word)
        self.text_info.delete(1.0, tk.END)
        self.text_info.insert(tk.END, f"Слово: {selected_word}\n")
        self.text_info.insert(tk.END, f"Формы: {', '.join(forms)}\n")

    def search_word_info(self):
        query = self.search_entry.get()
        selected_word = self.word_combobox.get()

        query_parts = query.split(maxsplit=1)            

        criteria = None
        query_value = None
        
        if len(query_parts) >= 2:
            
            if query_parts[0] in ["род", "число", "падеж", "часть"]:
                criteria = query_parts[0]
                query_value = query_parts[1]
            elif query_parts[1] in ["род", "число", "падеж", "часть"]:
                criteria = query_parts[1]
                query_value = query_parts[0]
            else:
                pass
            
            if criteria in ["род", "число", "падеж", "часть"]:
                if query_value == "женский":
                    query_value = "femn"
                elif query_value == "мужской":
                    query_value = "masc"
                elif query_value == "средний":
                    query_value = "neut"
                elif query_value == "множественное":
                    query_value = "plur"
                elif query_value == "единственное":
                    query_value = "sing"
                elif query_value == "именительный":
                    query_value = "nomn"
                elif query_value == "родительный":
                    query_value = "gent"
                elif query_value == "дательный":
                    query_value = "datv"
                elif query_value == "винительный":
                    query_value = "accs"
                elif query_value == "творительный":
                    query_value = "ablt"
                elif query_value == "предложный":
                    query_value = "loct"
                elif query_value == "звательный":
                    query_value = "voct"
                elif query_value == "деепричастие":
                    query_value = "GRND"
                elif query_value == "причастие":
                    query_value = "PRTS"
                elif query_value == "краткое":
                    query_value = "ADJS"
                elif query_value == "компаратив":
                    query_value = "COMP"
                elif query_value == "существительное":
                    query_value = "NOUN"
                elif query_value == "прилагательное":
                    query_value = "ADJF"
                elif query_value == "глагол":
                    query_value = "VERB"
                elif query_value == "наречие":
                    query_value = "ADVB"
                elif query_value == "числительное":
                    query_value = "NUMR"
                elif query_value == "местоимение":
                    query_value = "NPRO"
                elif query_value == "междометие":
                    query_value = "INTJ"
                elif query_value == "предикатив":
                    query_value = "PRED"


        forms = self.get_word_forms(selected_word, criteria, query_value)
        
        self.search_result_text.delete(1.0, tk.END)
        self.search_result_text.insert(tk.END, f"Формы слова {selected_word} по критерию '{query}':\n")
        self.search_result_text.insert(tk.END, f"{', '.join(forms)}\n")

    def get_word_forms(self, word, criteria=None, query_value=None):
        analyzed_word = self.morph_analyzer.parse(word)[0]
        if criteria == "род" and query_value:
            forms = [parsed_word.word for parsed_word in analyzed_word.lexeme if query_value in (parsed_word.tag.gender or [])]
        elif criteria == "число" and query_value:
            forms = [parsed_word.word for parsed_word in analyzed_word.lexeme if query_value in (parsed_word.tag.number or [])]
        elif criteria == "падеж" and query_value:
            forms = [parsed_word.word for parsed_word in analyzed_word.lexeme if query_value in (parsed_word.tag.case or [])]
        elif criteria == "часть" and query_value:
            forms = [parsed_word.word for parsed_word in analyzed_word.lexeme if query_value in (parsed_word.tag.POS or [])]
        else:
            forms = [parsed_word.word for parsed_word in analyzed_word.lexeme]
        return forms

if __name__ == "__main__":
    root = tk.Tk()
    app = WordViewerApp(root)
    root.mainloop()

