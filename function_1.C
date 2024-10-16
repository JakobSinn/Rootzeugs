double meinefunc(double *x, double *par){
  return par[0]*x[0]*x[0]+par[1]*x[0]+par[2];
}

void function_1(){
  TF1 *test1 = new TF1("test1", "meinefunc", 0,10,3);
  test1->SetParameters(1,-5,2);
  test1->SetLineStyle(1);
  test1->SetLineColor(2);
  test1->Draw();
  //Ab hier nächste Linie
  TF1 *test2 = (TF1*)test1->Clone("same");
  test2->SetLineColor(3);
  test2->SetParameters(-2,6,30);
  test2->Draw("same");
}
