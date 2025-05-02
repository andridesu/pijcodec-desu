import numpy as np
import pypij
import cv2

# Read input image
input_image = cv2.imread("input.jpg")
if input_image is None:
    raise FileNotFoundError("Could not read the input image file")

# Ensure image is in BGR format (OpenCV default)
input_image = cv2.cvtColor(input_image, cv2.COLOR_BGR2RGB)

# Define lossless boxes
lossless_boxes = [[10, 10, 250, 250]]

# Encode the image
encoded_data = pypij.encode(input_image, lossless_boxes, pad=5, jpeg_quality=90, zlib_level=6)

# Save encoded data to a file
with open("output.pij", "wb") as f:
    print("Encoded data saved to output.pij")
    f.write(encoded_data)

# Read encoded data from file
with open("output.pij", "rb") as f:
    loaded_data = f.read()

# Decode the image from loaded data
decoded_image = pypij.decode(loaded_data)

# Convert back to BGR for saving
decoded_image = cv2.cvtColor(decoded_image, cv2.COLOR_RGB2BGR)

# Save the decoded image
cv2.imwrite("decoded_image.png", decoded_image)