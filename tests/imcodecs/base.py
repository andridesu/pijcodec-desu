from abc import ABC, abstractmethod
from typing import List, Tuple
from PIL import Image
import numpy as np

class ImageCodec(ABC):
    @abstractmethod
    def encode(self, np_image: np.ndarray, boxes: List[Tuple[int, int, int, int]]) -> bytes:
        """Encode the image and return raw bytes of the container."""
        pass

    @abstractmethod
    def decode(self, buf: bytes) -> Image.Image:
        """Decode from raw container bytes and return a PIL Image."""
        pass