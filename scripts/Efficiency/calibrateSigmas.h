
void calibrateSigmas(TTree &t1, TTree &t2, ParticleEvent &originals, TrackRecons &reconstructions, string const &particletype){

	auto expected_theta = 0.;
	auto expected_area  = 0.;
	auto actual_theta   = 0.;
	auto actual_area    = 0.;

	for (unsigned int ev; ev < t2.GetEntries(); ++ev){
		t1.GetEntry(ev); t2.GetEntry(ev);

		auto &pars = originals.Particles;
		auto &recons = reconstructions.Recon;

		for (unsigned int p = 0; p < pars.size(); ++p){
			auto par = pars.at(p);
			auto recon = recons.at(p);
			auto emission_angles = par.EmissionAngleMap();
			auto expected_theta = emission_angles[par.name];

			for (unsigned int i = 0; i < recon.Options.size(); ++i){
				string &name = recon.Options.at(i);
				if (name == par.name){
					expected_area  = recon.ExpectedNumber.at(i);
					actual_area    = recon.Areas.at(i);
					actual_theta   = recon.Params.at(i).at(1);
				}
			}
		}
	}


}