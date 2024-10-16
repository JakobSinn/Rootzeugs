void interaction_atm_angle2(){
    TCanvas* c_angle = new TCanvas("angle");
    TH1F* angle_hist = new TH1F("angle", "angle", 100, 0, 100);
    
    TRandom* r = new TRandom();
    r -> SetSeed(23434);
    
    float cos_theta = 0.5;
    float lambda = 87.; // gr / cm^2
    float dl = 10000.;  // cm
    float dl_p = dl * cos_theta;
    long int simcount = 0;
    
    for (int i=1; i<=100000; i++){
        float hight = 100.; // Km
        float rho = 1.02 * exp(-hight / 8.) / 1000.; // gr / cm^3
        float p = rho * dl / lambda;
        float x = r -> Rndm();
        while ((x > p) && (hight > 0)){
            hight = hight - (dl_p / 100000.);
            rho = 1.02 * exp(-hight / 8.) / 1000.;
            p = rho * dl / lambda;
            x = r -> Rndm();
	    simcount++;
  
        }
        angle_hist -> Fill(hight);
	if ( i % 1000 == 0 ){
	  printf("Event %d simuliert\n",i);
	}
    }
    
    angle_hist -> SetXTitle("d(Km)");
    angle_hist -> Draw();
    c_angle -> Draw();
    
}

interaction_atm_angle2();
