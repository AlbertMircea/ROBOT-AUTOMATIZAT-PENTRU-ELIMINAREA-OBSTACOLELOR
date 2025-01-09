
import time
import board
import adafruit_dht
from azure.iot.device import IoTHubDeviceClient, Message
from flask import Flask, render_template, request
import serial
import threading
import queue
import signal

# Serial communication setup
ser = serial.Serial("/dev/ttyACM0", 9600, timeout=1)
ser.setDTR(False)
time.sleep(1)
ser.flushInput()
ser.setDTR(True)
time.sleep(2)

# DHT sensor setup
dht_device = adafruit_dht.DHT11(board.D4)  # Use board.D4 for GPIO4

# Azure IoT Hub connection string
CONNECTION_STRING = "HostName=RaspberryIOTCACTHub.azure-devices.net;DeviceId=RaspberryPI;SharedAccessKey=7gfz5PYHMrWLCs+f5gnhOBNxqIqkGSr/SCq5sVbPhNs="
MSG_SND = '{{"temperature": {temperature},"humidity": {humidity},"distance": {distance},"white_line_detected": {detected}}}'

# Flask setup
app = Flask(__name__)

# Command queue
command_queue = queue.Queue()

# Initialize IoT Hub client
def iothub_client_init():
    client = IoTHubDeviceClient.create_from_connection_string(CONNECTION_STRING)
    return client

# Serial communication handler
def serial_handler(client):
    while True:
        try:
            # Process commands from the queue
            if not command_queue.empty():
                command = command_queue.get()
                ser.write(f"{command}\n".encode())  # Send command to Arduino
                print(f"Sent to Arduino: {command}")

            # Read telemetry data from Arduino
            if ser.in_waiting > 0:  # Check if data is available
                ack = ser.readline().decode('utf-8').strip()  # Read a line from the serial port
                if ack.startswith("ACK:"):
                    print(f"Arduino ACK: {ack}")  # Handle acknowledgment
                else:
                    # Process sensor data
                    infr_value, cm_value = ack.split(",")
                    # Read temperature and humidity from the DHT sensor
                    temperature = dht_device.temperature
                    humidity = dht_device.humidity
                    print(f"newINFR = {infr_value}, cm = {cm_value}, humidity = {humidity}, temperature  = {temperature}")

                    if temperature is not None and humidity is not None:
                        # Format the message
                        msg_txt_formatted = MSG_SND.format(
                            temperature=temperature,
                            humidity=humidity,
                            distance=cm_value,
                            detected=infr_value
                        )
                        message = Message(msg_txt_formatted)

                        # Send the message to IoT Hub
                        print(f"Sending message: {message}")
                        client.send_message(message)
                        print("Message successfully sent")
                    else:
                        print("Failed to retrieve data from the sensor")
        except RuntimeError as error:
            print(f"Sensor error: {error.args[0]}")
        except ValueError:
            print(f"Unexpected data format from Arduino: {ack}")
        except Exception as e:
            print(f"Error: {e}")

# Flask routes
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/control', methods=['POST'])
def control():
    action = request.form['action']
    if action == 'Forward':
        command_queue.put('FORWARD')
    elif action == 'Backward':
        command_queue.put('BACKWARD')
    elif action == 'Left':
        command_queue.put('LEFT')
    elif action == 'Right':
        command_queue.put('RIGHT')
    elif action == 'Stop':
        command_queue.put('STOP')
    elif action == 'Reset':
        command_queue.put('RESET')
    return render_template('index.html')  # Stay on the control page

# Flask in a separate thread
def start_flask_app():
    app.run(host='0.0.0.0', port=5000)

# Handle exit
def handle_exit(signum, frame):
    print("Exiting...")
    dht_device.exit()
    ser.close()
    exit(0)

signal.signal(signal.SIGINT, handle_exit)
signal.signal(signal.SIGTERM, handle_exit)

if __name__ == '__main__':
    print("Starting the application...")

    # Start Flask app in a separate thread
    flask_thread = threading.Thread(target=start_flask_app)
    flask_thread.daemon = True
    flask_thread.start()

    # Start serial communication handler in a separate thread
    try:
        client = iothub_client_init()
        serial_thread = threading.Thread(target=serial_handler, args=(client,))
        serial_thread.daemon = True
        serial_thread.start()

        # Keep the main thread alive
        while True:
            time.sleep(1)

    except KeyboardInterrupt:
        print("Application stopped")
    finally:
        handle_exit(None, None)
