#include "axis.h"
#include <cmath>

Axis::Axis() : _max(0), _min(0) {}

Axis::~Axis() {
    delete _max;
    delete _min;
}

void Axis::setName(const std::string &name) {
    _name = name;
}

void Axis::tryUpdate(float value) {
    if (_min == 0) {
        _min = new float(value);
        _max = new float(value);
    } else {
        if (value > *_max) *_max = value;
        else if (value < *_min) *_min = value;
    }
}

const std::string &Axis::name() const {
    return _name;
}

float Axis::max() const {
    return *_max;
}

float Axis::min() const {
    return *_min;
}

float Axis::zero() const {
    float zero;
    if (0 < *_min || *_max < 0) {
        zero = *_min;
    } else if (*_min < 0 && 0 < *_max) {
        zero = 0;
    } else { // if (0 >= *_max) {
        zero = *_max;
    }

    return zero;
}

float Axis::length() const {
    return *_max - *_min;
}

float Axis::orderOfMagnitude() const {
    float length = abs(this->length());
    auto findLambda = [&length](float initValue, float factor) {
        float order = initValue;
        while (!(order < length && length < order * 10)) order *= factor;
        return order;
    };

    return (length < 1) ? findLambda(0.1, 0.1) : findLambda(1, 10);
}
