#!/usr/bin/env python
import serial
import cv2

serialPort = 'COM32'
serialBaud = 921600
bytecount = 0
bRun = True

# Connect to serial port
print('Trying to connect to ' + str(serialPort) +
      ' at ' + str(serialBaud) + ' BAUD.')
try:
    s = serial.Serial(serialPort, serialBaud, timeout=4)
    print('Connected!')
except:
    print("Failed to connect with " + str(serialPort) +
          ' at ' + str(serialBaud) + ' BAUD.')

# Create OpenCV window
cv2.namedWindow("JPEG Stream", cv2.WINDOW_NORMAL)

while bRun:
    try:
        # Read JPEG data from serial port
        rawData = s.read(128)
        bytecount += len(rawData)
        
        # Decode JPEG data and display
        image = cv2.imdecode(
            bytearray(rawData), cv2.IMREAD_COLOR)
        cv2.imshow("JPEG Stream", image)
        cv2.waitKey(1)  # Adjust waitKey delay if needed
    except KeyboardInterrupt:
        print("Interrupted")
        break
    except Exception as e:
        print("Error:", e)

    if bytecount > 60000:
        bRun = False

# Close serial port and OpenCV window
s.close()
cv2.destroyAllWindows()
