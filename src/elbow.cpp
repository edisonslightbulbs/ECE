#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>

#include "elbow.h"
#include "outliers.h"
#include "io.h"

/** evaluate the second derivative */
float secondDerivative(std::vector<float>& x)
{
    /** sort in descending order */
    std::sort(x.begin(), x.end(), std::greater<>());

    /** secant-line hyper-parameter, n.b., longer secant line is
     * more forgiving a shorter secant line is less forgiving */
    const int SIZE_OF_SECANT_LINE = 100;

    /** find max rate of change  */
    float yVal = 0;
    float slope = 0;
    for (int i = SIZE_OF_SECANT_LINE; i < x.size(); i++) {
        float roc = std::abs(x[i] - x[i - SIZE_OF_SECANT_LINE]);
        if (roc > slope) {
            yVal = x[i];
            slope = roc;
        }
    }
    // helpers for quick assessment:
    // std::cout << "  elbow value: " << yVal << std::endl;
    // const std::string dist = io::pwd() + "/build/bin/circumference.csv";
    // io::write_val(x, dist);
    return yVal;
}

/** evaluate the first derivative */
float firstDerivative(std::vector<float>& x)
{
    std::sort(x.begin(), x.end(), std::greater<>());
    io::write(x, "./output/knn.csv");

    /** keep track of the axis value (yValue) and its
     *  corresponding derivative d1 { yVal, d1 } using a map */
    std::map<float, float> derivativeMap;
    std::map<float, float>::iterator it;

    /** evaluate the first derivative */
    float yVal = 0;
    std::vector<float> derivatives;
    for (int i = 0; i < x.size() - 1; i++) {
        float d1 = x[i + 1] - x[i];
        yVal = x[i];
        derivativeMap.insert(std::pair<float, float>(d1, yVal));
        derivatives.push_back(d1);
    }

    /** estimate elbow point using first order derivatives */
    float elbow = secondDerivative(derivatives);
    it = derivativeMap.find(elbow);

    // std::cout << "  max value: " << *std::max_element(x.begin(), x.end()) <<
    // std::endl; std::cout << "  min value: " << *std::min_element(x.begin(),
    // x.end()) << std::endl; std::cout << "elbow value: " << it->second <<
    // std::endl;

    return it->second;
}

/** remove successive axis values*/
float elbow::analyze(std::vector<float>& x)
{
    float axisEdge = firstDerivative(x);
    return axisEdge;
}
