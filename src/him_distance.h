/***********************************************************************
* FILENAME:
*       him_distance.h
*
* DESCRIPTION:
*       Him Arduino Distance Sensor Routines
*
* NOTES:
*       This library was written to design just for fun.
*
* AUTHOR:
*       Sven Himstedt
*
* COPYRIGHT: 
*       Sven Himstedt 2021, All rights reserved.
*
* LICENSE:
*       This library is free software.
*       You can redistribute it and/or modify it.
*       But give credit or you are an asshole.
*
* START DATE:
*       2021/30/08
*
* CHANGES:
*
***********************************************************************/

#ifndef _HIM_DISTANCE_H_
#define _HIM_DISTANCE_H_


class HimUltrasonicSensor
{
public:
    HimUltrasonicSensor(unsigned int pinTrigger, unsigned int pinEcho, unsigned int max_distance = 200 /* cm */);
    ~HimUltrasonicSensor();

    unsigned int getValue();

    bool measure(unsigned int &distance);
    bool measure(unsigned int &distance, unsigned int oversampling);

private:
    unsigned int m_pinTrigger;
    unsigned int m_pinEcho;

    unsigned int m_max_distance;
    unsigned int m_max_distance_echo_time;
};

#endif /* _HIM_DISTANCE_H_ */
