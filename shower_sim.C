#include <list>
void Ptcl_Register(int id, int flag, double px, double py, double pz, double x, double y, double z, double t);
void Hillas_Split(int id, double E, double theta, double phi, double x, double y, double z, double t);
void E_split_N(double in, double branch[]);
void E_split_two(double E_in, double &E_A, double &E_B, double minimum);
void Get_Int_Position(int id, double theta, double phi, double h0, double&x_int, double &y_int, double &z_int);
double Get_H_from_X(double X, double h0, double theta);
void Make_CR_Shower(int id_primary, double e_primary, double theta, double phi);
double Get_Decay_Length(double e, double mass, double t);
void Pion_Decay(double px, double py, double pz, double x, double y, double z, double t);
void Get_Muon_Decay_Position(double e, double px, double py, double pz, double x, double y, double z, double &decay_x, double &decay_y, double &decay_z);

double dxde_muon (double *x, double *par);
double Get_Muon_Range(double e, int material_code);
double dedx_muon(double *x, double *par);
void Get_Muon_Stop_Position(double E, double px, double py, double pz, double x, double y, double z, double &stop_x, double &stop_y, double &stop_z);
			    

TRandom *r;
double pi = TMath::Pi();

double M_p = 0.938;
double M_pi = 0.13977;
double M_mu = 0.105658;
double M_neu = 0;
double M_e = 511e-06;
double Tau_pi = 2.6E-08;
double Tau_mu = 2.2E-06;
double C = 3e08;
double e_threshold_had_int = 10;//GeV

const int iterations = 1000;

int pdg_p=2212;
int pdg_pi=211;
int pdg_mu=13;
int pdg_neutrino=14;

double rho0=1.021/1000;
double A = 8;
const int n_branches = 4;

struct Particle{
  int id;
  int flag;
  double e;
  double t;
  double px;
  double py;
  double pz;
  double x;
  double y;
  double z;
};

std::list<Particle> particle_bank;

void Make_CR_Shower(int id_primary, double e_primary, double theta, double phi){
  double h0=1.e10;
    double t0=0;
    double x_int, y_int, z_int;

    particle_bank.clear();
  //void Get_Int_Position(int id, double theta, double phi, double h0, double&x_int, double &y_int, double &z_int);


    Get_Int_Position(id_primary, theta, phi, h0, x_int, y_int, z_int);
    //printf("Printing data from event no %d as recieved by main function, x, y, z\n",ievt);
    //printf("%f %f %f\n", x_int, y_int, z_int);
    Hillas_Split(id_primary, e_primary, theta, phi, x_int, y_int, z_int, t0);

    for (std::list<Particle>::iterator it = particle_bank.begin(); it != (particle_bank.end()); ++it){
      int id = it->id;
	int flag = it->flag;
	double e= it->e;
	double t = it->t;
	double px = it->px;
	double py =	   it->py;
	double pz =	   it->pz;
	double x =	   it->x;
	double y =	   it->y;
	double z =	   it->z;

	if (id==pdg_p || id == pdg_pi) {
	  Get_Int_Position(id, theta, phi, z, x_int, y_int, z_int);

	  double intr_length=sqrt((x-x_int)*(x-x_int)+(y-y_int)*(y-y_int)+(z-z_int)*(z-z_int));

	  double decay_time = 1.e20;
	  double decay_length = 1.e20;

	  if (id==pdg_pi) {
	    decay_time = r-> Exp(Tau_pi);
	    decay_length = Get_Decay_Length(e, M_pi, decay_time); //km
	  }

	  if (id==pdg_p || intr_length<decay_length) {
	    if (z_int > 0 && e>e_threshold_had_int){
	      double intr_time = intr_length*1000./C;
	      Hillas_Split(id, e, theta, phi, x_int, y_int, z_int, t+intr_time);
	      it->flag=2;
	    }
	  } else {
	    double pi_p = sqrt(px*px+py*py+pz*pz);
	    double pi_decay_x = x+decay_length*px/pi_p;
	    double pi_decay_y = y+decay_length*py/pi_p;
	    double pi_decay_z = z+decay_length*pz/pi_p;

	   
	    double decay_time_lab = decay_length * 1000. / C;
	    if (pi_decay_z>0){
	      Pion_Decay(px, py, pz, pi_decay_x, pi_decay_y, pi_decay_z, t+decay_time_lab);
	      it->flag=3;
	    }
	  }
	}
    }
}

