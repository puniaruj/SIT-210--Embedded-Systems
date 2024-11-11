# -*- coding: utf-8 -*-
import tkinter as tk
from tkinter import ttk
import paho.mqtt.client as mqtt  # Import the MQTT library
import json
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from collections import deque

# MQTT setup
BROKER = "broker.emqx.io"  # Replace with your broker address
PORT = 1883
TOPIC = "sensor/data"  # The topic to which Arduino publishes sensor data

# GUI setup
root = tk.Tk()
root.title("Sensor Data Display")
root.geometry("800x600")
root.configure(bg="#1f1f2e")

# Configure fonts and colors
font_style = ("Arial", 12, "bold")
label_bg = "#2e2e3e"
label_fg = "#ffffff"
highlight_color = "#ff6347"

# Create labels for displaying sensor data
temp_label = tk.Label(root, text="Room Temperature: -- \u00B0C", font=font_style, bg=label_bg, fg=label_fg)
temp_label.pack(pady=5)

humidity_label = tk.Label(root, text="Humidity: -- %", font=font_style, bg=label_bg, fg=label_fg)
humidity_label.pack(pady=5)

air_quality_label = tk.Label(root, text="Air Quality: --", font=font_style, bg=label_bg, fg=label_fg)
air_quality_label.pack(pady=5)

pulse_label = tk.Label(root, text="Pulse: -- BPM", font=font_style, bg=label_bg, fg=label_fg)
pulse_label.pack(pady=5)

hand_label = tk.Label(root, text="Hand Detected: --", font=font_style, bg=label_bg, fg=label_fg)
hand_label.pack(pady=5)

# Create containers for plot data
data_length = 50  # Number of data points to show in the plot
room_temp_data = deque([0] * data_length, maxlen=data_length)
humidity_data = deque([0] * data_length, maxlen=data_length)
air_quality_data = deque([0] * data_length, maxlen=data_length)
pulse_data = deque([0] * data_length, maxlen=data_length)

# Setup matplotlib figures and axes
fig, (ax1, ax2, ax3, ax4) = plt.subplots(4, 1, figsize=(6, 8), dpi=80)
fig.patch.set_facecolor('#1f1f2e')

# Room Temperature Plot
ax1.set_title("Room Temperature (\u00B0C)", color=highlight_color)
ax1.set_facecolor("#33334c")
ax1.plot(room_temp_data, color=highlight_color)

# Humidity Plot
ax2.set_title("Humidity (%)", color=highlight_color)
ax2.set_facecolor("#33334c")
ax2.plot(humidity_data, color="#87ceeb")

# Air Quality Plot
ax3.set_title("Air Quality", color=highlight_color)
ax3.set_facecolor("#33334c")
ax3.plot(air_quality_data, color="#adff2f")

# Pulse BPM Plot
ax4.set_title("Pulse (BPM)", color=highlight_color)
ax4.set_facecolor("#33334c")
ax4.plot(pulse_data, color="#ff69b4")

# Embed the matplotlib figure in Tkinter
canvas = FigureCanvasTkAgg(fig, master=root)
canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

# Function to update GUI labels and plots with sensor data
def update_display(room_temp, room_humidity, air_quality, pulse_bpm, hand_detected):
    temp_label.config(text=f"Room Temperature: {room_temp} \u00B0C")
    humidity_label.config(text=f"Humidity: {room_humidity} %")
    air_quality_label.config(text=f"Air Quality: {air_quality}")
    pulse_label.config(text=f"Pulse: {pulse_bpm} BPM")
    hand_label.config(text=f"Hand Detected: {hand_detected}")

    # Append new data to the plot data containers
    room_temp_data.append(float(room_temp) if room_temp != "--" else 0)
    humidity_data.append(float(room_humidity) if room_humidity != "--" else 0)
    air_quality_data.append(float(air_quality) if air_quality != "--" else 0)
    pulse_data.append(float(pulse_bpm) if pulse_bpm != "--" else 0)

    # Clear and redraw each plot with the updated data
    ax1.cla()
    ax1.set_title("Room Temperature (\u00B0C)", color=highlight_color)
    ax1.set_facecolor("#33334c")
    ax1.plot(room_temp_data, color=highlight_color)

    ax2.cla()
    ax2.set_title("Humidity (%)", color=highlight_color)
    ax2.set_facecolor("#33334c")
    ax2.plot(humidity_data, color="#87ceeb")

    ax3.cla()
    ax3.set_title("Air Quality", color=highlight_color)
    ax3.set_facecolor("#33334c")
    ax3.plot(air_quality_data, color="#adff2f")

    ax4.cla()
    ax4.set_title("Pulse (BPM)", color=highlight_color)
    ax4.set_facecolor("#33334c")
    ax4.plot(pulse_data, color="#ff69b4")

    # Redraw the canvas
    canvas.draw()

# MQTT on_connect callback
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
        client.subscribe(TOPIC)  # Subscribe to the topic where Arduino publishes data
    else:
        print("Failed to connect, return code %d\n", rc)

# MQTT on_message callback
def on_message(client, userdata, msg):
    try:
        # Print the raw payload to debug
        print("Received message:", msg.payload)

        # Preprocess the payload to replace "nan" with a valid JSON value (null)
        payload = msg.payload.decode()
        payload = payload.replace("nan", "null")

        # Decode and parse JSON data
        data = json.loads(payload)

        room_temp = data.get("roomTemp", "--")
        room_humidity = data.get("roomHumidity", "--")
        air_quality = data.get("airQuality", "--")
        pulse_bpm = data.get("pulseBPM", "--")
        hand_detected = data.get("handDetected", "--")

        # Update display with the received data
        update_display(room_temp, room_humidity, air_quality, pulse_bpm, hand_detected)
    except Exception as e:
        print("Error decoding message:", e)

# Initialize MQTT client
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

# Connect to the MQTT broker
client.connect(BROKER, PORT, 60)

# Start the MQTT client loop in a separate thread
client.loop_start()

# Start the Tkinter main loop
root.mainloop()
