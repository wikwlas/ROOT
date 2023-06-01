double_t density(int x) {
	int a = 4;
	return (TMath::Exp(-x/a))/a;
}
double dystr(double_t x) {
	
	return (1 - TMath::Exp(-x/4));
}

void macro(){
	int a = 4;
	double w = 1000;
	double h = 800;
	
	TF1* f1 = new TF1("f1", "(TMath::Exp(-x/4))/4", 0, 50);
	TF1* f2 = new TF1("analitycznie", "1 - TMath::Exp(-x/4)", 0, 50);
	f2->SetLineColor(kOrange);
	f1->SetParameters(-1, 2);
	f1->SetLineColor(kBlue);
	
	double_t tabx[50];
	double_t taby[50];
	
	tabx[0] = 0.5;
	taby[0] = f1->Eval(0.5);
	
	for (int i = 1; i < 50; i++) {
		tabx[i] = 1./2 + i;
		taby[i] = taby[i-1] + f1->Eval(i + 1./2);
	}
	TCanvas* canvas1 = new TCanvas("canvas1", "numerycznie", w, h);
	canvas1->Divide(2,1);
	canvas1->cd(1);
	f1->Draw();
	canvas1->cd(2);
	
	TGraph* graph1 = new TGraph(50, tabx, taby);
	graph1->SetLineColor(kRed);
	graph1->Draw();
	
	TGraph* dystrybuanta = new TGraph(50, taby, tabx);
	f2->Draw();
	double_t probab = f1->Integral(2, 8);
	double_t mean = f1->Mean(0, 50);
	double_t var = f1->Variance(0, 50);
	double_t moda = f1->GetMaximumX(0, 50);
	
	std::cout << "Prawdopodobienstwo z gestosci wynosi: " << probab << std::endl;
	std::cout << "Prawdopodobienstwo z dystr wynosi: " << dystr(8) - dystr(2) << std::endl;
	std::cout << "Średnia tf1 wynosi: " << mean << std::endl;
	std::cout << "Średnia analitycznie wynosi: " << a << std::endl;
	std::cout << "Wariancja tf1 wynosi: " << var << std::endl;
	std::cout << "Wariancja analitycznie wynosi: " << a*a << std::endl;
	
	std::cout << "Moda wynosi: " << moda << std::endl;
	
	double kwx = 0;
	double kwy = 0;
	
	
	double_t qx[3] = {0.25, 0.5, 0.75};
	double_t qy[3];
	f1->GetQuantiles(3, qy, qx);
	
	
	std::cout << "x0.25: " << qy[0] << std::endl;
	std::cout << "x0.5: " << qy[1] << std::endl;
	std::cout << "x0.75: " << qy[2] << std::endl;
	
}