from Tkinter import *

def callback(): 
    print "click!"

janela = Tk()

imagem = janela.PhotoImage(file="C:\Users\talle\Desktop\banco3.jpg")
w = janela.Label(janela, image=imagem)
w.imagem = imagem
w.pack()


janela.title("Janela Principal")

bt = Button(janela, text="OK", command=callback)
bt.pack()

janela.mainloop()

