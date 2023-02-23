# import telegram
# from telegram import Update, Bot
# from telegram.ext import Updater, MessageHandler, Filters
# from telegram.utils.request import Request
# import paho.mqtt.client as mqtt
# import requests
#
# MQTT_HOST = '54.87.92.106'
# MQTT_PORT = 1883
# BOT_TOKEN = "6031744449:AAFoNG8OKbcDCCT2saMyVj6N5r2WoNnFeMk"
#
# #telegram bot
# bot = telegram.Bot(BOT_TOKEN)
#
# def mesaage_handler(update, context):
#     print(update.message.text)
#     second_client = mqtt.Client()
#     second_client.connect(MQTT_HOST, MQTT_PORT)
#     second_client.publish("home/screen", update.message.text, qos=0, retain=True)
#     second_client.subscribe("home/screen")
#     pass
#
#
# def on_connect(client, userdata, flags, rc):
#     print("Connected with result code " + str(rc))
#     client.subscribe("home/button")
#     pass
#
#
# def on_message(client, userdata, msg):
#     # print("sun")
#     print(msg.topic + " " + str(msg.payload.decode()))
#     message = msg.payload.decode()
#     bot.send_message(chat_id = "1180645061", text = message)
#     pass
#
#
# # def telegram_bot(message):
# #     bot_token = BOT_TOKEN
# #     chatID = "1180645061"
# #     send_text = "https://api.telegram.org/bot" + bot_token + "/sendMessage?chat_id=" + chatID + \
# #                 "/&text=" + message
# #     response = requests.get(send_text)
# #     return response.json()
# #
# #
# # def sub(message):
# #     telegram_bot(message)
#
#
# def main():
#     req = Request(connect_timeout=5)
#     t_bot = Bot(request=req, token=BOT_TOKEN)
#     updater = Updater(bot=t_bot, use_context=True)
#     dp = updater.dispatcher
#     dp.add_handler(MessageHandler(filters=Filters.all, callback=mesaage_handler))
#     updater.start_polling()
#
#     client = mqtt.Client()
#     client.connect(MQTT_HOST, MQTT_PORT)
#     client.on_connect = on_connect
#     client.on_message = on_message
#     client.loop_start()
#
# if __name__ == '__main__':
#     main()
