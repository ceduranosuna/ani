#include <iostream>
#include  <vector>
#include "TVector3.h"
#include "TMatrixD.h"
#include "TLorentzVector.h"
#include "TRandom3.h"
#include "TFile.h"
#include "DataB_kaskey_2012.C"
#include "TMVA/Reader.h"
#include "TMVA/MethodBDT.h"
#include <TMath.h>
#include <TVector.h>

using namespace std;


Double_t V0_Lifetime(TVector3 pv, TVector3 sv, TMatrixD EPV, TMatrixD ESV, Double_t M, TVector3 pT, double &ct, double &ect)
{
  // NOTA1: Esta funcion calcula el tiempo de vida y su error usando la longitud propia de decaimiento transversal
  // Recuerde que estamos asumiendo que el error en el pt es despreciable por eso las matrices asociadas a este las definimos cero

  TVector3 svT(sv.X(),sv.Y(),0.0);
  TVector3 pvT(pv.X(),pv.Y(),0.0);
  TVector3 d = svT - pvT;

  TMatrixD VSV(2,2);
  VSV(0,0) = ESV(0,0);
  VSV(1,1) = ESV(1,1);
  VSV(1,0) = ESV(1,0);
  VSV(0,1) = VSV(1,0);

  TMatrixD VPV(2,2);
  VPV(0,0) = EPV(0,0);
  VPV(1,1) = EPV(1,1);
  VPV(1,0) = EPV(1,0);
  VPV(0,1) = VPV(1,0);

  TMatrixD VL(2,2); VL = VSV; VL+=VPV;

  TVector3 p = pT;

  TMatrixD VP(2,2);
  VP(0,0) = 0.0;
  VP(1,1) = 0.0;
  VP(0,1) = 0.0;
  VP(1,0) = 0.0;

  double Lxy = d.Dot(p)/p.Mag();
  double lf = Lxy*M/p.Mag();
  //cout<<" ---> "<<lf<<endl;
  ct = lf;
  
  //Ahora calaculamos el error en el tiempo de vida
  
  //computing Mass error
  //double sM2 = 0; //We assume 0 for now
  
  //computing Lxy error
  
  //Defining Matrix:
  TMatrixD A(2,2);
  TMatrixD B(2,2);
  TMatrixD C(2,2);
  TMatrixD EP(2,2);
  TMatrixD EL(2,2);
  
  //Aij = PiPj/p2
  //Bij = LiLj/Lxy2 (Li = SVi - PVi)
  //EPij = Vij(P)/p2
  //ELij = Vij(L)/Lxy^2;
  //Cij = LiPj/(pLxy)
  
  A(0,0) = p.X()*p.X()/p.Mag2();
  A(1,1) = p.Y()*p.Y()/p.Mag2();
  A(0,1) = p.X()*p.Y()/p.Mag2();
  A(1,0) = A(0,1);

  B(0,0) = d.X()*d.X()/(Lxy*Lxy);
  B(1,1) = d.Y()*d.Y()/(Lxy*Lxy);
  B(0,1) = d.X()*d.Y()/(Lxy*Lxy);
  B(1,0) = B(0,1);
  
  C(0,0) = d.X()*p.X()/(Lxy*p.Mag());
  C(1,1) = d.Y()*p.Y()/(Lxy*p.Mag());
  C(0,1) = d.X()*p.Y()/(Lxy*p.Mag());
  C(1,0) = d.Y()*p.X()/(Lxy*p.Mag());

  EP = VP;
  EP*= ((double)1.0/p.Mag2());
  EL = VL;
  EL*= ((double)1.0/(Lxy*Lxy));

  //Test
  //EL(0,1) = 0.0;
  //EL(1,0) = 0.0;

  //Calculando Sigma Lxy
  // Sigma Lxy^2 = Tr{A*EL + (B + 4*A - 4*C)*EP}
  // NOTA2: en nuestro caso basicamente es Sigma Lxy^2 = Tr{A*EL), dado que no consideramos el momentum P
  
  TMatrixD A1 = A;
  A1*=(double)4.0;
  A1+=B;
  TMatrixD C1 = C;
  C1*=(double)4.0;
  A1-=C1;
  
  TMatrixD A_EL(A,TMatrixD::kMult,EL);
  TMatrixD A1_EP(A1,TMatrixD::kMult,EP);
  TMatrixD SL = A_EL;SL+=A1_EP;
  double sLxy2 = SL(0,0) + SL(1,1); 
  
  return ect = (double) fabs(lf)*sqrt(sLxy2);
}


