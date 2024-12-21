import sys
sys.path.append('/mnt/m/scientificProject/standard-ellipse-detection/python_bindings/build')

import cv2
import numpy as np
from detect_module import detectEllipse

def main(image_paths):
    output_dir = "/mnt/m/scientificProject/output/"
    for i, image_path in enumerate(image_paths):
        print(f"Trying to read image from: {image_path}")
        board = cv2.imread(image_path)
        if board is None:
            print(f"Error: Could not read image from {image_path}")
            continue
        image = cv2.cvtColor(board, cv2.COLOR_BGR2GRAY)
        
        # 添加调试信息
        print(f"Image shape: {image.shape}")
        print(f"First pixel value: {image[0, 0]}")

        # 调用 detectEllipse 函数
        ells = detectEllipse(image, image.shape[0], image.shape[1], 0, 2.0)
        print(f"Find {len(ells)} ellipse(s)")
        for ell in ells:
            print(f"coverangle : {ell['coverangle']},\tgoodness : {ell['goodness']},\tpolarity : {ell['polarity']}")
            cv2.ellipse(board,
                        (int(ell['o'][1]), int(ell['o'][0])),
                        (int(ell['a']), int(ell['b'])),
                        np.degrees(ell['phi']),
                        0,
                        360,
                        (0, 255, 0),
                        2,
                        8,
                        0)
        output_filename = f"{output_dir}output_{i}.jpg"
        cv2.imwrite(output_filename, board)
        print(f"Saved result to {output_filename}")
    print(f"Processing complete. Please check the output images in {output_dir}")

if __name__ == "__main__":
    if len(sys.argv) <= 1:
        print("[Usage]: testdetect.py [image_dir1] [image_dir2] [image_dir3] ...")
    else:
        main(sys.argv[1:])