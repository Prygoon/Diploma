#include "tracksection.h"

TrackSection::TrackSection(int length, double slope) :
    length(length), slope(slope)
{

    //this->length = length;
    //this->slope = slope;
}

int TrackSection::getLength() const
{
    return length;
}

double TrackSection::getSlope() const
{
    return slope;
}
