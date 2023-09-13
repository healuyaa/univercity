use tokio::net::TcpListener;
use tokio::io::{AsyncReadExt, AsyncWriteExt};

mod crypt;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let listener = TcpListener::bind("127.0.0.1:8080").await?;
    println!("Server listening on port 8080...");

    loop {
        let (mut socket, _) = listener.accept().await?;
        tokio::spawn(async move {
            let mut buffer = [0; 1024];
            loop {
                let n = match socket.read(&mut buffer).await {
                    Ok(n) if n == 0 => return,
                    Ok(n) => n,
                    Err(e) => {
                        eprintln!("Failed to read from socket: {}", e);
                        return;
                    }
                };
                if let Err(e) = socket.write_all(&buffer[0..n]).await {
                    eprintln!("Failed to write to socket: {}", e);
                    return;
                }
            }
        });
    }
}
