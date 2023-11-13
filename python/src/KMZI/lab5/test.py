import random as rd
from Crypto.Cipher import ARC2
from Crypto.Random import get_random_bytes
from Crypto.Util.Padding import pad, unpad

class Selecteds:
    
    def __init__(self):
        self.num_voters = 100
        self.voter_list = self.gen_id(self.num_voters)
    
        self.shift_mount = 5
        self.key = get_random_bytes(16)
        self.block_size = 8
        
    def gen_id(self, num_voters: int) -> list[int]:
        identifiers = [rd.randint(1, 1000) for i in range(num_voters)]
        return identifiers
    
    def encrypt_shift(self, number):
        enc_number = number << self.shift_mount
        return enc_number
                
    def decrypt_shift(self, number):
        dec_number = number >> self.shift_mount
        return dec_number    
    
    def encrypt_RC2(self, message):
        padded_message = pad(message, self.block_size)
        cipher = ARC2.new(self.key, ARC2.MODE_ECB)
        ciphertext = cipher.encrypt(padded_message)
        return ciphertext
    
    def decrypt_RC2(self, ciphertext):
        cipher = ARC2.new(self.key, ARC2.MODE_ECB)
        decrypted_message = cipher.decrypt(ciphertext)
        return decrypted_message
    
    
    
def main():
    abc = Selecteds()
    
    
if __name__ == "__main__":
    main()