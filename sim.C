TRandom* r = new TRandom3(); 
r -> SetSeed(2390976634);
float rho0 = 1.021/1000;
float lambda_proton = 87;
float starth = 100000;
long simcounter = 0;
float p_stop = 0;
float d_strecke = 0.2;

float rho(float h){
   return rho0 * exp(-h/8);
}

float howfar(float cosval){
  //Angle is expected to be in degrees!!
  int ih = 0;
  float p_stop = 0;
  float h = starth;
  while (r->Rndm() > p_stop){
     ih++;
     simcounter++;
     h = starth - cosval * d_strecke * ih;
     float dX = rho(h) * d_strecke * 100000;
     p_stop =  1./lambda_proton*dX;
     //printf("Interaktion %d: Stoppwahrscheinlichkeit: %f\n",ih,p_stop);
  }
  return h;
}

void sim(){
  float angle = 60;
  float cosval = cos(angle * M_PI / 180);
  //TCanvas* c_bricks = new TCanvas("Neutrino Propagaton Simulation");
    TH1D* hist = new TH1D("neutrinos", "Neutrino Propagation Simulation at 60 degree Angle", 100, 0, 100);
    for (int i = 0; i < 2000; i++){
      hist->Fill(howfar(cosval));
      if (i % 100 == 0){
	printf("Event %d simuliert\n",i);
      }
    }
    printf("Fertig! ");
    hist->Draw();
    printf("Simulierte Interaktionen: %ld\n",simcounter);
}
