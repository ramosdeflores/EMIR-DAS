// Copyright (c) 2010 Gran Telescopio Canarias, S.A. (GRANTECAN)
// All Rights Reserved

# Module definition:
NAME=EMIRDas
TYPE=device

# Module content definition:
l:SDSUsim.l:solgnu:y
l:EMIRDasController.l:vx solgnu:y
l:EMIRDas.l:vx solgnu:y
l:SDSUlcu.l:vx :y
l:test.l:vx :y
#p:runEMIRDasSolaris.p:vx solgnu:y
p:runEMIRDas.p:vx solgnu:y
e:testMemMan.p:vx solgnu:y
#e:useEMIRDas.p:vx solgnu:y
#t:testEMIRDas.p:vx solgnu:y
