#include <iostream>
#include <dic.hxx>


///root 
#include <TApplication.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TGraph.h>


using namespace std;

class ErrorHandler : public DimErrorHandler
{
  void errorHandler(int severity, int code, char *msg)
  {
    int index = 0;
    char **services;
    if(code){}
    cout << severity << " " << msg << endl;
    services = DimClient::getServerServices();
    cout<< "from "<< DimClient::getServerName() << " services:" << endl;
    while(services[index])
      {
	cout << services[index] << endl;
	index++;
      }
  }
public:
  ErrorHandler() {DimClient::addErrorHandler(this);}
};

class StrService : public DimInfo
{
  
  void infoHandler()
  {
    int index = 0;
    char **services;
    char *format;
    //		cout << "Dns Node = " << DimClient::getDnsNode() << endl;
    format = getFormat();
    cout << "Received STRVAL : " << getString() << "format = " << format << endl;
    services = DimClient::getServerServices();
    cout<< "from "<< DimClient::getServerName() << " services:" << endl;
    while(services[index])
      {
	cout << services[index] << endl;
	index++;
      }
    int inCallback = DimClient::inCallback();
    cout << "infoHandler: In callback "<< inCallback << endl; 
  }
public :
  StrService() : DimInfo("TEST/STRVAL",(char *)"not available") {};
};

int main(int argc, char * argv[])
{

  TApplication app("app", &argc, argv);
  
  ErrorHandler errHandler;
  StrService servstr;
  char *server, *ptr, *ptr1;
  DimBrowser br;
  int type, n, pid;
  
  //	DimClient::addErrorHandler(errHandler);
  
  n = br.getServices("*");
  cout << "found " << n << " services" << endl; 
  
  while((type = br.getNextService(ptr, ptr1))!= 0)
    {
      cout << "type = " << type << " - " << ptr << " " << ptr1 << endl;
    }
  
  br.getServers();
  while(br.getNextServer(server, ptr1, pid))
    {
      cout << server << " @ " << ptr1 << ", pid = " << pid << endl;
    }
  
  br.getServerClients("DIS_DNS");
  while(br.getNextServerClient(ptr, ptr1))
    {
      cout << ptr << " @ " << ptr1 << endl;
    }
  /*
  DimInfo servint("TEST/INTVAL",-1); 
  
  while(1)
    {
      sleep(10);
      
      cout << "Current INTVAL : " << servint.getInt() << endl;
      DimClient::sendCommand("TEST/CMND","UPDATE_STRVAL");
      int inCallback = DimClient::inCallback();
      cout << "main: In callback "<< inCallback << endl;
      
      DimClient::addErrorHandler(0);
    }

  */

  DimInfo servint1("trd-fee_00_1_2_RCU2_TEMP",-1);
  DimInfo servint2("trd-fee_00_1_2_RCU2_RH",-1);
  DimInfo servint3("trd-fee_00_1_2_RCU2_Pressure",-1);
  //  DimInfo servint("trd-fee_00_1_2_RCU2_Pressure",-1);
  const int npoint = 1000;
  double x[npoint];
  double temp[npoint];
  double pressure[npoint];
  double humid[npoint];

  int icycle=0;

  TGraph *gtemp = new TGraph();
  TGraph *ghumid = new TGraph();
  TGraph *gpressure = new TGraph();

  gtemp->SetMarkerStyle(20);
  ghumid->SetMarkerStyle(20);
  gpressure->SetMarkerStyle(20);

  gtemp->SetLineColor(1);
  ghumid->SetLineColor(2);
  gpressure->SetLineColor(4);

  gtemp->SetMarkerColor(1);
  ghumid->SetMarkerColor(2);
  gpressure->SetMarkerColor(4);

  TCanvas *c1 = new TCanvas("c1","c1",1000,600);
  c1->Divide(3,1);


  while(icycle<npoint)
    {
      sleep(30);

      x[icycle] = icycle+1;
      temp[icycle] = servint1.getFloat();
      humid[icycle] = servint2.getFloat();
      pressure[icycle] = servint3.getFloat();

      gtemp->SetPoint(icycle, x[icycle], temp[icycle]);
      ghumid->SetPoint(icycle, x[icycle], humid[icycle]);
      gpressure->SetPoint(icycle, x[icycle], pressure[icycle]);


      cout << "Current TEMP : " << temp[icycle]  << endl;
      cout << "Current RH : " << humid[icycle] << endl;
      cout << "Current Pressure : " << pressure[icycle] << endl;
      int inCallback = DimClient::inCallback();
      cout << "main: In callback "<< inCallback << endl;
      DimClient::addErrorHandler(0);

      icycle++;


      c1->cd(1);      
      gtemp->Draw("apc");
      c1->cd(2);      
      ghumid->Draw("apc");
      c1->cd(3);      
      gpressure->Draw("apc");

      c1->Update();
    }

  app.Run(kTRUE);

  return 0;
}
