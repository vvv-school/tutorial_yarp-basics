/*
 * Copyright (C) 2017 iCub Facility
 * Authors: Alberto Cardellino
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include <map>
#include <string>
#include <TutorialModule.h>
#include <yarp/os/LogStream.h>
#include <yarp/cv/Cv.h>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace yarp::os;
using namespace yarp::sig;


TutorialModule::TutorialModule() { }

TutorialModule::~TutorialModule() { }

// Function called once at the startup.
// Do all init configuration here
bool TutorialModule::configure(yarp::os::ResourceFinder &rf)
{
    // check if a parameters called 'name' has been provided
    if(!rf.check("name"))
    {
        yError() << "No parameter 'name' has been provided";
        return false;
    }

    // Check the input value is of the correct type
    if(!rf.find("name").isString() )
    {
        yError() << "Input parameter 'name' is not a string as it should be. Found" << rf.find("name").toString();
        return false;
    }

    // all good, now let's get the value
    portName = rf.find("name").asString();

    yInfo() << "Configuring the module with the following parameters: \n";
    yInfo() << "name is " << portName;


    ResourceFinder r;
    r.setDefaultContext("tutorial_yarp-basics");
    std::string filePath = r.findFileByName("programmersLife.bmp");

    image = cv::imread(filePath.c_str(), cv::IMREAD_COLOR);
    if(!image.data)
    {
        yError() << "Error";
        return false;
    }

    // open all ports
    bool ret = commandPort.open(portName);
    if(!ret)
    {
        yError() << "Cannot open port " << portName;
        return false;
    }

    // opening port to publish image
    imagePort.open("/imagePort:o");

    // open a dummy fake robot
    yarp::os::Property fakeRobotConfig;
    fakeRobotConfig.put("device",    "controlBoard_nws_yarp");
    fakeRobotConfig.put("subdevice", "fakeMotionControl");
    fakeRobotConfig.put("name", "/fakeRobot/head");
    fakeRobot.open(fakeRobotConfig);

    // Create a device to 'remotely' access the dummy robot
    yarp::os::Property controlBoardConfig;
    controlBoardConfig.put("device", "remote_controlboard");
    controlBoardConfig.put("remote", "/fakeRobot/head");
    controlBoardConfig.put("local",  "/myApp");

    if(!controlBoard.open(controlBoardConfig))
    {
        yError() << "Cannot connect to /fakeRobot/head device";
        return false;
    }

    // Get access to some robot interface
    controlBoard.view(posControl);
    controlBoard.view(encoders);
    return true;
}

double TutorialModule::getPeriod()
{
    return 0.5; // module periodicity (seconds)
}

bool TutorialModule::updateModule()
{
    // watchdog message
    yInfo()<<"TutorialModule running happily...";

    // Read input from user
    Bottle b;
    commandPort.read(b);

    Value val = b.get(0);

    // If it is an integer, just add +1 and send it back (huge computation!!)
    if(val.isInt32())
    {
        int c = b.get(0).asInt32();
        b.clear();
        b.addString("Answer is");
        b.addInt32(++c);
        commandPort.write(b);
    }

    // If it is a string check for known commands
    if(val.isString())
    {
        if(val.asString() == "quit")
        {
            yInfo() << "Received quit command";
            return false;
        }
        else if(val.asString() == "enc")
        {
            // Read encoder values:
            double e = 0;
            if(encoders)
                encoders->getEncoder(0, &e);
            b.clear();
            b.addString("Joint 0 position is");
            b.addInt32(e);
            commandPort.write(b);
        }
        else if (val.asString() == "send_image")
        {
            // Publish the image
            yarp::sig::ImageOf<PixelBgr> &img = imagePort.prepare();
            img = yarp::cv::fromCvMat<PixelBgr>(image);
            imagePort.write();
        }
        else
            yError() << "Received unknown command " << val.toString();
    }
    return true;
}

// Catch the CTRL+C and unlock any resources which may
// hang on closure
bool TutorialModule::interruptModule()
{
    // wake up port from read.
    commandPort.interrupt();
    return true;
}

bool TutorialModule::close()
{
    yInfo()<<"closing TutorialModule module";
    commandPort.close();
    return true;
}
