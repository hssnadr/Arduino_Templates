# Serial communication from MaxMSP to Arduino
  
I based this template on this course (made by Chris Vecchio): http://www.noisemantra.com/Physical%20Computing/arduino-MaxMSP%20communications%2011.pdf  
  
This template enables to send data from MaxMSP to Arduino through serial port. More than just sending data, this template includes
routing function between data. This way you can more easily send specific data.  
Each message contains:
* An address = the first byte
* A value = the next bytes
* An indicator of end message = "32" which correspond to a "space" in bytes values  

In this example, we want to send RGB values. Each color data will be sent with an address defined by the first byte of the message, for example:  
* To send red/255: "r255 " -> (to byte) = "114 50 53 53 32" (144=r // 50=2 // 53=5 // 32=" ")
* To send green/120: "g120 " -> (to byte) = "103 49 50 48 32" (103=g // 49=1 ...)
* To send blue/40: "b40 " -> (to byte) = "98 52 48 32" etc...
  
###Note  
In the MaxMSP file, you first need to set the right serial port:  
  
![alt tag](https://raw.githubusercontent.com/hssnadr/Arduino_Templates/master/Serial_MaxMSP_to_Arduino/Max_DefinePort.JPG)  

You can also send specific values to Arduino just using the Arduino serial monitor. This way the bytes conversion is automatic.
Note that you can send several data in the same message, just take care to separate them with a white space.  
  
![alt tag](https://raw.githubusercontent.com/hssnadr/Arduino_Templates/master/Serial_MaxMSP_to_Arduino/SerialMonitorMessages01.PNG)  
  
![alt tag](https://raw.githubusercontent.com/hssnadr/Arduino_Templates/master/Serial_MaxMSP_to_Arduino/SerialMonitorMessages02.PNG)  
  

  
