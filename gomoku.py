from tkinter import *

class gomoku:

    def __init__(self, master):

        # start main frame
        self.frame = Frame(master)
        self.frame.pack(fill='both', expand=True)

        # start a canvas
        self.canvas = Canvas(self.frame, width=600, height=500)
        self.canvas.pack(fill='both', expand=True)

        # create a label
        self.label = Label(self.frame, text="Gomoku game", height=6, bg="black", fg="white")
        self.label.pack(fill='both', expand=True)

        # start button
        self.framebtn = Frame(self.frame)
        self.framebtn.pack(fill='both', expand=True)

        self.btn_single = Button(self.framebtn, text="Single player", 
                                 height=5, bg='blue', fg='yellow', command=self.sgplayer)
        self.btn_single.pack(fill='both', expand=True, side=LEFT)

        self.btn_double = Button(self.framebtn, text="Double player", 
                                 height=5, bg='yellow', fg='blue', command=self.dbplayer)
        self.btn_double.pack(fill='both', expand=True, side=RIGHT)

        # draw the game board
        self._board()

    def _board(self):
        pass

    def sgplayer(self):
        pass

    def dbplayer(self):
        pass




# start main program
root = Tk()
app = gomoku(root)
root.mainloop()
