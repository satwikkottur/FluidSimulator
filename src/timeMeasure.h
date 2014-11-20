#ifndef TIMEMEASURE
#define TIMEMEASURE

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <sys/time.h>

//Class to measure time elapsed between any two events in the code
class TimeMeasure{
    private:
        //Holds information regarding various points
        std::vector<struct timeval> timePoints;
        std::vector<std::string> timeDetails;

    public:
        //COnstructor
        TimeMeasure(){}

        //Method to add a new time check point
        void addTimePoint(std::string timeDescription){
            struct timeval now;

            gettimeofday(&now, NULL);
            timePoints.push_back(now);
            if(timePoints.size() == 0){
                timeDetails.push_back("Start");
            }
            else{
                timeDetails.push_back(timeDescription);
            }
        }

        void printTimePoints(){
            for(int i = 1; i < timePoints.size(); i++){
                long int timePassed = (timePoints[i].tv_usec - timePoints[i-1].tv_usec) + 1000000 * (timePoints[i].tv_sec - timePoints[i-1].tv_sec);
                printf("%s => %s  -  %d.%d ms \n", timeDetails[i-1].c_str(), timeDetails[i].c_str(), (int)timePassed/1000, (int)timePassed%1000);
            }
        }
};

#endif

