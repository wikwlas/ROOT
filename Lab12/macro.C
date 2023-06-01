TH1D *hist;

double model(double x, double *par){
    double mu = par[3];
    double sigma = par[4];
    double norm = 1./sqrt(2.*TMath::Pi())/sigma;
    double G = norm * exp(-0.5*pow((x-mu)/sigma,2)); // unormowana funkcja Gaussa
    double BinWidth = hist->GetBinWidth(1);
    return par[0] + par[1]*x + par[2]*BinWidth*G;
}

void fcn(int &npar, double *gin, double &f, double *par, int iflag){
  f = 0.;
  for(int iBin=1;iBin<=hist->GetNbinsX();iBin++){
    double x = hist->GetBinCenter(iBin);
    double measure = hist->GetBinContent(iBin);
    double error = TMath::Sqrt(measure);
    double func = model(x,par);
    double delta = (func - measure);
    f += TMath::Power(delta,2.)/TMath::Power(error,2.);
  }
}

void macro(){
    //otwieramy plik z danymi i wyciagamy histogram
    TFile *file = new TFile("example_data.root");
    hist = (TH1D*)file->Get("hist");

    //tworzymy obiekt TMinuit
    Int_t nParams = 5;
    TMinuit *gMinuit = new TMinuit(nParams);
    gMinuit->SetFCN(fcn);
    
    gMinuit->DefineParameter(0,"p0"   ,100.,1.,50.,150.); //iterator
    gMinuit->DefineParameter(1,"p1",-50.,1.,-75.,0.); //nazwa
    gMinuit->DefineParameter(2,"area",160.,1.,0.,16000.);
    gMinuit->DefineParameter(3,"mean",1.,1.,0.8,1.2);
    gMinuit->DefineParameter(4,"width",0.2,1.,0.001,0.3);

    // wykonanie dopasowania
    gMinuit->Command("MIGRAD");
    gMinuit->Command("MINOS");
    
    // odczytanie parametrow
    double par[5];
    double par_err[5];
    for(int i=0; i<5; i++)gMinuit->GetParameter(i,par[i],par_err[i]);

    // tworzymy funkcję na podstawie dopasowanych parametrow
    TH1D *hModel = new TH1D("hModel","Dopasowanie",hist->GetNbinsX(),hist->GetXaxis()->GetXmin(),hist->GetXaxis()->GetXmax());
    for(int i=1;i<=hModel->GetNbinsX();i++){
      double x = hModel->GetBinCenter(i);
      double f = model(x,par);
      hModel->SetBinContent(i,f);
    }
    
    // rysowanie
    TCanvas* c1 = new TCanvas("c1","zadanie 12",20,20,600,600);

    // histogram z danymi
    hist->SetTitle("Rozklad masy;masa;liczba zliczen");
    hist->SetStats(false);
    hist->SetFillColor(kRed - 9);
    hist->Draw();

    // dopasowanie
    hModel->SetLineColor(kBlue);
    hModel->SetLineWidth(3);
    hModel->Draw("same l");

    // TF1 *fModel = new TF1("fModel",model,0, 2, 5);
    // fModel->SetParameters(par);
    // fModel->SetLineColor(kGreen);
    // fModel->Draw("same");
    
    c1->SaveAs("minuit-fit.png");
}
