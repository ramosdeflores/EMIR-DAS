#include "SimpleStorageManager.h"
#include <unistd.h>


#define ADDSTRING(val1,val2,val3,val4)	addKeyword(DPKF::Keyword<string>(val1,val2,val3,val4));
#define ADDLONG(val1,val2,val3,val4)	addKeyword(DPKF::Keyword<long>(val1,val2,val3,val4));
#define ADDDOUBLE(val1,val2,val3,val4)	addKeyword(DPKF::Keyword<double>(val1,val2,val3,val4));

EMIRDFAGENT::EMIRDFAgent_ifce_var dfAgentInstance_;

TinyVector<unsigned long, 2> frameDimension(2048, 2048);
Frame<UINT16, 2> frame(frameDimension, NoErrors, BeginWithOne);


extern unsigned int AcqStatusFlag;

SimpleStorageManager::SimpleStorageManager(staticData data)
{
  trace_ = new Trace("SimpleStorageManager");
  fixData = data;
;
}

SimpleStorageManager::~SimpleStorageManager()
{
;
}

void SimpleStorageManager::initEMIRDFAgent_()
{
  trace_->out ("initEMIRDFAgent() starts");
  flushingFrames = false;
  try
  {
    if(CORBA::is_nil(const_cast<const EMIRDFAGENT::EMIRDFAgent_ifce_var&>(dfAgentInstance_)))
    {
      trace_->out ("Connecting to Data Factory Agent");
      CORBARef<EMIRDFAGENT::EMIRDFAgent_ifce> df_ref(fixData.dataFactoryName, 0);
      df_ref.roundtripTimeout( TimeValue(60, 0) );
      dfAgentInstance_ = df_ref.typedRef();
      dfAgentInstance_->ping();
      trace_->out ("Connected to Data Factory Agent!");
    }
  }
  catch(const CORBA::Exception &ex)
  {
    GCSException gcsex = DException::mapCORBAException(const_cast<CORBA::Exception&>(ex));
    cout << "initEMIRDFAgent() ERROR: " << gcsex << endl;
    fixData.logAgent_->logError("initEMIRDFAgent() exceptio %s", gcsex.text());
    if (gcsex.isA ("CorbaException"))
    {  //It is a timeout, set null the corba pointer
      dfAgentInstance_ = EMIRDFAGENT::EMIRDFAgent_ifce::_nil();
    }
    throw;
  }
  catch (GCSException& ex)
  {
    ex.addToTrace ("DataAcquisitionComponentSim::initEMIRDFAgent_()");
    cout << "initEMIRDFAgent() ERROR: " << ex.text() << endl;
    fixData.logAgent_->logError("initEMIRDFAgent() exception %s", ex.text());
    dfAgentInstance_ = EMIRDFAGENT::EMIRDFAgent_ifce::_nil();
    throw;
  }
  catch(...)
  {
    fixData.logAgent_->logError( "start() -  Unable to start the DAS Controller- Unknown Exception found");
    dfAgentInstance_ = EMIRDFAGENT::EMIRDFAgent_ifce::_nil();
    throw;
  }

  trace_->out ("initEMIRDFAgent() ends");
}

