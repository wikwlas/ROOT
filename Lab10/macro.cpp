double chi2(TH1D *h, TF1 *f, int kmax) {
	double sum = 0;
	double tmp = 0;
	for (int i = 0; i < kmax; i++) {
		tmp = (h -> GetBinContent(i+1) - f -> Eval(i)) * (h -> GetBinContent(i+1) - f -> Eval(i));
		sum += tmp / (f -> Eval(i));
	}
	return sum;
}

int testchi2(double T, double alpha, int ndf) {
	TF1 *chi = new TF1("chi", "[0] * TMath::Power(x, [1] - 1) * TMath::Exp(-1 * x/2)", 0, 20);
	int k = 1 / (TMath::Gamma(ndf) * TMath::Power(2, ndf));
	chi -> SetParameter(0, k);
	chi -> SetParameter(1, ndf);
	double checking = chi -> Integral(0, T);
	if ((1 - alpha) < checking) {
		return 1;
	}
	else 
		return 0;
}


void macro(void) {
    //Histogram rozkladu czestosci
	TH1D *hist = new TH1D("hist", "Wynik eksperymentu", 9, 0, 9);
	ifstream ifile;
	ifile.open("dane10.txt");
	double val1, val2;
    double lambda;
    double sum1 = 0., sum2 = 0.;
	while (ifile >> val1) {
		ifile >> val2;
		hist -> SetBinContent(val1+1, val2);
        sum1 += val1*val2;
        sum2 += val2;
	}
	ifile.close();

	TCanvas *c1 = new TCanvas ("c1", "Zadanie 10", 1200, 800);
    hist -> SetFillColorAlpha(45, 0.35);
    hist -> GetXaxis() -> SetTitle("k");
	hist -> GetYaxis() -> SetTitle("n_{k}, np_{k}");
	hist -> Draw();

    lambda = sum1 / sum2;
    cout << "Lambda najwiekszej wiarygodnosci: " << lambda << endl;

    //Dopasowanie Poissona
    TF1 *fitted = new TF1("poifit", "[1]*TMath::PoissonI(x, [0])", 0, 9);
	fitted -> SetParameter(1, hist -> Integral());
	fitted -> SetParameter(0, 1);
	hist -> Fit("poifit", "LR", 0, 9);
	fitted = hist -> GetFunction("poifit");
    fitted -> SetLineColor(kRed);
	fitted -> Draw("same");
	
	
	TF1 *poison = new TF1("poi", "[1]*TMath::PoissonI(x, [0])", 0, 9);
	poison -> SetParameter(0, lambda);
	poison -> SetParameter(1, hist -> Integral());
	poison -> SetLineColor(kBlue);
	poison -> Draw("same");

    cout << "Lambda najwiekszej wiarygodnosci: " << lambda << endl;
    cout << "Lambda (ROOT Fit): " << fitted -> GetParameter(0) << endl;

	double T = chi2(hist, fitted, 9);
	int ndf = 6;
	double alpha = 0.01;
	int checking = testchi2(T, alpha, ndf);
	cout << "chi2 (wartosc statystyki testowej T): " << T << endl;
	cout << "chi2 / NDF: " << T / ndf << endl;
	
	
	cout << "Chi2 (ROOT Fit): " << fitted -> GetChisquare() << endl;
	cout << "Chi2 (ROOT Fit) / NDF: " << fitted -> GetChisquare() / ndf << endl;
    cout << "Poziom istotnosci alpha: " << alpha << endl;
	if (checking == 1)
		cout << "Wynik testu: odrzucamy hipotezę" << endl;
	else
		cout << "Wynik testu: nie ma podstaw do odrzucenia hipotezy" << endl;
	
}
