def pad_message_with_length(message):
    message_length = len(message) * 8
    extension_length = (448 - (message_length + 1)) % 512
    padded_message = bytearray(message)
    padded_message.append(0x80)
    padded_message.extend(b'\x00' * (extension_length // 8))
    length_low = message_length & 0xFFFFFFFF
    length_high = (message_length >> 32) & 0xFFFFFFFF
    padded_message.extend(length_low.to_bytes(4, byteorder='little'))
    padded_message.extend(length_high.to_bytes(4, byteorder='little'))
    return padded_message

def non_linear_functions(j, x, y, z):
    if 0 <= j <= 15:
        return x ^ y ^ z
    elif 16 <= j <= 31:
        return (x & y) | (~x & z)
    elif 32 <= j <= 47:
        return (x | ~y) ^ z
    elif 48 <= j <= 63:
        return (x & z) | (y & ~z)
    elif 64 <= j <= 79:
        return x ^ (y & ~z)

def constants(j):
    if 0 <= j <= 15:
        return 0x00000000
    elif 16 <= j <= 31:
        return 0x5A827999
    elif 32 <= j <= 47:
        return 0x6ED9EBA1
    elif 48 <= j <= 63:
        return 0x8F1BBCDC
    elif 64 <= j <= 79:
        return 0xA953FD4E

def rotation(j):
    if 0 <= j <= 15:
        return j
    elif 16 <= j <= 31:
        return (7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8)[j - 16]
    elif 32 <= j <= 47:
        return (3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12)[j - 32]
    elif 48 <= j <= 63:
        return (1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2)[j - 48]
    elif 64 <= j <= 79:
        return (4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13)[j - 64]

def rotate_left(x, n):
    return ((x << n) | (x >> (32 - n))) & 0xFFFFFFFF

def ripemd160(message):
    def rols(x, s):
        return ((x << s) | (x >> (32 - s))) & 0xFFFFFFFF

    def int_to_hex_string(n):
        return format(n, '08X')

    def process_block(block, h0, h1, h2, h3, h4):
        A, B, C, D, E = h0, h1, h2, h3, h4

        changed_bits_list = []

        for j in range(80):
            if 0 <= j <= 15:
                f = non_linear_functions(j, B, C, D)
                k = constants(j)
                r = rotation(j)
            elif 16 <= j <= 31:
                f = non_linear_functions(j, B, C, D)
                k = constants(j)
                r = rotation(j)
            elif 32 <= j <= 47:
                f = non_linear_functions(j, B, C, D)
                k = constants(j)
                r = rotation(j)
            elif 48 <= j <= 63:
                f = non_linear_functions(j, B, C, D)
                k = constants(j)
                r = rotation(j)
            elif 64 <= j <= 79:
                f = non_linear_functions(j, B, C, D)
                k = constants(j)
                r = rotation(j)

            T = (rols(A, 5) + f + E + k + block[r]) & 0xFFFFFFFF
            E = D
            D = C
            C = rols(B, 30)
            B = A
            A = T

            changed_bits_list.append(int_to_hex_string(A) + int_to_hex_string(B) + int_to_hex_string(C) + int_to_hex_string(D) + int_to_hex_string(E))

        h0 = (h0 + A) & 0xFFFFFFFF
        h1 = (h1 + B) & 0xFFFFFFFF
        h2 = (h2 + C) & 0xFFFFFFFF
        h3 = (h3 + D) & 0xFFFFFFFF
        h4 = (h4 + E) & 0xFFFFFFFF

        return h0, h1, h2, h3, h4, changed_bits_list

    padded_message = pad_message_with_length(message)
    t = len(padded_message) // 64
    h0 = 0x67452301
    h1 = 0xEFCDAB89
    h2 = 0x98BADCFE
    h3 = 0x10325476
    h4 = 0xC3D2E1F0

    total_changed_bits_list = []

    for i in range(t):
        block = [int.from_bytes(padded_message[i * 64 + j:i * 64 + j + 4], byteorder='little') for j in range(0, 64, 4)]
        h0, h1, h2, h3, h4, changed_bits_list = process_block(block, h0, h1, h2, h3, h4)
        total_changed_bits_list.extend(changed_bits_list)

    digest = (h0 << 128) | (h1 << 96) | (h2 << 64) | (h3 << 32) | h4
    hex_digest = format(digest, '02X')
    with open("src/KMZI/lab2/hash_output.txt", "w") as file:
        file.write(hex_digest)

    return hex_digest, total_changed_bits_list

def read_message_from_file(file_path):
    try:
        with open(file_path, "rb") as file:
            return file.read()
    except FileNotFoundError:
        print(f"File '{file_path}' not found.")
        return None

file_path = "src/KMZI/lab2/input.txt"
original_message = read_message_from_file(file_path)

hashed_message, changed_bits_list = ripemd160(original_message)
print(f"Hash: {hashed_message}")

# Записываем измененные биты в файл
with open("src/KMZI/lab2/iterations.txt", "w") as bits_file:
    for i, changed_bits in enumerate(changed_bits_list):
        bits_file.write(f"Iteration {i + 1} Changed Bits: {changed_bits}\n")