void slimB_kaskey_2012()
{

  TChain * ch = new TChain("ntuple","");
 

  
//************* estos son todos los datos ******************//


//corrida B
ch->Add("/store/mc/Summer12/XibToJpsiXi_BFilter_MSEL5_TuneZ2star_8TeV-Pythia6-evtgen/GEN-SIM/START53_V7C-v1/70000/001357D1-3934-E611-95D8-002590550546.root")

//corrida C
//ch->Add("/afs/cern.ch/work/j/jmejiagu/public/2012_dataset/test_newvtx2016/jobs_kaskey/roofilescarpetaC/*.root/mkcands/ntuple");

//corrida D
//ch->Add("/afs/cern.ch/work/j/jmejiagu/public/2012_dataset/test_newvtx2016/jobs_kaskey/roofilescarpetaD/*.root/mkcands/ntuple");


 //******************************************

 TTree *tree = (TTree*)ch;

  DataB_kaskey_2012 t(tree);

  Long64_t nentries = t.fChain->GetEntries();
  
  cout<<" Entries : "<<nentries<<endl;

  
   cout<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
   cout<<" Reading data ..."<<nentries<<endl;

   
   // ************* cada uno de los siguientes archivos .root los hicimos para pruebas o comparaciones
//claramente se compilaron uno por uno.                                        
   //     ****************************
   
   //Datos 2012   



  
   // corrida B
   TString outfileName( "prueba.root" ); 

   //corrida C
   //TString outfileName( "ROOTSB_kaskey_2012parkedata_C.root " ); 

   //corrida D
   //TString outfileName( "ROOTSB_kaskey_2012parkedata_D.root" ); 

   // *****************************************  *********************************************


   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );


   /*
      aca se definen las variables con las que se van a hacer los cortes para hallar la señal.
   */

 
    //*******************************
    //*     VARIABLES TO SAVE       *
    //*******************************
    
    Double_t massB, massJ, masskm,  masslamb;

    Double_t Bdl,BdlBS,BdlRf,BdlIP,BdlIPBSc,BSct;
    Double_t BdlE,BdlBSE,BdlRfE,BdlIPE,BdlIPBScE,BSctE;
    Double_t Bdlkminus, BdlkminusE, Bdllamb, BdllambE;

    Double_t Beta, Bphi;
    Double_t sigLxyJ,cosalfaJ;
    Double_t lambpt, Jpsipt, kmpt, Bpt;
    Double_t maxPtMu, maxPtPi;
    Double_t minPtMu, minPtPi;
    
    Double_t mu1phi,  mu2phi,   Pi1phi,  Pi2phi;
    Double_t Jeta, mu1eta,  mu2eta,   Pi1eta,  Pi2eta;

    Double_t mu1pt,  mu2pt,  Pi1pt, Pi2pt, Pi3pt;
    Double_t lambchi2, Jchi2, kmchi2, Bchi2, lambprob, Jprob, kmprob, Bprob;

    Int_t B_kaskeym_charge1,B_kas_lambda_charge2;
    
    Double_t massKs0, massOmem;
    Int_t mupDisplace, mumDisplace;
    Int_t runn, eventn;
    Bool_t softmu1, softmu2,tightmu1, tightmu2, PFmu1, PFmu2;
    Int_t mupcat, mumcat;

    Int_t triggerv4, triggerv5, triggerv6, triggerv7;

    
    TTree *treeS =  new TTree("treeS","signal");
    
    treeS->Branch("massB",   &massB,    "massB/D");
    treeS->Branch("massJ",   &massJ,    "massJ/D");
    treeS->Branch("masslamb",  &masslamb,   "masslamb/D");
    treeS->Branch("masskm",  &masskm,   "masskm/D");

    treeS->Branch("Bdl",     &Bdl,      "Bdl/D");
    treeS->Branch("BdlBS",     &BdlBS,      "BdlBS/D");
    treeS->Branch("BdlRf",     &BdlRf,      "BdlRf/D");
    treeS->Branch("BdlIP",     &BdlIP,      "BdlIP/D");
    treeS->Branch("BSct",     &BSct,      "BSct/D");
    treeS->Branch("BdlIPBSc",  &BdlIPBSc,   "BdlIPBSc/D");
    treeS->Branch("BdlE",     &BdlE,      "BdlE/D");
    treeS->Branch("BdlBSE",     &BdlBSE,      "BdlBSE/D");
    treeS->Branch("BdlRfE",     &BdlRfE,      "BdlRfE/D");
    treeS->Branch("BdlIPE",     &BdlIPE,      "BdlIPE/D");
    treeS->Branch("BSctE",     &BSctE,      "BSctE/D");
    treeS->Branch("BdlIPBScE",  &BdlIPBScE,   "BdlIPBScE/D");
    
    treeS->Branch("Bdlkminus",     &Bdlkminus,      "Bdlkminus/D");
    treeS->Branch("BdlkminusE",     &BdlkminusE,      "BdlkminusE/D");
    
    treeS->Branch("Bdllamb",     &Bdllamb,      "Bdllamb/D");
    treeS->Branch("BdllambE",     &BdllambE,      "BdllambE/D");


    treeS->Branch("Beta",    &Beta,     "Beta/D");
    treeS->Branch("Bphi",    &Bphi,     "Bphi/D");
    treeS->Branch("sigLxyJ", &sigLxyJ,  "sigLxyJ/D");
    treeS->Branch("cosalfaJ", &cosalfaJ,  "cosalfaJ/D");

    treeS->Branch("Bpt",     &Bpt,      "Bpt/D");
    treeS->Branch("lambpt",    &lambpt,     "lambpt/D");
    treeS->Branch("Jpsipt",  &Jpsipt,   "Jpsipt/D");
    treeS->Branch("kmpt",  &kmpt,   "kmpt/D");
    
    treeS->Branch("mu1pt",   &mu1pt,     "mu1pt/D");
    treeS->Branch("mu2pt",   &mu2pt,     "mu2pt/D");
    treeS->Branch("Pi1pt",   &Pi1pt,     "Pi1pt/D");
    treeS->Branch("Pi2pt",   &Pi2pt,     "Pi2pt/D");
    treeS->Branch("Pi3pt",   &Pi3pt,     "Pi3pt/D");
    
    treeS->Branch("mu1phi",  &mu1phi,    "mu1phi/D");
    treeS->Branch("mu2phi",  &mu2phi,    "mu2phi/D");
    treeS->Branch("Pi1phi",  &Pi1phi,    "Pi1phi/D");
    treeS->Branch("Pi2phi",  &Pi2phi,    "Pi2phi/D");
    
    treeS->Branch("Jeta",  &Jeta,    "Jeta/D");
    treeS->Branch("mu1eta",  &mu1eta,    "mu1eta/D");
    treeS->Branch("mu2eta",  &mu2eta,    "mu2eta/D");
    treeS->Branch("Pi1eta",  &Pi1eta,    "Pi1eta/D");
    treeS->Branch("Pi2eta",  &Pi2eta,    "Pi2eta/D");

    
    treeS->Branch("maxPtMu", &maxPtMu,   "maxPtMu/D");
    treeS->Branch("maxPtPi", &maxPtPi,   "maxPtPi/D");
    
    treeS->Branch("minPtMu", &minPtMu,   "minPtMu/D");
    treeS->Branch("minPtPi", &minPtPi,   "minPtPi/D");
    
    
    treeS->Branch("lambchi2",  &lambchi2,   "lambchi2/D");
    treeS->Branch("Jchi2",   &Jchi2,    "Jchi2/D");
    treeS->Branch("Bchi2",   &Bchi2,    "Bchi2/D");
    treeS->Branch("kmchi2",   &kmchi2,    "kmchi2/D");
    treeS->Branch("lambprob",  &lambprob,   "lambprob/D");
    treeS->Branch("Jprob",  &Jprob,   "Jprob/D");
    treeS->Branch("kmprob",  &kmprob,   "kmprob/D");	
    treeS->Branch("Bprob",  &Bprob,   "Bprob/D");
 
    treeS->Branch("B_kaskeym_charge1",&B_kaskeym_charge1,"B_kaskeym_charge1/I");
    treeS->Branch("B_kas_lambda_charge2",&B_kas_lambda_charge2,"B_kas_lambda_charge2/I");

    treeS->Branch("massOmem",   &massOmem,    "massOmem/D");
    treeS->Branch("massKs0",   &massKs0,    "massKs0/D");
    treeS->Branch("mupDisplace", &mupDisplace, "mupDisplace/I");
    treeS->Branch("mumDisplace", &mumDisplace, "mumDisplace/I");
    treeS->Branch("runn", &runn, "runn/I");
    treeS->Branch("eventn", &eventn, "eventn/I");

    treeS->Branch("mupcat", &mupcat, "mupcat/I");
    treeS->Branch("mumcat", &mumcat, "mumcat/I");

    treeS->Branch("softmu1", &softmu1);
    treeS->Branch("softmu2", &softmu2);
    treeS->Branch("tightmu1", &tightmu1);
    treeS->Branch("tightmu2", &tightmu2);
    treeS->Branch("PFmu1", &PFmu1);
    treeS->Branch("PFmu2", &PFmu2);

    treeS->Branch("triggerv4", &triggerv4, "triggerv4/I");
    treeS->Branch("triggerv5", &triggerv5, "triggerv5/I");
    treeS->Branch("triggerv6", &triggerv6, "triggerv6/I");
    treeS->Branch("triggerv7", &triggerv7, "triggerv7/I");



   TVector3 pT,pv,sv,pvbs,pvrf,bsv,pvip,pvipBSc,svKminus,svlamb;
   Double_t ct,ect,M,ctbs,ectbs,ctrf,ectrf,ctip,ectip,ctipBSc,ectipBSc,ctBS, ctBSE;
   Double_t ctKminus,ectKminus, ctlamb, ectlamb;
   TMatrix ESV(3,3);
   TMatrix EPV(3,3);
   TMatrix EPVBS(3,3);
   TMatrix EPVRf(3,3);
   TMatrix EBSV(3,3);
   TMatrix EPVip(3,3);
   TMatrix EPVipBSc(3,3);
   TMatrix ESVKminus(3,3);
   TMatrix ESVlamb(3,3);



 Int_t nTen = nentries/10;

   Int_t nbytes = 0, nb = 0;
   for(Long64_t jentry=0; jentry<nentries;jentry++) 
     {
       Long64_t ientry = t.LoadTree(jentry);
       if (ientry < 0) break;
       nb = t.fChain->GetEntry(jentry);   nbytes += nb;
       if(jentry%nTen==0) cout<<10*(jentry/nTen)<<"%-"<<flush;
       if(jentry==nentries-1) cout<<endl;

      
 /////////////// This is trigger ip requirements /////////////////////
		// First we ask for one of the Displace  triggers in the trigger list of the event
       
                string alltnames = t.triggersL;
		Bool_t checkflag = false;
		
		//string::size_type trigger1 = alltnames.find("HLT_Dimuon8_Jpsi",0);

		//string::size_type trigger1 = alltnames.find("HLT_Dimuon8_Jpsi_v3",0);
		string::size_type trigger2 = alltnames.find("HLT_Dimuon8_Jpsi_v4",0);
		string::size_type trigger3 = alltnames.find("HLT_Dimuon8_Jpsi_v5",0);
		string::size_type trigger4 = alltnames.find("HLT_Dimuon8_Jpsi_v6",0);
		string::size_type trigger5 = alltnames.find("HLT_Dimuon8_Jpsi_v7",0);

		//cout<<" todos los nombres : "<<alltnames<<endl;

		//cout<<alltnames<<endl;			

		/*					    	
 if(trigger1!=string::npos || trigger2!=string::npos || trigger3!=string::npos || trigger4!=string::npos  )

	{
	  // si dejamos este continue, es porque queremos un archivo SIN triggers displace
	  //cout<<" ********* Triggers of displaced (ver la lista anterior) component found ***********"<<endl;
	  //cout<<alltnames<<endl;

	  //  checkflag=true;continue;
	  //checkflag=true;	cout<<" todos los nombres : "<<alltnames<<endl;// todos los que si tienen
	}
		*/	 

		
                  	
		if(trigger2==string::npos && trigger3==string::npos && trigger4==string::npos && trigger5==string::npos)
		//if(trigger1==string::npos )
		  {
		    // si dejamos este continue, es porque queremos un archivos CON triggers Dimuon8
		    //cout<<" ********* Triggers without displaced component found ***********"<<endl;
		    //cout<<alltnames<<endl;
		    
		    checkflag=true;continue;
		    //checkflag=true;	cout<<" todos los nombres : "<<alltnames<<endl;// todos los que no tienen
		    
		  }
		
		Int_t triv4 = 10, triv5 = 10, triv6 = 10, triv7 = 10;


        if(trigger2!=string::npos && trigger3==string::npos && trigger4==string::npos && trigger5==string::npos  )

	{
         triv4 = 1;  
          //checkflag=true;     cout<<" todos los nombres : "<<alltnames<<endl;// todos los que NO tienen V9                                                                                                     
        }

        else{
             triv4 = 0;

         }

	if(trigger3!=string::npos && trigger2==string::npos && trigger4==string::npos && trigger5==string::npos  )

	{
         triv5 = 1;  
          //checkflag=true;     cout<<" todos los nombres : "<<alltnames<<endl;// todos los que NO tienen V9                                                                                                     
        }

        else{
             triv5 = 0;

         }

	if(trigger4!=string::npos && trigger2==string::npos && trigger3==string::npos && trigger5==string::npos  )

	{
         triv6 = 1;  
          //checkflag=true;     cout<<" todos los nombres : "<<alltnames<<endl;// todos los que NO tienen V9                                                                                                     
        }

        else{
             triv6 = 0;

         }

	if(trigger5!=string::npos && trigger2==string::npos && trigger3==string::npos && trigger4==string::npos  )

	{
         triv7 = 1;  
          //checkflag=true;     cout<<" todos los nombres : "<<alltnames<<endl;// todos los que NO tienen V9                                                                                                     
        }

        else{
             triv7 = 0;

         }


      pv.SetXYZ(t.priVtxX,t.priVtxY,t.priVtxZ);
      EPV(0,0) = t.priVtxXE;
      EPV(1,1) = t.priVtxYE;
      EPV(2,2) = t.priVtxZE;
      EPV(0,1) = t.priVtxXYE;
      EPV(0,2) = t.priVtxXZE;
      EPV(1,2) = t.priVtxYZE;

      pvbs.SetXYZ(t.priVtxXBS,t.priVtxYBS,t.priVtxZ);
      EPVBS(0,0) = t.priVtxXBSE;
      EPVBS(1,1) = t.priVtxYBSE;
      EPVBS(2,2) = t.priVtxZBSE;
      EPVBS(0,1) = t.priVtxXYBSE;
      EPVBS(0,2) = t.priVtxXZBSE;
      EPVBS(1,2) = t.priVtxYZBSE;

      bsv.SetXYZ(t.PVXBS,t.PVYBS,t.PVZBS);
      EBSV(0,0) = t.PVXBSE;
      EBSV(1,1) = t.PVYBSE;
      EBSV(2,2) = t.PVZBSE;
      EBSV(0,1) = t.PVXYBSE;
      EBSV(0,2) = t.PVXZBSE;
      EBSV(1,2) = t.PVYZBSE;		 
       

       for(unsigned int i=0;i<t.B_kaskeym_mass->size();i++)
	 {


	 if(t.B_kas_lambda_charge1->at(i)==t.B_kas_lambda_charge2->at(i)) continue;
	 
	 if(t.B_kas_lambda_chi2->at(i)>26.) continue; 
         if(t.B_kaskeym_chi2->at(i)>26.) continue;  
         if(t.B_J_chi2->at(i)>26.) continue;  
         if(t.B_chi2->at(i)>26.) continue;
	 

	   TVector3 B(t.B_px->at(i),t.B_py->at(i),t.B_pz->at(i));

	   TVector3 Jpsi( t.B_J_px->at(i), t.B_J_py->at(i), t.B_J_pz->at(i) );
	   TVector3 kaskeym( t.B_kaskeym_px->at(i), t.B_kaskeym_py->at(i), t.B_kaskeym_pz->at(i) );
	   TVector3 lamb( t.B_kas_lambda_px->at(i), t.B_kas_lambda_py->at(i), t.B_kas_lambda_pz->at(i) );

	   TVector3 mu1( t.B_J_px1->at(i), t.B_J_py1->at(i), t.B_J_pz1->at(i) );
	   TVector3 mu2( t.B_J_px2->at(i), t.B_J_py2->at(i), t.B_J_pz2->at(i) );


	   TVector3 pi1( t.B_kas_lambda_px1->at(i), t.B_kas_lambda_py1->at(i), t.B_kas_lambda_pz1->at(i) );//proton
	   TVector3 pi2( t.B_kas_lambda_px2->at(i), t.B_kas_lambda_py2->at(i), t.B_kas_lambda_pz2->at(i) );//pion
	  
	   TVector3 pi3( t.B_kaskeym_px1->at(i), t.B_kaskeym_py1->at(i), t.B_kaskeym_pz1->at(i) );        
	   // viendo la distribucion me doy cuenta que el pion es px1 y lambda es px2

	   // if(t.B_kaskeym_charge1->at(i)!=t.B_kas_lambda_charge2->at(i)) continue;// conservacion del numero barionico
                                                                                 
	   TVector3 mumu = mu1+mu2;
	   TVector3 pipi = pi1+pi2;

	   pvrf.SetXYZ(t.priRfVtxX->at(i),t.priRfVtxY->at(i),t.priRfVtxZ->at(i));
	   EPVRf(0,0) = t.priRfVtxXE->at(i);
	   EPVRf(1,1) = t.priRfVtxYE->at(i);
	   EPVRf(2,2) = t.priRfVtxZE->at(i);
	   EPVRf(0,1) = t.priRfVtxXYE->at(i);
	   EPVRf(0,2) = t.priRfVtxXZE->at(i);
	   EPVRf(1,2) = t.priRfVtxYZE->at(i);	 

	   pvip.SetXYZ(t.pVtxIPX->at(i),t.pVtxIPY->at(i),t.pVtxIPZ->at(i));
	   EPVip(0,0) = t.pVtxIPXE->at(i);
	   EPVip(1,1) = t.pVtxIPYE->at(i);
	   EPVip(2,2) = t.pVtxIPZE->at(i);
	   EPVip(0,1) = t.pVtxIPXYE->at(i);
	   EPVip(0,2) = t.pVtxIPXZE->at(i);
	   EPVip(1,2) = t.pVtxIPYZE->at(i);

	   pvipBSc.SetXYZ(t.pVtxBSIPX->at(i),t.pVtxBSIPY->at(i),t.pVtxBSIPZ->at(i));
	   EPVipBSc(0,0) = t.pVtxBSIPXE->at(i);
	   EPVipBSc(1,1) = t.pVtxBSIPYE->at(i);
	   EPVipBSc(2,2) = t.pVtxBSIPZE->at(i);
	   EPVipBSc(0,1) = t.pVtxBSIPXYE->at(i);
	   EPVipBSc(0,2) = t.pVtxBSIPXZE->at(i);
	   EPVipBSc(1,2) = t.pVtxBSIPYZE->at(i);

	  pT.SetXYZ(t.B_px->at(i),t.B_py->at(i),0.0);

	  sv.SetXYZ(t.bDecayVtxX->at(i),t.bDecayVtxY->at(i),t.bDecayVtxZ->at(i));

	  ESV(0,0) = t.bDecayVtxXE->at(i);
	  ESV(1,1) = t.bDecayVtxYE->at(i);
	  ESV(2,2) = t.bDecayVtxZE->at(i);
	  ESV(0,1) = t.bDecayVtxXYE->at(i);
	  ESV(0,2) = t.bDecayVtxXZE->at(i);
	  ESV(1,2) = t.bDecayVtxYZE->at(i);

	  V0_Lifetime(pv,sv,EPV,ESV, 5.795, pT, ct, ect);
	  V0_Lifetime(pvbs,sv,EPVBS,ESV, 5.795, pT, ctbs, ectbs);
	  V0_Lifetime(pvrf,sv,EPVRf,ESV, 5.795, pT, ctrf, ectrf);
	  V0_Lifetime(bsv,sv,EBSV,ESV,5.795 , pT, ctBS, ctBSE);
	  V0_Lifetime(pvip,sv,EPVip,ESV, 5.795, pT, ctip, ectip);
	  V0_Lifetime(pvipBSc,sv,EPVipBSc,ESV, 5.795, pT, ctipBSc, ectipBSc);


	  // Aca calculamos el tiempo de vida de lambdacero y del kaskeymenos
       
       // tiempo de vida del kaskeymenos
       TVector3 kaskadapt(t.B_kaskeym_px->at(i), t.B_kaskeym_py->at(i),0.0);
       svKminus.SetXYZ(t.VDecayVtxX->at(i),t.VDecayVtxY->at(i),t.VDecayVtxZ->at(i));
       ESVKminus(0,0) = t.VDecayVtxXE->at(i);
       ESVKminus(1,1) = t.VDecayVtxYE->at(i);
       ESVKminus(2,2) = t.VDecayVtxZE->at(i);
       ESVKminus(0,1) = t.VDecayVtxXYE->at(i);
       ESVKminus(0,2) = t.VDecayVtxXZE->at(i);
       ESVKminus(1,2) = t.VDecayVtxYZE->at(i);
       V0_Lifetime(sv,svKminus,ESV,ESVKminus, 1.3217, kaskadapt, ctKminus, ectKminus);
       

       
       // tiempo de vida del lambdacero 	  
       TVector3 lambceropt(t.B_kas_lambda_px->at(i), t.B_kas_lambda_py->at(i),0.0);
       svlamb.SetXYZ(t.V1DecayVtxX->at(i),t.V1DecayVtxY->at(i),t.V1DecayVtxZ->at(i));
       ESVlamb(0,0) = t.V1DecayVtxXE->at(i);
       ESVlamb(1,1) = t.V1DecayVtxYE->at(i);
       ESVlamb(2,2) = t.V1DecayVtxZE->at(i);
       ESVlamb(0,1) = t.V1DecayVtxXYE->at(i);
       ESVlamb(0,2) = t.V1DecayVtxXZE->at(i);
       ESVlamb(1,2) = t.V1DecayVtxYZE->at(i);
       V0_Lifetime(sv,svlamb,ESV,ESVlamb, 1.1156, lambceropt, ctlamb, ectlamb);
       



         //*******************
         //*  Triggers Cuts  *
         //*******************
	 Double_t dxJ = t.JDecayVtxX->at(i) - t.priRfVtxX->at(i);
         Double_t dyJ = t.JDecayVtxY->at(i) - t.priRfVtxY->at(i);

	 
         Double_t dxJE = t.JDecayVtxXE->at(i);// + t.priVtxXE;
         Double_t dyJE = t.JDecayVtxYE->at(i);// + t.priVtxYE;
         
         // TVector3 LxyJtmp  = ( dxJ, dyJ, 0.);
         // Double_t LxyJmag2 = LxyJtmp.Mag2();
         // Double_t LxyJEtmp = ( dxJ*dxJE + dyJ*dyJE )/sqrt(dxJ*dxJ +dyJ*dyJ);
         
         Double_t sigLxyJtmp =(dxJ*dxJ +dyJ*dyJ)/sqrt( dxJ*dxJ*dxJE*dxJE + dyJ*dyJ*dyJE*dyJE );
         //Double_t sigLxyJtmp = LxyJmag2/sqrt( dxJ*dxJ*dxJE*dxJE + dyJ*dyJ*dyJE*dyJE );

	 // Double_t cosAlphaXY = ( t.B_J_px->at(i)*dxJ + t.B_J_py->at(i)*dyJ )/( sqrt(dxJ*dxJ+dyJ*dyJ)*Jpsi.Pt() );
         
	 
         if( t.B_J_mass->at(i)<2.9 || t.B_J_mass->at(i)>3.3 ) continue;
         if(fabs(mu1.Eta())>2.2 || fabs(mu2.Eta())>2.2) continue;
	 if(t.muon_dca->at(i)>0.5) continue;
         if( mu1.Pt()<4.0 || mu2.Pt()<4.0 )continue;
         if( Jpsi.Pt()<7.9) continue;
         Double_t cosAlphaXY = ( t.B_J_px->at(i)*dxJ + t.B_J_py->at(i)*dyJ )/( sqrt(dxJ*dxJ+dyJ*dyJ)*Jpsi.Pt() );
         //if(cosAlphaXY<0.9) continue;
         //if( sigLxyJtmp < 3.0 )continue;
	 
         
	 
         //*******************
         //*   Final Cuts    *
         //*******************
         
         //if( t.B_J_mass->at(i)<2.95 || t.B_J_mass->at(i)>3.25 ) continue;
	 
         //if( B.Pt()<=10. ) continue;
         //if( pi2.Pt()<1.5 ) continue;
         //if( Jpsi.Pt()<4.) continue;
         if( t.B_kas_lambda_mass->at(i)<1.05 || t.B_kas_lambda_mass->at(i)>1.16 ) continue;
         //if( lamb.Pt()<5.) continue;
         
	 // veto a la masa del ks0
	 TLorentzVector pp,pm, Kpp;
           //pp = (t.B_lamb_px1->at(i), t.B_lamb_py1->at(i), t.B_lamb_pz1->at(i),0.13957);
           //pm = (t.B_lamb_px2->at(i), t.B_lamb_py2->at(i), t.B_lamb_pz2->at(i),0.13957);
	   pp.SetXYZM( t.B_kas_lambda_px1->at(i), t.B_kas_lambda_py1->at(i), t.B_kas_lambda_pz1->at(i),0.13957);
	   pm.SetXYZM( t.B_kas_lambda_px2->at(i), t.B_kas_lambda_py2->at(i), t.B_kas_lambda_pz2->at(i),0.13957);
           Kpp = pp+pm;
           massKs0 = Kpp.M();

	 // veto a la masa del Omega^-
	 TLorentzVector Lcero,Kome, LceroKome;
	 Lcero.SetXYZM( t.B_kas_lambda_px->at(i), t.B_kas_lambda_py->at(i), t.B_kas_lambda_pz->at(i),1.11568);
	 Kome.SetXYZM( t.B_kaskeym_px1->at(i), t.B_kaskeym_py1->at(i), t.B_kaskeym_pz1->at(i),0.49367);
	 LceroKome = Lcero+Kome;
	 massOmem = LceroKome.M();
	  
   //string alltnames = t.triggersL;
         //cout<< "New Cand" << endl;
         string alltnamesM = t.triggersMuML->at(i);
         string alltnamesP = t.triggersMuPL->at(i);
         //cout<< " size = " << t.triggersMuML->size()<< endl;
         //cout<< " MuM \t\t" << t.triggersMuML->at(i)<< endl;
         //cout<< " MuP \t\t" << t.triggersMuPL->at(i)<< endl;HLT_Dimuon8_Jpsi
         
         // Trigger 1 cubre corrida 2012A
         string::size_type triggerM1 = alltnamesM.find("HLT_Dimuon8_Jpsi",0);
	 string::size_type triggerM2 = alltnamesM.find("HLT_Dimuon8_Jpsi",0);
	 string::size_type triggerM3 = alltnamesM.find("HLT_Dimuon8_Jpsi",0);
	 string::size_type triggerM4 = alltnamesM.find("HLT_Dimuon8_Jpsi",0);



	 // trigger 1 cubre corrida 2012A
         string::size_type triggerP1 = alltnamesP.find("HLT_Dimuon8_Jpsi",0);
	 string::size_type triggerP2 = alltnamesP.find("HLT_Dimuon8_Jpsi",0);
	 string::size_type triggerP3 = alltnamesP.find("HLT_Dimuon8_Jpsi",0);
	 string::size_type triggerP4 = alltnamesP.find("HLT_Dimuon8_Jpsi",0);



	 Int_t muM = 10, muP = 10;
         
         //string::size_type trigger1 = alltnames.find("HLT_Dimuon8_Jpsiv1",0);
         
          
         if(triggerM1!=string::npos || triggerM2!=string::npos || triggerM3!=string::npos || triggerM4!=string::npos )
         {
             muM = 1;
             //muP = 1;
	     //cout<< "MuM  = "<< muM << "\t" <<alltnamesM<< endl;
	     //cout<< "MuM2  = "<< muM << "\t" <<t.triggersMuML->at(i)<< endl;
             //cout<< "MuP  = "<< muM << "\t" <<t.triggersMuP<< endl;
         }
         else{
             muM = 0;
	     //cout<< "MuM  = "<< muM << "\t" <<alltnamesM<< endl;
	     //cout<< "MuM2  = "<< muM << "\t" <<t.triggersMuML->at(i)<< endl;

	     //continue;

         }
         
   
         if(triggerP1!=string::npos || triggerP2!=string::npos || triggerP3!=string::npos || triggerP4!=string::npos)
         {
             //muM = 1;
             muP = 1;
	     //cout<< "MuP  = "<< muP << "\t" <<alltnamesP<< endl;
	     //cout<< "MuP  = "<< muP << "\t" <<t.triggersMuPL->at(i)<< endl;
             //cout<< "MuP  = "<< muM*muP << "\t" <<t.triggersMuP<< endl;
         }
         else{
             //muM = 0;
             muP = 0;
	     //cout<< "MuP  = "<< muP << "\t" <<alltnamesP<< endl;
	     //cout<< "MuP  = "<< muP << "\t" <<t.triggersMuPL->at(i)<< endl;
             //             cout<< "MuP  = "<< muM*muP << "\t" <<t.triggersMuP<< endl;
	     //continue;
         }

	  mumDisplace = muM;
	  mupDisplace = muP;  

	  runn= t.run;
	  eventn =t.event;

	  triggerv4 = triv4;
	  triggerv5 = triv5;
	  triggerv6 = triv6;
	  triggerv7 = triv7;   
	  
	  softmu1 = t.mu1soft->at(i);
	  softmu2 = t.mu2soft->at(i);
	  tightmu1 =t.mu1tight->at(i);
	  tightmu2 = t.mu2tight->at(i);
	  PFmu1 = t.mu1PF->at(i);
	  PFmu2 = t.mu2PF->at(i);
	  mupcat = t.mupCat->at(i);
	  mumcat = t.mumCat->at(i);	 

          massB   = t.B_mass->at(i);
          massJ   = t.B_J_mass->at(i);
	  masskm  = t.B_kaskeym_mass->at(i); 
          masslamb  = t.B_kas_lambda_mass->at(i);
          sigLxyJ = sigLxyJtmp;
	  cosalfaJ= cosAlphaXY;

	  Bdl   = ct;
          BdlBS = ctbs;
          BdlRf = ctrf;
          BdlIP = ctip;
          BSct =  ctBS;
          BdlIPBSc = ctipBSc;
      	  BdlE  = ect;
	  BdlBSE = ectbs;
          BdlRfE = ectrf;
          BdlIPE = ectip;
	  BSctE  = ctBSE;
          BdlIPBScE = ectipBSc;

	  Bdlkminus   = ctKminus;
	  BdlkminusE   = ectKminus;
	  Bdllamb   = ctlamb;
	  BdllambE   = ectlamb;


          Beta    = B.Eta();
          Bphi    = B.Phi();
          Bpt     = B.Pt();
	  kmpt    = kaskeym.Pt();
          Jpsipt  = Jpsi.Pt();
          lambpt    = lamb.Pt();
	  Pi1pt   = pi1.Pt();
	  Pi2pt   = pi2.Pt();
	  Pi3pt   = pi3.Pt();

          mu1pt   = mu1.Pt();
          mu2pt   = mu2.Pt();

          mu1phi  = mu1.Phi();
          mu2phi  = mu2.Phi();
          Pi1phi  = pi1.Phi();
          Pi2phi  = pi2.Phi();
         
	  Jeta  =  Jpsi.Eta();
          mu1eta  = mu1.Eta();
          mu2eta  = mu2.Eta();
          Pi1eta  = pi1.Eta();
          Pi2eta  = pi2.Eta();
         
          lambchi2  = t.B_kas_lambda_chi2->at(i);
          Jchi2   = t.B_J_chi2->at(i);
          Bchi2   = t.B_chi2->at(i);
          kmchi2  = t.B_kaskeym_chi2->at(i);

	  lambprob = t.B_kas_lambda_Prob->at(i);
          Jprob = t.B_J_Prob->at(i);
	  kmprob = t.B_kaskey_Prob->at(i);
	  Bprob = t.B_Prob->at(i);
          
         B_kaskeym_charge1 = t.B_kaskeym_charge1->at(i);
         B_kas_lambda_charge2 = t.B_kas_lambda_charge2->at(i);

         if( pi1.Pt() > pi2.Pt() ){ 
             maxPtPi = pi1.Pt();
             minPtPi = pi2.Pt();
         }
         else{
             maxPtPi = pi2.Pt();
             minPtPi = pi1.Pt();
         }
         
         if( mu1.Pt() > mu2.Pt() ){
             maxPtMu = mu1.Pt();
             minPtMu = mu2.Pt();
         }
         else{
             maxPtMu = mu2.Pt();
             minPtMu = mu1.Pt();
         }
	     

	     

	     
	      // bdt = reader->EvaluateMVA( "BDT method"); //comentare la linea de pANN igual que arriba en la linea 123
		   // pANN = reader->EvaluateMVA( "CFMlpANN method"); 
	  
	   treeS->Fill();

	  
	 }
      

     }

   outputFile->Write("",TObject::kOverwrite);

}
