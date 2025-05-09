import io
import pypij
import numpy as np
from PIL import Image
from typing import List, Tuple

from imcodecs.base import ImageCodec

class PIJCodec(ImageCodec):
    def encode(self, np_image: np.ndarray, boxes: List[Tuple[int, int, int, int]]) -> bytes:        # simply save full image as PNG in memory
        encoded_image = pypij.encode(np_image, boxes)
        return encoded_image

    def decode(self, img_path) -> Image.Image:
        with open(img_path, "rb") as f:
            encoded_data = f.read()

        # Decode the encrypted image data
        decoded_img_bgr = pypij.decode(encoded_data)

        return Image.fromarray(decoded_img_bgr)

