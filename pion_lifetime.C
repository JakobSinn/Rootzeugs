double M_pi = 0.13977;
double Tau_pi = 2.6E-08;
double C = 3e08;

TRandom *r;

double Get_Decay_Length(double e, double alss, double t);

void pion_lifetime(){
  r = new TRandom();

  TCanvas *c1 = new TCanvas("c1", "c1", 10, 10, 600, 300);
  c1->Divide(2,1);

  TH1F *hist_t = new TH1F("hist_t", "Pion Decay time", 100, 0, 3e-07);
  TH1F *hist_l = new TH1F("hist_l", "Decay Length", 100, 0, 0.5);

  for (long i=0; i<10000 ;i++){
    double e = 1;
    double decay_time = r->Exp(Tau_pi);
    double decay_length = Get_Decay_Length(e, M_pi, decay_time);
    hist_t->Fill(decay_time);
    hist_l->Fill(decay_length);
  }

  c1->cd(1);
  hist_t->Draw();
  hist_t->SetXTitle("Time [s]");

  c1->cd(2);
  hist_l->Draw();
  hist_l->SetXTitle("Length [km]");
}

double Get_Decay_Length(double e, double mass, double t){
  double gamma = e/mass;
  double decay_length=C*t*gamma/1000; //from  to km
  return decay_length;
}
