double DALEC_LIKELIHOOD_GPP(DATA D){
/*Data structure, includes model and data*/

/*EWT constraint*/
double tot_exp=0;
double am=0, mam=0;
int n,m,dn;
double P=0;
double mgpp=1,msif=1;
/*General notes*/
/* If D.et_annual_unc<1, then ET constraint is occurring on monthly basis*/
/* For log_et_obs*/
double th=D.gpp_obs_threshold;

/*GPP likelyhood*/
if (D.ngpp>0){
tot_exp=0;

/*Normalization option for SIF*/
if (D.gppabs==0){
mgpp=0,msif=0;
for (n=0;n<D.ngpp;n++){dn=D.gpppts[n];mgpp+=D.M_GPP[dn];msif+=D.GPP[dn];}
mgpp=mgpp/(double)D.ngpp;
msif=msif/(double)D.ngpp;}


if (D.gpp_annual_unc<1){
/*Standard NEE likelihood approach*/
for (n=0;n<D.ngpp;n++){dn=D.gpppts[n];tot_exp+=pow(log(max(D.M_GPP[dn]/mgpp,th)/max(D.GPP[dn]/msif,th))/log(D.gpp_obs_unc),2);}}
else{
/*Decoupling seasonal from interannual variations*/
/*Only use with monthly resolution fluxes, complete years & no missing data*/
/*Step 1. Mean model & data annual NBE*/
/*Step 2. Compare means*/
/*Step 3. Remove means from months for cost function*/
for (m=0;m<D.ngpp/12;m++){
/*Calculate annual mean*/
mam=0;am=0;
for (n=0;n<12;n++){dn=D.gpppts[n+m*12];mam=mam+D.M_GPP[dn]/mgpp;am=am+D.GPP[dn]/msif;}
/*Calculate seasonal cost function*/
for (n=0;n<12;n++){dn=D.gpppts[n+m*12];tot_exp+=pow(log(max(D.M_GPP[dn]/mgpp,th)/mam/(max(D.GPP[dn]/msif,th)/am))/log(D.gpp_obs_unc),2);}
/*Calculate annual cost function*/
tot_exp+=pow(log(am/mam)/log(D.gpp_annual_unc),2);
}}
P=P-0.5*tot_exp;}




/*Constrain mean GPP*/
if (D.otherpriors[5]>-9999){mgpp=0;
/*Step 1. Sum gpp*/
for (n=0;n<D.nodays;n++){mgpp+=D.M_FLUXES[n*D.nofluxes+0];}
/*Normalize gpp constraint to daily mean flux*/
mgpp=mgpp/((double)D.nodays);
/*Step 2. Constrain against gpp*/
if (D.otherpriorunc[5]>0){P=P-0.5*pow(log(mgpp/D.otherpriors[5])/log(D.otherpriorunc[5]),2);}
else {P=P-0.5*pow((mgpp-D.otherpriors[5])/D.otherpriorunc[5],2);}
/*P=P-0.5*pow((mgpp-D.otherpriors[5])/D.otherpriorunc[5],2);*/

}

//printf("Done with GPP cost function\n");




return P;
}
