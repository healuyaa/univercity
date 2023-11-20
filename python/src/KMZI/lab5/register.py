import random as rd
import string

class Register:
    def gen_rand_values(self):
        return ''.join(rd.choices(string.ascii_uppercase + string.digits, k=10))
    
    def __init__(self):
        self.list_r: dict[str, str] = {
            "first" : self.gen_rand_values(),
            "second" : self.gen_rand_values(),
            "third" : self.gen_rand_values(),
            "fourth" : self.gen_rand_values(),
            "fifth" : self.gen_rand_values()
        }
        self.list_c: dict = {}
        self.list_names: dict[int, str] = {}
    
    def public_list_r(self): 
        for cond, uniq in self.list_r.items():
            print(f"{cond} : {uniq}")
        print()
            
    def shift_left(self, id) -> int:
        id_ = id << 5
        return id_
            
    def get_uniq_id_c(self, id):
        id_ = self.shift_left(id)
        self.list_c[id] = id_
        self.list_names[id] = f"full name{len(self.list_c)}"
        if len(self.list_names) == 10:
            for id, name in self.list_names.items():
                print(f"{name} : {id}")
        return id_
        
    def send_uniq_id_c(self, id) -> int:
        uniq = self.list_c[id]
        # print(uniq)
        return uniq
        
    def send_uniq_id_cs(self) -> dict:
        # for cond, uniq in self.list_c.items():
        #     print(f"{cond} : {uniq}") 
        return self.list_c            