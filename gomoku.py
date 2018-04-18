from tkinter import *

class gomoku:

    def __init__(self, master):
        # start main frame
        self.frame = Frame(master)
        self.frame.pack(fill='both', expand=True)

        # start a canvas
        self.canvas = Canvas(self.frame, width=320, height=340)
        self.canvas.pack(fill='both', expand=True)

        # create a label
        self.label = Label(self.frame, text="Gomoku game", height=6, bg="black", fg="white")
        self.label.pack(fill='both', expand=True)

        # start button
        self.framebtn = Frame(self.frame)
        self.framebtn.pack(fill='both', expand=True)
        self.btn_single = Button(self.framebtn, text="Single player", 
                                 height=5, bg='blue', fg='yellow', command=self.startSg)
        self.btn_single.pack(fill='both', expand=True, side=LEFT)
        self.btn_double = Button(self.framebtn, text="Double player", 
                                 height=5, bg='yellow', fg='blue', command=self.startDb)
        self.btn_double.pack(fill='both', expand=True, side=RIGHT)

        # draw the game board
        self._board()

    def _board(self):
        for x in range(9):
            self.canvas.create_line(80+x*20,0,x*10,160+x*20)
            self.canvas.create_line(320-(80+x*20),320,320-x*10,160-x*20)

        for y in range(9):
            self.canvas.create_line(80+y*20,0,240+y*10,320-y*20)
            self.canvas.create_line(80-y*10,0+y*20,240-y*20,320)

        for z in range(9):
            self.canvas.create_line(80-z*10,0+z*20,240+z*10,0+z*20)
            self.canvas.create_line(80-z*10,320-z*20,240+z*10,320-z*20)

        self.canvas.create_line(80,0,240,0)

    def startSg(self):
        pass

    def startDb(self):
        # set up double player GUI
        self.canvas.delete(ALL)
        self.label['text'] = "Gomoku game: double player mode"
        self.canvas.bind("<Button-1>", self.dbPlayer)
        self._board()
        # set up game state
        self.game_state = [0 for x in range(217)] 
        # set up game turns
        self.turn = 0

    def sgPlayer(self):
        pass

    def dbPlayer(self):
        pass




# start main program
root = Tk()
app = gomoku(root)
root.mainloop()
