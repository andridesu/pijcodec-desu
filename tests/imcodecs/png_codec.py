import io
from typing import List, Tuple
from PIL import Image

import numpy as np

from imcodecs.base import ImageCodec

class PNGCodec(ImageCodec):
    def encode(self, np_image: np.ndarray, boxes: List[Tuple[int, int, int, int]]) -> bytes:        # simply save full image as PNG in memory
        bio = io.BytesIO()
        pil_image = Image.fromarray(np_image)
        pil_image.save(bio, format="PNG")
        return bio.getvalue()

    def decode(self, img_path) -> Image.Image:
        return Image.open(img_path)