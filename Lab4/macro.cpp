double fnc(double *x, double *params) {
	if (x[0] >= 0 && x[0] <= TMath::Pi()/2 && x[1] >= 0 && x[1] <= TMath::Pi()/2)
		return params[0]*TMath::Sin(x[0]*x[1]);
	else
		return 0;
}

void macro() {
	TCanvas *c1 = new TCanvas();
	c1->Divide(2, 2);
	
	TF2 *fun = new TF2("fun1", fnc, 0, TMath::Pi()/2, 0, TMath::Pi()/2, 1);
	fun->SetParameters(1);
	
	double c = 1./fun->Integral(0, TMath::Pi()/2, 0, TMath::Pi()/2);
	fun->SetParameters(c);
	
	TH2D *fun2 = new TH2D("fun2", "histogram", 20, 0, TMath::Pi()/2, 20, 0, TMath::Pi()/2);
	
	double  n, m;
	for (int i = 0; i < 100000; i++) {
		fun->GetRandom2(n, m);
		fun2->Fill(n, m);
	}
	
	
	fun2->Scale(1./fun2->Integral(1, 20, 1, 20, "width"));
	c1->cd(1);
	fun2->Draw("lego2");
	
	
	TH2D *fun3 = new TH2D("fun3", "histogram dystrybuanty", 20, 0, TMath::Pi()/2, 20, 0, TMath::Pi()/2);
	
	for (int i = 1; i <= 20; i++) {
		for (int j = 1; j <= 20; j++) {
			fun3->SetBinContent(i, j, fun2->Integral(0, i, 0, j));
		}
	}
	
	c1->cd(2);
	fun3->Draw("lego2");
	
	
	TH1D *fung = fun2->ProjectionX();
	c1->cd(3);
	fung->SetTitle("g(x)");
	fung->Draw();
	
	
	TH1D *funh = fun2->ProjectionY();
	c1->cd(4);
	funh->SetTitle("h(x)");
	funh->Draw();
	
	
	double Ex = fun2->GetMean(1);
	double Ey = fun2->GetMean(2);
	
	
	cout << "E(x): " << Ex << endl;
	cout << "E(y): " << Ey << endl;
	
	
	double sigmax = fun2->GetRMS(1);
	double sigmay = fun2->GetRMS(2);
	
	cout << "sigma(x): " << sigmax << endl;
	cout << "sigma(y): " << sigmay << endl;
	
	double covxy = fun2->GetCovariance(1,2);
	double cof = fun2 -> GetCorrelationFactor(1,2);
	
	cout <<"cov(x,y): " <<covxy << endl;
	cout <<"cof(x,y):" <<cof << endl;
}
