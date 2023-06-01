// f - tablica wskaznikow do obiektow TF1, rozklady skladowe
// k - wymiar tablicy f
// h - histogram do ktorego wrzucamy wynik splotu
// n - liczba losowan

void splot(TF1 **f, int k, TH1D *h, int n){
	double sum = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < k; j++) {
			sum += f[j]->GetRandom();
		}
		h->Fill(sum);
		sum = 0;
	}
}

void deskaGaltona(TH1D * h, int n, int l, double p) {
	TRandom *r = new TRandom();
	for (int i = 0; i < n; i++){
		double sum = 0;
		for(int j = 0; j < l; j++){
			if(r -> Uniform(0,1) < p)
				sum--;
			else 
				sum++;
		}
		h->Fill(sum);
	}
}

void macro(){
	TCanvas *c1 = new TCanvas();
	c1 -> Divide(2,2);

	TF1 *fa1 = new TF1("fa1", "1", 2.0, 5.0);
	TF1 *fa2 = new TF1("fa2", "1", 2.0, 5.0);
	TH1D *hist = new TH1D("h1", "Splot 2U", 100, 4, 10);
	TF1 *fa[2];
	fa[0] = fa1;
	fa[1] = fa2;

	splot(fa, 2, hist, 100000);
	c1->cd(1);
	hist->Draw();
	

	TF1 *fb1 = new TF1("fu1", "1", 2.0, 5.0);
	TF1 *fb2 = new TF1("fu2", "1", 2.0, 5.0);
	TF1 *fb3 = new TF1("fu3", "1", 2.0, 5.0);
	TH1D *hist2 = new TH1D("h2", "Splot 3U", 100, 6, 15);
	TF1 *fb[3];
	fb[0] = fb1;
	fb[1] = fb2;
	fb[2] = fb3;

	splot(fb, 3, hist2, 100000);
	c1->cd(2);
	hist2->Draw();


	TF1 *fn1 = new TF1("fn1", "ROOT::Math::gaussian_pdf(x, 3.0, 10.0)", -50.0, 50.0);
	TF1 *fn2 = new TF1("fn2", "ROOT::Math::gaussian_pdf(x,  4.0, 15.0)", -50.0, 50.0);
	TF1 *fn[2];
	fn[0] = fn1;
	fn[1] = fn2;
	TH1D *hist3 = new TH1D("h3", "Splot 2N", 100, 0, 50);

	splot(fn, 2, hist3, 100000);
	c1->cd(3);
	hist3->Draw();
	double m1 = fn1->Mean(-50.0, 50.0);
	double m2 = fn2->Mean(-50.0, 50.0);

	cout << "EX 1 " << m1 << endl;
	cout << "EX 2 " << m2 << endl;
	cout << "EX " << hist3->GetMean() << endl;

	//double s1 = fn1->GetParameter(1);
	//double s2 = fn2->GetParameter(1);

	cout << "VX 1 " << 3.0 << endl;
	cout << "VX 2 " << 4.0 << endl;
	cout << "VX " << hist3->GetRMS() << endl;


	TH1D *hist4 = new TH1D("h4", "Tablica", 21, -10.5, 10.5);

	deskaGaltona(hist4, 1000, 10, 0.5);
	c1->cd(4);
	hist4->Draw();

}