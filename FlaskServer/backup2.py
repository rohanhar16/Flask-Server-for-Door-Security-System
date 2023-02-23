import os
from flask import Flask, request, redirect, flash, url_for, render_template
from flask_wtf import FlaskForm
from wtforms import FileField
from flask_uploads import configure_uploads, IMAGES, UploadSet

# libraries for telegram post
import telegram
from telegram import Update, Bot
from telegram.ext import Updater, MessageHandler, Filters
from telegram.utils.request import Request
import paho.mqtt.client as mqtt

# libraries for face recognition
import cv2
from simple_facerec import SimpleFacerec

app = Flask(__name__)

photos = UploadSet("photos", IMAGES)
app.config["UPLOADED_PHOTOS_DEST"] = "static/img"
app.config["SECRET_KEY"] = os.urandom(24)
configure_uploads(app, photos)
count = 1
flag = False
# telegram bot
BOT_TOKEN = "6031744449:AAFoNG8OKbcDCCT2saMyVj6N5r2WoNnFeMk"
bot = telegram.Bot(BOT_TOKEN)

names = ["Elon Musk","Jeff Bezoz", "Ryan Renold", "Sergio Canu"]




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
        # filepath = os.getcwd().__str__()+"/static/img/capture%02d.jpeg" % count
        filepath = "/static/img/capture%02d.jpeg" % count
        f = open(filepath, "wb")
        # count = count + 1
        f.write(bytes(request.data))
        f.close()

        flag = True
        # return render_template('show_image.html', user_image=filepath)
        return f'{filepath}'
    return render_template('upload.html')


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=1066)
    print(flag)

    while(True):

