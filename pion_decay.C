double M_pi = 0.13977;
double Tau_pi = 2.6E-08;
double M_mu = 0.105658;
double M_neu = 0;
double C = 3e08;

TRandom *r;

//double Get_Decay_Length(double e, double alss, double t);

void pion_decay(){
  r = new TRandom();

  TCanvas *c1 = new TCanvas("c1", "c1", 10, 10, 600, 300);
  c1->Divide(2,1);

  TH1F *hist_theta = new TH1F("hist_t", "Muon Theta", 100, 170, 180);
  TH1F *hist_energy = new TH1F("hist_l", "Muon Energy", 100, 0, 1);

  for (long i=0; i<1000000 ;i++){
    if (i%100000==0&&i>1) printf("Event %ld\n",i);
    double px = 0;
    double py = 0;
    double pz = -1;

    double e = sqrt(px*px+py*py+pz*pz+M_pi*M_pi);

    TLorentzVector Pion(px, py, pz, e);
    Double_t decay_particle_masses[2] = {M_mu*0.5, M_mu*0.5};
    TGenPhaseSpace Event;
    Event.SetDecay(Pion,2, decay_particle_masses);
    Event.Generate();

    TLorentzVector Muon = *(Event.GetDecay(0));
    TLorentzVector Neu=*(Event.GetDecay(1));

    double Mu_theta=Muon.Theta()*TMath::RadToDeg();
    double Mu_e = Muon.E();

    hist_theta->Fill(Mu_theta);
    hist_energy->Fill(Mu_e);
  }

  c1->cd(1);
  hist_theta->Draw();
  hist_theta->SetXTitle("Theta Muon [deg]");

  c1->cd(2);
  hist_energy->Draw();
  hist_energy->SetXTitle("Energy [GeV]");
}

//double Get_Decay_Length(double e, double mass, double t){
//double gamma = e/mass;
//double decay_length=C*t*gamma/1000; //from  to km
//return decay_length;
//}
