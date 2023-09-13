use tokio::net::{TcpListener, TcpStream};
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use std::sync::Arc;
use tokio::sync::Mutex;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let listener = TcpListener::bind("127.0.0.1:8080").await?;
    let shared_data = Arc::new(Mutex::new(false));

    while let Ok((mut first_socket, _)) = listener.accept().await {
        let shared_data = shared_data.clone();

        tokio::spawn(async move {
            let mut data = shared_data.lock().await;

            if !*data {
                // Первый клиент
                let mut buffer = [0u8; 1024];
                let n = first_socket.read(&mut buffer).await.expect("Ошибка чтения");

                if n == 0 {
                    return;
                }

                let message = String::from_utf8_lossy(&buffer[..n]);
                println!("Получено сообщение от первого клиента: {}", message);

                // Разрешаем второму клиенту отправить свое сообщение
                *data = true;
            } else {
                // Второй клиент
                let mut buffer = [0u8; 1024];
                let n = first_socket.read(&mut buffer).await.expect("Ошибка чтения");

                if n == 0 {
                    return;
                }

                let message = String::from_utf8_lossy(&buffer[..n]);
                println!("Получено сообщение от второго клиента: {}", message);
            }
        });
    }

    Ok(())
}
