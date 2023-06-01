void macro2() {
	TF1 *fun = new TF1("fun", "1./[0] * TMath::Exp(-x/[0])", 0, 8);
	fun->SetParameter(0, 3);
	
	int N = 10000;
	
	TH1D *hUniform = new TH1D("hUniform","", 100, 0, 8);
	TH1D *hUniform_rand = new TH1D("hUniform_rand","", 100, 0, 8);
	ifstream ifile;
	ifile.open("Dataout_zad6_3.dat");
	double val;
	
	for(int i = 0; i < N; i++){
		ifile >> val;
		hUniform->Fill(-3*TMath::Log(1-(double)val));
		gRandom->Rndm(1);
		hUniform_rand->Fill(-3*TMath::Log(1-gRandom->Uniform(0,1)));
	}
	
	ifile.close();
	TCanvas *c1 = new TCanvas();
	
	hUniform->Scale(1./(hUniform->Integral("width")));
	hUniform_rand->Scale(1./(hUniform_rand->Integral("width")));
	hUniform->Draw();
	fun->Draw("same");
	hUniform_rand->SetMarkerStyle(kFullSquare);
	hUniform_rand->Draw("same");
}
