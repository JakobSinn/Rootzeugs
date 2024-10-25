double M_mu=0.105658; //GeV;
double M_e=5.11e-06; // GeV
double dedx_muon(double *x, double *par);


void draw_dedx(){

  gStyle->SetOptLogy(1);
  gStyle->SetOptLogx(1);

  TF1 *f1 = new TF1("dedx_air",dedx_muon,M_mu,1000,1);
  TF1 *f2 = new TF1("dedx_rock",dedx_muon,M_mu,1000,1);
  f1->SetParameter(0,0);
  f1->SetLineColor(4);
  f1->Draw();
  f2->SetParameter(0,1);
  f2->SetLineColor(2);
  f2->Draw("same");
  f1->GetXaxis()->SetTitle("Energy (GeV)");
  f1->GetYaxis()->SetTitle("dE/dx (GeV/(g/cm2))");

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
