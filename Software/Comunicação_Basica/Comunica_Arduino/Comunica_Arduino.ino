void setup() {
Serial.begin(9600);
pinMode(13, OUTPUT);
}

void loop() {
    char Leitura = Serial.read();
    if(Leitura == '1'){
      digitalWrite(13, HIGH);
      Serial.println("ligou");
    }
    if(Leitura == '0'){
      digitalWrite(13, LOW);
      Serial.println("desligou");
    }
}
