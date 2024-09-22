import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QSlider, QVBoxLayout, QWidget, QLabel
from PyQt5.QtCore import Qt
import RPi.GPIO as GPIO

# Set up the GPIO for RGB LED
GPIO.setmode(GPIO.BCM)
RED_PIN = 18
GREEN_PIN = 23
BLUE_PIN = 24

GPIO.setup(RED_PIN, GPIO.OUT)
GPIO.setup(GREEN_PIN, GPIO.OUT)
GPIO.setup(BLUE_PIN, GPIO.OUT)

pwm_red = GPIO.PWM(RED_PIN, 100)   # Frequency = 100Hz
pwm_green = GPIO.PWM(GREEN_PIN, 100)
pwm_blue = GPIO.PWM(BLUE_PIN, 100)

pwm_red.start(0)   # Start with 0% duty cycle
pwm_green.start(0)
pwm_blue.start(0)

# Function to change LED brightness
def change_red_brightness(value):
    pwm_red.ChangeDutyCycle(value)

def change_green_brightness(value):
    pwm_green.ChangeDutyCycle(value)

def change_blue_brightness(value):
    pwm_blue.ChangeDutyCycle(value)

# Create the GUI
class LEDControlGUI(QMainWindow):
    def __init__(self):
        super().__init__()
        
        self.setWindowTitle("LED Brightness Control")
        
        layout = QVBoxLayout()
        
        # Slider for Red brightness
        self.red_label = QLabel("Red Brightness", self)
        self.red_slider = QSlider(Qt.Horizontal)
        self.red_slider.setRange(0, 100)
        self.red_slider.setValue(0)
        self.red_slider.valueChanged.connect(change_red_brightness)
        
        # Slider for Green brightness
        self.green_label = QLabel("Green Brightness", self)
        self.green_slider = QSlider(Qt.Horizontal)
        self.green_slider.setRange(0, 100)
        self.green_slider.setValue(0)
        self.green_slider.valueChanged.connect(change_green_brightness)
        
        # Slider for Blue brightness
        self.blue_label = QLabel("Blue Brightness", self)
        self.blue_slider = QSlider(Qt.Horizontal)
        self.blue_slider.setRange(0, 100)
        self.blue_slider.setValue(0)
        self.blue_slider.valueChanged.connect(change_blue_brightness)
        
        # Add widgets to layout
        layout.addWidget(self.red_label)
        layout.addWidget(self.red_slider)
        layout.addWidget(self.green_label)
        layout.addWidget(self.green_slider)
        layout.addWidget(self.blue_label)
        layout.addWidget(self.blue_slider)
        
        # Set layout to central widget
        central_widget = QWidget()
        central_widget.setLayout(layout)
        self.setCentralWidget(central_widget)

# Main function to start the GUI
if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = LEDControlGUI()
    window.show()
    sys.exit(app.exec_())

# Clean up GPIO on exit
pwm_red.stop()
pwm_green.stop()
pwm_blue.stop()
GPIO.cleanup()
