#ifndef TRACKSECTION_H
#define TRACKSECTION_H


class TrackSection
{
private:
    int length; //длина
    double slope; //уклон
public:
    TrackSection(int lenght, double slope);

    int getLength() const;
    double getSlope() const;
};

#endif // TRACKSECTION_H
