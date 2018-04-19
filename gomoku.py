from tkinter import *
import math

# global parameter
GAP = 10 # gap between board and boarder of the frame
VERTICE_GAP = 28 # gap between vertices (even number only!!!!)
HALF_VGAP = (int)(VERTICE_GAP/2) # half of vertice gap

BRD_START = (int)(VERTICE_GAP*4) # game board start coordinate
BRD_END = BRD_START*4 # game board end coordinate

NUM_VERTICE = 217 # total number of vertices
STONE_SIZE = (int)(VERTICE_GAP*2/5)

WIDTH = 16 * VERTICE_GAP + 2*GAP # game board width
HIEGHT = 16 * VERTICE_GAP + 2*GAP # game board height


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
                    self.game_state.append([(BRD_START+GAP+j*VERTICE_GAP-i*HALF_VGAP,0+GAP+i*VERTICE_GAP), 0])
            else:
                for j in range(17-(i-8)):
                    self.game_state.append([(HALF_VGAP+GAP+j*VERTICE_GAP+(i-9)*HALF_VGAP,0+GAP+i*VERTICE_GAP), 0])

    def _board(self):
        # draw hexigon game board
        for x in range(9):
            self.canvas.create_line(BRD_START+x*VERTICE_GAP+GAP,0+GAP,x*HALF_VGAP+GAP,BRD_START*2+x*VERTICE_GAP+GAP)
            self.canvas.create_line(BRD_END-(BRD_START+x*VERTICE_GAP)+GAP,BRD_END+GAP,BRD_END-x*HALF_VGAP+GAP,BRD_START*2-x*VERTICE_GAP+GAP)
        for y in range(9):
            self.canvas.create_line(BRD_START+y*VERTICE_GAP+GAP,0+GAP,BRD_START*3+y*HALF_VGAP+GAP,BRD_END-y*VERTICE_GAP+GAP)
            self.canvas.create_line(BRD_START-y*HALF_VGAP+GAP,0+y*VERTICE_GAP+GAP,BRD_START*3-y*VERTICE_GAP+GAP,BRD_END+GAP)
        for z in range(9):
            self.canvas.create_line(BRD_START-z*HALF_VGAP+GAP,0+z*VERTICE_GAP+GAP,BRD_START*3+z*HALF_VGAP+GAP,0+z*VERTICE_GAP+GAP)
            self.canvas.create_line(BRD_START-z*HALF_VGAP+GAP,BRD_END-z*VERTICE_GAP+GAP,BRD_START*3+z*HALF_VGAP+GAP,BRD_END-z*VERTICE_GAP+GAP)

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
            if self.distance(event.x,event.y,self.game_state[idx][0][0], self.game_state[idx][0][1]) < HALF_VGAP:
                # if the vertice never played before
                if self.game_state[idx][1] == 0:
                    self.turn = self.turn+1
                    # first serv: black
                    if self.turn%2 == 1: 
                        # update game state table
                        self.game_state[idx][1] = 1
                        self.canvas.create_oval(self.game_state[idx][0][0]-STONE_SIZE, self.game_state[idx][0][1]-STONE_SIZE, 
                                                self.game_state[idx][0][0]+STONE_SIZE, self.game_state[idx][0][1]+STONE_SIZE, fill='black')
                    # second serv: white
                    else: 
                        # update game state table
                        self.game_state[idx][1] = 2
                        self.canvas.create_oval(self.game_state[idx][0][0]-STONE_SIZE, self.game_state[idx][0][1]-STONE_SIZE, 
                                                self.game_state[idx][0][0]+STONE_SIZE, self.game_state[idx][0][1]+STONE_SIZE, fill='white')
                    # check win condition
                    self.check(idx)
                break


    def check(self, idx):
        var = idx-1
        in_a_row = 0

        # check --
        while var > 0 and self.game_state[var][0][1] == self.game_state[idx][0][1] and \
                self.game_state[var][1] == self.game_state[idx][1]: 
            in_a_row = in_a_row + 1
            var = var-1
        var = idx+1
        while var <= NUM_VERTICE-1 and self.game_state[var][0][1] == self.game_state[idx][0][1] and \
                self.game_state[var][1] == self.game_state[idx][1]:
            in_a_row = in_a_row + 1
            var = var+1
        if in_a_row == 4:
            # win
            if(self.game_state[idx][1] == 1):
                self.label['text'] = "Player black wins!"
            else:
                self.label['text'] = "Player white wins!"
            self.end()
        else:
            in_a_row = 0

        # check /

        # check \
        pass

    def end(self):
        self.canvas.unbind("<Button-1>")

    def distance(self,x1,y1,x2,y2):
        return math.sqrt((x1-x2)**2 + (y1-y2)**2)




# start main program
root = Tk()
app = gomoku(root)
root.mainloop()
