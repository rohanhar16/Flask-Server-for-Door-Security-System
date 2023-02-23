<body>
	<h1>Flask Server for Door Security System</h1>
<p>This is a Flask web server that interfaces with an ArduCam camera module and performs facial recognition on captured images. The server is integrated with Telegram, MQTT, and OpenCV libraries for image processing and messaging capabilities. Once the door bell has been pressed, notification has been sent to telegram bot of the user “Someone is at the door” then camera module captures the snapshot, which has been sent to program which is used to recognised the face of the visitor, and then the snapshot of the visitor and the name of visitor (if it’s image is in database) will be sent to user’s telegram bot.</p>
  <h2>Dependencies</h2>
<ul>
	<li>Python 3.7+</li>
	<li>OpenCV</li>
	<li>Flask</li>
	<li>Flask-WTF</li>
	<li>Flask-Uploads</li>
	<li>Telegram API</li>
	<li>Paho MQTT</li>
	<li>SimpleFacerec</li>
</ul>

<h2>Circuit Diagram</h2>
<img src="/path/to/img.jpg" alt="Alt text" title="Optional title">

<h2>Getting Started</h2>
<ol>
	<li>Clone this repository.</li>
	<li>Set up a Telegram bot and obtain its API token. Replace the BOT_TOKEN variable in main.py with your token.</li>
	<li>Modify the names list in app.py with the names of the people you want to recognize.</li>
	<li>Start the Flask server using python main.py.</li>
	<li>Open your web browser and navigate to http://localhost:1066.</li>
	<li>Upload an image of a person in front of the door to trigger face recognition.</li>
</ol>

<h2>Component required</h2>
<ol>
<li>Hardware IDE</li>
  <ul>
    <li>WIZnet W5100S-EVB-Pico</li>
    <li>Arducam Mini 2MP Plus – OV2640 SPI Camera Module</li>
    <li>Push button</li>
    <li>Jumper wires</li>
    <li>Tripod stand</li>
 </ul>
<li>Software IDE</li>
<ul>
  <li>WIZnet W5100S-EVB-Pico</li>
  <li>Arducam Mini 2MP Plus – OV2640 SPI Camera Module</li>
  <li>Push button</li>
  <li>Jumper wires</li>
  <li>Tripod stand</li>
 </ul>
</ol>

<h2>Methodology</h2>
  <p>The program is designed to implement a home security system using Flask web framework, OpenCV for face recognition, Telegram API for notification and MQTT protocol for communication between devices. The program contains two functions, face_recognition() and mesaage_handler(), and a Flask web application. The face_recognition() function is used for recognizing the faces in the images by using the OpenCV library. This function takes the path of the image file as input and returns the name of the person identified in the image. The mesaage_handler() function is used for handling incoming messages from Telegram. This function receives messages and sends them to a MQTT broker.
  The Flask web application allows users to upload images to the server. When an image is uploaded, the program detects if someone is at the door by checking the image for any known faces. If a known face is detected, the program sends a notification to the user through Telegram.To achieve this functionality, the program uses a separate thread for continuously checking if an image has been uploaded, and if so, runs the face recognition algorithm on the image. If a face is detected, a notification is sent through Telegram.</p>

<h2>Code Explaination</h2>
<ol>
  <li>Pyhton code</li>
    <p>Python script that uses Flask web framework and OpenCV for face recognition. It also uses Telegram Bot API and MQTT protocol for communication. The script starts a web server that accepts image uploads and performs face recognition on them. If a face is detected, the script sends a message to a Telegram chat with the person's name. Additionally, the script sends a message over MQTT to a subscribed client. The script also has a message handler for Telegram messages and a function for running the face recognition process in a separate thread.</p>
  <li>Arduino code</li>
    <p>This code captures an image using an ArduCAM OV2640 camera and sends it to an HTTP server using an Ethernet shield. The captured image is stored in a buffer called img_buf and the function http_postData is responsible for sending the data to the server via HTTP POST. The server address is defined in the server variable and the port in the port variable. The function ethernet_transfer is a helper function used by http_postData to send the image data in chunks of 1024 bytes. The HTTP POST request includes a header specifying the size and type of the image being sent. The code includes a button connected to pin 6 and a buzzer connected to pin 8, but these are not used in the image capture and transfer process.</p>
</ol>

<h2>Additional Notes</h2>
<ul>
	<li>When a face is detected, a Telegram message is sent to the user with the name of the recognized person (if found).</li>
  <li>This program also supports sending and receiving MQTT messages. You can modify the mesaage_handler function in app.py to handle incoming messages as needed.</li>
  </ul>

<h2>Reference</h2>
<ul>
	<li>Whttps://www.hackster.io/innovation4x/webcamserver-arducam-ov2640-module-wiznet-evb-pico-73ceae</li>
  <li>https://api.telegram.org/bot6031744449:AAFoNG8OKbcDCCT2saMyVj6N5r2WoNnFeMk/getUpdates</li>
  <li>https://www.geeksforgeeks.org/send-message-to-telegram-user-using-python/</li>
  <li>https://mqtt.org/</li>
</ul>
