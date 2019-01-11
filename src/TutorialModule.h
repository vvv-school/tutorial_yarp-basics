/*
 * Copyright (C) 2017 iCub Facility
 * Authors: Alberto Cardellino
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include <string>
#include <yarp/os/Port.h>
#include <yarp/sig/Image.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/BufferedPort.h>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include <opencv2/opencv.hpp>

class TutorialModule : public yarp::os::RFModule
{
public:
    TutorialModule();
    virtual ~TutorialModule();

    virtual bool configure(yarp::os::ResourceFinder &rf);
    virtual double getPeriod();
    virtual bool updateModule();
    virtual bool close();
    virtual bool interruptModule();

private:
    cv::Mat image;
    yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelBgr> > imagePort;
    std::string portName;
    yarp::os::Port commandPort;     // command port
    yarp::dev::PolyDriver fakeRobot;
    yarp::dev::PolyDriver controlBoard;

    // Interfaces can only be used as a pointer, they cannot be instatiated
    yarp::dev::IPositionControl *posControl;
    yarp::dev::IEncoders        *encoders;
};

