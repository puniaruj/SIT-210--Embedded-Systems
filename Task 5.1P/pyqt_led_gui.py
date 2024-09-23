import sys
from PyQt5.QtWidgets import QApplication, QWidget, QRadioButton, QVBoxLayout, QPushButton
import RPi.GPIO as GPIO

# GPIO setup
GPIO.setmode(GPIO.BCM)
RED_LED_PIN = 17
GREEN_LED_PIN = 27
BLUE_LED_PIN = 22

GPIO.setup(RED_LED_PIN, GPIO.OUT)
GPIO.setup(GREEN_LED_PIN, GPIO.OUT)
GPIO.setup(BLUE_LED_PIN, GPIO.OUT)

class LEDController(QWidget):
    def __init__(self):
        super().__init__()
        
        # GUI setup
        self.setWindowTitle('LED Controller')
        self.setGeometry(100, 100, 200, 200)
        
        # Layout and radio buttons
        layout = QVBoxLayout()
        
        self.red_radio = QRadioButton('Red')
        self.green_radio = QRadioButton('Green')
        self.blue_radio = QRadioButton('Blue')
        
        self.red_radio.toggled.connect(self.turn_on_led)
        self.green_radio.toggled.connect(self.turn_on_led)
        self.blue_radio.toggled.connect(self.turn_on_led)
        
        layout.addWidget(self.red_radio)
        layout.addWidget(self.green_radio)
        layout.addWidget(self.blue_radio)
        
        # Exit button
        exit_button = QPushButton('Exit')
        exit_button.clicked.connect(self.close_application)
        layout.addWidget(exit_button)
        
        self.setLayout(layout)
        
    def turn_on_led(self):
        GPIO.output(RED_LED_PIN, GPIO.LOW)
        GPIO.output(GREEN_LED_PIN, GPIO.LOW)
        GPIO.output(BLUE_LED_PIN, GPIO.LOW)
        
        if self.red_radio.isChecked():
            GPIO.output(RED_LED_PIN, GPIO.HIGH)
        elif self.green_radio.isChecked():
            GPIO.output(GREEN_LED_PIN, GPIO.HIGH)
        elif self.blue_radio.isChecked():
            GPIO.output(BLUE_LED_PIN, GPIO.HIGH)
    
    def close_application(self):
        GPIO.cleanup()
        self.close()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = LEDController()
    window.show()
    sys.exit(app.exec_())
