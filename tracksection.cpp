#include "tracksection.h"

TrackSection::TrackSection(int length, double slope) :
    length(length), slope(slope), calcClimb(false), curveLength(0), curveRadius(0)
{

}

int TrackSection::getLength() const
{
    return length;
}

void TrackSection::setLength(int value)
{
    length = value;
}

double TrackSection::getSlope() const
{
    return slope;
}

void TrackSection::setSlope(double value)
{
    slope = value;
}

bool TrackSection::getCalcClimb() const
{
    return calcClimb;
}

void TrackSection::setCalcClimb(bool value)
{
    calcClimb = value;
}

int TrackSection::getCurveLength() const
{
    return curveLength;
}

void TrackSection::setCurveLength(int value)
{
    curveLength = value;
}

int TrackSection::getCurveRadius() const
{
    return curveRadius;
}

void TrackSection::setCurveRadius(int value)
{
    curveRadius = value;
}
