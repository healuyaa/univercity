#[allow(dead_code)]
#[allow(unused_imports)]
#[allow(unused_variables)]

fn make_sequence(m_string: &String) -> Vec<bool> {
    let b = m_string.len() * 8;
    let mut binary_sequence: Vec<bool> = Vec::new();
    for byte in m_string.bytes() {
        for i in (0..8).rev() {
            let bit = (byte >> i) & 1 == 1;
            binary_sequence.push(bit);
        }
    }
    return binary_sequence;
}

fn make_blocks(binary_sequence: &Vec<bool>, t: usize, block_size_words: u32) -> Vec<Vec<u32>> {
    let mut message_blocks: Vec<Vec<u32>> = vec![vec![0; block_size_words as usize]; t];
    let mut bit_index = 0;
    for i in 0..t {
        for j in 0..block_size_words {
            let mut word: u32 = 0;
            for k in 0..4 {
                if bit_index < binary_sequence.len() {
                    if binary_sequence[bit_index] {
                        word |= (binary_sequence[bit_index] as u32) << (8 * k);
                    }
                    bit_index += 1;
                }
            }
            message_blocks[i][j as usize] = word;
        }
    }
    return message_blocks;
}

fn add_apdding_and_length(binary_sequence: Vec<bool>) -> Vec<bool> {
    let b = binary_sequence.len() as u64;
    let padded_length = (b + 1 + 64) as usize;
    let mut padded_sequence = vec![false; padded_length];

    for i in 0..binary_sequence.len() {
        padded_sequence[i] = binary_sequence[i];
    }

    padded_sequence[binary_sequence.len()] = true;
    let mut length_bits = Vec::new();
    let mut temp = b;
    for _ in 0..64 {
        length_bits.push(temp & 1 == 1);
        temp >>= 1;
    }

    for i in 0..64 {
        padded_sequence[binary_sequence.len() + 1 + i] = length_bits[63 - i];
    }

    return padded_sequence;
}

pub fn main() {
    let m_string = String::from("flakshjifopasif"); 
    let binary_sequence =  make_sequence(&m_string);

    let block_size_words: u32 = 16;
    let t = (binary_sequence.len() as f64 / (block_size_words * 32) as f64).ceil() as usize;
    let message_blocks = make_blocks(&binary_sequence, t, block_size_words);

    let padded_sequance = add_apdding_and_length(binary_sequence);
}