int SimpleStorageManager::initFrame()
{
   int count = 0;
   bool cont = true;
   char firmStr[256];

   trace_->out ("initFrame() starts");

   // Add code here to check for access to the directory
   baseDir = fixData.frameDir;
   frameCount = 0;

   if ( fixData.sendFrame ) {
      while ( cont ) {
         try {
            trace_->out ("sendFrame() checking CORBA connection status");
            // If thre is no  conection to the DFAgent, initialize
            if (CORBA::is_nil(const_cast<const EMIRDFAGENT::EMIRDFAgent_ifce_var&>(dfAgentInstance_)))
            {
               trace_->out ("sendFrame() initializacing CORBA conection");
               initEMIRDFAgent_();
               cont = false;
            }
         } catch(...) {
            fixData.logAgent_->logError("SimpleStorageManager() FAILED TO START CONNECTION - TRY = %d", count);
            std::cout << "FAILED TO START CONNECTION - TRY = " << ++count << endl;
            sleep(1);
         }
      }
      std::cout << "CONNECTION SUCCESSFUL" << endl;
   }

   sprintf(firmStr, "%02X", (unsigned long)fixData.profileFirmware);
   trace_->out ("Initializaing keywords");
   frame.ADDSTRING("DETVER",fixData.detectorType.c_str(),"401 Detector used: science or engineering","");
   frame.ADDLONG("NSEC"    ,-1,"402 Number of sequences executed at detector","");
   frame.ADDLONG("NRST"    ,-1,"403 Number of resets","");
   frame.ADDLONG("OPTRST"  ,-1,"404 Type of reset","");
   frame.ADDLONG("NIMGRST" ,-1,"405 Number of reset images","");
   frame.ADDLONG("NRDBL_ND",-1,"406 Number of non-productive reads before loop","");
   frame.ADDLONG("NRDBL"   ,-1,"407 Number of productive reads before loop","");
   frame.ADDLONG("NLOOP"   ,-1,"408 Iteration number","");
   frame.ADDLONG("TOPER"   ,-1,"409 Time periods in the detector sequence formula [ms]", "ms");
   frame.ADDLONG("NRDIL_ND",-1,"410 Number of non-productibe reads per loop iteration","");
   frame.ADDLONG("NRDIL"   ,-1,"411 Number of productive reads per loop iteration","");
   frame.ADDLONG("Y_SIZE"  ,-1,"412 Offset in pixels for window mode","");
   frame.ADDLONG("Y_SIZE_B",-1,"413 Number of readable columns (1024 - Y_SIZE)","");
   frame.ADDLONG("NPARAM_X",-1,"414 Number of dummy readouts of a line","");
   frame.ADDDOUBLE("GAIN",     fixData.gain,"416 Gain, in e-/ADU","");
   frame.ADDDOUBLE("RON",      fixData.ron,"417 Read-out noise, in ADU","");
   frame.ADDDOUBLE("VOFFSET",  fixData.voffset,"418 Voff","");
   frame.ADDDOUBLE("VRESET",   fixData.vreset,"419 Vreset","");
   frame.ADDLONG("WINMODE", -1,"420 Window mode: full, 128px","");
   frame.ADDDOUBLE("BIASGATE", fixData.biasgate,"421 Biasgate","");
   frame.ADDSTRING("FIRMVER", firmStr,"422 Controller firmware version","");


   frame.ADDLONG("NFRSEC",  -1,"423 Frames produced per detector exposure","");
   frame.ADDLONG("FRSEC",   -1,"424 Counter of the frame sequence","");
   frame.ADDLONG("NEXP",    -1,"425 Number of detector exposures","");
   frame.ADDLONG("EXP",     -1,"426 Detector exposures counter","");
   frame.ADDLONG("TSTAMP",-1,"Time stamp in UTC","s");

   // Engineering
   frame.ADDLONG("READMODN",-1,"E00 Readout (numerico) mode used for the observation","");
   frame.ADDDOUBLE("TSUTC1", -1.0, "E01 Time stamp before read detector (UTC)", "s");
   frame.ADDDOUBLE("TSUTC2", -1.0, "E02 Time stamp after read detector (UTC)", "s");
   frame.ADDDOUBLE("TSUTCD", -1.0, "E03 Diff. between TSUTC2 and TSUTC1", "s");
   frame.ADDLONG("ITER_NUM",-1.0, "E04 frame counter", "");
   frame.ADDLONG("FRSTAT",-1.0, "E05 status 0=ST,1=RUN,2=END,3=ABR,4=ERR", "");
   frame.ADDSTRING("APPNAME"  ,fixData.acqApp.c_str(),"E06 App","");

   trace_->out ("initFrame() ends");

   return DAS::OPERATION_OK;
}

