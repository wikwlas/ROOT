int g = 16807;
int m = 147483647;
long seed = time(NULL);
	
double uniform(double max) {
	int x1 = (g*seed)%m;
	seed = x1;
	return x1*max/m;
}
void macro() {
	TCanvas *c1 = new TCanvas();
	c1->Divide(2, 2);
	
	TH1D *hUniform1 = new TH1D("hUniform1", "", 100, 0, 1);
	TH2D *hCorr1 = new TH2D("hCorr1", "", 100, 0, 1, 100, 0, 1);
	
	int N = 10000;
	double range = 1.0;
	
	ofstream ofile;
	ofile.open("Dataout_zad6_3.dat");
	
	double x0, x11;
	for (int i = 0; i < N; i++) {
		x0 = uniform(range);
		x11 = uniform(range);
		ofile << x0 << endl;
		hCorr1->Fill(x0, x11);
		hUniform1->Fill(x0);
	}
	
	ofile.close();
	c1->cd(1);
	hCorr1->Draw("colz");
	
	c1->cd(2);
	hUniform1->Draw();
	
	c1->SaveAs("Zad6_3.png");
}
