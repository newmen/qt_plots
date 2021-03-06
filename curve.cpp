#include "curve.h"

Curve::Curve(const std::string &name) : _name(name) {
}

void Curve::addValue(float value) {
    _values.push_back(value);
}

const std::string &Curve::name() const {
    return _name;
}

const std::vector<float> &Curve::values() const {
    return _values;
}
