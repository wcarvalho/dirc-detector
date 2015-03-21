#include <map>
#include <vector>

#include "TH1D.h"
#include "TMath.h"

#include "GaussianIntegralGradHessian.h"
#include "ChiSquareGradHessian.h"
#include "NewtonMinimizerGradHessian.h"

using namespace std;
using namespace Eigen;
using namespace FitNewton;

class GaussianPlusConstant : public FunctionGradHessian
{
public:
  GaussianPlusConstant() : FunctionGradHessian(4, 2), width_lo(0.), width_hi(0.), center_lo(0.), center_hi(0.)
  {

  }
  ~GaussianPlusConstant(){}

  bool calcValGradHessian(const Eigen::VectorXd& x, double& val, Eigen::VectorXd& grad, Eigen::MatrixXd& hessian)
  {
    gaus.setFixedPar(0, fixedpars[0]);
    gaus.setFixedPar(1, fixedpars[1]);
    gaus.calcValGradHessian( x, val, grad, hessian );
    val += x(3)*( fixedpars[1] - fixedpars[0] );
    grad(3) = 1.*( fixedpars[1] - fixedpars[0] );

    hessian(0,3) = 0.;
    hessian(1,3) = 0.;
    hessian(2,3) = 0.;
    hessian(3,3) = 0.;
    hessian(3,0) = 0.;
    hessian(3,1) = 0.;
    hessian(3,2) = 0.;

    if( (x(1)<center_lo) || (x(1)>center_hi) || (x(2)<width_lo) || (x(2)>width_hi) ){return false;}

    return true;
  }

  void setWidthBounds( double lo, double hi ){width_lo = lo;width_hi = hi;}
  void setCenterBounds( double lo, double hi ){center_lo = lo;center_hi = hi;}

  FunctionGradHessian* Clone() const
  {
    GaussianPlusConstant* clone = new GaussianPlusConstant( *this );
    clone->setFixedPar(0, fixedpars[0]);
    clone->setFixedPar(1, fixedpars[1]);
    return clone;
  }

  private:
    GaussianIntegralGradHessian gaus;
    double width_lo;
    double width_hi;
    double center_lo;
    double center_hi;
};


void FitGaussianPlusConstant(TH1D &h, double xlow, double xhi, double centerbounds_low, double centerbounds_hi, double widthbounds_low, double widthbounds_hi, double& center, double& width, double& constant, double& height, unsigned converge_at){

  static NewtonMinimizerGradHessian minimizer;
  static GaussianPlusConstant gpc;
  static ChiSquareGradHessian chi2(&gpc);
  minimizer.setFunction(&chi2);

  static vector<vector<double> > points;
  static vector<double> data;
  static vector<double> errors;
  static vector<double> one_point(2,0.);

  double pi = TMath::Pi();

  points.clear();
  data.clear();
  errors.clear();

  int lbin = h.FindBin(xlow);
  int hbin = h.FindBin(xhi);

  double yhi = 0.;
  double ylo = 0.;

  for(int i=lbin;i<=hbin;++i)
  {
    data.push_back( h.GetBinContent(i) * h.GetBinWidth(i) );
    one_point[0] = h.GetBinLowEdge(i);
    one_point[1] = h.GetBinLowEdge(i) + h.GetBinWidth(i);
    points.push_back(one_point);
    if(h.GetBinContent(i) < 1.)
    {
      errors.push_back( h.GetBinWidth(i) );
    }
    else
    {
      errors.push_back( h.GetBinWidth(i) * sqrt( h.GetBinContent(i) ) );
    }

    if(i == lbin)
    {
      yhi = data.back();
      ylo = data.back();
    }
    else
    {
      if( data.back() < ylo ){ ylo = data.back(); }
      if( data.back() > yhi ){ yhi = data.back(); }
    }
  }

  chi2.setPoints(points);
  chi2.setData(data);
  chi2.setErrors(errors);

  gpc.setCenterBounds(centerbounds_low, centerbounds_hi);
  gpc.setWidthBounds(widthbounds_low, widthbounds_hi);


  VectorXd start = VectorXd::Zero(4);
  start[0] = (yhi - ylo)/h.GetBinWidth(lbin);
  start[1] = center;
  start[2] = width;
  start[3] = ylo/h.GetBinWidth(lbin);

  VectorXd min_point = VectorXd::Zero(4);

  unsigned int count = 0;
  bool converged = false;
  // minimizer.fixParameter(1);
  while( converged == false )
  {
    converged = minimizer.minimize( start, min_point, 1.0e-5 );
    count += 1;
    start = min_point;
    if(count == converge_at){break;}
  }
  if(converged == false)
  {
    // min_point = VectorXd::Zero(4);
  }
  // else
  // {
  //   double val;
  //   Eigen::VectorXd grad = VectorXd::Zero(4);
  //   Eigen::MatrixXd hessian = MatrixXd::Zero(4,4);
  //   chi2.calcValGradHessian(min_point, val, grad, hessian);
  //   val /= (data.size());
  //   // cout<<"chi^2/ndf = "<<val<<endl;
  //   // if(val > 1.4){min_point = VectorXd::Zero(4);}
  // }
  // string name = h.GetName();

  // static TF1 f2(name.c_str(), "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) ) + [3]", xlow, xhi);
  // f2.SetRange(xlow, xhi);

  // f2.FixParameter(0, min_point(0));
  // f2.FixParameter(1, min_point(1));
  // f2.FixParameter(2, min_point(2));
  // f2.FixParameter(3, min_point(3));

  height = min_point(0);
  center = min_point(1);
  width = min_point(2);
  constant = min_point(3);
  // PushBackParams();
  // auto &params = Recon.back().Params.back();
  // for(int i=0;i<4;++i){
  //     params.push_back(f2.GetParameter(i));
  // }

  // params.push_back(xlow);
  // params.push_back(xhi);

  // // height * width * sqrt(2pi) (divided by bin width to go from counts to amount)
  // Area = min_point(0) * min_point(2) * sqrt(2*pi) / h.GetBinWidth(1);
}