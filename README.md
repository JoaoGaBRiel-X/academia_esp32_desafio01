# Desafio 1 - Sistema de Monitoramento Ambiental com MQTT

Este projeto foi desenvolvido como parte do Desafio 1 de Fevereiro/2025 do curso **Academia ESP32 Profissional** do [Embarcados](https://embarcados.com.br/). O objetivo é criar um sistema de monitoramento ambiental utilizando o **ESP32** e o protocolo **MQTT** no ESP-IDF, coletando dados de temperatura e umidade com o sensor **DHT11** e enviando-os a um broker MQTT, com um sistema de alarme baseado em limites configurados.

## Descrição do Projeto

O sistema utiliza uma placa **Franzininho WiFi (ESP32-S2)** para coletar dados de temperatura e umidade a cada 60 segundos com o sensor DHT11. Esses dados são publicados em tópicos específicos no broker MQTT público `mqtt.eclipseprojects.io` e exibidos em um display OLED conectado à Franzininho WiFi. Caso os valores de temperatura o **limite pré-definido de 30°C**, uma mensagem é enviada a um tópico de alarme. Uma segunda placa, um **ESP32**, subscreve-se ao tópico de alarme e acende um LED para indicar o estado de alarme.

### Funcionalidades
- Coleta de dados de temperatura e umidade a cada 60 segundos pela Franzininho WiFi (ESP32-S2).
- Publicação dos dados em tópicos MQTT no broker publico `mqtt.eclipseprojects.io`. Tópicos:
    - `desafio01_jg\temperature`
    - `desafio01_jg\humidity`
    - `desafio01_jg\alert`
- Exibição de temperatura e umidade em um display OLED.
- Envio de mensagem de alarme ao ultrapassar limites configurados.
- Acendimento de um LED no ESP32 subscrito ao tópico de alarme: `desafio01_jg\alert`.

## Hardware Utilizado
- **Franzininho WiFi (ESP32-S2)**: Responsável por coletar, publicar os dados e exibir no display OLED.
- **ESP32**: Responsável por subscrever o tópico de alarme e acender o LED.
- **Sensor DHT11**: Para medição de temperatura e umidade.
- **Display OLED 128x64**: Para visualização de temperatura e umidade.
- **LED**: Indicador de alarme.
- **Resistores e Jumpers**: Para conexão dos circuitos.

## Como Reproduzir o Projeto

### Pré-requisitos
- ESP-IDF instalado e configurado (versão 5.4.0).
- Acesso à internet para conectar ao broker público `mqtt.eclipseprojects.io`.
- Ferramentas de build e flash (como `idf.py`).

### Passos
1. **Clone o repositório**:
   ```bash
   git clone https://github.com/JoaoGaBRiel-X/academia_esp32_desafio01.git
   cd academia_esp32_desafio01