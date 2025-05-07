import numpy as np
import pypij
import cv2
import matplotlib.pyplot as plt

def encode_image(input_path, output_path, lossless_boxes, jpeg_quality=90, zlib_level=6):
    # Read input image
    img_bgr = cv2.imread(input_path)
    if img_bgr is None:
        raise FileNotFoundError(f"Could not read the input image file: {input_path}")
    
    encoded_data = pypij.encode(img_bgr, lossless_boxes, pad=5, 
                              jpeg_quality=jpeg_quality, zlib_level=zlib_level)

    # Save encoded data to a file
    with open(output_path, "wb") as f:
        print(f"Encoded data saved to {output_path}")
        f.write(encoded_data)

def decode_image(input_path, show_preview=True):
    if not input_path.endswith(".pij"):
        raise ValueError("Input file must be a .pij file")

    with open(input_path, "rb") as f:
        data = f.read()

    # pypij.decode returns a BGR‐ordered array
    img_bgr = pypij.decode(data)
    cv2.imwrite("debug_decode.png", img_bgr)
    
    img_rgb = cv2.cvtColor(img_bgr, cv2.COLOR_BGR2RGB)
    plt.figure(figsize=(12, 8))
    plt.imshow(img_rgb)
    plt.axis("off")
    plt.show()

    return img_bgr

if __name__ == "__main__":
    # Example usage
    lossless_boxes = [[577, 147, 770, 387]]
    # encode_image("input-csp.jpg", "output.pij", lossless_boxes)
    decoded_img = decode_image("output.pij")