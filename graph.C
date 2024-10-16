void graph(){
  float x[10] = {1,2,3,4,5,10,12,14,18,25};
  float y[10] = {4,5,2,3,5,8,19,6,3,4};
  float yerr[10] = {0.1, 0.2, 0.1, 0.3, 0.3, 0.4, 1, 0.6, 0.2, 0.1}; 
  TGraph* test = new TGraphErrors(10,x,y,yerr,yerr);
  test->SetMarkerStyle(20);
  test->SetMarkerSize(0.8);
  test->Draw("APL");
}
