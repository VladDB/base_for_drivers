# Base for drivers

### If you need to connect to a device via tcp, this simplest can be used in your project

The basic concept of this solution is to have an abstract class Driver that implements such function as initialization connection via tcp and send/receive data. The main method is **sendData** where you need to pass on sending data, size of it and pointers for variables receive data and data size. Also class Driver has two abstract methods for getting information and current date-time which you can implement in children classes. If you want to have the same task for all your devices you can implement method **startTask**. The main assignment of Driver class is inheritance for new devices and given one place for realization of sending data (later will be added sending via upd and com port).

In **global.h** you can find some usefull function, such as calculation of crc16-CCITT, realization of ReplaceAll function, getting current time in string and simple realization of logger. Also in include directory there are realizations for AES and md5.

_The build is tested in the Windows with mingw64 and in the Ubuntu_
  
