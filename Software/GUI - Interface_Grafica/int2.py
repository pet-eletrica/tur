from Tkinter import *

def start():
    print "clique"
    
janela = Tk()
    

bt = Button(janela,width=20, text="start", command=start)
bt.place(x=75,y=100)

lb = Label(janela,text="text")
lb.place(x=125, y=150)

janela.geometry("300x300+100+100")
janela.mainloop()
