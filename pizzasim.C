void splitbyn(double in, double branch[], double minimum, int piecenum);

TRandom* r;

void pizzasim(){
    r = new TRandom();
    r -> SetSeed(23434);
    TCanvas* pizza = new TCanvas("sim2");
    TH1F* hist_e = new TH1F("E", "E-Hist", 100, 0, 150);
    TH1F* hist_n = new TH1F("N", "N-Hist", 100, 0, 50);

    const int n_branches = 4;
    
    float E = 1000.;
    float M_p = 0.938;
    double M_pi = 0.13977;


    for (int i=0; i<100000; i++){
      if (i % 50000 == 0)
      printf("Simulation %d wird angefangen\n ",i);
      double E_A=0;
      double E_B=0;
      double eab[2];
      if (E>2*M_p) splitbyn(E, eab, M_p, 2);
      E_A = eab[0];
      E_B = eab[1];
      double E_branch[n_branches];
      if (E_B>n_branches*M_pi) splitbyn(E_B, E_branch, M_pi, n_branches);

      int npi=0;
      for (int j=0; j<n_branches; j++){
	double E_in = E_branch[j];
	double E_a, E_b;
	while (E_in>2*M_pi){
	  splitbyn(E_in, eab, M_pi, 2);
	  E_a = eab[0];
	  E_b = eab[1];
	  double E_pi = E_a;
	  hist_e->Fill(E_pi);
	  E_in = E_b;
	  npi++;
	}
      }
      hist_n -> Fill(npi);
      
    }
    pizza->Divide(2,1);
    pizza->cd(1);
    hist_n -> SetXTitle("N of pions");
    hist_n -> Draw();
    pizza->cd(2);
    hist_e->SetXTitle("pion energy [GeV]");
    //Funktioniert nicht hist_e -> SetLogY();
    hist_e->Draw();
    pizza -> Draw();
}

void splitbyn(double in, double branch[], double minimum, int piecenum){  

  //printf("Splitbyn betreten!\n");
    
  const int nrandom = piecenum - 1;
  double randnum[nrandom];

  //Zufallszahlnen generieren
  for (int i=0; i<nrandom; i++){
    randnum[i] = r->Rndm();
    //printf("Zufallszahl %f",randnum[i]);
  }

  //Sortieren
  for (int i=0; i<nrandom-1; i++){
    for (int j=i; j<nrandom; j++){
      if (randnum[i] > randnum[j]){
	double temp = randnum[i];
	randnum[i]=randnum[j];
	randnum[j] = temp;
      }
    }
  }
  //printf("New Simulation:\n");
  for (int i=0; i<nrandom; i++){
    //printf("Zufallszahl %d ist %f\n", i, randnum[i]);
  }

  
  double budget = in - piecenum*minimum;
  branch[0] = randnum[0]*budget+minimum;
  for (int i=1; i<piecenum-1; i++){
    branch[i]=(randnum[i]-randnum[i-1])*budget+minimum;
  }
  branch[piecenum-1] = (1-randnum[nrandom-1])*budget+minimum;
  for (int i=0; i<piecenum; i++){
    //printf("Stück %d ist %f\n", i, branch[i]);
  }
}

