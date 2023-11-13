import socket
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend

def shift_left_encrypt(label):
    # Пример простого сдвига влево для шифрования меток
    return label[1:] + label[0]

def rc2_encrypt(key, plaintext):
    cipher = Cipher(algorithms.RC2(key), mode=modes.ECB(), backend=default_backend())
    encryptor = cipher.encryptor()
    ciphertext = encryptor.update(plaintext) + encryptor.finalize()
    return ciphertext

def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('localhost', 12345))
    server.listen(1)
    print('Server listening on port 12345...')

    conn, addr = server.accept()
    print(f'Connection from {addr}')

    # Принимаем ID от избирателя
    voter_id = conn.recv(1024).decode()
    print(f'Received voter ID: {voter_id}')

    # Генерируем и отправляем уникальную метку
    label = shift_left_encrypt(voter_id)
    conn.send(label.encode())
    print(f'Sent label: {label}')

    # Принимаем зашифрованный файл {М, B}en
    encrypted_data = conn.recv(1024)
    print('Received encrypted data')

    # Дешифруем файл
    decrypted_data = rc2_encrypt(label.encode(), encrypted_data)
    print(f'Decrypted data: {decrypted_data.decode()}')

    conn.close()

if __name__ == "__main__":
    main()
