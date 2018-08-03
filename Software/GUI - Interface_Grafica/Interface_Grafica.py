from Tkinter import *

def callback(): 
    print "click!"

janela = Tk()


janela.title("Janela Principal")

bt = Button(janela, text="OK", command=callback)
bt.pack()

janela.mainloop()

