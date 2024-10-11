String msg;//String para armazenar a mensagem recebida pela porta serial 3.

void setup() {
  //Defina o pino LED_BUILTIN (13), como saida.
  pinMode(LED_BUILTIN, OUTPUT);
  //Defina as porta serial para comunicação usb.
  Serial.begin(115200);
  //Defina as porta serial para comunicação com ESP8266.
  Serial3.begin(115200);
}

void loop() {
  //Aguarde dados vindos do ESP8266.
  if (Serial3.available()) {
    //Leitura de um byte.
    char data = Serial3.read();
    //Imprima o mesmo dado pela porta usb.
    Serial.print(data);
    //Acrescente o caractere recebido a string de mensagem.
    msg += data;

    if (data == 13) {//Limpa a string ao receber caractere CR(Carriage return).
      msg = "";
    }
    if (msg.indexOf("LED_ON") > 0) {//Verifica a ocorrencia do trecho "LED_ON" na mensagem recebida.
      //Ative a saida.
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else if (msg.indexOf("LED_OFF")> 0) {//Verifica a ocorrencia do trecho "LED_OFF" na mensagem recebida.
      //Desligue a saida.
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  delay(50);
}