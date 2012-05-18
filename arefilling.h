#ifndef AREFILLING_H
#define AREFILLING_H

#include <deque>
#include <string>
#include <sstream>
#include "curve.h"

template <class ConcreteDerived>
class AreFilling : public ConcreteDerived
{
public:
    AreFilling() {}

    void parseStrLine(const std::string &line);

private:
    std::deque<std::string> split(const std::string &str) const;

    void setAxisNames(const std::string &xAxisName, const std::string &yAxisName);
    void createCurves(const std::deque<std::string> &names);
    void addValuesSlice(const std::deque<float> &values);
    void addValuesSlice(const std::string &line);

    void pushXValue(float value);
    void pushYValue(Curve *curve, float value);
};

template <class ConcreteDerived>
void AreFilling<ConcreteDerived>::parseStrLine(const std::string &line) {
    if (line.length() == 0) return;

    static int callingCounter = 0;
    if (callingCounter == 0) {
        std::deque<std::string> axisNames = split(line);
        auto name = axisNames.cbegin();
        setAxisNames(*name, *(++name));
    } else if (callingCounter == 1) {
        std::deque<std::string> names = split(line);
        auto createCurvesLambda = [this, &names]() {
            names.pop_front();
            this->createCurves(names);
        };

        auto firstColumn = names.cbegin();
        if (*firstColumn == "#") {
            createCurvesLambda();
        } else {
            std::deque<float> values;
            for (std::string &name : names) {
                values.push_back(atof(name.c_str()));
                name = "";
            }

            createCurvesLambda();
            this->addValuesSlice(values);
        }
    } else {
        this->addValuesSlice(line);
    }

    ++callingCounter;
}

template <class ConcreteDerived>
std::deque<std::string> AreFilling<ConcreteDerived>::split(const std::string &str) const {
    std::stringstream strStream(str);
    std::string item;
    std::deque<std::string> values;
    while (std::getline(strStream, item, '\t')) {
        if (item.length() == 0) continue;
        values.push_back(item);
    }
    return values;
}

template <class ConcreteDerived>
void AreFilling<ConcreteDerived>::setAxisNames(const std::string &xAxisName, const std::string &yAxisName) {
    this->_axisX.setName(xAxisName);
    this->_axisY.setName(yAxisName);
}

template <class ConcreteDerived>
void AreFilling<ConcreteDerived>::createCurves(const std::deque<std::string> &names) {
    for (const std::string &name : names) {
        this->_curves.push_back(name);
    }
}

template <class ConcreteDerived>
void AreFilling<ConcreteDerived>::addValuesSlice(const std::deque<float> &values) {
    auto valuesIter = values.cbegin();
    pushXValue(*valuesIter);

    int curveIndex = 0;
    for (; valuesIter != values.cend(); valuesIter++) {
        pushYValue(&this->_curves[curveIndex++], *valuesIter);
    }
}

template <class ConcreteDerived>
void AreFilling<ConcreteDerived>::addValuesSlice(const std::string &line) {
    std::stringstream strStream(line);
    auto readValue = [&strStream]() {
        float value;
        strStream >> value;
        return value;
    };

    pushXValue(readValue());

    for (size_t i = 0; i < this->_curves.size(); i++) {
        pushYValue(&this->_curves[i], readValue());
    }
}

template <class ConcreteDerived>
void AreFilling<ConcreteDerived>::pushXValue(float value) {
    this->_xValues.push_back(value);
    this->_axisX.tryUpdate(value);
}

template <class ConcreteDerived>
void AreFilling<ConcreteDerived>::pushYValue(Curve *curve, float value) {
    curve->addValue(value);
    this->_axisY.tryUpdate(value);
}

#endif // AREFILLING_H
