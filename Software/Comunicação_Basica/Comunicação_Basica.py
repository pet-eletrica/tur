import serial

Port = "COM4"
baudrate = 115200
conexao = serial.Serial(Port, baudrate);
rx = 0;
estado_led = 0;

while True:

    estado_led = raw_input("Digite o estado do led\n 1 para HIGH e 0 para LOW\n")

    if estado_led == "1":
        conexao.write(estado_led);
        rx = conexao.readline();

    elif estado_led == "0":
        conexao.write(estado_led);
        rx = conexao.readline();

    elif estado_led == "9":
        print'fechando conexao'
        conexao.close();

    else:
        print'comando nao reconhecido'
        
    print rx
conexao.close()
