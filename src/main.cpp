/*
 * Copyright (C) 2017 iCub Facility
 * Authors: Alberto Cardellino
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include <cstdlib>

#include <yarp/os/Network.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/ResourceFinder.h>

#include <TutorialModule.h>

using namespace yarp::os;


int main(int argc, char * argv[])
{
    Network yarp;

    if(!yarp.checkNetwork())
    {
        yError() << "Cannot find yarp server";
        return EXIT_FAILURE;
    }

    ResourceFinder rf;
    rf.configure(argc, argv);
//     rf.setDefaultContext("tutorial_yarp-basic");
//     rf.setVerbose(true);

    TutorialModule module;
    module.runModule(rf);

    yInfo()<<"Main returning...";
    return EXIT_SUCCESS;
}


