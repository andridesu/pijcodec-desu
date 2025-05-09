import io
import numpy as np
from PIL import Image
from typing import List, Tuple

from imcodecs.base import ImageCodec

class JPEGCodec(ImageCodec):
    def __init__(self, quality: int = 85):
        self.quality = quality

    def encode(self, np_image: np.ndarray, boxes: List[Tuple[int, int, int, int]]) -> bytes:
        bio = io.BytesIO()
        pil_image = Image.fromarray(np_image)
        pil_image.save(bio, format="JPEG", quality=self.quality)        
        return bio.getvalue()

    def decode(self, img_path) -> Image.Image:
        return Image.open(img_path)