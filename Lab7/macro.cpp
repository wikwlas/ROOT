//Viktoriia Vlasenko

double pi() {
	TRandom *rand = new TRandom();
	int N = 500000;
	int na = 0, nn = 0;
	TGraph *accepted = new TGraph();
	TGraph *naccepted = new TGraph();
	TH1* h1 = new TH1I("h1", "Accepted x", 100, 0.0, 1.0);
	for (int i = 0; i < N; i++) {
		double x = rand->Uniform(1);
		double y = rand->Uniform(1);
		if (x*x + y*y <= 1) {
			na ++;
			accepted->SetPoint(na, x, y);
			h1->Fill(x);
		} else {
			nn ++;
			naccepted->SetPoint(nn, x, y);
		}
	}

	TCanvas *c1 = new TCanvas();
	c1->Divide(1, 2);
	c1->cd(1);
	accepted->Draw("AP");
	naccepted->Draw("SameP");
	accepted->SetMarkerColor(kRed);
	naccepted->SetMarkerColor(kGreen);
	c1->cd(2);
	
	h1->SetTitle("Accepted x");
	h1->Draw();

	double liczba_pi = 4.0*na/N;
	return liczba_pi;
}

double losujVonNeumann(TF1 *g, double min, double max) {
	TRandom *rand = new TRandom();
	double u, y;
	while(g->Eval(u) < y) {
		u = rand->Uniform(min, max);
		y = rand->Uniform(0, g->GetMaximum(min, max));
	}
	return u;
}

double wydajnoscVonNeumanna(TF1 *g, double min, double max, int n) {
	TRandom *rand = new TRandom();
	double u, y;
	double k = 0;
	for (int i = 0; i < n; i++) {
		u = rand->Uniform(min, max);
		y = rand->Uniform(0, g->GetMaximum(min, max));
		if(g->Eval(u) > y) {
			k ++;
		}
	}
	return k/n;
}

double calkaVonNeumann(TF1 *g, double min, double max, int n) {
	return wydajnoscVonNeumanna(g, min, max, n)*(max - min)*g->GetMaximum(min, max);
}

double calkaFunkcjiLiniowej(double x, double a, double b) {
	return a*x*x/2 + b*x;
}


double odwrotnaDystrybuanta(double x, double a, double b) {
	return -b/(a)+TMath::Sqrt((2*x)/a + (b*b)/(a*a));
}

double calkaVonNeumannZPomoc(TF1 *g, TF1 *s, double min, double max, int n, double &wydajnosc) {
	TRandom *rand = new TRandom();
	int w = 0;
	double u, y, x;
	for (int i = 0; i < n; i++) {
		u = rand->Uniform(0, 1);
		x = rand->Uniform(calkaFunkcjiLiniowej(min, s->GetParameter(0), s->GetParameter(1)), calkaFunkcjiLiniowej(max, s->GetParameter(0), s->GetParameter(1)));
		y = odwrotnaDystrybuanta(x, s->GetParameter(0), s->GetParameter(1));
		if ((g->Eval(y)/s->Eval(y))>u) {
			w++;
		}
	}
	wydajnosc = (double) wyd / n;
	return wydajnosc*(max - min)*(s->Eval(min) + s->Eval(max))/2;
}
void macro(void) {
	//Part 1
	cout << "Wyliczone PI: " << pi() << endl;


	//Part 2
	TF1 *g = new TF1 ("g", "0.4 + (TMath::Sin(TMath::Pi() * (x + 1.2))) / (TMath::Pi() * (x + 1.2) + 1)", 0, 1.0);
	int N = 500000;

	cout << "Calka metoda Integral: " << g->Integral(0, 1.0) << endl;

	cout << "# von Neumann #" << endl;
	cout << "Calka: " << calkaVonNeumann(g, 0, 1.0, N) << endl;
	cout << "Wydajnosc: " << wydajnoscVonNeumanna(g, 0, 1.0, N) << endl;

	double wydajnosc;

	TF1 *s = new TF1 ("s", "[0]*x+[1]", 0, 1.0);
	s->SetParameters(0, 1);
	s->SetParameters(1, 2);
	cout << "# von Neumann z funkcja pomocnicza #" << endl;
	cout << "Calka: " << calkaVonNeumannZPomoc(g, s, 0, 1.0, N, wydajnosc) << endl;
	cout << "Wydajnosc: " << wydajnosc << endl;
}