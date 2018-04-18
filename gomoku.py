from tkinter import *

# global parameter
GAP = 10 # gap between board and boarder of the frame
VERTICE_GAPE = 20 # gap between vertices
NUM_VERTICE = 217 # total number of vertices
WIDTH = 16 * VERTICE_GAPE + 2*GAP # game board width
HIEGHT = 16 * VERTICE_GAPE + 2*GAP # game board height

class gomoku:

    def __init__(self, master):
        # start main frame
        self.frame = Frame(master)
        self.frame.pack(fill='both', expand=True)

        # start a canvas
        self.canvas = Canvas(self.frame, width=WIDTH, height=HIEGHT)
        self.canvas.pack(fill='both', expand=True)

        # create a label
        self.label = Label(self.frame, text="Gomoku game", height=5, bg="black", fg="white")
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
            self.canvas.create_line(80+x*20+GAP,0+GAP,x*10+GAP,160+x*20+GAP)
            self.canvas.create_line(320-(80+x*20)+GAP,320+GAP,320-x*10+GAP,160-x*20+GAP)
        for y in range(9):
            self.canvas.create_line(80+y*20+GAP,0+GAP,240+y*10+GAP,320-y*20+GAP)
            self.canvas.create_line(80-y*10+GAP,0+y*20+GAP,240-y*20+GAP,320+GAP)
        for z in range(9):
            self.canvas.create_line(80-z*10+GAP,0+z*20+GAP,240+z*10+GAP,0+z*20+GAP)
            self.canvas.create_line(80-z*10+GAP,320-z*20+GAP,240+z*10+GAP,320-z*20+GAP)

    def startSg(self):
        pass

    def startDb(self):
        # set up double player GUI
        self.canvas.delete(ALL)
        self.label['text'] = "Gomoku game: double player mode"
        self.canvas.bind("<Button-1>", self.dbPlayer)
        self._board()
        # set up game state
        self.game_state = [0 for x in range(NUM_VERTICE)] 
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