void shower_sim(){
  TNtuple *ntuple = new TNtuple("ntuple","ntuple","evt:id:flag:e:px:py:pz:x:y:z");

  TH1F *hist_all = new TH1F("hist_all", "hist_all", 100, 0, 500);
  TH1F *hist_decay = new TH1F("hist_decay", "hist_decay", 100, 0, 500);
  TH1F *hist_stop = new TH1F("hist_stop", "hist_stop", 100, 0, 500);
  TH1F *hist_arrival = new TH1F("hist_arrival", "hist_arrival", 100, 0, 500);
  
  r = new TRandom();

  double e_threshold_had_int = 10;

  TCanvas *sky = new TCanvas("sky", "sky", 300, 10, 500, 800 );
  TView *view = TView::CreateView(1,0,0);
  view->ShowAxis();
  view->SetRange(-5, -5, 0, 5, 5, 20);

  for (int ievt=0; ievt<iterations; ievt++){
    if (ievt % 100 == 0 && ievt > 0) printf("Simulating particle: %d\n", ievt);
    int id_primary = pdg_p;
    double e_primary = 1000;
    
    //double theta=0.2;
    double theta = 0;
    double phi=r->Rndm()*pi*2;

    Make_CR_Shower(id_primary, e_primary, theta, phi);

    int n_mu_all = 0;
    int n_mu_decay = 0;
    int n_mu_arrival = 0;
    int n_mu_stop = 0;
	    

	// if (id == pdg_pi && it->flag!=2){
  // 	  float decay_time = r-> Exp(Tau_pi);
  // 	  float decay_length=Get_Decay_Length(e, M_pi, decay_time);

  // 	  TLorentzVector Pion(px, py, pz, e);
  // 	  Double_t decay_particle_masses[2] = {M_mu*0.5, M_mu*0.5};
  // 	  TGenPhaseSpace Event;
  // 	  Event.SetDecay(Pion,2, decay_particle_masses);
  // 	  Event.Generate();

  // 	  TLorentzVector Muon = *(Event.GetDecay(0));
  // 	  TLorentzVector Neu=*(Event.GetDecay(1));

  // 	  float pi_p = sqrt(px*px+py*py+pz*pz);
  // 	  float pi_decay_x = x+decay_length*px/pi_p;
  // 	  float pi_decay_y = y+decay_length*py/pi_p;
  // 	  float pi_decay_z = z+decay_length*pz/pi_p;
	  
  // 	  double mu_px = Muon.Px();
  // 	  double mu_py = Muon.Py();
  // 	  double mu_pz = Muon.Pz();

  // 	  double neu_px = Neu.Px();
  // 	  double neu_py = Neu.Py();
  // 	  double neu_pz = Neu.Pz();

  // 	  Ptcl_Register(pdg_mu, 1, mu_px, mu_py, mu_pz, pi_decay_x, pi_decay_y, pi_decay_z,t);
  // 	  Ptcl_Register(pdg_neutrino, 1, neu_px, neu_py, neu_pz, pi_decay_x, pi_decay_y, pi_decay_z, t);
  // 	  it->flag = 3;
  //   }
  // }
    //Drawing
  for (std::list<Particle>::iterator it = particle_bank.begin(); it != (particle_bank.end()); ++it){
      int id = it->id;
      int flag = it->flag;
      double e= it->e;
      double px =it->px;
      double py =	   it->py;
      double pz =	   it->pz;
      double x =	   it->x;
      double y =	   it->y;
      double z =	   it->z;

      //Added for Muon

      double det_z = 0;

      if (pz > 0) continue;

      if (id == 13 && flag==1){
	double decay_x, decay_y, decay_z;

	Get_Muon_Decay_Position(e,px,py,pz,x,y,z,decay_x, decay_y, decay_z);

	if (decay_z>det_z) it->flag = 3;

	double stop_x, stop_y, stop_z;
	Get_Muon_Stop_Position(e,px,py,pz,x,y,z,stop_x,stop_y,stop_z);

	if(stop_z>det_z && stop_z>decay_z) it->flag = 4;

	n_mu_all++;

	if (it->flag==3) n_mu_decay++;
	if (it->flag==1) n_mu_arrival++;
	if (it->flag==4) n_mu_stop++;
      }

      // if (flag==1){
      // 	TLorentzVector Muon(px, py, pz, e);
      // 	TParticle *Particle_Muon  = new TParticle();

      // 	Particle_Muon->SetMomentum(Muon);
      // 	Particle_Muon->SetProductionVertex(x,y,z,0);

      // 	if (id == pdg_p) Particle_Muon->SetLineColor(1);
      // 	if (id == pdg_pi) Particle_Muon->SetLineColor(2);
      // 	if (id == pdg_mu) Particle_Muon->SetLineColor(7);
      // 	if (id == pdg_neutrino) Particle_Muon->SetLineColor(920);

      // 	Particle_Muon->Draw();

      // }
      ntuple->Fill(ievt, id, it->flag, e, px, py, pz, x, y, z);

      hist_all -> Fill(n_mu_all);
      hist_decay->Fill(n_mu_decay);
      hist_arrival->Fill(n_mu_arrival);
      hist_stop->Fill(n_mu_stop);
  }
  }
  // ntuple -> SetLineColor(1);
  // ntuple -> Draw("z", "id==211");
  // ntuple -> SetLineColor(2);
  // ntuple -> Draw("z", "id==211 && flag==2","same");
  // ntuple -> SetLineColor(4);
  // ntuple -> Draw("z", "id==211 && flag==3","same");
  // ntuple -> SetLineColor(6);
  // ntuple -> Draw("z", "id==211 && flag==1","same");

  hist_arrival->Draw();
  hist_all->SetLineColor(2);
  hist_all->Draw("same");
  hist_decay->SetLineColor(6);
  hist_decay->Draw("same");
}

