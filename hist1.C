void hist1(){
  TH1F* hist_test = new TH1F("test1testvariables", "testtitle", 100,0,100);
  int sum=0;

  for( int i=1; i<=100; i++) {
    sum = sum+i;
    if (i > 50 && i < 60) continue;
    hist_test->Fill(i);
  }
  hist_test->SetFillClor(
  hist_test->Draw();
}
