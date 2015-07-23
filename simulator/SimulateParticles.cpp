#include "simulator.h"
#include "Rotater.h"

void SimulateParticles(Detector const& d, Particles& particles, PhotonEvent& photon_event, int& seed, bool print){

	for (unsigned int par = 0; par < particles.size(); par++)
		{
			auto& particle = particles.at(par);
			if (print) cout << "\n\tParticle = " << par << " with X, Y, Z, Theta, Phi, Beta =\n\t\t" << particle.X << ", " << particle.Y << ", " << particle.Z << ", " << particle.Theta << ", " << particle.Phi << ", " << particle.Beta << endl;
			if (print) cout << "\t\tPhotonsPerCm = " << particle.PhotonsPercm << endl;


			static double *par_theta, *par_phi;
			static double *pho_theta, *pho_phi;
 			par_theta = &particle.Theta;
			par_phi = &particle.Phi;

			Simulate_ParticlePath(d, particles[par], par, photon_event, seed, print);
			static Rotater r;
			r.Feed_Particle(*par_theta, *par_phi);

			for(int &pho = photon_event.iterator; pho < photon_event.Photons.size(); pho++)
			{
				pho_theta = &photon_event.Photons[pho].Theta;
				pho_phi = &photon_event.Photons[pho].Phi;

				pho_phi = &photon_event.Photons[pho].Phi;
				r.Rotate_Photon(*pho_theta, *pho_phi);
				photon_event.Photons[pho].UnitVector = Get_UnitVector(*pho_theta, *pho_phi);

			}
		}
}