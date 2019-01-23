#include "project.h"

Project::Project(QString *name, Train *train, QVector<TrackSection> *track) :
    name(name), train(train), track(track)
{

}

QString *Project::getName() const
{
    return name;
}

void Project::setName(QString *value)
{
    name = value;
}

Train *Project::getTrain() const
{
    return train;
}

void Project::setTrain(Train *value)
{
    train = value;
}

QVector<TrackSection> *Project::getTrack() const
{
    return track;
}

void Project::setTrack(QVector<TrackSection> *value)
{
    track = value;
}


