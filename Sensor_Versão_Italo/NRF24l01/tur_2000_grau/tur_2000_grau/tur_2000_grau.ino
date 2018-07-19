#include "nrf24le01Module.h"
#include "led_rgb.h"

#define UART_BAUDRATE       9600

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
uint16_t tempo1 = 0;
uint16_t tempo2 = 0;

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
  //Serial.println("Host Iniciado.");
  //Serial.println("#define INICIA_TIMER_CMD 'I' #define DESLIGA_TIMER_CMD 'D' #define LER_COUNTER_CMD 'R' #define MUDAR_LED_CMD 'L' #define LER_BTN_FLAG_CMD 'B'");

  //Manda todos os sensores iniciarem o timer
  host_nrf.tx_buf[0] = BROADCAST_ADDRESS;
  host_nrf.tx_buf[1] = INICIA_TIMER_CMD;
  host_nrf.TX_Mode_NOACK(2);
}

uint8_t requisitando_de = 1;
void loop() {
  actual_millis = millis();
  if (host_nrf.newPayload) {
    received_millis = actual_millis;
    host_nrf.newPayload = 0;
    status_led.acender(LED_COLOR_AQUA);
    uint8_t sensor = host_nrf.rx_buf[0]; //ql sensor enviou a msg
    //cmd_recebido = host_nrf.rx_buf[1]; //ql comando o sensor esta enviando
    tempo1 = host_nrf.rx_buf[2] << 8 | host_nrf.rx_buf[3];
    if (sensor == '1') {
      Serial.print("A: ");
      Serial.print(tempo1 * 5);
      Serial.print(" ms.\t");
    } else if (sensor == '2') {
      Serial.print("B: ");
      Serial.print(tempo1 * 5);
      Serial.println(" ms.");
    }
  }

  if (Serial.available()) {
    sent_millis = actual_millis;
    status_led.acender(LED_COLOR_YELLOW);
    host_nrf.tx_buf[0] = BROADCAST_ADDRESS;
    host_nrf.tx_buf[1] = Serial.read();
    host_nrf.TX_Mode_NOACK(2);
    //Serial.println("**Payload Enviada**");
    Serial.print("\n");
    Serial.print("Payload Width: "); Serial.println(2);
    //Serial.print("Payload Data: "); //Serial.write(host_nrf.tx_buf, 2);
    Serial.print("\n");
    for (int i = 0; i < 2; i++) {
      Serial.print("0x"); Serial.print(host_nrf.tx_buf[i], HEX); //Serial.print(" ");
    }
    Serial.print("\n");
  }

  if (actual_millis - requisitou_dado_millis > 125) {
    //a cada intervalo de tempo ele requisita os dados
    requisitou_dado_millis = actual_millis;
   if(requisitando_de == 1){
    host_nrf.tx_buf[0] = '1'; //trocar para ler cada sensor
    host_nrf.tx_buf[1] = LER_COUNTER_CMD;
    host_nrf.TX_Mode_NOACK(2);
    requisitando_de = 2;
   } else if(requisitando_de == 2){
    host_nrf.tx_buf[0] = '2'; //trocar para ler cada sensor
    host_nrf.tx_buf[1] = LER_COUNTER_CMD;
    host_nrf.TX_Mode_NOACK(2);
    requisitando_de = 1;
   }
  }
  if (actual_millis - received_millis  > 250) {
    status_led.apagar();
  }
  if (actual_millis - sent_millis > 250) {
    status_led.apagar();
  }
}

void rf_interrupt() {
  host_nrf.RF_IRQ();
}

void piscas_iniciais_led() {
  status_led.acender(LED_COLOR_RED); delay(500);
  status_led.apagar(); delay(500);
  status_led.acender(LED_COLOR_GREEN); delay(500);
  status_led.apagar(); delay(500);
  status_led.acender(LED_COLOR_BLUE); delay(500);
  status_led.apagar(); delay(500);
}


