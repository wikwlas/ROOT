double dopasuj(int st, int n, double *tj, double *yj, double *sigmaj, double *wsp, double *bwsp) {
    TMatrixD *A = new TMatrixD(n, st);
	TMatrixD *H = new TMatrixD(n, n);

    for (int i = 0; i < n; i++) {
        (*H)(i, i) = 1. / sigmaj[i];
        for (int j = 0; j < st; j++) {
            (*A)(i, j) = TMath::Power(tj[i], j) ;
        }
    }
    TMatrixD *Aprim = new TMatrixD(*H, TMatrixD::kMult, *A);

    TMatrixD *y = new TMatrixD(n, 1);
    y -> SetMatrixArray(yj); 

    TMatrixD *cprim = new TMatrixD(*H, TMatrixD::kMult, *y);
	TMatrixD *AprimT = new TMatrixD(TMatrixD::kTransposed, *Aprim);

	TMatrixD *AprimT_mult_Aprim = new TMatrixD(*AprimT, TMatrixD::kMult, *Aprim);
	TMatrixD *AprimT_mult_cprim = new TMatrixD(*AprimT, TMatrixD::kMult, *cprim);

    AprimT_mult_Aprim -> Invert();
	TMatrixD *Xtilde = new TMatrixD(*AprimT_mult_Aprim, TMatrixD::kMult, *AprimT_mult_cprim);
    TMatrixD *etatilde = new TMatrixD(*A, TMatrixD::kMult, *Xtilde);

    double M = 0;
	for (int j = 0; j < n; j++) {
		M += TMath::Power((yj[j] - (*etatilde)(j, 0))/sigmaj[j], 2);
	}

    for (int i = 0; i < st; i++) {
		wsp[i] = (*Xtilde)(i, 0);
		bwsp[i] = TMath::Sqrt((*AprimT_mult_Aprim)(i, i));
	}

    return M;
}

int testchi2(double T, double alpha, int ndf) {
	TF1 *chi = new TF1("chi", "[0] * TMath::Power(x, [1] - 1) * TMath::Exp(-1 * x/2)", 0, 20);
	int k = 1 / (TMath::Gamma(ndf) * TMath::Power(2, ndf));
	chi -> SetParameter(0, k);
	chi -> SetParameter(1, ndf);
	double checking =TMath::ChisquareQuantile(1-alpha, ndf);
	if (T < checking) {
		return 0;
	}
	else 
		return 1;
}


void macro(){
    ifstream ifile("dane.dat");
	double val1, val2;
	double tj[11], yj[11], sigmaj[11];
	int n = 0;
    int ndf;
    double alpha = 0.01;
	while(ifile >> val1) {
		ifile >> val2;
		tj[n] = val1;
		yj[n] = val2;
		sigmaj[n] = TMath::Sqrt(val2);
		n++;
	}
	ifile.close();
	
    TCanvas *c1 = new TCanvas("c1", "Zadanie 11", 1200, 800);
    TGraphErrors *errgraph = new TGraphErrors(n, tj, yj, NULL, sigmaj);
	c1 -> cd(1);
    errgraph -> SetTitle("Data");
    errgraph -> SetMarkerStyle(kFullCircle);
    errgraph -> SetMarkerSize(1.5);
    auto *legend = new TLegend(0.6, 0.7, 0.9, 0.9);
	legend -> AddEntry("errgraph", "dane pomiarowe");
	errgraph -> Draw();

    double wsp[11], bwsp[11];
	double M;
    
    for (int i = 1; i < 7; i++) {
        M = dopasuj(i, n, tj, yj, sigmaj, wsp, bwsp);
		ndf = n - i;
		cout << "*****Wielomian stopnia " << i-1 << "*****" << endl;
		cout << "Funkcja M = " << M << endl;
		
		TF1 *fun = new TF1 ("Data", Form("pol%d", i), -11, 11);
		for (int j = 0; j < i; j++) {
			cout << "X" << j << " = " << wsp[j] << " +- " << bwsp[j] << endl;
			fun -> SetParameter(j, wsp[j]);
		}
		fun -> SetLineColor(i);
        
	    legend -> AddEntry(fun, Form("st %d", i-1), "l");
	    
		fun -> Draw("sameC");
        cout << "Liczba stopni swobody = " << ndf << endl;
        cout << "Kwantyl = " << TMath::ChisquareQuantile(1-alpha, ndf) << endl;
		cout << "Poziom istotności = " << alpha << endl;
		if (testchi2(M, alpha, ndf) == 1) {
			cout << "Odrzucamy" << endl;
            cout << "**************************************" << endl;
		}
		else {
			cout << "Nie ma podstaw do odrzucenia" << endl;
            cout << "**************************************" << endl;
		}
    }
    legend -> Draw();
}