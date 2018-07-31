import time
leitura = raw_input()
start_time = time.time()
ciclo = True
while ciclo:
    if leitura == "A":
        run = time.time()- start_time
        print '{:.2f}'.format(run)
        

        

