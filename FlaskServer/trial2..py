# libraries for face recognition
import cv2
from simple_facerec import SimpleFacerec

def face_recognition(path):
    sfr = SimpleFacerec()
    sfr.load_encoding_images("/home/christ-infotech-007/Rohan/ArduCam_FlaskWebServer/FlaskServer/images/")
    # Load Camera
    cap = cv2.imread(path)
    return sfr.detect_known_faces(cap)


face_location, face_names = face_recognition()
    print(face_names)