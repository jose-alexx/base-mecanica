function sendCommand(command) {
    fetch(`/command?cmd=${command}`)
        .then(response => response.text())
        .then(data => {
            document.getElementById('status').innerText = data;
        })
        .catch(err => {
            console.error("Erro ao enviar comando:", err);
        });
}