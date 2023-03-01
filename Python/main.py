import os
import time
from flask import Flask, request, render_template
from flask_uploads import configure_uploads, IMAGES, UploadSet
import telegram
from telegram import Bot
from telegram.ext import Updater, MessageHandler, Filters
from telegram.utils.request import Request
import paho.mqtt.client as mqtt
import cv2
from Python.simple_facerec import SimpleFacerec
import threading

BOT_TOKEN = " "
bot = telegram.Bot(BOT_TOKEN)

app = Flask(__name__)

photos = UploadSet("photos", IMAGES)
app.config["UPLOADED_PHOTOS_DEST"] = "static/img"
app.config["SECRET_KEY"] = os.urandom(24)
configure_uploads(app, photos)
count = 1

names = ["Elon Musk", "Jeff Bezoz", "Ryan Renold", "Sergio Canu","Rohan Harchandani","Kim"]

flag = False

def face_recognition(path):
   sfr = SimpleFacerec()
   sfr.load_encoding_images("/home/christ-infotech-007/Rohan/ArduCam_FlaskWebServer/Python/images/")
   # Load Camera
   cap = cv2.imread(path)
   return sfr.detect_known_faces(cap)


def mesaage_handler(update, context):
   print(update.message.text)
   client = mqtt.Client()
   client.connect("54.87.92.106", 1883)
   client.publish("home/screen", update.message.text, qos=0, retain=True)
   client.subscribe("home/screen")
   pass


def func():
   global flag
   while True:
       time.sleep(0.1)
       if flag == True:
           # filepath = "/home/christ-infotech-007/Rohan/ArduCam_FlaskWebServer/Python/static/img/Rohan Harchandani.jpeg"
           filepath = "/home/christ-infotech-007/Rohan/ArduCam_FlaskWebServer/Python/static/img/capture01.jpeg"
           message = "Someone is at the door"
           link = "http://192.168.0.107:1066/" + filepath

           # print(face_names)
           try:
               bot.send_message(chat_id="1180645061", text=message)
               bot.send_photo(chat_id="1180645061", photo=open(filepath, 'rb'))
               face_location, face_names = face_recognition(filepath)

               if face_names:
                   for name in names:
                       if name == face_names[0]:
                           print(face_names)
                           bot.send_message(chat_id="1180645061", text=face_names[0] + " " + "visited")
                           break
               else:
                   bot.send_message(chat_id="1180645061", text="Face not found")
               flag = False
           except Exception as e:
               print("face not found")
               print(e)
               flag = False
           flag = False



@app.route("/", methods=['GET', 'POST'])
def upload():
   global count
   global flag
   """
   if request.method == 'POST' and 'photo' in request.files:
       filename = photos.save(request.files['photo'])
       filepath = os.path.join(app.config['UPLOADED_PHOTOS_DEST'], filename)
       print(filepath)
       #return render_template('show_image.html', user_image=filepath)
       return f'{filepath}'
   return render_template('upload.html')
   :return:
   """
   if request.method == 'POST' and 'image/jpeg' == request.content_type:
       filepath = "/home/christ-infotech-007/Rohan/ArduCam_FlaskWebServer/Python/static/img/capture%02d.jpeg" % count
       f = open(filepath, "wb")
       # count = count + 1
       f.write(bytes(request.data))
       f.close()
       # time.sleep(1)
       flag = True
       # print(filepath)
       # print("Hello")
       # return render_template('show_image.html', user_image=filepath)
       return f'{filepath}'


   return render_template('upload.html')




def main():
   req = Request(connect_timeout=5)
   t_bot = Bot(request=req, token=BOT_TOKEN)
   updater = Updater(bot=t_bot, use_context=True)
   dp = updater.dispatcher
   dp.add_handler(MessageHandler(filters=Filters.all, callback=mesaage_handler))
   updater.start_polling()


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
   process = threading.Thread(target=func, args=())
   process.start()
   main()
   app.run(host='0.0.0.0', port=1066)
