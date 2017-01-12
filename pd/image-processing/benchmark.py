import numpy as np
import search
import time

def main():
    _cnt = 0
    _time = 0.0
    _score = 0.0
    _combo = 0
    while True:
        start = time.time()
        state = np.random.randint(6, size=(5, 6))
        print(state)
        bstate = search.beam_search(state)
        _cnt += 1
        _time += time.time() - start
        _score += bstate.score
        _combo += bstate.combo_num
        bstate.visualizeRoute(state)

        print("time: {0}".format(_time / _cnt))
        print("score: {0}".format(_score / _cnt))
        print("combo: {0}".format(_combo / _cnt))

if __name__ == "__main__":
    main()
