from tkinter import *
import math

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

        # init game state as [(coord.x, coord.y), state]
        self.game_state = []
        for i in range(17):
            if i < 9:
                for j in range(9+i):
                    self.game_state.append([(80+GAP+j*20-i*10,0+GAP+i*20), 0])
            else:
                for j in range(17-(i-8)):
                    self.game_state.append([(10+GAP+j*20+(i-9)*10,0+GAP+i*20), 0])

    def _board(self):
        # draw hexigon game board
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
        for item in self.game_state:
            item[1] = 0
        # set up game turns
        self.turn = 0

    def sgPlayer(self, event):
        pass

    def dbPlayer(self, event):
        for idx in range(len(self.game_state)):
            # find closest vertice
            if self.distance(event.x,event.y,self.game_state[idx][0][0], self.game_state[idx][0][1]) < 10:
                # if the vertice never played before
                if self.game_state[idx][1] == 0:
                    self.turn = self.turn+1
                    # first serv: black
                    if self.turn%2 == 1: 
                        # update game state table
                        self.game_state[idx][1] = 1
                        self.canvas.create_oval(self.game_state[idx][0][0]-5, self.game_state[idx][0][1]-5, 
                                                self.game_state[idx][0][0]+5, self.game_state[idx][0][1]+5, fill='black')
                    # second serv: white
                    else: 
                        # update game state table
                        self.game_state[idx][1] = 2
                        self.canvas.create_oval(self.game_state[idx][0][0]-5, self.game_state[idx][0][1]-5, 
                                                self.game_state[idx][0][0]+5, self.game_state[idx][0][1]+5, fill='white')
                    # check win condition
                    self.check(idx)
                break


    def check(self, idx):
        # check /

        # check \
        # check --
        pass

    def distance(self,x1,y1,x2,y2):
        return math.sqrt((x1-x2)**2 + (y1-y2)**2)




# start main program
root = Tk()
app = gomoku(root)
root.mainloop()
