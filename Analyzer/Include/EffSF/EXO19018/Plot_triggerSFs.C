{

  gStyle->SetOptTitle(0);

  TCanvas *c1 = new TCanvas("c1","c1",600,500);
  c1->SetGrid(1);

  double trig_m17[40], trig_eff17[40], trig_m18[40], trig_eff18[40];
    
  trig_m17[0]=10; trig_eff17[0]=0.727503;
  trig_m17[1]=11; trig_eff17[1]=0.746364;
  trig_m17[2]=12; trig_eff17[2]=0.810494;
  trig_m17[3]=13; trig_eff17[3]=0.8204;
  trig_m17[4]=14; trig_eff17[4]=0.898983;
  trig_m17[5]=15; trig_eff17[5]=0.90467;
  trig_m17[6]=16; trig_eff17[6]=1.01008;
  trig_m17[7]=17; trig_eff17[7]=1.00477;
  trig_m17[8]=18; trig_eff17[8]=1.02568;
  trig_m17[9]=19; trig_eff17[9]=1.01105;
  trig_m17[10]=20; trig_eff17[10]=1.00677;
  trig_m17[11]=21; trig_eff17[11]=0.999231;
  trig_m17[12]=22; trig_eff17[12]=0.988251;
  trig_m17[13]=23; trig_eff17[13]=0.980694;
  trig_m17[14]=24; trig_eff17[14]=0.975543;
  trig_m17[15]=25; trig_eff17[15]=0.967542;
  trig_m17[16]=26; trig_eff17[16]=0.96623;
  trig_m17[17]=27; trig_eff17[17]=0.966218;
  trig_m17[18]=28; trig_eff17[18]=0.964542;
  trig_m17[19]=29; trig_eff17[19]=0.962014;
  trig_m17[20]=30; trig_eff17[20]=0.956478;
  trig_m17[21]=31; trig_eff17[21]=0.955751;
  trig_m17[22]=32; trig_eff17[22]=0.956883;
  trig_m17[23]=33; trig_eff17[23]=0.958776;
  trig_m17[24]=34; trig_eff17[24]=0.95482;
  trig_m17[25]=35; trig_eff17[25]=0.959728;
  trig_m17[26]=36; trig_eff17[26]=0.95988;
  trig_m17[27]=37; trig_eff17[27]=0.952963;
  trig_m17[28]=38; trig_eff17[28]=0.962361;
  trig_m17[29]=39; trig_eff17[29]=0.957897;
  trig_m17[30]=40; trig_eff17[30]=0.96577;
  trig_m17[31]=41; trig_eff17[31]=0.965084;
  trig_m17[32]=42; trig_eff17[32]=0.96419;
  trig_m17[33]=43; trig_eff17[33]=0.963852;
  trig_m17[34]=44; trig_eff17[34]=0.964465;
  trig_m17[35]=45; trig_eff17[35]=0.961237;
  trig_m17[36]=46; trig_eff17[36]=0.968556;
  trig_m17[37]=47; trig_eff17[37]=0.960362;
  trig_m17[38]=48; trig_eff17[38]=0.957999;
  trig_m17[39]=49; trig_eff17[39]=0.964524;
 
  trig_m18[0]=10; trig_eff18[0]=0.758032;
  trig_m18[1]=11; trig_eff18[1]=0.768598;
  trig_m18[2]=12; trig_eff18[2]=0.838427;
  trig_m18[3]=13; trig_eff18[3]=0.854779;
  trig_m18[4]=14; trig_eff18[4]=0.96348;
  trig_m18[5]=15; trig_eff18[5]=0.98152;
  trig_m18[6]=16; trig_eff18[6]=1.15563;
  trig_m18[7]=17; trig_eff18[7]=1.15339;
  trig_m18[8]=18; trig_eff18[8]=1.19541;
  trig_m18[9]=19; trig_eff18[9]=1.17814;
  trig_m18[10]=20; trig_eff18[10]=1.04647;
  trig_m18[11]=21; trig_eff18[11]=1.02751;
  trig_m18[12]=22; trig_eff18[12]=1.01191;
  trig_m18[13]=23; trig_eff18[13]=0.994165;
  trig_m18[14]=24; trig_eff18[14]=0.98092;
  trig_m18[15]=25; trig_eff18[15]=1.00391;
  trig_m18[16]=26; trig_eff18[16]=1.00124;
  trig_m18[17]=27; trig_eff18[17]=0.996933;
  trig_m18[18]=28; trig_eff18[18]=0.994192;
  trig_m18[19]=29; trig_eff18[19]=0.992214;
  trig_m18[20]=30; trig_eff18[20]=0.99332;
  trig_m18[21]=31; trig_eff18[21]=0.992092;
  trig_m18[22]=32; trig_eff18[22]=0.989655;
  trig_m18[23]=33; trig_eff18[23]=0.987916;
  trig_m18[24]=34; trig_eff18[24]=0.986879;
  trig_m18[25]=35; trig_eff18[25]=0.985675;
  trig_m18[26]=36; trig_eff18[26]=0.984387;
  trig_m18[27]=37; trig_eff18[27]=0.98357;
  trig_m18[28]=38; trig_eff18[28]=0.982824;
  trig_m18[29]=39; trig_eff18[29]=0.982225;
  trig_m18[30]=40; trig_eff18[30]=0.978093;
  trig_m18[31]=41; trig_eff18[31]=0.978513;
  trig_m18[32]=42; trig_eff18[32]=0.978749;
  trig_m18[33]=43; trig_eff18[33]=0.978802;
  trig_m18[34]=44; trig_eff18[34]=0.979245;
  trig_m18[35]=45; trig_eff18[35]=0.979236;
  trig_m18[36]=46; trig_eff18[36]=0.979381;
  trig_m18[37]=47; trig_eff18[37]=0.979502;
  trig_m18[38]=48; trig_eff18[38]=0.979546;
  trig_m18[39]=49; trig_eff18[39]=0.979765;

  double avg_unc_2017 = 0, avg_unc_2018 = 0;
  double total = 0;
  for (int i = 1; i < 36; i++) {
    total++;
    std::cout<<"mass "<<trig_m17[i]<<" eff17 "<<fabs(1 - trig_eff17[i])<<" eff18 "<<fabs(1 - trig_eff18[i])<<" total "<<total<<"\n";

    avg_unc_2017 += fabs(1 - trig_eff17[i]);
    avg_unc_2018 += fabs(1 - trig_eff18[i]);
  }
  std::cout<<"avg_unc_2017 "<<avg_unc_2017/total<<"\n";
  std::cout<<"avg_unc_2018 "<<avg_unc_2018/total<<"\n";

  TGraph *tg_trig17 = new TGraph(40, trig_m17, trig_eff17);
  TGraph *tg_trig18 = new TGraph(40, trig_m18, trig_eff18);

  tg_trig17->GetXaxis()->SetTitle("dimuon mass (GeV)");
  tg_trig17->GetYaxis()->SetTitle("Trigger eff. SFs");

  tg_trig17->GetYaxis()->SetRangeUser(0.6,1.4);

  tg_trig17->SetMarkerColor(kRed);
  tg_trig17->SetLineColor(kRed);
  tg_trig18->SetMarkerColor(kBlue);
  tg_trig18->SetLineColor(kBlue);

  tg_trig17->Draw();
  tg_trig17->Draw("*L");
  tg_trig18->Draw("*L");

  TLegend *legend = new TLegend(0.65,0.65,0.8,0.8,NULL,"brNDC");
  legend->SetTextSize(0.04);
  legend->SetTextFont(42);
  legend->SetFillColor(kWhite);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);

  legend->AddEntry(tg_trig17,"2017","PL");
  legend->AddEntry(tg_trig18,"2018","PL");

  legend->Draw("same");

  c1->SaveAs("Scouting_triggerSFs_2017_2018.pdf");

}
