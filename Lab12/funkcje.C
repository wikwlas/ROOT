double model(double x, double *par){
    double mu = par[3];
    double sigma = par[4];
    double norm = 1./sqrt(2.*TMath::Pi())/sigma;
    double G = norm * exp(-0.5*pow((x-mu)/sigma,2)); // unormowana funkcja Gaussa
    double BinWidth = hist->GetBinWidth(1);
    return par[0] + par[1]*x + par[2]*BinWidth*G;
}

void fcn(int &npar, double *gin, double &f, double *par, int iflag);

//Wczytywanie histogramu z pliku
TFile *file = new TFile("example_data.root");
hist = (TH1D*)file->Get("hist");
