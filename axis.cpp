#include "axis.h"

Axis::Axis() : _max(0), _min(0) {}

void Axis::setName(const std::string &name) {
    _name = name;
}

void Axis::tryUpdate(float value) {
    if (value > _max) _max = value;
    else if (value < _min) _min = value;
}

float Axis::max() const {
    return _max;
}

float Axis::min() const {
    return _min;
}

float Axis::zero() const {
    float zero;
    if (0 < _min || _max < 0) {
        zero = _min;
    } else if (_min < 0 && 0 < _max) {
        zero = 0;
    } else if (0 >= _max) {
        zero = _max;
    }

    return zero;
}
