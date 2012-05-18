#ifndef AXIS_H
#define AXIS_H

#include <string>

class Axis
{
public:
    Axis();
    virtual ~Axis();

    // эти методы надо вынести в роль установки значений оси
    void setName(const std::string &name);
    void tryUpdate(float value);

    // эти методы надо вынести в роль рисования оси
    float max() const;
    float min() const;
    float zero() const;
    float length() const;
    float orderOfMagnitude() const;

private:
    std::string _name;
    float *_max;
    float *_min;
};

#endif // AXIS_H
