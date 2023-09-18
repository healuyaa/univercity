use tokio::net::TcpStream;
use tokio::io::{AsyncReadExt, AsyncWriteExt, AsyncBufReadExt};
use std::io;
use tokio::sync::mpsc;
use tokio::sync::Mutex; // Use tokio's Mutex
use std::sync::Arc;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let address = "127.0.0.1:8080";
    let stream = Arc::new(Mutex::new(TcpStream::connect(address).await?));
    let (tx, mut rx): (mpsc::Sender<String>, mpsc::Receiver<String>) = mpsc::channel(32);

    // Асинхронная задача для чтения сообщений от сервера и вывода их на консоль
    let reader_task = {
        let stream = stream.clone();
        tokio::spawn(async move {
            let mut stream = stream.lock().await; // Use tokio's Mutex
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

    let writer_task = {
        let stream = stream.clone();
        tokio::spawn(async move {
            let mut stdin = tokio::io::BufReader::new(tokio::io::stdin());
            let mut stream = stream.lock().await;

            loop {
                let mut message = String::new();

                if let Err(e) = stdin.read_line(&mut message).await {
                    eprintln!("{}", e);
                    break;
                }
                let message = message.trim().to_string();

                if message.is_empty() {
                    continue;
                }

                if let Err(e) = stream.write_all(message.as_bytes()).await {
                    eprintln!("Ошибка отправки сообщения серверу: {}", e);
                    break;
                }
            }
        })
    };

    // Ожидаем завершения задач
    reader_task.await?;
    writer_task.await?;

    Ok(())
}
