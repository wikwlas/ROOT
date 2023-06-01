void macro() {
	TCanvas *c1 = new TCanvas();
	c1->Divide(2, 2);
	
	double x1_min = 1, x1_max = 5.00, x2_min = 0, x2_max = 3.00, x3_min = 4, x3_max = 16;
	
	ifstream ifile1;
	ifile1.open("Dane1.dat");
	double val1;
	
	ifstream ifile2;
	ifile2.open("Dane2.dat");
	double val2;
	
	ifstream ifile3;
	ifile3.open("Dane3.dat");
	double val3;
	
	TH2D *h12 = new TH2D("h12", "hist12", 100, x1_min, x1_max, 100, x2_min, x2_max);
	TH2D *h23 = new TH2D("h23", "hist23", 100, x2_min, x2_max, 100, x3_min, x3_max);
	TH2D *h13 = new TH2D("h13", "hist13", 100, x1_min, x1_max, 100, x3_min, x3_max);
	
	while(ifile1 >> val1 && ifile2 >>val2 && ifile3 >> val3) {
		h12->Fill(val1, val2);
		h23->Fill(val2, val3);
		h13->Fill(val1, val3);
	}
	
	c1->cd(1);
	h12->Draw("colz");
	
	c1->cd(2);
	h13->Draw("colz");
	
	c1->cd(3);
	h23->Draw("colz");
	
	ifile1.close();
	ifile2.close();
	ifile3.close();
	
	double x1 = h12->GetMean(1);
	double x3 = h13->GetMean(2);
	double x2 = h23->GetMean(1);
	
	double x1_error = h12->GetStdDev(1);
	double x3_error = h13->GetStdDev(2);
	double x2_error = h23->GetStdDev(1);
	
	cout << "X1=" << x1 << " u(X1)=" << x1_error << endl;
	cout << "X2=" << x2 << " u(X2)=" << x2_error << endl;
	cout << "X3=" << x3 << " u(X3)=" << x3_error << endl;
	
	TMatrixD matcov(3,3);
	
	double covx1x2 = h12 -> GetCovariance(1,2);
	double covx1x3 = h13 -> GetCovariance(1,2);
	double covx2x3 = h23 -> GetCovariance(1,2);
	
	double x1_sigma = (h12 -> GetRMS(1)) * (h12 -> GetRMS(1));
	double x2_sigma = (h12 -> GetRMS(2)) * (h12 -> GetRMS(2));
	double x3_sigma = (h23 -> GetRMS(2)) * (h23 -> GetRMS(2));
	
	double rho12 = covx1x2/x1_sigma/x2_sigma;
	double rho13 = covx1x3/x1_sigma/x3_sigma;
	double rho23 = covx2x3/x2_sigma/x3_sigma;
	
	cout << "rho12=" << rho12 << endl;
	cout << "rho13=" << rho13 << endl;
	cout << "rho23=" << rho23<< endl;
	
	matcov(0,0) = x1_sigma;
	matcov(1,1) = x2_sigma;
	matcov(2,2) = x3_sigma;
	
	matcov(0,1) = covx1x2;
	matcov(1,0) = covx1x2;
	
	matcov(0,2) = covx1x3;
	matcov(2,0) = covx1x3;
	
	matcov(1,2) = covx2x3;
	matcov(2,1) = covx2x3;
	
	matcov.Print();
	
	TMatrixD tmat(2,3);
	tmat(0,0) = 3;
	tmat(0,1) = 2;
	tmat(0,2) = 1;
	
	tmat(1,0) = 5;
	tmat(1,1) = 2;
	tmat(1,2) = 0;
	
	TMatrix help1 = tmat * matcov;
	TMatrix covhelp(3,2);
	covhelp.Transpose(tmat);
	
	TMatrix result = help1 * covhelp;
	
	result.Print();
	
	double y1 = 3 * x1 + 2 * x2 + x3;
	double y2 = 2 + 5 * x1 + 2 * x2;
	double y1_error = sqrt(result(0,0));
	double y2_error = sqrt(result(1,1));
	
	cout << "Y1=" << y1 << " u(Y1)=" << y1_error << endl;
	cout << "Y2=" << y2 << " u(Y2)=" << y2_error << endl;
	
	double rhoy1y2 = result(0,1)/sqrt(result(0,0)*result(1,1));
	
	cout << "rhoy1y2=" << rhoy1y2 << endl;
	
}
