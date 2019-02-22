#ifndef TRACKSECTION_H
#define TRACKSECTION_H


class TrackSection
{
private:
    int index; //Номер по порядку
    int length; //длина
    double slope; //уклон
    int curveLength; //Длина кривой
    int curveRadius; // Радиус кривой
    //bool calcClimb; //Расчетный подъем

    /* #ToDo
     * bool station;
     */

public:
    TrackSection(int index, int lenght, double slope);

    int getLength() const;
    void setLength(int value);

    double getSlope() const;
    void setSlope(double value);

//    bool getCalcClimb() const;
//    void setCalcClimb(bool value);

    int getCurveLength() const;
    void setCurveLength(int value);

    int getCurveRadius() const;
    void setCurveRadius(int value);

    int getIndex() const;
    void setIndex(int value);
};

#endif // TRACKSECTION_H
