#ifndef M_PI
#define M_PI 3.14159265358979312
#endif

#include "../headers/generator.h"

static inline double sign(double x)
{
  return ((double)(x > 0.)) - ((double)(x < 0.));
}


static inline int sign(int x)
{
  return ((x > 0) - (x < 0));
}


// pT in GeV/c , B in T , kappa in cm^-1
static inline double pT_to_kappa(double B, double pT)
{
  return 0.003*B/pT;
}


static inline double kappa_to_pT(double B, double kappa)
{
  return 0.003*B/kappa;
}



bool intersect_circles(bool hel, double startx, double starty, double rad_det, double k_trk, double phi_trk, double d_trk, double& x, double& y)
{
  double kcx = (d_trk*k_trk + 1.)*cos(phi_trk);
  double kcy = (d_trk*k_trk + 1.)*sin(phi_trk);
  double k2d2 = (d_trk*k_trk + 1.)*(d_trk*k_trk + 1.);
  double kd = sqrt(k2d2);
  if(kd > (k_trk*rad_det + 1.)){return false;}
  if(kd < fabs(k_trk*rad_det - 1.)){return false;}
  double kd_inv = 1./kd;
  double R2 = rad_det*rad_det;
  double a = 0.5*(k_trk*R2 + ( d_trk*d_trk*k_trk + 2.*d_trk ))*kd_inv;
  double tmp1 = a*kd_inv;
  double P2x = kcx*tmp1;
  double P2y = kcy*tmp1;
  
  double h = sqrt(R2 - a*a);
  
  double ux = -kcy*kd_inv;
  double uy = kcx*kd_inv;
  double P3x1 = P2x + ux*h;
  double P3y1 = P2y + uy*h;
  ux = -ux;
  uy = -uy;
  double P3x2 = P2x + ux*h;
  double P3y2 = P2y + uy*h;
  
  double d1_2 = (startx - P3x1)*(startx - P3x1) + (starty - P3y1)*(starty - P3y1);
  double d2_2 = (startx - P3x2)*(startx - P3x2) + (starty - P3y2)*(starty - P3y2);
  
  //   if((d1_2 < d2_2) == hel)
  if(!hel)
  {
    x = P3x1;
    y = P3y1;
  }
  else
  {
    x = P3x2;
    y = P3y2;
  }
  
  return true;
}


bool intersect_helix_cylinder(bool hel, double startx, double starty, double startz, double rad_det, double k_trk, double phi_trk, double d_trk, double dzdl_trk, double& x, double& y, double& z)
{
  bool madeit = intersect_circles(hel, startx, starty, rad_det, k_trk, phi_trk, d_trk, x, y);
  if(madeit==false)
  {
    return false;
  }
  
  double k = k_trk;
  double D = sqrt((startx-x)*(startx-x) + (starty-y)*(starty-y));
  
  double s=0.;
  if(0.5*k*D > 0.1)
  {
    double v = 0.5*k*D;
    if(v >= 0.999999){v=0.999999;}
    s = 2.*asin(v)/k;
  }
  else
  {
    double temp1 = k*D*0.5;temp1*=temp1;
    double temp2 = D*0.5;
    s += 2.*temp2;
    temp2*=temp1;
    s += temp2/3.;
    temp2*=temp1;
    s += (3./20.)*temp2;
    temp2*=temp1;
    s += (5./56.)*temp2;
  }
  double dz = sqrt(s*s*dzdl_trk*dzdl_trk/(1. - dzdl_trk*dzdl_trk));
  if(dzdl_trk>0.){z = startz + dz;}
  else{z = startz - dz;}
  return true;
}



bool intersect_with_dirc(double dirc_height, double eta, double pt, double phi_input, double mass, int charge, double radius, double magfield, double& x_dirc, double& y_dirc, double& phi_dirc, double& theta_dirc, double& beta)
{
  double vx=0.;
  double vy=0.;
  double vz=0.;
  
  double B = magfield;
  
  double px = pt*cos(phi_input);
  double py = pt*sin(phi_input);
  double theta = 2.*atan( exp( -eta ) ) - 0.5*M_PI;
  // pt = p*cos(theta)
  // pz = p*sin(theta) = pt*tan(theta)
  double pz = pt*tan(theta);
  
  double p = sqrt(px*px + py*py + pz*pz);
  double k = pT_to_kappa(B, pt);
  double dzdl = pz/p;
  double p_inv = 1./p;
  double ux = px*p_inv;
  double uy = py*p_inv;
  double uz = pz*p_inv;
  double phi_u = atan2(uy,ux);
  double dphi = -sign((double)(charge))*0.5*M_PI;
  double phi = phi_u + dphi;
  
  //convert back to coordinates 0,0,0 -> vx,vy,vz
  double cosphi = cos(phi);
  double sinphi = sin(phi);
  double tx = cosphi + k*vx;
  double ty = sinphi + k*vy;
  double dk = sqrt( tx*tx + ty*ty ) - 1.;
  double d = dk/k;
  phi = atan2(ty, tx);
  // now solve for the new z0
  double dx = d*cosphi;
  double dy = d*sinphi;
  double D = sqrt( (vx-dx)*(vx-dx) + (vy-dy)*(vy-dy) );
  double s=0.;
  if(0.5*k*D > 0.1)
  {
    s = 2.*asin(0.5*k*D)/k;
  }
  else
  {
    double temp1 = k*D*0.5;temp1*=temp1;
    double temp2 = D*0.5;
    s += 2.*temp2;
    temp2*=temp1;
    s += temp2/3.;
    temp2*=temp1;
    s += (3./20.)*temp2;
    temp2*=temp1;
    s += (5./56.)*temp2;
  }
  double dz = sqrt(s*s*dzdl*dzdl/(1. - dzdl*dzdl));
  double z0 = 0.;
  if(dzdl > 0.){z0 = vz - dz;}
  else{z0 = vz + dz;}
  
  // we now have k,d,phi,dzdl,z0
  
  
  
  bool hel = (bool)((sign(charge) + 1)>>1);
  double x=0.;double y=0.;double z=0.;
  bool madeit = intersect_helix_cylinder(hel, 0.,0.,0., radius, k, phi, d, dzdl, x, y, z);
  
  x_dirc = z;
  
  double phi_intersect = atan2(y,x);
  double phi_span = 0.5*dirc_height/radius;
  if(fabs(phi_intersect) > phi_span){madeit = false;}
  
  
  double phi_out = atan2((1. + k*d)*sin(phi) - k*y, (1. + k*d)*cos(phi) - k*x);
  double px_new = pt*cos(phi_out + M_PI/2.);
  double py_new = pt*sin(phi_out + M_PI/2.);
  double pz_new = dzdl*sqrt(px_new*px_new + py_new*py_new)/sqrt(1. - dzdl*dzdl);
  
  double p_new = sqrt(px_new*px_new + py_new*py_new + pz_new*pz_new);
  
  // m*b/sqrt(1 - b^2) = p
  // m^2*b^2 = p^2*(1 - b^2)
  // b = p/sqrt( p^2 + m^2 )
  beta = p_new/sqrt(p_new*p_new + mass*mass);
  
  y_dirc = radius*sin(phi_intersect);
  
  theta_dirc = acos( px_new/p_new );
  phi_dirc = atan2( py_new, pz_new );
  
  return madeit;
}


