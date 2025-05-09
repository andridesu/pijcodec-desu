import os
import numpy as np
from PIL import Image
from snapsafe.utils.secret_utils import generate_key_iv
from snapsafe.image_protection import face_protect, face_reveal
from snapsafe.cryptography.params_provider.aes_params_provider import AESParamsProvider

from imcodecs.png_codec import PNGCodec
from imcodecs.jpeg_codec import JPEGCodec
from imcodecs.pij_codec import PIJCodec

def main():
    generate_key_iv("test_key.key", 32, "test_iv.key", 16)        

    aes_provider = AESParamsProvider()
    img_path = "/media/engineering/projects/andridesu/snapsafe-server-desu/server/tests/test_input/ai_generated/g_medium.jpg"
    input_img_np = np.array(Image.open(img_path))
    bbox, metrics = face_protect(input_img_np, aes_provider, False)

    codecs = {
        'pij': PIJCodec(),
        'png': PNGCodec(),
        'jpg': JPEGCodec(quality=85),
    }

    os.makedirs('output', exist_ok=True)

    for fmt, codec in codecs.items():
        # Encode
        buf = codec.encode(input_img_np, bbox)
        output_path = f"output/protected.{fmt}"
        with open(output_path, "wb") as f:
            f.write(buf)
        file_size = os.path.getsize(output_path)
        print(f"Protected image written to: {output_path} ({file_size} bytes)")

        # Decode
        decoded = codec.decode(output_path)
        decoded_np = np.array(decoded)
        
        # Save decoded image
        decoded_img = Image.fromarray(decoded_np)
        out_decoded = f"output/decoded_{fmt}.jpg"
        decoded_img.save(out_decoded)
        print(f"Wrote decoded image: {out_decoded}")

        # Reveal face
        face_reveal(decoded_np, bbox, aes_provider)
        revealed = Image.fromarray(decoded_np)
        out_revealed = f"output/revealed_{fmt}.jpg"
        revealed.save(out_revealed)
        print(f"Wrote revealed image: {out_revealed}")


if __name__ == "__main__":
    main()