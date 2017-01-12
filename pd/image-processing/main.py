import numpy as np
import search
# import searchtest as search

#state = np.loadtxt("example.txt").astype(int)
state = np.random.randint(6, size=(5,6))
print(state)
bstate = search.beam_search(state)
print(bstate.state)
print(bstate.route)
print(bstate.score)
print(bstate.combo_num)
# print(bstate.simulate_state)
# for x in bstate.mark:
#     print(x)
bstate.visualizeRoute(state)
