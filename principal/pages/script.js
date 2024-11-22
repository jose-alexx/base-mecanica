// Função para enviar comandos ao servidor ESP32
function sendCommand(command) {
    const statusDiv = document.getElementById('status');
    statusDiv.innerText = "Enviando comando...";
    
    // Enviar comando via HTTP GET
    fetch(`/command?cmd=${command}`)
        .then(response => response.text())
        .then(data => {
            // Atualizar o status recebido
            statusDiv.innerText = `Resposta do ESP32: ${data}`;
        })
        .catch(err => {
            statusDiv.innerText = "Erro ao comunicar com o ESP32!";
            console.error(err);
        });
}