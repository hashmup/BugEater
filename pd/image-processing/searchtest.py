import copy
import random
import heapq
import numpy as np
# constants
H = 5
W = 6
DEPTH_LIMIT = 6
BEAM = 1000
dx = [0, 1, 0, -1]
dy = [1, 0, -1, 0]

def inRange(x, y):
    return 0 <= x and x < W and 0 <= y and y < H

def markCombo(state):
    simulate_state = np.copy(state)
    # search raw wise
    for y in range(H):
        for x in range(W):
            length = 0
            while True:
                if not inRange(x+length, y) or (simulate_state[y][x] % 10 != simulate_state[y][x+length] % 10):
                    break
                length += 1
            if length >= 3:
                for k in range(x, x+length):
                    simulate_state[y][k] += 10
    # search column wise
    for x in range(W):
        for y in range(H):
            length = 0
            while True:
                if not inRange(x, y+length) or (simulate_state[y][x] % 10 != simulate_state[y+length][x] % 10):
                    break
                length += 1
            if length >= 3:
                for k in range(y, y+length):
                    simulate_state[k][x] += 10
    return simulate_state
def beam_search(start_state):
    print(markCombo(start_state))
