# -*- coding: utf-8 -*-
"""
Created on Wed Apr 16 16:27:51 2025

@author: user

import numpy as np
import matplotlib.pyplot as plt
import cv2
image = cv2.imread('lenna2.png')
h,w = image.shape[:2]

cv2.waitKey(0)
cv2.detroyAllwindows()
cv2.imshow('LENNA IMAGE', image)
RGB_image = cv2.cvtColor(image.cv2.COLOR_BGR2RGB)
plt.imshow(RGB_image)
*/





import cv2
import matplotlib.pyplot as plt

gray_img = cv2.imread('lenna2.png', cv2.IMREAD_GRAYSCALE)

status = cv2.imwrite('lenna2_gray.png', gray_im
img_rgb = cv2.cvtColor(gray_img, cv2.COLOR_GRAY2RGB
plt.imshow(img_rgb)
plt.axis('off')  # Hide axes
plt.show()

print("Image written to file system:", status)


import cv2 as cv
import matplotlib.pyplot as plt
image = cv.imread("lenna2.png")
b, g, r = cv.split(image)
fig, ax = plt.subplots(1, 3, figsize=(10, 8))
ax[0].imshow(cv.cvtColor(r, cv.COLOR_BGR2RGB))
ax[0].set_title("Red")


ax[1].imshow(cv.cvtColor(g, cv.COLOR_BGR2RGB))
ax[1].set_title("Green")

ax[2].imshow(cv.cvtColor(b, cv.COLOR_BGR2RGB))
ax[2].set_title("Blue")


reconstructed = cv.merge((b,g,r))
cv.imshow("original", image)
cv.imshow("Reconstructed", reconstructed)
cv.waitKey(0)
cv.destroyAllWindows()




import cv2 
import numpy as np

image = cv2.imread("C:/Users/user/.spyder-py3/lenna2.png")
identity_matrix = np.eye(3)[:2]

transformed = cv2.warpAffine(image,identity_matrix,(image.shape[1],image.shape[0]))
cv2.imshow("original", image)
cv2.imshow("Trasformed", transformed)
cv2.waitKey(0)
cv2.destroyAllWindows()





import cv2 
import numpy as np
image = cv2.imread("asme.jpeg")
c = 255 / np.log(1 + np.max(image))
log_transformed = c * np.log(1 + image.astype(np.float32))

log_transformed = np.uint8(log_transformed)
status = cv2.imwrite("log_transformed.png", log_transformed)

print("image writteing status", status)
cv2.imshow("original", image)
cv2.imshow("log transformed", log_transformed)

cv2.waitKey(0)
cv2.destroyAllWindows()



import cv2
import numpy as np

# Assume img = log transformed image
img = cv2.imread('log_transformed.png', 0)

# Use the same 'c' value as in log transform (assumed here)
c = 255 / np.log(1 + 255)

# Apply inverse log
inverse_log = np.exp(img / c) - 1

# Normalize to 0-255 and convert to uint8
inverse_log = np.uint8(np.clip(inverse_log, 0, 255))

# Display
cv2.imshow("Dark Original Image", img)
cv2.imshow("Inverse Log Result", inverse_log)
cv2.waitKey(0)
cv2.destroyAllWindows()
"""



import cv2
import numpy as np
gray_image = cv2.imread("asme.jpeg")

min_val =np.min(gray_image)
max_val =np.max(gray_image)

lower_limit = 0
upper_limit = 255


contrast_stretched_image = (gray_image - min_val) * ((upper_limit - lower_limit) / (max_val - min_val)) + lower_limit

contrast_stretched_image = contrast_stretched_image.astype(np.uint8)

cv2.imshow('Original Image', gray_image)
cv2.imshow('Contrast Stretched Image', contrast_stretched_image)
cv2.waitKey(0)
cv2.destroyAllWindows()