######################################################################
## Simply read files from the file photon_ncap_events.txt
######################################################################
#NCap_File = '../output/photon_ncap_events.txt' 

#InputFile = open(NCap_File, 'r')

#lines_InputFile = InputFile.readlines()

#InputFile.close()

#########################################################################
## Format photon_ncap_events.txt file into a HEPEvt style 
## to be used as input for NeutronBackground.cpp script
#########################################################################
Open_new_nCapFile = open('../output/photon_ncap.txt', 'r')

lines_InputFile = Open_new_nCapFile.readlines()

events = 0

for k in range(0, len(lines_InputFile)):
#  multiplicity = lines_InputFile[k].split()[3]
#  print(multiplicity)
#  print(len(lines_InputFile[k].split()))

  if(len(lines_InputFile[k].split()) != 6): #9 + 3*int(multiplicity))):
    continue

  events+=1

 # event_info = [lines_InputFile[k].split()[3], " ", lines_InputFile[k].split()[4]," ", lines_InputFile[k].split()[5]," ", 
  #              lines_InputFile[k].split()[6], " ", lines_InputFile[k].split()[7]," ", lines_InputFile[k].split()[8], "\n"]
  
  #Open_new_nCapFile.writelines(event_info)
  
 # for l in range(0, int(multiplicity)):
 #   momentum_dir = [lines_InputFile[k].split()[9+3*l], " ", lines_InputFile[k].split()[10+3*l]," ", lines_InputFile[k].split()[11+3*l], "\n"]
 #   Open_new_nCapFile.writelines(momentum_dir)

Open_new_nCapFile.close()

#########################################################################
## Use Template_Neutron_Background.mac and insert the run
## command with the calculated number of events above
#########################################################################
Open_Template = open('../macros/Template_Neutron_Background.mac', 'r')

lines_template= Open_Template.readlines()
lines_template[37] = '/run/beamOn '+str(events )

Open_Template.close()


Open_Macro = open('../macros/Template_Neutron_Background_forRun.mac', 'w')

Open_Macro.writelines(lines_template)
Open_Macro.close()
