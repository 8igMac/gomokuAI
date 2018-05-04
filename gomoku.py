from tkinter import *
import math
import time

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

TIME_CONSTRAIN = 5.001 
STATE_FILE = 'state_27.txt'
MOVE_FILE = 'move_27.txt'

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
                                 height=5, bg='blue', fg='yellow', command=self.popup_chose_stone)
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


    def popup_chose_stone(self):
        self.popwin = Tk()
        popframe = Frame(self.popwin)
        popframe.pack()
        poplabel = Label(popframe, text='Chose white or black.', height=3)
        poplabel.pack()
        btn_black = Button(popframe, text='Black', fg='blue', command=lambda serv=0 : self.startSg(serv))
        btn_black.pack(side=LEFT, fill='both', expand=True)
        btn_white = Button(popframe, text='White', fg='blue', command=lambda serv=1 : self.startSg(serv))
        btn_white.pack(side=RIGHT, fill='both', expand=True)


    def startSg(self, serv):
        self.serv = serv
        self.popwin.destroy()
        # set up single player GUI
        self.canvas.delete(ALL)
        self.label['text'] = "Gomoku game: single player mode"
        self._board()
        # set up game state
        for item in self.game_state:
            item[1] = 0
        # set up game turns
        self.turn = 0
        # clean move_27.txt
        try:
            with open(MOVE_FILE,'w') as f:
                f.write('')
        except FileNotFoundError:
            pass

        self.canvas.bind("<Button-1>", self.sg_place_stone)

    def ai(self):
        input_line = ['ai']
        self.turn = self.turn+1
        ai_move = -1

        # write game state
        with open('state_27.txt', 'w') as f:
            f.write('{}\n'.format(self.turn))
            for item in self.game_state:
                if item[1] == 0:
                    f.write('0 ')
                elif item[1]%2 == self.turn%2: 
                    f.write('1 ')
                else:
                    f.write('2 ')
            f.write('\n-1')
        # clock start
        begin_time = time.monotonic()

        self.label['text'] = "AI is thinking..."
        print("AI is thinking...")

        # read ai move
        while input_line == [] or str(self.turn) != input_line[0]:
            try:
                with open('move_27.txt', 'r') as f:
                    input_line = f.readline().split()

                # exame ai move: time exam
                used_time = time.monotonic() - begin_time
                if used_time > TIME_CONSTRAIN:
                    self.times_up()
                    return
            except FileNotFoundError:
                continue
        # clock stop
        used_time = time.monotonic() - begin_time
        self.label['text'] = 'time take: {0:.5f} seconds'.format(used_time)
        print('time take: {0:.5f} seconds'.format(used_time)) 

        ai_move = int(input_line[1])
        # exame ai move: out of bound
        if ai_move< 0 or ai_move > 216:
            self.label['text'] = self.label['text'] + "\nai move out of bound! You win!"
            print("ai move out of bound! You win!")
            return

        # exame ai move: move already taken
        if self.game_state[ai_move][1] != 0:
            self.label['text'] = self.label['text'] + "\nai illegal move! You win!"
            print("ai illegal move! You win!")
            return

        # place ai move on board
        if self.turn%2 == 0:
            self.game_state[ai_move][1] = 2;
            self.canvas.create_oval(self.game_state[ai_move][0][0]-STONE_SIZE, self.game_state[ai_move][0][1]-STONE_SIZE, 
                                    self.game_state[ai_move][0][0]+STONE_SIZE, self.game_state[ai_move][0][1]+STONE_SIZE, fill='white')
        else:
            self.game_state[ai_move][1] = 1;
            self.canvas.create_oval(self.game_state[ai_move][0][0]-STONE_SIZE, self.game_state[ai_move][0][1]-STONE_SIZE, 
                                    self.game_state[ai_move][0][0]+STONE_SIZE, self.game_state[ai_move][0][1]+STONE_SIZE, fill='black')

        print('ai move: {}'.format(input_line[1]))
        self.check(ai_move)
        print("Your turn!")

    def sg_place_stone(self,event):
        if self.turn%2 == self.serv:
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
        else:
            self.ai()



    def startDb(self):
        # set up double player GUI
        self.canvas.delete(ALL)
        self.label['text'] = "Gomoku game: double player mode"
        self.canvas.bind("<Button-1>", self.db_place_stone)
        self._board()
        # set up game state
        for item in self.game_state:
            item[1] = 0
        # set up game turns
        self.turn = 0


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


    def db_place_stone(self, event):
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
        # check --
        in_a_row = 0
        dummy_state = [(self.game_state[idx][0][0]-VERTICE_GAP, self.game_state[idx][0][1]), self.game_state[idx][1]]

        while dummy_state in self.game_state:
            in_a_row = in_a_row + 1
            dummy_state = [(dummy_state[0][0]-VERTICE_GAP, dummy_state[0][1]), dummy_state[1]]
        dummy_state = [(self.game_state[idx][0][0]+VERTICE_GAP, self.game_state[idx][0][1]), self.game_state[idx][1]]
        while dummy_state in self.game_state:
            in_a_row = in_a_row + 1
            dummy_state = [(dummy_state[0][0]+VERTICE_GAP, dummy_state[0][1]), dummy_state[1]]

        if in_a_row == 4:
            # win
            if(self.game_state[idx][1] == 1):
                self.label['text'] = "Player black wins!"
                print("Player black wins!")
            else:
                self.label['text'] = "Player white wins!"
                print("Player white wins!")
            self.end()

        # check \
        in_a_row = 0
        dummy_state = [(self.game_state[idx][0][0]-HALF_VGAP, self.game_state[idx][0][1]-VERTICE_GAP), self.game_state[idx][1]]

        while dummy_state in self.game_state:
            in_a_row = in_a_row+1
            dummy_state = [(dummy_state[0][0]-HALF_VGAP, dummy_state[0][1]-VERTICE_GAP), dummy_state[1]]
        dummy_state = [(self.game_state[idx][0][0]+HALF_VGAP, self.game_state[idx][0][1]+VERTICE_GAP), self.game_state[idx][1]]
        while dummy_state in self.game_state:
            in_a_row = in_a_row+1
            dummy_state = [(dummy_state[0][0]+HALF_VGAP, dummy_state[0][1]+VERTICE_GAP), dummy_state[1]]

        if in_a_row == 4:
            # win
            if(self.game_state[idx][1] == 1):
                self.label['text'] = "Player black wins!"
                print("Player black wins!")
            else:
                self.label['text'] = "Player white wins!"
                print("Player white wins!")
            self.end()

        # check /
        in_a_row = 0
        dummy_state = [(self.game_state[idx][0][0]-HALF_VGAP, self.game_state[idx][0][1]+VERTICE_GAP), self.game_state[idx][1]]

        while dummy_state in self.game_state:
            in_a_row = in_a_row+1
            dummy_state = [(dummy_state[0][0]-HALF_VGAP, dummy_state[0][1]+VERTICE_GAP), dummy_state[1]]
        dummy_state = [(self.game_state[idx][0][0]+HALF_VGAP, self.game_state[idx][0][1]-VERTICE_GAP), self.game_state[idx][1]]
        while dummy_state in self.game_state:
            in_a_row = in_a_row+1
            dummy_state = [(dummy_state[0][0]+HALF_VGAP, dummy_state[0][1]-VERTICE_GAP), dummy_state[1]]

        if in_a_row == 4:
            # win
            if(self.game_state[idx][1] == 1):
                self.label['text'] = "Player black wins!"
                print("Player black wins!")
            else:
                self.label['text'] = "Player white wins!"
                print("Player white wins!")
            self.end()

        # check draw
        for i in range(NUM_VERTICE):
            if self.game_state[i][1] == 0:
                return
        self.label['text'] = "Draw!"
        self.end()


    def end(self):
        self.canvas.unbind("<Button-1>")


    def distance(self,x1,y1,x2,y2):
        return math.sqrt((x1-x2)**2 + (y1-y2)**2)

    def times_up(self):
        self.label['text'] = "ai time expired. You win!"
        print("ai time expired. You win!")




# start main program
root = Tk()
app = gomoku(root)
root.mainloop()
