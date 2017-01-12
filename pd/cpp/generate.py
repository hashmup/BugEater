import numpy as np

def main():
    board = np.random.randint(1, 7, size=(5, 6))
    np.savetxt('sample.dat', board, fmt='%d')

if __name__ == '__main__':
    main()
