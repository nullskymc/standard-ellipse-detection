import cv2
import glob
import os

output_dir = "/mnt/m/scientificProject/output/"
image_files = glob.glob(os.path.join(output_dir, "output_*.jpg"))

for image_file in image_files:
    image = cv2.imread(image_file)
    if image is None:
        print(f"Error: Could not read image from {image_file}")
        continue
    cv2.imshow("Detected Ellipses", image)
    cv2.waitKey(0)

cv2.destroyAllWindows()
