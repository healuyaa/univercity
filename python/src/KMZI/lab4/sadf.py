import random

# Параметры эллиптической кривой E751(-1,1)
p = 2**751 - 1
a = -1
b = 1
G = (0, 1)

# Алфавит и его обратное соответствие точкам на кривой
alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 ,.!?()[]{}"
point_mapping = {}

# Генератор случайного ключа
def generate_private_key():
    return random.randint(1, p - 1)

# Сложение точек
def add_points(P, Q):
    if P is None:
        return Q
    if Q is None:
        return P

    x1, y1 = P
    x2, y2 = Q

    if P != Q:
        m = (y2 - y1) * pow(x2 - x1, -1, p)
    else:
        m = (3 * x1**2 + a) * pow(2 * y1, -1, p)

    x3 = (m**2 - x1 - x2) % p
    y3 = (m * (x1 - x3) - y1) % p

    return (x3, y3)

# Умножение точки на скаляр
def multiply_point(k, P):
    Q = None
    for i in range(k.bit_length()):
        if (k >> i) & 1:
            Q = add_points(Q, P)
        P = add_points(P, P)
    return Q

# Создаем соответствие символов и точек на кривой
for i, char in enumerate(alphabet):
    x, y = multiply_point(i + 1, G)  # Генерируем точку на кривой для символа
    point_mapping[char] = (x, y)

# Шифрование текста
def encrypt(plaintext, public_key):
    k = generate_private_key()
    C1 = multiply_point(k, G)
    C2 = []
    for char in plaintext:
        x, y = multiply_point(k, public_key[char])
        C2.append((x, y))
    return C1, C2

# Расшифрование текста
def decrypt(C1, C2, private_key):
    decrypted_text = ""
    for x, y in C2:
        decrypted_char = None
        # Находим символ, соответствующий точке (x, y)
        for char, point in point_mapping.items():
            if point == (x, y):
                decrypted_char = char
                break
        if decrypted_char is not None:
            decrypted_text += decrypted_char
    return decrypted_text

# Пример использования
plaintext = "Hello, World!"
public_key = {}
for char in plaintext:
    public_key[char] = multiply_point(generate_private_key(), G)
C1, C2 = encrypt(plaintext, public_key)
decrypted_text = decrypt(C1, C2, generate_private_key())

print(f"Открытый текст: {plaintext}")
print(f"Зашифрованный текст: {C1}, {C2}")
print(f"Расшифрованный текст: {decrypted_text}")
