#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include <string>

class Curve
{
public:
    Curve(const std::string &name);

    // методы в роль заполнения
    void addValue(float value);

    // методы в роль рисования
    const std::string &name() const;
    const std::vector<float> &values() const;

private:
    std::string _name;
    std::vector<float> _values;
};

#endif // CURVE_H