int SimpleStorageManager::sendFrame(EMIRFrame *emirFrame) {
   UINT16 *iMatrix, *oMatrix;
   char  fileName[256];
   bool cont;
   long toper, nfrsec, nnsec, nimgrst, frsec;
   double hutc;
   float transTime;
   TimeValue ttamp0, ttamp1, utc;
   TimeValue tstamp0, tstamp1;

   trace_->out ("sendFrame() starts");

   if ( emirFrame->status == SEC_ABORT )
      if ( flushingFrames ) {
         trace_->out ("sendFrame() ABORT DETECTED FLUSSING FRAMES");
         return DAS::OPERATION_OK;
       } else {
         flushingFrames = true;
         trace_->out ("sendFrame() ABORT DETECTED SENDING NOTIFICATION FRAME");
    } else
      flushingFrames = false;

   iMatrix = (UINT16 *)(emirFrame->DetectorData + SDSU_HEADER_SIZE / 2);
   oMatrix = frame.data().begin();

   memcpy(oMatrix, iMatrix, sizeof(UINT16) * SDSU_SIZE);

   trace_->out ("Loading keywords");
   if ( ! emirFrame->nloop ) {
      nimgrst = (long)(((emirFrame->optrst & 2) ? 1 : 0));
      nfrsec  = (long)((((nimgrst) ? emirFrame->nrst : 0) + emirFrame->nrdbl + emirFrame->numExp * emirFrame->nrdil));
      toper   = (long)(emirFrame->texp);
      // EXPOSURE KEYWORDS
      frame.modifyKeyword("READMODN",(long)emirFrame->readoutmodeval);
      frame.modifyKeyword("WINMODE",(long)emirFrame->windowMode);
      frame.modifyKeyword("NRST",(long)emirFrame->nrst);
      frame.modifyKeyword("NRDBL_ND",(long)emirFrame->nrdbl_nd);
      frame.modifyKeyword("NRDBL",(long)emirFrame->nrdbl);
      frame.modifyKeyword("NSEC",(long)emirFrame->nsec);
      frame.modifyKeyword("NRDIL_ND",(long)emirFrame->nrdil_nd);
      frame.modifyKeyword("NRDIL",(long)emirFrame->nrdil);
      frame.modifyKeyword("Y_SIZE",(long)emirFrame->y_size);
      frame.modifyKeyword("Y_SIZE_B",(long)(1024 - emirFrame->y_size));
      frame.modifyKeyword("NPARAM_X",(long)emirFrame->nparam_x);
      frame.modifyKeyword("OPTRST",(long)emirFrame->optrst);
      frame.modifyKeyword("TOPER",(long)toper);
      frame.modifyKeyword("NLOOP",(long)emirFrame->numExp);
      frame.modifyKeyword("NFRSEC",(long)nfrsec);
      frame.modifyKeyword("NIMGRST",(long)nimgrst);
      frame.modifyKeyword("NEXP",(long)(emirFrame->nsec));
      sprintf(baseName, "%s/%s_", baseDir.c_str(), emirFrame->imageName);
      
   }
   frsec   = (long)(emirFrame->nloop % nfrsec + 1);
   frame.modifyKeyword("FRSEC",(long)frsec);
   frame.modifyKeyword("EXP", (long)((emirFrame->nloop / nfrsec) + 1));

   utc = emirFrame->timeStamp1 - TimeService::TAI_UTCOffset;

   frame.modifyKeyword("TSTAMP",(long)utc.sec());

   hutc = emirFrame->timeStamp1.toFloat();
   frame.modifyKeyword("TSUTC1", hutc);

   hutc = emirFrame->timeStamp2.toFloat();
   frame.modifyKeyword("TSUTC2", hutc);

   hutc = emirFrame->timeStamp2.toFloat() -  emirFrame->timeStamp1.toFloat();
   frame.modifyKeyword("TSUTCD", hutc);
   

   frame.modifyKeyword("ITER_NUM", (long)(emirFrame->nloop));
   frame.modifyKeyword("FRSTAT", (long)(emirFrame->status));

   trace_->out ("sendFrame() preparing to send frame %d", emirFrame->nloop);

   // Prepare to create the GTC frame
   TinyVector<unsigned long, 2>topLeft(0, 2048);
   DGT::TinyVector idlTopLeft;
   toIDL(topLeft, idlTopLeft);

   TinyVector<unsigned long, 2>bottomRight(2048, 0);
   DGT::TinyVector idlBottomRight;
   toIDL(bottomRight, idlBottomRight);

   DDPKF::Frame idlFrame;
   toIDL(frame, idlFrame, SHALLOW_COPY_DONT_RELEASE); 

   DGT::TimeValue idlTimeStamp = toIDL(emirFrame->timeStamp1);
   TimeValue exposureTime(0,emirFrame->texp * 1000);
   DGT::TimeValue idlExposureTime = toIDL(exposureTime);

   // If the MAKEFITS bit is on, a FITS file is created
   if ( fixData.saveFile ) {
      trace_->out ("sendFrame() creating FITS file");
      std::cout << "WRITING FRAME " << emirFrame->nloop << endl;
      fixData.logAgent_->logInfo("WRITING FRAME %d", emirFrame->nloop);
      tstamp0 = TimeService::getTAI();
      sprintf(fileName, "%s_%05d.fits", baseName, emirFrame->nloop);
      unlink(fileName);
      frame.toFits(fileName);
      tstamp1 = TimeService::getTAI();

      std::cout << "FRAME " << emirFrame->nloop << " READTIME " << hutc;
      std::cout << " WRITTEN -" << " TIME ELAPSED = " << tstamp1.toFloat() - tstamp0.toFloat() << " TOTAL ELAPSED = " << tstamp1.toFloat() - ttamp0.toFloat() << endl;
   }

   // If SENDFRAME bit is on, the frame is sent via CORBA
   if ( fixData.sendFrame ) {
      cont = false;
      do {
         try {
            trace_->out ("sendFrame() checking CORBA connection status");
            // If thre is no  conection to the DFAgent, initialize
            if (CORBA::is_nil(const_cast<const EMIRDFAGENT::EMIRDFAgent_ifce_var&>(dfAgentInstance_)))
            {
               trace_->out ("sendFrame() initializacing CORBA conection");
	       initEMIRDFAgent_();
            }
            trace_->out ("sendFrame() sending frame");
            try {
               std::cout << "SENDING FRAME " << emirFrame->nloop << endl;
               tstamp0 = TimeService::getTAI();
               dfAgentInstance_->receiveFrame(idlFrame, 0, 0, idlTopLeft, idlBottomRight, idlTimeStamp, idlExposureTime, "EMIR");
               tstamp1 = TimeService::getTAI();

               transTime = tstamp1.toFloat() - tstamp0.toFloat();
               std::cout << " SENT -" << " TRANS TIME = " << transTime << endl;
               fixData.logAgent_->logInfo("FRAME %d TRANS TIME = %f", emirFrame->nloop, transTime);
               cont = false;
            } catch(...) {
               dfAgentInstance_ = EMIRDFAGENT::EMIRDFAgent_ifce::_nil();
               cont = ! cont;
	       if ( ! cont ) {
                  std::cout << "sendFrame() CLONK ! - receiveFrame exception, retry failed - FRAME LOST !!! (NOT SENT)" << endl;
                  fixData.logAgent_->logError("SimpleStorageManager() receiveFrame exception, retry failed - FRAME %d NOT SENT !!!", emirFrame->nloop); 
                  AcqStatusFlag |= FRAME_LOST_T;
               } else 
                  std::cout << "sendFrame() OUTCH ! - receiveFrame exception, let's give it another try" << endl;
            }
         }
         catch(GCSException& ex)
         {
            AcqStatusFlag |= TRANS_FAILED;
            ex.addToTrace("SimpleStorageManager::sendFrame FAILED");
            std::cout << "sendFrame() CLONK ! - FAILED TO INIT CONNECTION - FRAME NOT SENT" << endl;
            fixData.logAgent_->logError("SimpleStorageManager() FAILED TO INIT CONNECTION - FRAME NOT SENT");
         }
       } while (cont);
   }

   ++frameCount;
   trace_->out ("sendFrame() ends");

   return DAS::OPERATION_OK;
}