void Get_Muon_Decay_Position(double e, double px, double py, double pz,
                             double x, double y, double z,
                             double &decay_x, double &decay_y, double &decay_z) {
  double decay_time = r->Exp(Tau_mu);
  double decay_length = Get_Decay_Length(e, M_mu, decay_time);

  double p = sqrt(px*px + py*py + pz*pz);
  decay_x = x + decay_length * px / p;
  decay_y = y + decay_length * py / p;
  decay_z = z + decay_length * pz / p;
}

void Get_Muon_Stop_Position(double e, double px, double py, double pz,double x, double y, double z,double &stop_x, double &stop_y, double &stop_z){
  int material_code=0; // air
  double Range = Get_Muon_Range(e, material_code);
  double X = r->Exp(Range);
  double h0=z;
  double p=sqrt(px*px+py*py+pz*pz);
  double theta=acos(-pz/p); //note the - sign, theta is in observer's frame;

  stop_z=Get_H_from_X(X, h0, theta);

  double dz=z-stop_z;
  double dI=dz*tan(theta);

  stop_x=x+dI*px/p;
  stop_y=y+dI*py/p;
}

double Get_Muon_Range(double e, int material_code) {
  TF1 *fr = new TF1 ("fr_air",dxde_muon,0.2,1000,1);
  fr->SetNpx(1000);
  fr->SetParameter(0,material_code);
  if (e<=0.2) return 0; // ignore slow muons
  double range=fr->Integral(0.2, e);
  delete fr;
  return range;
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

void Ptcl_Register(int id, int flag, double px, double py, double pz, double x, double y, double z, double t){
  double mass = 0;

  if (id==pdg_pi) mass = M_pi;
  if (id==pdg_p) mass = M_p;

  Particle particle_tmp;
  particle_tmp.id = id;
  particle_tmp.flag = flag;
  particle_tmp.e = sqrt(px*px + py*py + pz*pz + mass*mass);
  particle_tmp.t = t;
  particle_tmp.px = px;
  particle_tmp.py = py;
  particle_tmp.pz = pz;
  particle_tmp.x = x;
  particle_tmp.y = y;
  particle_tmp.z = z;

  particle_bank.push_back(particle_tmp);
}

void Hillas_Split(int id, double E, double theta, double phi, double x, double y, double z, double t){
  double E_A=0;
  double E_B=0;

  double mass;
  if (id==pdg_pi) mass = M_pi;
  else mass = M_p;

  while (E_A < mass) {
    E_split_two(E, E_A, E_B, mass);
  }

  double p = sqrt(E_A*E_A - mass*mass);
  double px = -p*sin(theta)*cos(phi);
  double py = -p*sin(theta)*sin(phi);
  double pz = -p*cos(theta);

  Ptcl_Register(id, 1, px, py, pz, x, y, z, t);

  double E_branch[n_branches];
  E_split_N(E_B, E_branch);

  for (int j=0; j<n_branches; j++){
    double E_in=E_branch[j];
    double E_a, E_b;
    while (E_in > M_pi){
      E_split_two(E_in, E_a, E_b, M_pi);
      double E_kine_pi = E_a;

      E_in = E_b - M_pi;
      double E_pi = E_kine_pi + M_pi;
      double p_pi = sqrt(E_pi*E_pi - M_pi*E_pi);
      double px_pi = -p_pi*sin(theta)*cos(phi);
      double py_pi = -p_pi*sin(theta)*sin(phi);
      double pz_pi = -p_pi*cos(theta);

      Ptcl_Register(pdg_pi, 1, px_pi, py_pi,pz_pi,x,y,z,t);
    }
  }
}

void E_split_N(double in, double branch[]){  

  //printf("Splitbyn betreten!\n");

  const int piecenum = n_branches;
  const int minimum = 0;
    
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

void E_split_two(double E_in, double &E_A, double &E_B, double minimum){
  double frac = r->Rndm();
  E_A = frac*(E_in-2*minimum)+minimum;
  E_B = E_in - E_A;
}

void Get_Int_Position(int id, double theta, double phi, double h0, double &x_int, double &y_int, double &z_int){
  double lambda = 87;
  if (id==pdg_pi) lambda = 116;
  double X = r->Exp(lambda);

  z_int=Get_H_from_X(X,h0,theta);
  double radius = z_int * tan(theta);
  x_int = radius*cos(phi);
  y_int = radius*sin(phi);
  //printf("Get_Int_Position called!\n");
  //printf("x y z X theta phi\n");
  //printf("%f %f %f %f %f %f\n", x_int, y_int, z_int, X, theta, phi);
}

double Get_H_from_X(double X, double h0, double theta){
  double costheta = fabs(cos(theta));
  double h;

  if (costheta!=0){
    h=-log(X*costheta/(rho0*A*100000)+exp(-h0/(A)))*A;
  } else h=h0;

  return h;
}
  
 double Get_Decay_Length(double e, double mass, double t){
   double gamma = e/mass;
   double decay_length = C*t*gamma/1000;
   return decay_length;
 }

 void Pion_Decay(double px, double py, double pz, double x, double y, double z, double t){
   double e = sqrt(px*px+py*py+pz*pz+M_pi*M_pi);
   TLorentzVector Pion(px, py ,pz, e);
   Double_t decay_particle_masses[2] = {M_mu, M_neu};
   TGenPhaseSpace event;
   event.SetDecay(Pion, 2, decay_particle_masses);
   event.Generate();

   TLorentzVector Muon = *(event.GetDecay(0));
   TLorentzVector Neu = *(event.GetDecay(1));

   double mu_px = Muon.Px();
   double mu_py = Muon.Py();
   double mu_pz = Muon.Pz();
   double neu_px = Neu.Px();
   double neu_py = Neu.Py();
   double neu_pz = Neu.Pz();

   Ptcl_Register(pdg_mu, 1, mu_px, mu_py, mu_pz, x, y, z, t);
   Ptcl_Register(pdg_neutrino, 1, neu_px, neu_py, neu_pz, x, y, z, t);

 }
    
  
