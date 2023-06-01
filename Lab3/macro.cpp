double fnc(double *x, double *params) {
	if (x[0] < 0 && x[0] > 1 && x[1] < 0 && x[1] > 1)
		return 0;
	else
		return params[0]*TMath::Power(x[0], params[1])*TMath::Power(x[1], params[2]);
}


void macro() {
	TCanvas *c1 = new TCanvas();
	c1-> Divide(2,2);
	
	//Gestosc f(x,y)
	TF2 *fun = new TF2("fun", fnc, 0, 1, 0, 1, 3);
	fun->SetParameters(1, 1, 3);
	
	c1->cd(1);
	fun->SetTitle("Gestosc");
	fun->GetXaxis()->SetTitle("x");
	fun->GetYaxis()->SetTitle("y");
	fun->GetZaxis()->SetTitle("f(x,y)");
	fun->Draw("Surf1");
	
	double c = fun->Integral(0, 1, 0, 1);
	cout << "Stala normujaca: " << 1/c << "\n";
	
	//Dystrybuanta F(x,y)
	TGraph2D *g = new TGraph2D();
	for (double i = 0; i <= 1; i+=0.01)
		for (double j = 0; j <= 1; j+=0.01) {
			double integfunc = fun->Integral(0, i, 0, j);
			g->AddPoint(i, j, integfunc);
		}
	c1->cd(2);
	g->SetTitle("Dystrybuanta");
	g->GetXaxis()->SetTitle("x");
	g->GetYaxis()->SetTitle("y");
	g->GetZaxis()->SetTitle("F(x,y)");
	g->Draw("surf1");
	
	//Gestosc brzegowa g(x)
	double epsilon = 0.0001;
	
	TGraph *fung = new TGraph();
	for(double i = 0; i <= 1; i+=0.01)
		fung->AddPoint(i, fun->Integral(i, i + epsilon, 0, 1));
	
	c1->cd(3);
	fung->SetTitle("Gestosc brzegowa g(x)");
	fung->GetXaxis()->SetTitle("x");
	fung->GetYaxis()->SetTitle("g(x)");
	fung->Draw();
	
	
	//Gestosc brzegowa h(y) 
	TGraph *funh = new TGraph();
	for(double j = 0; j <= 1; j+=0.01)
		funh->AddPoint(j, fun->Integral(0, 1, j, j + epsilon));
		
	c1->cd(4);
	funh->SetTitle("Gestosc brzegowa h(y)");
	funh->GetXaxis()->SetTitle("y");
	funh->GetYaxis()->SetTitle("h(y)");
	funh->Draw();
}
