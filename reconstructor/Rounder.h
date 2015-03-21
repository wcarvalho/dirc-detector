#ifndef __Rounder__
#define __Rounder__

#include "TMath.h"

using namespace std;

class Rounder
{
public:
  Rounder(const double& step_size, double min_val = 0): step(step_size), lower(min_val) { }
  ~Rounder(){}

  void setStepSize(const double& step_size){ step = step_size; }
  double getStepSize(){ return step; }
  double getMinimumValue(){ return lower; }
  void setMinimumValue(const double& min_val){ lower = min_val; }

  double round(const double& value){
    return (TMath::Nint(value*(1/step))/double(1/step));
  }

  int getIndex(const double& value){
    return (int)(value/step);
  }

  double getLowerBound(const double& value){
      double x = round(value);
      int i = getIndex(x);
      if (x > value) --i;
      return i*step;
  }

  double getHigherBound(const double& value){
      double x = round(value);
      int i = getIndex(x);
      if (x < value) ++i;
      return i*step;
  }

  int getNormalizedIndex(const double& value, const double& start){
    int index = getIndex(getLowerBound(value));
    int normalization_index = getIndex(start);
    index -= normalization_index;
    return std::move(index);
  }

  double getLowerBoundFromIndex(const int& index){
    return index*step;
  }

  double getHigherBoundFromIndex(const int& index){
    return getLowerBoundFromIndex(index) + step;
  }

private:
  double step;
  double lower;
};

#endif