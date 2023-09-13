use tokio::net::TcpStream;
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use std::io;
use tokio::sync::mpsc;
use std::sync::{Arc, Mutex};

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let address = "127.0.0.1:8888";
    let stream = Arc::new(Mutex::new(TcpStream::connect(address).await?));
    let (tx, mut rx): (mpsc::Sender<String>, mpsc::Receiver<String>) = mpsc::channel(32);

    // Асинхронная задача для чтения сообщений от сервера и вывода их на консоль
    let reader_task = {
        let stream = stream.clone();
        tokio::spawn(async move {
            let mut stream = stream.lock().expect("Ошибка при блокировке мьютекса");
            loop {
                let mut buffer = [0; 1024];
                match stream.read(&mut buffer).await {
                    Ok(n) if n == 0 => {
                        println!("Сервер закрыл соединение.");
                        break;
                    }
                    Ok(n) => {
                        let message = String::from_utf8_lossy(&buffer[..n]);
                        println!("Сообщение от сервера: {}", message);
                    }
                    Err(e) => {
                        eprintln!("Ошибка чтения от сервера: {}", e);
                        break;
                    }
                }
            }
        })
    };

    // Асинхронная задача для отправки сообщений серверу
    let writer_task = {
        let stream = stream.clone();
        tokio::spawn(async move {
            loop {
                let mut message = String::new();
                io::stdin().read_line(&mut message).expect("Ошибка ввода");
                let message = message.trim().to_string();

                if message.is_empty() {
                    continue;
                }

                let result = {
                    let stream = stream.clone();
                    tokio::spawn(async move {
                        let mut stream = stream.lock().expect("Ошибка при блокировке мьютекса");
                        if let Err(e) = stream.write_all(message.as_bytes()).await {
                            eprintln!("Ошибка отправки сообщения серверу: {}", e);
                        }
                    })
                };
                result.await.expect("Ошибка при выполнении асинхронной задачи");
            }
        })
    };

    // Асинхронная задача для обработки входящих сообщений от других клиентов
    let mut client_messages = Vec::new();
    while let Some(message) = rx.recv().await {
        client_messages.push(message);
    }

    // Ожидаем завершения задач
    reader_task.await?;
    writer_task.await?;

    Ok(())
}
