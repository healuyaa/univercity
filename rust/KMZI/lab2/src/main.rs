#[allow(dead_code)]
#[allow(unused_imports)]
#[allow(unused_variables)]

use std::fs::File;
use std::io::{self, Read, Write};
mod test;

fn read_file(file_path: &str) -> Result<String, std::io::Error> {
    let mut file = File::open(file_path)?;
    let mut content = String::new();
    file.read_to_string(&mut content)?;

    Ok(content)
}

fn write_file(file_path: &str, content: &str) -> Result<(), std::io::Error> {
    let mut file = File::create(file_path)?;
    file.write_all(content.as_bytes())?;

    Ok(())
}

fn menu() {
    println!("Меню:");
    println!("1. Просмотр содержимого файла");
    println!("2. Просмотр хэш-значения файла");
    println!("3. Изменение считанного из файла");
    println!(". Выход");
}

fn main() {

    test::main();
    // loop {
    //     menu();

    //     let mut choice = String::new();
    //     io::stdin().read_line(&mut choice).expect("Error");
    
    //     match choice.trim() {
    //         "1" => {
                
    //         }
    //         "2" => {
                
    //         }
    //         "3" => {
    
    //         }
    //         "4" => {
    //             // break;
    //         }
    //         _ => println!("wrong input")
    //     }
    // }
}