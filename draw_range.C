double M_mu=0.105658; //GeV
double M_e=511e-06; // GeV
double dxde_muon (double *x, double *par);
double range_muon(double *x, double *par);
double dedx_muon(double *x, double *par);
void draw_range() {
  gStyle->SetOptLogy(1);
  gStyle->SetOptLogx(1);
  TF1 *f_r1 = new TF1("range_air", range_muon,0.2,1000,1);
  f_r1-> SetParameter (0,0);
  f_r1-> SetLineColor(1);
  f_r1->Draw();
  TF1 *f_r2 = new TF1("range_rock",range_muon,0.2,1000,1);
  f_r2->SetParameter(0,1);
  f_r2->SetLineColor(2);
  f_r2-> Draw("same");

}
double range_muon(double *x, double *par){
  double E=x[0];
  TF1 *fr = new TF1("fr_air",dxde_muon,0.2,1000,1);
  if (par[0]==1) fr->SetParameter(0,1); //material code, default is 0 (air) and 1 for rock.
  double R=fr->Integral(0.2, E); //range that E becomes lower than 0.2GeV return R;
  return R;
}
double dxde_muon(double *x, double *par){
  double dedx= dedx_muon(x, par);
  double dxde=1/dedx;
  return dxde;
}

double dedx_muon(double *x, double *par) {
  double E=x[0];
  int material_code=par[0];

  double K=0.307e-03; //GeV/g/cm2
  double delta = 0; //for air and rock
  double ZA=0.49919;   //for air
  double I=85.7e-09;  //for air
  
  if (material_code==1){ 
    I=136.4e-09;     //for rock, GeV
    ZA=0.50;           //for rock
  }

  double z=1;
  double p = sqrt(E*E-M_mu*M_mu);
  double gamma = sqrt(1.+pow(p/M_mu,2));
  double beta = sqrt(1.-1./(gamma*gamma));
  double g_b = p/M_mu;
  double tmax = 2*M_e*p*p/(M_mu*M_mu+M_e*M_e+2*M_e*E);
  double dedx = K*z*z*ZA/(beta*beta)*(1/2.*log(2*M_e*g_b*g_b*tmax/(I*I))-beta*beta-delta/2.);

  return dedx;
}
