double chi(double *x, double *n) {
	if(x[0] > 0)
		return (TMath::Power(x[0], n[0]/2 -1)*TMath::Exp(-x[0]/2))/(TMath::Gamma(n[0]/2)*TMath::Power(2, n[0]/2));
	else
		return 0;
}

void splot(int k, TH1D *h, int n){
	double sum = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < k; j++) {
			sum += gRandom->Uniform(0, 1);
		}
		h->Fill(sum);
		sum = 0;
	}
}


void macro(){
	TCanvas *c1 = new TCanvas();
	c1-> Divide(1,2);
	c1->cd(1);
	TF1 *fun1 = new TF1("Rozklad Chi", chi, 0, 30, 1);
	fun1->Draw();
	c1->cd(2);
	fun1->DrawIntegral();	
	
	for (int i = 1; i < 20; i++){
		c1->cd(1);
		fun1 = new TF1(Form("fun%d", i), chi, 0, 30, 1);
		fun1->SetParameter(0, i);
		fun1->Draw("Same");
		fun1->SetLineColor(i);
		c1->cd(2);
		fun1->DrawIntegral("Same");	
	}

	TCanvas *c2 = new TCanvas("c2", "Splot", 500, 500);
	c2 -> cd(1);
	TF1 *gauss = new TF1();
	TH1D *hist;
	int i = 2;
	int n = 10000;
	double stopswob = 1, chi = 2;
	while(chi/stopswob >= 1) {
		hist = new TH1D("hist", "Splot", 100, 0, 10);
		splot(i, hist, n);
		hist -> Fit("gaus");
		gauss = hist -> GetFunction("gaus");
		stopswob = gauss -> GetNDF();
		chi = gauss -> GetChisquare();
		i++;
	}
	
	
	cout << "Zrobiono splot " << i << "  rozkladow" << endl;
	cout << chi/stopswob << endl;
	
	hist -> Draw();
	gauss -> Draw("Same"); 
}