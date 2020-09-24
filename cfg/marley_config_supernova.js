// Use this example configuration file as a starting point for your own files.
{
  seed: 123456, // Random number seed (omit to use time since Unix epoch)

  // Reaction matrix element files
  reactions: [ "ve40ArCC_Bhattacharya2009.react" ],

  // Neutrino source specification
  // source: {
  //  type: "fermi-dirac",
  //  neutrino: "ve",       // The source produces electron neutrinos
  //  Emin: 0,              // Minimum neutrino energy (MeV)
  //  Emax: 60,             // Maximum neutrino energy (MeV)
  //  temperature: 3.5,     // Temperature (MeV)
  //  eta: 0                // Pinching parameter (dimensionless, default 0)
  // },

  //  HISTOGRAM
  // taken from fig10 https://arxiv.org/pdf/2008.06647.pdf
  //  source: {
  //    type: "histogram",
  //    neutrino: "ve",
  //    E_bin_lefts: [ 5. ,  5.5,  6. ,  6.5,  7. ,  7.5,  8. ,  8.5,  9. ,  9.5, 10. ,
  //     10.5, 11. , 11.5, 12. , 12.5, 13. , 13.5, 14. , 14.5, 15. , 15.5,
  //     16. , 16.5, 17. , 17.5, 18. , 18.5, 19. , 19.5, 20. , 20.5, 21. ,
  //     21.5, 22. , 22.5, 23. , 23.5, 24. , 24.5, 25. , 25.5, 26. , 26.5,
  //     27. , 27.5, 28. , 28.5, 29. , 29.5, 30. , 30.5, 31. , 31.5, 32. ,
  //     32.5, 33. , 33.5, 34. , 34.5, 35. , 35.5, 36. , 36.5, 37. , 37.5,
  //     38. , 38.5, 39. , 39.5, 40. , 40.5, 41. , 41.5],   // Low edges of energy bins (MeV)

  //    weights: [0.0003949 , 0.0009217 , 0.00190852, 0.0032898 , 0.00526266,
  //      0.00756416, 0.01026004, 0.0130872 , 0.01598036, 0.01893916,
  //      0.02170068, 0.02433075, 0.02656652, 0.02860519, 0.03038076,
  //      0.03176205, 0.03294606, 0.03373569, 0.03419675, 0.0343949 ,
  //      0.0342644 , 0.03393654, 0.03341166, 0.0326895 , 0.03177024,
  //      0.03078534, 0.02980018, 0.02861809, 0.02743582, 0.02612219,
  //      0.02480855, 0.02349491, 0.02218119, 0.02086747, 0.01948819,
  //      0.01817455, 0.01699228, 0.01574429, 0.01462793, 0.0135114 ,
  //      0.01239495, 0.01140979, 0.01049053, 0.00950554, 0.00865201,
  //      0.00786413, 0.00714214, 0.00648563, 0.00589511, 0.00523868,
  //      0.0047138 , 0.00425465, 0.0037955 , 0.00333627, 0.00300866,
  //      0.0026808 , 0.00248457, 0.00222252, 0.00196065, 0.00169869,
  //      0.00150228, 0.00130614, 0.00110982, 0.00097941, 0.00091446,
  //      0.00078388, 0.00065338, 0.00058878, 0.00045828, 0.00045906,
  //      0.0003942 , 0.00026388, 0.00026492, 0.00013434],       // Bin weights (dimensionless)
  //    Emax: 42.,                        // Upper edge of the final bin (MeV)
  //  },

  // source: {
  //   type: "histogram",
  //   neutrino: "ve",
  //   E_bin_lefts: [ 10., 20., 30. ],   // Low edges of energy bins (MeV)
  //   weights: [ 0.2, 0.5, 0.3 ],       // Bin weights (dimensionless)
  //   Emax: 40.,                        // Upper edge of the final bin (MeV)
  // },

  //  MONOENERGETIC
  // source: {
  //   type: "monoenergetic",
  //   neutrino: "ve",
  //   energy: 10,        // Neutrino energy (MeV)
  // },

  //  TH1
   source: {
     type: "th1",
     neutrino: "ve",
     tfile: "Supernova_TH1.root",  // Name of the ROOT file containing
                                  // the TH1 object
  
     namecycle: "no_soup_for_u",         // Name under which the TH1 object
                                  // appears in the file (used to
                                  // retrieve it)
   },

  // Incident neutrino direction 3-vector
  direction: { x: 0.0, y: -1.0, z: 0.0 },

}
