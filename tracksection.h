#ifndef TRACKSECTION_H
#define TRACKSECTION_H


class TrackSection
{
private:
    int length; //длина
    double slope; //уклон
    bool calcClimb; //Расчетный подъем
    int curveLength; //Длина кривой
    int curveRadius; // Радиус кривой
    /* #ToDo
     * bool station;
     */

public:
    TrackSection(int lenght, double slope);

    int getLength() const;
    void setLength(int value);

    double getSlope() const;
    void setSlope(double value);

    bool getCalcClimb() const;
    void setCalcClimb(bool value);

    int getCurveLength() const;
    void setCurveLength(int value);

    int getCurveRadius() const;
    void setCurveRadius(int value);
};

#endif // TRACKSECTION_H
