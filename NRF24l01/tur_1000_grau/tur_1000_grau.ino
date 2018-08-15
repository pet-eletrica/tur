
#include "nrf24le01Module.h"
#include "led_rgb.h"

#define UART_BAUDRATE       115200

//Endereços
#define BROADCAST_ADDRESS '0'
#define SENSOR_A_ADDRESS '1'

//Comandos
#define INICIA_TIMER_CMD 'I'
#define DESLIGA_TIMER_CMD 'D'
#define LER_COUNTER_CMD 'R'
#define MUDAR_LED_CMD 'L'
#define LER_BTN_FLAG_CMD 'B'
#define RESET_TIMER 'A'

//Variáveis
uint16_t tempo = 0;
uint8_t tempos[10];

led_rgb status_led(7, 6, 5); //R_pin,G_pin,B_pin
nrf24le01Module host_nrf(2, 4, 3); //IRQ, CE, CSN

char serialOp;

unsigned long actual_millis, received_millis, sent_millis, requisitou_dado_millis, momento_para_req_s2;
void setup() {
  Serial.begin(UART_BAUDRATE);
  status_led.init();
  host_nrf.rf_init(host_nrf.ADDR_HOST, host_nrf.ADDR_HOST, 92, RF_DATA_RATE_2Mbps, RF_TX_POWER_0dBm); //RF Communication
  attachInterrupt(digitalPinToInterrupt(host_nrf.RFIRQ), rf_interrupt, FALLING); //Todo: put inside library
  piscas_iniciais_led();
}

void loop() {
  actual_millis = millis();
  if (host_nrf.newPayload) {
    received_millis = actual_millis;
    host_nrf.newPayload = 0;
    status_led.acender(LED_COLOR_AQUA);
    //Serial.print(sensor); // envia um valor inteiro 1 ou 2 ou 3 ...
    //Serial.print(tempo);
    //Serial.print(tempo);
  }

  if (Serial.available()) {
    status_led.acender(LED_COLOR_YELLOW);
    host_nrf.tx_buf[0] = BROADCAST_ADDRESS;
    host_nrf.tx_buf[1] = Serial.read(); // 
    host_nrf.TX_Mode_NOACK(2);
    delay(50);
  }
  while( actual_millis - sent_millis > 100 )
    {
    if (host_nrf.tx_buf[1] == 'I' ) { // se caso serial read for = 1
      //Leitura do sensor 1
      host_nrf.tx_buf[0] = '1'; //trocar para ler cada sensor
      host_nrf.tx_buf[1] = LER_COUNTER_CMD;
      host_nrf.TX_Mode_NOACK(2);
      if (host_nrf.RX_OK)
      {
        status_led.apagar();
        status_led.acender(LED_COLOR_BLUE);
        tempos[0] = host_nrf.rx_buf[2];
        tempos[1] = host_nrf.rx_buf[3];
        tempo = (tempos[0] * 256) + tempos[1];
        Serial.println(tempo); // convertido
        Serial.println(tempos[0]); // HIGH Tempo 
        Serial.println(tempos[1]); // LOW Tempo
        Serial.println(host_nrf.rx_buf[0]); // Sensor
      }
      //Leitura do sensor 2
      host_nrf.tx_buf[0] = '2'; //trocar para ler cada sensor
      host_nrf.tx_buf[1] = LER_COUNTER_CMD;
      host_nrf.TX_Mode_NOACK(2);
      if (host_nrf.RX_OK)
      {
        status_led.apagar();
        status_led.acender(LED_COLOR_RED);
        tempos[2] = host_nrf.rx_buf[2];
        tempos[3] = host_nrf.rx_buf[3];

        Serial.print(tempos[2]);
        Serial.print(tempos[3]);
        Serial.print(host_nrf.rx_buf[0]);
      }
      //Leitura do sensor 3
      host_nrf.tx_buf[0] = '3'; //trocar para ler cada sensor
      host_nrf.tx_buf[1] = LER_COUNTER_CMD;
      host_nrf.TX_Mode_NOACK(2);
      if (host_nrf.RX_OK)
      { status_led.apagar();
        status_led.acender(LED_COLOR_GREEN);
        tempos[4] = host_nrf.rx_buf[1];
        tempos[5] = host_nrf.rx_buf[2];
        Serial.print(tempos[4]);
        Serial.print(tempos[5]);
      }
      //Leitura do sensor 4
      host_nrf.tx_buf[0] = '4'; //trocar para ler cada sensor
      host_nrf.tx_buf[1] = LER_COUNTER_CMD;
      host_nrf.TX_Mode_NOACK(2);
      if (host_nrf.RX_OK)
      {
        status_led.apagar();
        tempos[6] = host_nrf.rx_buf[1];
        tempos[7] = host_nrf.rx_buf[2];
        Serial.print(tempos[6]);
        Serial.print(tempos[7]);
      }
      //Leitura do sensor 5
      host_nrf.tx_buf[0] = '5'; //trocar para ler cada sensor
      host_nrf.tx_buf[1] = LER_COUNTER_CMD;
      host_nrf.TX_Mode_NOACK(2);
      if (host_nrf.RX_OK)
      {
        tempos[8] = host_nrf.rx_buf[1];
        tempos[9] = host_nrf.rx_buf[2];
        Serial.print(tempos[8]);
        Serial.print(tempos[9]);
      }
    }
    sent_millis = actual_millis;
  }
}
void rf_interrupt() {
  host_nrf.RF_IRQ();
}

void piscas_iniciais_led() {
  status_led.acender(LED_COLOR_RED); delay(100);
  status_led.apagar(); delay(100);
  status_led.acender(LED_COLOR_GREEN); delay(100);
  status_led.apagar(); delay(100);
  status_led.acender(LED_COLOR_BLUE); delay(100);
  status_led.apagar(); delay(100);
}


