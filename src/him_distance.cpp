/***********************************************************************
* FILENAME:
*       him_distance.cpp
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

#define HIM_LIBRARY_COMPILE

#include "Arduino.h"

#include "him_distance.h"


HimUltrasonicSensor::HimUltrasonicSensor(unsigned int pinTrigger, unsigned int pinEcho, unsigned int max_distance) :
    m_pinTrigger(pinTrigger),
    m_pinEcho(pinEcho)
{
    pinMode(m_pinEcho, INPUT);
    pinMode(m_pinTrigger, OUTPUT);
    digitalWrite(m_pinTrigger, HIGH);

    // speed of sound in air @ 20`C
    // 343.5 m/s
    // -> 29,112 us/cm
    m_max_distance = max_distance;
    m_max_distance_echo_time = m_max_distance * 2 * 29.112;
}


HimUltrasonicSensor::~HimUltrasonicSensor()
{
}


bool HimUltrasonicSensor::measure(unsigned int &distance)
{
    bool valid_range = true;
    unsigned int echo_time;

    // measurement is starting at the falling edge 
    digitalWrite(m_pinTrigger, LOW);
    
    // measure the length of echo pulse
    echo_time = pulseIn(m_pinEcho, HIGH);

    // check for measurement abort after ~20 ms
    if ( echo_time > 19000 ) {
        distance = m_max_distance;
        valid_range = false;

    } else {
        if ( echo_time > m_max_distance_echo_time ) {
            // return the max distance
            distance = m_max_distance;
        } else {
            // calculate the real distance in cm 
            distance = echo_time / 2 / 29.112;
        }
     
        delay( 20 - echo_time/1000 );
        valid_range = true;
    }

    digitalWrite(m_pinTrigger, HIGH);
    return valid_range;
}


bool HimUltrasonicSensor::measure(unsigned int &distance, unsigned int oversampling)
{
    if(oversampling < 2) {
        return false;

    } else if(oversampling > 10) {
        return false;

    } else {
        bool valid_range;
        struct {
            bool valid_range;
            unsigned int distance;
        } measurement[oversampling];
        unsigned int index[oversampling];
        unsigned int tmp;

        // take the measurements
        for (int i=0; i<oversampling; i++) {
            index[i] = i;
            measurement[i].valid_range = measure(measurement[i].distance);
        }

        // sort the results
        for (int bubble=0; bubble < (oversampling/2)+1; bubble++) {
            for (int i=1; i < (oversampling-bubble); i++) {
                if ( measurement[index[i-1]].distance > measurement[index[i]].distance ) {
                    tmp = index[i-1];
                    index[i-1] = index[i];
                    index[i] = tmp;
                }
            }
        }

        // get the median value
        if (oversampling & 0x01)  {
            distance = measurement[index[(oversampling)/2]].distance;
        } else {
            distance = ( measurement[index[oversampling/2]].distance + measurement[index[oversampling/2-1]].distance ) / 2;
        }

        // when at least one measurement for all biggest measurements - the measurement is valid I guess
        valid_range = false;
        for (int i=oversampling/2 ;i<oversampling; i++) {
            valid_range |= measurement[index[i]].valid_range;
        }

        return valid_range;
    }
}

