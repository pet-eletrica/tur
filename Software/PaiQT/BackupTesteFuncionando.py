import sys
import serial
import time
import serial.tools.list_ports as serial_tools
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
import pista as base

# referencia: https://nikolak.com/pyqt-threading-tutorial/
from time import sleep
class ThreadDeLerUmaLinhaSemTravar(QThread):

    def __init__(self, conecao_com_a_porta):
        QThread.__init__(self)
        self.porta_serial = conecao_com_a_porta
        self.contador_do_timer = 0

    def __del__(self):
        self.wait()

    def run(self):
        print("Thread Iniciada...")
        print("Tempo passou..")

        linha_lida = ''
        aux1 =self.porta_serial.inWaiting() #serve pra ver quantos bytes tem na fila
        if aux1 != None :
            self.sensor = ord(self.porta_serial.read(1))
            if self.sensor == 49:
                self.tempoA = self.porta_serial.read(1)
                self.tempoA = (ord(self.tempoA) * 256) + ord(self.porta_serial.read(1))
        self.contador_do_timer += 0.53 # variave que aparece no lcd (sempre somar pois é o tempo pra ler todo mundo)
        tempo = self.tempoA * 5; # calcula o tempo
        #print(aux1)
        print("sensor")
        print(self.sensor)
        print(tempo)
        print("cronometro")
        print(self.contador_do_timer)

class ExampleApp(QMainWindow, base.Ui_MainWindow):
    def __init__(self, parent=None):
        super(ExampleApp, self).__init__(parent)
        self.contador_do_timer = 0;
        self.meu_timer = QTimer()
        self.port = None
        if self.port is None:
            self.port = ExampleApp.get_arduino_serial_port()
        self.baudrate = 115200
        self.conexao = serial.Serial(self.port, self.baudrate)
        print("conectado\n")
        self.setupUi(self)
        self.setup_signals_connections()
        print("Codigo comecou a rodar")



    def get_arduino_serial_port():
        serial_ports = serial_tools.comports()
        if len(serial_ports) == 0:
            return ""
        if len(serial_ports) == 1:
            return serial_ports[0].device
        for serial_port_found in serial_ports:
            if serial_port_found.manufacturer == 'Arduino (www.arduino.cc)':
                return serial_port_found.device
        return ""

    def setup_signals_connections(self):
        self.Iniciar.clicked.connect(self.btn_clicado)
        self.Finalizar.clicked.connect(self.btn_desclicado)
        self.meu_timer.timeout.connect(self.timer_do_lcd)
        self.myThread = ThreadDeLerUmaLinhaSemTravar(self.conexao)

    def btn_clicado(self):
        self.conexao.write(b'I')
        self.contador_do_timer = 0;
        print("codigo enviado\n")
        self.meu_timer.start(1)
        self.timer_do_lcd()

    def timer_do_lcd(self):
        """ Incrementa o tempo nos LCDs disponíveis na interface.
        """
        self.myThread.start()
        #self.receber()
        # self.lineEdit_B.setText(self.sensor_B_C)
        # self.lineEdit_C.setText(self.sensor_C_D)
        # self.lineEdit_D.setText(self.sensor_D_A)
        # self.lineEdit_E.setText("sensor desabilitado")
        #self.lcdNumber.display(self.contador_do_timer)


    def btn_desclicado(self):
        self.conexao.close()
        self.meu_timer.stop()

def main():
    app = QApplication(sys.argv)
    form = ExampleApp()
    form.show()
    app.exec_()

if __name__ == "__main__":
    main()
    sys.exit(app.exec_())
