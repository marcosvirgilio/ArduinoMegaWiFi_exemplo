//selecionar placa ESPino (ESP-12 Module ou ESP-13 Module) 

#include <ESP8266WiFi.h>

// Substitua pelas suas credenciais de rede.
const char* ssid     = "SCNET_1511";
const char* password = "dd04061977";

// Defina um servidor com a porta 80.
WiFiServer server(80);

// Variável para armazenar a solicitação HTTP.
String header;

// Variável para armazenar o estado de saída atual.
String outputState = "Desligado";

unsigned long currentTime = millis();
unsigned long previousTime = 0;
// Defina um timeout de 2000 milisegundos.
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  delay(5000);
  // Conecte com a rede Wi-Fi com SSID e senha.
  //Serial.print("Conectando a rede ");
  //Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
  //Inicie o servidor.
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Escute os clientes conectados 
  if (client) {                             // Se um novo cliente se conectar
    String currentLine = "";                // Faça uma String para armazenar dados recebidos do cliente
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop enquanto cliente estiver conectado.
      currentTime = millis();
      if (client.available()) {             // Se holver bytes para ler do cliente,
        char c = client.read();             // faça a leitura.
        header += c;
        if (c == '\n') {                    // Se o byte é um caractere de nova linha, 
                                            // é o fim da solicitação HTML,entao envie uma resposta.
          if (currentLine.length() == 0) {  
            //Envie um cabeçalho HTTP de resposta.
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Procure o trecho "GET /13/on" dentro da solicitação do recebida do cliente.
            if (header.indexOf("GET /13/on") >= 0) {
              //Envie um comando para Mega2560 via serial.
              Serial.println("LED_ON");
              //Altere a variavel de estado.
              outputState = "Ligado";
            } else if (header.indexOf("GET /13/off") >= 0) {
              //Envie um comando para Mega2560 via serial.
              Serial.println("LED_OFF");
              //Altere a variavel de estado.
              outputState = "Desligado";
            }
              // Pagina HTML
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              // CSS para estilizar a pagina
              client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
              client.println("h1,p {font-weight: bold;color: #126e54; font-size: 32px;}");
              client.println("p {font-size: 16px;}");
              client.println(".button { background-color: #1BAE85; border: none; color: white; padding: 16px 40px;");
              client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
              client.println("</style></head>");

              client.println("<body><h1>Arduino Mega WiFi Web Server</h1>");

              // Mostre o estado atual do pino 13, aqui representado pela variavel de estado outputState. 
              client.println("<p>Pino 13 - Estado " + outputState + "</p>");
              // Se outputState estiver como Desligado, crie um botao com texto Ligar.
              if (outputState == "Desligado") {
                client.println("<p><a href=\"/13/on\"><button class=\"button\">Ligar</button></a></p>");
              } else {
                // Se outputState estiver como Ligado, crie um botao com texto Desligar.
                client.println("<p><a href=\"/13/off\"><button class=\"button button2\">Desligar</button></a></p>");
              }
              client.println("</body></html>");
              // A resposta HTTP termina com outra linha em branco.
              client.println();
              break;
            } else { // Se você recebeu uma nova linha, limpe currentLine
              currentLine = "";
            }
          } else if (c != '\r') {  // Se você tiver mais alguma coisa além de um caractere de retorno de carro,
            currentLine += c;      // Adicione-o ao final do currentLine.
          }
        }
      }
      // Limpe a variável de cabeçalho
      header = "";
      // Feche a conexão.
      client.stop();
    }
  }