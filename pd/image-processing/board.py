import numpy as np
import params
def getBoard(state):
    img = np.zeros((250, 300, 3), np.uint8)
    for y in range(5):
        for x in range(6):
            if state[y][x] == -1:
                img[y*50:(y+1)*50, x*50:(x+1)*50] = (0, 0, 0)
            else:
                img[y*50:(y+1)*50, x*50:(x+1)*50] = params.drop_color[state[y][x]]['color']
    return img
