# YARP basics tutorial

This tutorial shows some of the basic features of YARP, usually required when dealing with a robot :robot:

## Prerequisites

For this assignment, you just need to be aware of:
- What a class is in C++
- Optionally run CMake

## You will see how to:
- Handle a [yarp::os::Bottle](http://www.yarp.it/classyarp_1_1os_1_1Bottle.html);
- Read and write on [yarp::os::Port](http://www.yarp.it/classyarp_1_1os_1_1Port.html) and [yarp::os::BufferedPort](http://www.yarp.it/classyarp_1_1os_1_1BufferedPort.html); 
- instantiate a yarp device
- use IEncoder Motor Interfaces: [yarp::dev::IEncoders](http://www.yarp.it/classyarp_1_1dev_1_1IEncoders.html);
- write a [yarp::sig::Image](http://www.yarp.it/classyarp_1_1sig_1_1Image.html) through a YARP port


## Build and Install the code
Follow these steps to build and properly install your module: 
```
$ cd tutotial_RFModule
$ mkdir build; cd build
$ cmake ../
$ make
$ make install
```
the `make install` will install your module (binary, xml files, etc) in the icub contrib folder which is already setup on your machine. 

## Have fun with the tutorial

Terminal 1: Start the yarp server if not already running
```
$ yarpserver
```

Terminal 2: run the tutorial executable. It requires an input parameter to run!!
The parameter is the name of the port the executable will open. The parameter must be given in the form of `--name <portName>` where <portName> is a string of your choise. Remember, portname must start with `/` and cannot contain `@` charaters.
```
$ tutorial_yarp-basics  --name <portName>
```

Terminal 3: open a yarp port to send commands to the tutorial executable
```
$ yarp write ... <portName>
```

Terminal 4: open a yarp port to receive answers from the tutorial executable
```
$ yarp read ... <portName>
```

Terminal 5: start a yarpview to see the image being published
```
$ yarpview
```

Terminal 6: connect 
```
$ yarp connect /imagePort:o /yarpview/img:i
```

Now everything is set up. The tutorial will open a fake robot part and instantiate a remote_controlboard device to connect to it. In the yarpview windows you'll see a dummy image being shown.
In the yarp write terminal you are able to send command to the tutorial executable and the output will be shown in the yarp read terminal.

Available commands are:
- any integer number: the application will reply with that number +1
- 'enc': the application will reply with the encoder position of the first joint of the fake robot
- 'quit': the application will quit

