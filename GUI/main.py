import sys
import time
import logging
import numpy as np
import datetime
import csv

from PyQt5.QtWidgets import * 
from PyQt5 import QtCore, QtGui
from PyQt5.QtGui import * 
from PyQt5.QtCore import *

# We import library dedicated to data plot
import pyqtgraph as pg
from pyqtgraph import PlotWidget

import serial 
import serial.tools.list_ports


#Global
CONN_STATUS = False

baudRate = 9600 

xChestData = np.full(100,0,dtype=np.int16)
yChestData = np.full(100,0,dtype=np.int16)
zChestData = np.full(100,0,dtype=np.int16)
xAnkleData = np.full(100,0,dtype=np.int16)
yAnkleData = np.full(100,0,dtype=np.int16)
zAnkleData = np.full(100,0,dtype=np.int16)

data = []

clock = list(range(100)) 

flag_transition = 0
saved = 0

position_names = ['supine straight', 'supine left', 'supine right', 'prone straight', 'prone left', 'prone right', 'left straight', 'left bent leg', 'left fetal', 'right straight', 'right bent leg', 'right fetal']

# Logging config
logging.basicConfig(format="%(message)s", level=logging.INFO)


#########################
# SERIAL_WORKER_SIGNALS #
#########################
class SerialWorkerSignals(QObject):
    """!
    @brief Class that defines the signals available to a serialworker.
    Available signals (with respective inputs) are:
        - device_port:
            str --> port name to which a device is connected
        - status:
            str --> port name
            int --> macro representing the state (0 - error during opening, 1 - success)
    """
    device_port = pyqtSignal(str)
    status = pyqtSignal(str, int)


#################
# SERIAL_WORKER #
#################
class SerialWorker(QRunnable):
    """!
    @brief Main class for serial communication: handles connection with device.
    """
    def __init__(self, serial_port_name):
        """!
        @brief Init worker.
        """
        global baudRate
        self.is_killed = False
        super().__init__()
        # init port, params and signals
        self.port = serial.Serial()
        self.port_name = serial_port_name
        self.baudrate = baudRate #hard coded but can be a global variable, or an input param
        self.signals = SerialWorkerSignals()

    @pyqtSlot()
    def run(self):
        """!
        @brief Estabilish connection with desired serial port.
        """
        global CONN_STATUS

        if not CONN_STATUS:
            try:
                self.port = serial.Serial(port=self.port_name, baudrate=self.baudrate,
                                        write_timeout=0, timeout=2)
                if self.port.is_open:
                    CONN_STATUS = True
                    self.signals.status.emit(self.port_name, 1)
                    print("Successfully connected to port {}.".format(self.port_name))
                    time.sleep(0.01)

            except serial.SerialException:
                logging.info("Error with port {}.".format(self.port_name))
                self.signals.status.emit(self.port_name, 0)
                time.sleep(0.01)


    @pyqtSlot()
    def readData(self):

        global line

        """!
        @brief Read data from desired serial port
        """
        line = ''
        flag = 0

        if CONN_STATUS:
            try:
                while flag != 1:
                    read = str(self.port.read())
                    read = read[2]
                    if (read != 'E'):
                        line += read

                    if (read == 'E'):
                        if (line[0] != 'S'):
                            line = ''
                        else:
                            flag = 1
                            line = line[2:-1]
                            line = line.split(',')
                            time.sleep(1)

            except: # except serial.SerialException:
                logging.info("Error with reading data from port {}.".format(self.port_name))
                # self.signals.status.emit(self.port_name, 0)
                time.sleep(0.01)

        else:
            print("Not connected to any port. Please first connect to a port.")


    @pyqtSlot()
    def storeData(self, clicked_date):

        global line, data, old_position_idx, flag_transition, beginning_transition, saved, position_idx

        current_date = datetime.datetime.now()
        minutes_diff = int(np.floor((((current_date - clicked_date).total_seconds() - 20.0 * (old_position_idx-1))/ 60.0)))
        position_idx = minutes_diff + 1

        if (position_idx != old_position_idx and position_idx < 13):
            if flag_transition == 0:
                beginning_transition = current_date
                flag_transition = 1

            if round((current_date - beginning_transition).total_seconds() > 20.0):
                old_position_idx = position_idx
                flag_transition = 0


        if (position_idx >= 13 and saved == 0):
            # Saving data as CSV file
            labels = ['x_chest', 'y_chest', 'z_chest', 'x_ankle', 'y_ankle', 'z_ankle', 'position']

            date = datetime.datetime.now()
            date = date.strftime("%Y%m%d_%H%M%S")
            csv_name = "{}.csv".format(date)

            with open(csv_name, 'w') as f:

                # using csv.writer method from CSV package
                write = csv.writer(f, delimiter=',', lineterminator='\n')
                write.writerow(labels)
                write.writerows(data)
            
            time.sleep(0.01)
            saved = 1

        elif (flag_transition == 0 and saved == 0):
            acc_data = line.copy()
            acc_data.append('{}'.format(minutes_diff+1))
            data.append(acc_data)


    @pyqtSlot()
    def send(self, char):
        """!
        @brief Basic function to send a single char on serial port.
        """
        try:
            self.port.write(char.encode('utf-8'))
            logging.info("Written {} on port {}.".format(char, self.port_name))
        except:
            logging.info("Could not write {} on port {}.".format(char, self.port_name))


    @pyqtSlot()
    def killed(self):
        """!
        @brief Close the serial port before closing the app.
        """
        global CONN_STATUS

        if self.is_killed and CONN_STATUS:
            self.port.close()
            time.sleep(0.01)
            CONN_STATUS = False
            self.signals.device_port.emit(self.port_name)
            
        logging.info("Killing the process")


