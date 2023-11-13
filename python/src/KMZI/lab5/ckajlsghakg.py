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
    voter_id = input("Enter your voter ID: ")

    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(('localhost', 12345))

    # Отправляем ID на сервер
    client.send(voter_id.encode())
    print(f'Sent voter ID: {voter_id}')

    # Получаем уникальную метку от сервера
    label = client.recv(1024).decode()
    print(f'Received label: {label}')

    # Создаем выбор в бюллетене
    choice = input("Enter your choice: ")
    ballot = f'{{M: {label}, B: {choice}}}'.encode()

    # Шифруем файл {М, B}en
    encrypted_data = rc2_encrypt(label.encode(), ballot)
    print(f'Encrypted data: {encrypted_data}')

    # Отправляем зашифрованный файл на сервер
    client.send(encrypted_data)

    client.close()

if __name__ == "__main__":
    main()
