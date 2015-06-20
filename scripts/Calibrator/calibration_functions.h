#include "TMath.h"

using namespace std;

double round(const double& value, const double& step){
  return (TMath::Nint(value*(1/step))/double(1/step));
}

int getIndex(const double& value, const double& step){
  return (int)(value/step);
}

double getLowerBound(const double& value, const double& step){
    double x = round(value, step);
    int i = getIndex(x, step);
    if (x > value) --i;
    return i*step;
}

double getHigherBound(const double& value, const double& step){
    double x = round(value, step);
    int i = getIndex(x, step);
    if (x < value) ++i;
    return i*step;
}

int getNormalizedIndex(const double& value, const double& start, const double& step){
  int index = getIndex(getLowerBound(value, step), step);
  int normalization_index = getIndex(start, step);
  index -= normalization_index;
  return std::move(index);
}

double getLowerBoundFromIndex(const double lower, const int& index, double& step){
  return lower + index*step;
}

double getHigherBoundFromIndex(const double lower, const int& index, double& step){
  return getLowerBoundFromIndex(lower, index,step) + step;
}