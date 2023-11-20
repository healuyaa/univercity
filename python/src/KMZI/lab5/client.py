from Crypto.Cipher import ARC2
from Crypto.Util.Padding import pad
from Crypto.Random import get_random_bytes
import base64
import random as rd

class Client:
    def __init__(self):
        self.key: bytes = get_random_bytes(16)
        self.id: int
        self.vote: str = ""
        
    def get_uniq_id(self, id):
        self.id = id
        return id
    
    def gen_send_pair_MB(self, voice, id):
        self.vote = ""
        self.vote += str(self.get_uniq_id(id))
        self.vote += ':'
        self.vote += voice
        
    def send_vote(self):
        return self.RC2_init(self.vote)
    
    def send_end_vote(self, id_: int) -> tuple:
        vote = (id_, self.send_vote())
        # print(vote)
        return (id_, vote)
    
    def RC2_init(self, data) -> str:
        cipher = ARC2.new(self.key, ARC2.MODE_ECB)
        data_ = data.encode('utf-8')
        padded_data = pad(data_, ARC2.block_size)
        ciphertext = cipher.encrypt(padded_data)
        encoded_ciphertext = base64.b64encode(ciphertext).decode('utf-8')
        # print(encoded_ciphertext)
        return encoded_ciphertext
    
    def send_key_id(self, id_):
        # print(id_, self.key)
        return (id_, self.key)
    
    def gen_name_of_izb(self):
        list_r = {
            0 : "first",
            1 : "second",
            2 : "third",
            3 : "fourth",
            4 : "fifth",
        }
        rd1 = rd.randrange(0, 5)
        return list_r[rd1 % 5]