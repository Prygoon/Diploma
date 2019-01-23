#ifndef PROJECT_H
#define PROJECT_H

#include "train.h"
#include "tracksection.h"

class Project
{
private:
    QString *name;
    Train *train;
    QVector<TrackSection> *track;
public:
    Project(QString *name, Train *train, QVector<TrackSection> *track);

    QString *getName() const;
    void setName(QString *value);

    Train *getTrain() const;
    void setTrain(Train *value);

    QVector<TrackSection> *getTrack() const;
    void setTrack(QVector<TrackSection> *value);
};

#endif // PROJECT_H