###############
# MAIN WINDOW #
###############
class MainWindow(QMainWindow):

    global clock
    def __init__(self):
        """!
        @brief Init MainWindow.
        """
        # Define worker
        self.serial_worker = SerialWorker(None)

        super(MainWindow, self).__init__()

        # Title and geometry
        self.setWindowTitle('Sleep Position Acceleration Acquisition')
        width = 1080
        height = 720
        self.resize(width, height)

        # Create thread handler
        self.threadpool = QThreadPool()
        
        self.connected = CONN_STATUS
        self.serialscan()
  
        self.initUI()
    
    #####################
    # GRAPHIC INTERFACE #
    #####################
    def initUI(self):
        """!
        @brief Set up the graphical interface structure.
        """
        # Create the plot widget
        self.graphWidget = PlotWidget()
        
        # Plot settings
        self.graphWidget.showGrid(x=True, y=True)   # Add grid
        self.graphWidget.setBackground('eeeeee')   # Set background color
        self.graphWidget.setTitle("Accelerometer data",color='black', size="10pt")  # Add title
        # Add axis labels
        styles = {'color':'k', 'font-size':'15px'}  
        self.graphWidget.setLabel('left', 'Acc data [m/s2]', **styles)
        self.graphWidget.setLabel('bottom', 'Time [pt]', **styles)
        self.graphWidget.addLegend((1,1))   # Add legend
        self.graphWidget.setMouseEnabled(x=False, y=False)

        # Display 100 time points
        self.horAxis = list(range((100))) 
        self.xChestGraph = [0]*100
        self.yChestGraph = [0]*100
        self.zChestGraph = [0]*100
        self.xAnkleGraph = [0]*100
        self.yAnkleGraph = [0]*100
        self.zAnkleGraph = [0]*100
        self.count = 0

        self.draw()

        verticalLayout = QVBoxLayout()
    
        font = QFont()
        font.setBold(True)
        font.setWeight(75)
        
        verticalLayout.addWidget(self.com_list_widget)
        verticalLayout.addWidget(self.conn_btn)

        # Create textbox
        self.textbox = QLineEdit()
        self.textbox.setReadOnly(True)
        self.textbox.setAlignment(QtCore.Qt.AlignCenter)
        self.textbox.setText('Ready ? :-)')
        verticalLayout.addWidget(self.textbox)

        # Create Start/Stop button
        self.updateBtn = QPushButton(
            text = "Start", 
            checkable= True, 
            toggled = self.dataUpdate
        )
        self.updateBtn.setDisabled(True)    # disabled by default

        verticalLayout.addWidget(self.updateBtn)
        verticalLayout.addWidget(self.graphWidget)

        self.timer = QtCore.QTimer()
        self.timer.setInterval(50)

        self.dataTimer = QtCore.QTimer()
        self.dataTimer.setInterval(50)

        self.graphTimer= QtCore.QTimer()
        self.graphTimer.setInterval(50)

        self.textboxTimer= QtCore.QTimer()
        self.textboxTimer.setInterval(50)

        widget = QWidget()
        widget.setLayout(verticalLayout)
        self.setCentralWidget(widget)


    def drawGeneralGraph(self):
        """!
        @brief Draw the live plots
        """
        global line

        # Remove the first y element
        if (self.count < 101):
            self.count += 1
        else:
            self.horAxis = self.horAxis[1:]
            self.horAxis.append(self.horAxis[-1] + 1)  # Add a new value 1 higher than the last.

        x_chest = int(line[0])
        y_chest = int(line[1])
        z_chest = int(line[2])
        x_ankle = int(line[3])
        y_ankle = int(line[4])
        z_ankle = int(line[5])

        # X-axis chest acceleration
        self.xChestGraph = self.xChestGraph[1:]
        self.xChestGraph.append(x_chest)
        self.dataXChest.setData(self.horAxis, self.xChestGraph)  # Update the data

        # Y-axis chest acceleration
        self.yChestGraph = self.yChestGraph[1:]
        self.yChestGraph.append(y_chest)
        self.dataYChest.setData(self.horAxis, self.yChestGraph)  # Update the data

        # Z-axis chest acceleration
        self.zChestGraph = self.zChestGraph[1:]
        self.zChestGraph.append(z_chest)
        self.dataZChest.setData(self.horAxis, self.zChestGraph)  # Update the data

        # X-axis ankle acceleration
        self.xAnkleGraph = self.xAnkleGraph[1:]
        self.xAnkleGraph.append(x_ankle)
        self.dataXAnkle.setData(self.horAxis, self.xAnkleGraph)  # Update the data

        # Y-axis ankle acceleration
        self.yAnkleGraph = self.yAnkleGraph[1:]
        self.yAnkleGraph.append(y_ankle)
        self.dataYAnkle.setData(self.horAxis, self.yAnkleGraph)  # Update the data

        # Z-axis ankle acceleration
        self.zAnkleGraph = self.zAnkleGraph[1:]
        self.zAnkleGraph.append(z_ankle)
        self.dataZAnkle.setData(self.horAxis, self.zAnkleGraph)  # Update the data


    def draw(self):
        """!
             @brief Draw the plots.
        """
        global xChestData, yChestData, zChestData, xAnkleData, yAnkleData, zAnkleData

        self.dataXChest = self.plot(self.graphWidget, clock, xChestData, 'x-axis chest', 'b2cdb7')
        self.dataYChest = self.plot(self.graphWidget, clock, yChestData, 'y-axis chest', '8ba590')
        self.dataZChest = self.plot(self.graphWidget, clock, zChestData, 'z-axis chest', '5d7261')
        self.dataXAnkle = self.plot(self.graphWidget, clock, xAnkleData, 'x-axis ankle', 'ffbe59')
        self.dataYAnkle = self.plot(self.graphWidget, clock, yAnkleData, 'y-axis ankle', 'orange')
        self.dataZAnkle = self.plot(self.graphWidget, clock, zAnkleData, 'z-axis ankle', 'c18932')

    
    def plot(self, graph, x, y, curve_name, color):
        """!
        @brief Draw graph.
        """
        pen = pg.mkPen(color=color,width=3)
        line = graph.plot(x, y, name=curve_name, pen=pen)
        return line

    def updateTextBox(self):
        """!
        @brief Update the textbox output to notify the user about the ongoing protocol
        """

        global position_names, position_idx, flag_transition, saved

        if saved == 0:
            if flag_transition == 0:
                self.textbox.setText('Sampling position {}'.format(position_names[position_idx-1]))
            elif flag_transition == 1 and position_idx < 13:
                self.textbox.setText('Go to position {} within the next 20 seconds'.format(position_names[position_idx-1]))
        
        else:
            self.textbox.setText('End of the protocol - Data saved')


    ####################
    # SERIAL INTERFACE #
    ####################
    def serialscan(self):
        """!
        @brief Scans all serial ports and creates a list.
        """
        # create the combo box to host port list
        self.port_text = ""
        self.com_list_widget = QComboBox()
        self.com_list_widget.currentTextChanged.connect(self.port_changed)

        # create the connection button
        self.conn_btn = QPushButton(
            text=("Connect to port {}".format(self.port_text)),
            checkable=True,
            toggled=self.on_toggle
        )

        # acquire list of serial ports and add it to the combo box
        serial_ports = [
                p.name
                for p in serial.tools.list_ports.comports()
            ]
        self.com_list_widget.addItems(serial_ports)


    ##################
    # SERIAL SIGNALS #
    ##################

    def port_changed(self):
        """!
        @brief Update conn_btn label based on selected port.
        """
        self.port_text = self.com_list_widget.currentText()
        self.conn_btn.setText("Connect to port {}".format(self.port_text))


    @pyqtSlot(bool)
    def on_toggle(self, checked):
        """!
        @brief Allow connection and disconnection from selected serial port.
        """
        if checked:
            # setup reading worker
            self.serial_worker = SerialWorker(self.port_text) # needs to be re defined
            # connect worker signals to functions
            self.serial_worker.signals.status.connect(self.check_serialport_status)
            self.serial_worker.signals.device_port.connect(self.connected_device)
            # execute the worker
            self.threadpool.start(self.serial_worker)
        else:
            # kill thread
            self.serial_worker.is_killed = True
            self.serial_worker.killed()
            self.com_list_widget.setDisabled(False) # enable the possibility to change port
            self.conn_btn.setText(
                "Connect to port {}".format(self.port_text)
            )

    def check_serialport_status(self, port_name, status):
        """!
        @brief Handle the status of the serial port connection.
        Available status:
            - 0  --> Error during opening of serial port
            - 1  --> Serial port opened correctly
        """
        if status == 0:
            self.conn_btn.setChecked(False)
            
        elif status == 1:
            # enable all the widgets on the interface
            self.com_list_widget.setDisabled(True) # disable the possibility to change COM port when already connected
            self.conn_btn.setText(
                "Disconnect from port {}".format(port_name)
            )
            self.updateBtn.setDisabled(False)

    def connected_device(self, port_name):
        """!
        @brief Checks on the termination of the serial worker.
        """
        logging.info("Port {} closed.".format(port_name))
        self.updateBtn.setDisabled(True)


    def ExitHandler(self):
        """!
        @brief Kill every possible running thread upon exiting application.
        """
        self.serial_worker.is_killed = True
        self.serial_worker.killed()


    def dataUpdate(self,checked):
        global line, old_position_idx, flag_transition, saved

        clicked_date = datetime.datetime.now()

        if checked:
            old_position_idx = 1
            self.updateBtn.setText("Stop")
            self.timer.timeout.connect(lambda: self.serial_worker.readData())
            self.timer.start()
            time.sleep(0.1)    # Needed to stabilize the first reading (line)
            self.dataTimer.timeout.connect(lambda: self.serial_worker.storeData(clicked_date))
            self.dataTimer.start()
            time.sleep(0.1)
            self.graphTimer.timeout.connect(lambda: self.drawGeneralGraph())
            self.graphTimer.start() 
            time.sleep(0.1)
            self.textboxTimer.timeout.connect(lambda: self.updateTextBox())
            self.textboxTimer.start() 

        else:
            # Clear the graph
            self.graphWidget.clear()
            self.horAxis = list(range(100)) 
            self.xChestGraph = [0]*100
            self.yChestGraph = [0]*100
            self.zChestGraph = [0]*100
            self.xAnkleGraph = [0]*100
            self.yAnkleGraph = [0]*100
            self.zAnkleGraph = [0]*100
            self.count = 0
            self.draw()
            # Clear the textbox
            self.textbox.setText('Ready ? :-)')
            # Update button 
            self.updateBtn.setText("Start")
            # Stop the timers
            self.timer.stop()
            self.graphTimer.stop()
            self.dataTimer.stop()
            self.textboxTimer.stop()
            # Reinitialize flags
            flag_transition = 0
            saved = 0
        
      

#############
#  RUN APP  #
#############
if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = MainWindow()
    app.aboutToQuit.connect(w.ExitHandler)
    w.show()
    sys.exit(app.exec_())