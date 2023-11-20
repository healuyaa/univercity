from Crypto.Cipher import ARC2
from Crypto.Util.Padding import pad, unpad
import base64
import time

class Counter:
    def __init__(self):
        self.key: bytes = 0        
        self.list_c = {}
        self.list_M_MB: tuple = ()
        self.list_res = {
            "first" : 0,
            "second" : 0,
            "third" : 0,
            "fourth" : 0,
            "fifth" : 0
        }
    
    def init_key(self, key: tuple):
        self.key = key[1]
        
    def get_list_cs(self, list_c):
        self.list_c = list_c
            
    def RC2_init(self, data) -> str:
        cipher = ARC2.new(self.key, ARC2.MODE_ECB)
        data_ = data.encode('utf-8')
        padded_data = pad(data_, ARC2.block_size)
        ciphertext = cipher.encrypt(padded_data)
        encoded_ciphertext = base64.b64encode(ciphertext).decode('utf-8')
        # print(encoded_ciphertext)
        return encoded_ciphertext
    
    def RC2_decrypt(self, encoded_ciphertext) -> str:
        ciphertext = base64.b64decode(encoded_ciphertext)
        cipher = ARC2.new(self.key, ARC2.MODE_ECB)
        decrypted_data = unpad(cipher.decrypt(ciphertext), ARC2.block_size)
        d_c = decrypted_data.decode('utf-8')
        print(d_c)
        return d_c
    
    def public_M_MB(self, MB_B):
        self.list_M_MB = MB_B
        for i_lis in self.list_M_MB:
            print(i_lis)
            
    def find_res(self, res: str):
        res_ = res.split(':')[1]
        if res_ in self.list_res:
            self.list_res[res_] += 1