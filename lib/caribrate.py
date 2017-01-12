import cv2
import params
import numpy as np
from scipy import ndimage
filtered_images = []

def nothing(x):
    pass
board = [[5, 2, 3, 4, 1, 1], [2, 5, 3, 4, 4, 5], [4, 1, 4, 5, 5, 2], [5, 4, 1, 3, 1, 5], [1, 4, 1, 1, 0, 5]]
color = [[(4,4)], [(0,4),(0,5),(2,1),(3,2),(3,4),(4,0),(4,2),(4,3)], [(0, 1),(1,0),(2,5)], [(0,2),(1,2),(3,3)], [(0,3),(1,3),(1,4),(2,0),(2,2),(3,1),(4,1)],[(0,0),(1,1),(1,5),(2,3),(2,4),(3,0),(3,5),(4,5)]]
hl = [0 for i in range(6)]
hh = [0 for i in range(6)]
sl = [0 for i in range(6)]
sh = [0 for i in range(6)]
vl = [0 for i in range(6)]
vh = [0 for i in range(6)]

def get_drop_type_count(x, y):
    global filtered_images
    sx = x * 50
    sy = y * 50
    color = board[y][x]
    return sum(1 for pix in filtered_images[color][sy+1:sy+49, sx+1:sx+49] if pix.any())
def draw_image(drops):
    img = np.zeros((250, 300, 3), np.uint8)
    for y in range(5):
        for x in range(6):
            img[y*50:(y+1)*50, x*50:(x+1)*50] = params.drop_color[drops[y][x]]['color']
    return img
    # cv2.imshow("result", img)
    # cv2.waitKey(0)
def caribrate(img):
    hsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)

    for k in range(6):
        for h in range(180):
            for length in range(180-h-1):
                
        lower_blue = np.array([params.drop_color[k]['h'][0], params.drop_color[k]['s'][0], params.drop_color[k]['v'][0]])
        upper_blue = np.array([params.drop_color[k]['h'][1], params.drop_color[k]['s'][1], params.drop_color[k]['v'][1]])
        mask = cv2.inRange(hsv,lower_blue, upper_blue)

        result = cv2.bitwise_and(img,img,mask = mask)
        kernel = np.ones((3,3),np.uint8)
        opening = cv2.morphologyEx(result, cv2.MORPH_OPEN, kernel)
        caribrate(opening, )
def main():
    global filtered_images
    # Creating a window for later use
    cv2.namedWindow('result')
    cv2.namedWindow('image')
    for i in range(len(params.drop_color)):
        cv2.namedWindow(params.drop_color[i]['name'])
    cap = cv2.VideoCapture(params.cam_number)
    Y = np.ones((256, 1), dtype= 'uint8') * 0
    for i in range(256):
        Y[i][0] = 255 * pow(float(i)/255.0, 1.0 / .9)
    while(cap.isOpened()):
    # img = cv2.imread('images/'+ str(i) + '.png')
        ret, frame = cap.read()
        # frame = cv2.imread('images/1.png')
        # img = cv2.resize(frame, (300, 400))
        # img = img[185:, 6:294]
        img = frame[82:299, 366:546]
        img = ndimage.rotate(img, -90, reshape=True)
        img = cv2.resize(img, (300, 250))
        img = cv2.LUT(img, Y)
        #converting to HSV
        hsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
        filtered_images = [None for i in range(6)]
        for k in range(len(params.drop_color)):
            lower_blue = np.array([params.drop_color[k]['h'][0], params.drop_color[k]['s'][0], params.drop_color[k]['v'][0]])
            upper_blue = np.array([params.drop_color[k]['h'][1], params.drop_color[k]['s'][1], params.drop_color[k]['v'][1]])
            mask = cv2.inRange(hsv,lower_blue, upper_blue)

            result = cv2.bitwise_and(img,img,mask = mask)
            kernel = np.ones((3,3),np.uint8)
            opening = cv2.morphologyEx(result, cv2.MORPH_OPEN, kernel)
            caribrate(opening, )

        drops = [['' for i in range(6)] for j in range(5)]
        for y in range(5):
            for x in range(6):
                drops[y][x] = get_drop_type(x, y)
        image = draw_image(drops)
        cv2.imshow("image", img)
        cv2.waitKey(100)
        cv2.imshow("result", image)
        cv2.waitKey(100)
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
