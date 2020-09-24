// Use this example configuration file as a starting point for your own files.
{
  seed: 123456, // Random number seed (omit to use time since Unix epoch)

  // Reaction matrix element files
  reactions: [ "ve40ArCC_Bhattacharya2009.react" ],

  // Neutrino source specification
  source: {
   type: "fermi-dirac",
   neutrino: "ve",       // The source produces electron neutrinos
   Emin: 0,              // Minimum neutrino energy (MeV)
   Emax: 60,             // Maximum neutrino energy (MeV)
   temperature: 3.5,     // Temperature (MeV)
   eta: 0                // Pinching parameter (dimensionless, default 0)
  },

  // Incident neutrino direction 3-vector
  direction: { x: 0.0, y: -1.0, z: 0.0 },

}
