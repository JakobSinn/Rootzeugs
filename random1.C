#include <math.h>

float g(float x){
  return float(cbrt(double(3.)) * cbrt(double(x)));
};

void random1(){
  TFile *f = new TFile("beispieltuple.root", "RECREATE");

  TNtuple *ntuple = new TNtuple("ntuple", "ntuple", "x:y:z");

  TRandom *r = new TRandom();

  for (int i=0; i<100000; i++){
    float x = r->Gaus(0,1);
    float y = r->Gaus(0,1);
    float z = r->Gaus(0,1);
    ntuple->Fill(x,y,z);
  }

  TCanvas *c1 = new TCanvas("c1", "c1", 100, 100, 600, 300);
  c1 -> Divide(3,1);

  c1 -> cd(1);
  ntuple->Draw("x");
  c1 -> cd(2);
  ntuple->Draw("y:x");
  ntuple->SetMarkerColor(2);
  ntuple->Draw("y:x","sqrt(x**2+y**2)<1","same");
  ntuple->SetMarkerColor(4);
  c1 -> cd(3);
  ntuple -> Draw("z:y:x");
  ntuple->SetMarkerColor(2);
  ntuple->Draw("z:y:x","sqrt(x**2+y**2)<1","same");
  ntuple->SetMarkerColor(4);
  c1 -> cd();

  f->Write();
}

