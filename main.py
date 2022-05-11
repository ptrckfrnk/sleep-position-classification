## ---------------------------------------------------------------------------------------
## 
## PYQT APPLICATION FOR DATA VISUALIZATION OF ACCELEROMETER DATA + SLEEP POSITION ANALYSIS
## 
## ---------------------------------------------------------------------------------------


## Importing necessary libraries
from PyQt5.QtWidgets import QApplication, QLabel, QWidget, QPushButton, QVBoxLayout


## Starting the PyQt application
app = QApplication([])


## Creating the widgets
window = QWidget()
label = QLabel('Output of accelerometer data:')
button1 = QPushButton('Pause')
button2 = QPushButton('Close')


## Setting the layout of the widget
layout = QVBoxLayout()
layout.addWidget(label)
layout.addWidget(button1)
layout.addWidget(button2)
window.setLayout(layout)


## Running the application
window.show()
app.exec()