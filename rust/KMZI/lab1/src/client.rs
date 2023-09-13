use tokio::net::TcpStream;
use tokio::io::{AsyncReadExt, AsyncWriteExt};

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut stream = TcpStream::connect("127.0.0.1:8080").await?;
    let message = "Hello, server!";
    
    // Отправляем сообщение на сервер
    stream.write_all(message.as_bytes()).await?;
    
    // Читаем ответ от сервера
    let mut buffer = [0; 1024];
    let n = stream.read(&mut buffer).await?;
    let response = String::from_utf8_lossy(&buffer[0..n]);
    println!("Server response: {}", response);
    
    Ok(())
}
