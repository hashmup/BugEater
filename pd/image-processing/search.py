import copy
import random
import board
import heapq
import cv2
import numpy as np
# constants
H = 5
W = 6
DEPTH_LIMIT = 30
PROCESS_NUM = 6
BEAM = 10000
dx = [0, 1, 0, -1]
dy = [1, 0, -1, 0]
f = False
class State:
    def __init__(self, state, curx, cury, depth, route):
        self.state = np.copy(state)
        self.curx = curx
        self.cury = cury
        self.depth = depth
        self.route = copy.copy(route)
        self.score = -1
        self.drop_score = [0 for i in range(6)] # count the score for each types
        self.mark = []
    def __hash__(self):
        return hash(hashable(self.state))
    def __eq__(self, other):
        self.state == other.state
    def copy(self):
        return copy.deepcopy(self)
    def getScore(self):
        # base 1
        # number of drops in one combo -> * (1 + 0.25 * (# of drops - 3))
        # number of combos -> * (1 + 0.25 * (# of combos - 1))
        # total =  {for each combo (1 * (1 + 0.25 * (# of drops -3)))}* (1 + 0.25 * (# of combos -1))
        if self.score != -1:
            return self.score
        self.combo_num = self.simulate()
        self.score = sum(self.drop_score) * (1 + 0.25 * (self.combo_num - 1))
        if self.combo_num == 0:
            self.score = 0
        return self.score
    def getAllNextStates(self):
        next_states = []
        for i in range(4):
            nx, ny = self.curx + dx[i], self.cury + dy[i]
            if not inRange(nx, ny):
                continue
            self.state[self.cury][self.curx], self.state[ny][nx] = self.state[ny][nx], self.state[self.cury][self.curx]
            self.route.append((nx, ny))
            next_states.append(State(self.state, nx, ny, self.depth + 1, self.route))
            self.route.pop()
            self.state[self.cury][self.curx], self.state[ny][nx] = self.state[ny][nx], self.state[self.cury][self.curx]
        return next_states
    def simulate(self):
        self.simulate_state = np.copy(self.state)
        combo_num = 0
        while True:
            tmp = self.simulateDrops()
            combo_num += tmp
            if not tmp:
                break
        return combo_num
    def simulateDrops(self):
        self.markCombo()
        combo_num = self.deleteDrop()
        self.fillDrops()
        return combo_num
    def fillDrops(self):
        # shift drops to empty space
        for x in range(W):
            pos1 = H-1
            while True:
                if not pos1:
                    break
                if self.simulate_state[pos1][x] == -1:
                    pos2 = pos1 - 1
                    while True:
                        if pos2 < 0:
                            break
                        if self.simulate_state[pos2][x] != -1:
                            self.simulate_state[pos1][x], self.simulate_state[pos2][x] = self.simulate_state[pos2][x], self.simulate_state[pos1][x]
                            break
                        else:
                            pos2 -= 1
                    if pos2 == -1:
                        break
                else:
                    pos1 -= 1
        # fill empty space
        # for x in range(W):
        #     for y in range(H):
        #         if self.simulate_state[y][x] == -1:
        #             self.simulate_state[y][x] = random.random() % 6
    def markCombo(self):
        # search raw wise
        for i in range(H):
            for j in range(W):
                length = 0
                if self.simulate_state[i][j] == -1:
                    continue
                while True:
                    if not inRange(j+length, i) or (self.simulate_state[i][j] % 10 != self.simulate_state[i][j+length] % 10):
                        break
                    length += 1
                if length >= 3:
                    for k in range(j, j+length):
                        self.simulate_state[i][k] += 10
        # search column wise
        for i in range(W):
            for j in range(H):
                length = 0
                if self.simulate_state[j][i] == -1:
                    continue
                while True:
                    if not inRange(i, j+length) or (self.simulate_state[j][i] % 10 != self.simulate_state[j+length][i] % 10):
                        break
                    length += 1
                if length >= 3:
                    for k in range(j, j+length):
                        self.simulate_state[k][i] += 10
    def deleteDrop(self):
        combo_num = 0
        for i in range(W):
            for j in range(H):
                if self.simulate_state[j][i] >= 10:
                    dtype = self.simulate_state[j][i] % 10
                    self.count = 0
                    self.bfs(i, j, dtype)
                    self.drop_score[dtype] += 1 + 0.25 * (self.count - 3)
                    combo_num += 1
        return combo_num


    def bfs(self, x, y, v):
        self.simulate_state[y][x] = -1
        self.count += 1
        for i in range(4):
            nx, ny = x + dx[i], y + dy[i]
            if not inRange(nx, ny):
                continue
            if self.simulate_state[ny][nx] != -1 and self.simulate_state[ny][nx] >= 10 and self.simulate_state[ny][nx] % 10 == v:
                self.bfs(nx, ny, v)
    def visualizeRoute(self, start_state):
        cv2.namedWindow("path")
        copy_state = np.copy(start_state)
        for i in range(len(self.route)):
            pos = self.route[i]
            if i > 0:
                pos2 = self.route[i-1]
                copy_state[pos[1]][pos[0]], copy_state[pos2[1]][pos2[0]] = copy_state[pos2[1]][pos2[0]], copy_state[pos[1]][pos[0]]
            bd = board.getBoard(copy_state)
            cv2.circle(bd, getCenter(pos), 3, (0, 0, 0))
            cv2.imshow("path", bd)
            cv2.waitKey(1000)
        self.simulate_state = copy_state
        while True:
            self.markCombo()
            combo_num = self.deleteDrop()
            bd = board.getBoard(copy_state)
            cv2.circle(bd, getCenter(pos), 3, (0, 0, 0))
            cv2.imshow("path", bd)
            cv2.waitKey(2000)
            self.fillDrops()
            bd = board.getBoard(copy_state)
            cv2.circle(bd, getCenter(pos), 3, (0, 0, 0))
            cv2.imshow("path", bd)
            cv2.waitKey(2000)
            if not combo_num:
                break
def hashable(state):
    return tuple(map(tuple, state))
def getCenter(pos):
    # 300 * 250
    return (pos[0] * 50 + 25, pos[1] * 50 + 25)
def inRange(x, y):
    return 0 <= x and x < W and 0 <= y and y < H

def beam_search(start_state):
        # initialize states and create threads
        current_states = []
        visited_states = set()
        good_states = []
        for x in range(W):
            for y in range(H):
                state = State(start_state, x, y, 0, [(x, y)])
                heapq.heappush(current_states, (-state.getScore(), state))
        for t in range(DEPTH_LIMIT):
            print(t)
            next_states = []
            for r in range(BEAM):
                if not current_states:
                    break
                cur_score, cur_state = heapq.heappop(current_states)
                heapq.heappush(good_states, (cur_score, cur_state))
                _next_states = cur_state.getAllNextStates()
                for state in _next_states:
                    if state not in visited_states:
                        visited_states.add(state)
                    heapq.heappush(next_states, (-state.getScore(), state))
            current_states = next_states
        best_state = heapq.heappop(good_states)[1]
        return best_state
