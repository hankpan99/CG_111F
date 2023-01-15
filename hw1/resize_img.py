import cv2
import numpy as np

image = cv2.imread('./texture_file/shield.jpg')
# image = image[:, 10:360, :]
print(image.shape)
image = cv2.resize(image, (100, 100), interpolation=cv2.INTER_AREA)

# cv2.imshow('My Image', image)
# cv2.waitKey(0)
# cv2.destroyAllWindows()

cv2.imwrite('./texture_file/shield.bmp', image)