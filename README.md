# Projeto Comunicação Serial com RP2040 e BitDogLab

Repositório criado para o projeto do programa de residência **EmbarcaTech**, realizado pelo **CEPEDI**, e apresentado ao professor **WILTON LACERDA SILVA**, com conteúdo da ***UNIDADE 4 CAPÍTULO 6*** sobre **Interfaces de Comunicação Serial (UART, SPI, I2C)**.

## Objetivos do Projeto

O objetivo deste projeto é consolidar o uso de interfaces de comunicação serial com o microcontrolador RP2040, aplicando o conhecimento adquirido em UART e I2C. O projeto também abrange a manipulação de LEDs comuns, LEDs endereçáveis WS2812, e o uso de interrupções e debouncing em botões.

---

## Funcionalidades

### 1. Modificação da Biblioteca `font.h`
- A biblioteca `font.h` foi modificada para incluir caracteres minúsculos, permitindo exibir caracteres maiúsculos e minúsculos no display SSD1306.

### 2. Entrada de Caracteres via PC
- O **Serial Monitor** do VS Code é usado para enviar caracteres via UART.
- Cada caractere digitado é exibido no display SSD1306.
- Quando um número entre 0 e 9 é digitado, ele é exibido também na matriz 5x5 de LEDs WS2812.

### 3. Interação com o Botão A
- O **Botão A** alterna o estado do **LED RGB verde** (ligado/desligado).
- O estado do LED é mostrado no display SSD1306 e enviado ao Serial Monitor.

### 4. Interação com o Botão B
- O **Botão B** alterna o estado do **LED RGB azul** (ligado/desligado).
- O estado do LED é mostrado no display SSD1306 e enviado ao Serial Monitor.

---

## Dependências

Para compilar e executar este projeto, você precisará:

- **Placa BitDogLab**
- **Microcontrolador RP2040**
- **Ambiente de Desenvolvimento RP2040** (CMake, GCC)
- **Bibliotecas necessárias para manipulação de GPIOs e LEDs RGB**
- **Simulador Wokwi** (opcional, mas recomendado para testes)
- **Programa de Comunicação Serial**: É necessário utilizar um programa para comunicação serial com a taxa padrão de **115200 bauds** para se comunicar com a placa.

---

## Instruções de Instalação

1. **Clonar o repositório**
    ```bash
    git clone https://github.com/seu-usuario/bitDogInterface.git
    cd bitDogInterface
    ```

2. **Criar diretório para build**
    ```bash
    mkdir build
    cd build
    ```

3. **Gerar arquivos para compilação**
    Dentro do diretório, execute `cmake` para gerar os arquivos para compilação:
    ```bash
    cmake ..
    ```

4. **Compilar o projeto**
    Dentro da pasta `build`, execute `make` para compilar:
    ```bash
    make
    ```

5. **Carregar o código na placa BitDogLab**
    Utilize o ambiente de desenvolvimento adequado para carregar o código na sua placa.

6. **Abrir o Serial Monitor**
    Abra o Serial Monitor no VS Code para interagir com o sistema. As informações sobre o estado dos LEDs serão enviadas para o monitor. Lembre-se de configurar a taxa de transmissão para **115200 bauds**.

---

## Estrutura do Projeto

1. `root`: Contém o código-fonte do projeto.
2. `include/libraries`: Contém os arquivos de cabeçalho (.h).
3. `include/modules`: Contém os modulos de implementação (.c).
4. `build/`: Diretório de saída dos binários.
5. `CMakeLists.txt`: Arquivo de configuração do CMake.

---

## Link do Vídeo de Demonstração

Veja uma demonstração em vídeo do funcionamento do projeto no YouTube:

[**Assista à Demonstração no YouTube**](https://www.youtube.com/live/7H03ePpwC-s)

---

## Licença

Este projeto está licenciado sob a **Licença GNU**.

---

<div align="center">
    GNUs not Unix
<pre>
    _-`````-,           ,- '- .
  .'   .- - |          | - -.  `.
 /.'  /                     `.   \
:/   :      _...   ..._      ``   :
::   :     /._ .`:'_.._\.    ||   :
::    `._ ./  ,`  :    \ . _.''   .
`:.      /   |  -.  \-. \\_      /
  \:._ _/  .'   .@)  \@) ` `\ ,.'
     _/,--'       .- .\,-.`--`.
       ,'/''     (( \ `  )    
        /'/'  \    `-'  (      
         '/''  `._,-----'
          ''/'    .,---'
           ''/'      ;:
             ''/''  ''/
               ''/''/''
                 '/'/'
                  `;      
<pre>
</div>
