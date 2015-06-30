package EMIRDAS;


/**
* EMIRDAS/EMIRDas_ifcePOA.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from EMIRDAS.idl
* jueves 11 de julio de 2013 19H47' WEST
*/

public abstract class EMIRDas_ifcePOA extends org.omg.PortableServer.Servant
 implements EMIRDAS.EMIRDas_ifceOperations, org.omg.CORBA.portable.InvokeHandler
{

  // Constructors

  private static java.util.Hashtable _methods = new java.util.Hashtable ();
  static
  {
    _methods.put ("setupDetector", new java.lang.Integer (0));
    _methods.put ("expose", new java.lang.Integer (1));
    _methods.put ("setupReadoutMode", new java.lang.Integer (2));
    _methods.put ("setupImageTitle", new java.lang.Integer (3));
    _methods.put ("setupExposureTime", new java.lang.Integer (4));
    _methods.put ("setupWindowMode", new java.lang.Integer (5));
    _methods.put ("setupNSEC", new java.lang.Integer (6));
    _methods.put ("setupNO_EXPOSURES", new java.lang.Integer (7));
    _methods.put ("setupNRDBL", new java.lang.Integer (8));
    _methods.put ("setupNRDBL_ND", new java.lang.Integer (9));
    _methods.put ("setupNRST", new java.lang.Integer (10));
    _methods.put ("setupNPARAM_X", new java.lang.Integer (11));
    _methods.put ("setupNRDIL", new java.lang.Integer (12));
    _methods.put ("setupNRDIL_ND", new java.lang.Integer (13));
    _methods.put ("setupOPTRST", new java.lang.Integer (14));
    _methods.put ("setupX_SIZE", new java.lang.Integer (15));
    _methods.put ("setupY_SIZE", new java.lang.Integer (16));
    _methods.put ("setupY_SIZE_B", new java.lang.Integer (17));
    _methods.put ("_get_isSettingup", new java.lang.Integer (18));
    _methods.put ("_get_isConfigurationDone", new java.lang.Integer (19));
    _methods.put ("_get_isExposing", new java.lang.Integer (20));
    _methods.put ("_get_isDetectorControllerIdle", new java.lang.Integer (21));
    _methods.put ("_get_currentReadoutMode", new java.lang.Integer (22));
    _methods.put ("_get_currentSDSUControllerStatus", new java.lang.Integer (23));
    _methods.put ("_get_currentElapsedTime", new java.lang.Integer (24));
    _methods.put ("_get_currentExposureTime", new java.lang.Integer (25));
    _methods.put ("_get_currentNumberOfImages", new java.lang.Integer (26));
    _methods.put ("_get_currentNSEC", new java.lang.Integer (27));
    _methods.put ("_get_currentNO_EXPOSURES", new java.lang.Integer (28));
    _methods.put ("_get_currentNRDBL", new java.lang.Integer (29));
    _methods.put ("_get_currentNRDBL_ND", new java.lang.Integer (30));
    _methods.put ("_get_currentNPARAM_X", new java.lang.Integer (31));
    _methods.put ("_get_currentNRST", new java.lang.Integer (32));
    _methods.put ("_get_currentNRDIL", new java.lang.Integer (33));
    _methods.put ("_get_currentNRDIL_ND", new java.lang.Integer (34));
    _methods.put ("_get_currentOPTRST", new java.lang.Integer (35));
    _methods.put ("_get_currentX_SIZE", new java.lang.Integer (36));
    _methods.put ("_get_currentY_SIZE", new java.lang.Integer (37));
    _methods.put ("_get_currentY_SIZE_B", new java.lang.Integer (38));
    _methods.put ("_get_poweronApp", new java.lang.Integer (39));
    _methods.put ("_set_poweronApp", new java.lang.Integer (40));
    _methods.put ("_get_poweroffApp", new java.lang.Integer (41));
    _methods.put ("_set_poweroffApp", new java.lang.Integer (42));
    _methods.put ("_get_adqApp", new java.lang.Integer (43));
    _methods.put ("_set_adqApp", new java.lang.Integer (44));
    _methods.put ("_get_imgDir", new java.lang.Integer (45));
    _methods.put ("_set_imgDir", new java.lang.Integer (46));
    _methods.put ("enable", new java.lang.Integer (47));
    _methods.put ("disable", new java.lang.Integer (48));
    _methods.put ("start", new java.lang.Integer (49));
    _methods.put ("init", new java.lang.Integer (50));
    _methods.put ("halt", new java.lang.Integer (51));
    _methods.put ("shutdown", new java.lang.Integer (52));
    _methods.put ("restore", new java.lang.Integer (53));
    _methods.put ("abort", new java.lang.Integer (54));
    _methods.put ("reset", new java.lang.Integer (55));
    _methods.put ("test", new java.lang.Integer (56));
    _methods.put ("configure", new java.lang.Integer (57));
    _methods.put ("ping", new java.lang.Integer (58));
    _methods.put ("report", new java.lang.Integer (59));
    _methods.put ("internals", new java.lang.Integer (60));
    _methods.put ("isOff", new java.lang.Integer (61));
    _methods.put ("isStarting", new java.lang.Integer (62));
    _methods.put ("isShuttingDown", new java.lang.Integer (63));
    _methods.put ("isOn", new java.lang.Integer (64));
    _methods.put ("isInitialising", new java.lang.Integer (65));
    _methods.put ("isHalting", new java.lang.Integer (66));
    _methods.put ("isIdle", new java.lang.Integer (67));
    _methods.put ("isRunning", new java.lang.Integer (68));
    _methods.put ("isDisabled", new java.lang.Integer (69));
    _methods.put ("isFault", new java.lang.Integer (70));
    _methods.put ("isResetting", new java.lang.Integer (71));
    _methods.put ("isReady", new java.lang.Integer (72));
    _methods.put ("isConfigurable", new java.lang.Integer (73));
    _methods.put ("isComposite", new java.lang.Integer (74));
    _methods.put ("isIntegrated", new java.lang.Integer (75));
    _methods.put ("_get_name", new java.lang.Integer (76));
    _methods.put ("_get_className", new java.lang.Integer (77));
    _methods.put ("_get_version", new java.lang.Integer (78));
    _methods.put ("_get_state", new java.lang.Integer (79));
    _methods.put ("_get_startTime", new java.lang.Integer (80));
    _methods.put ("_get_magnitudes", new java.lang.Integer (81));
    _methods.put ("_get_properties", new java.lang.Integer (82));
    _methods.put ("_get_alarms", new java.lang.Integer (83));
    _methods.put ("_get_configuration", new java.lang.Integer (84));
    _methods.put ("_set_configuration", new java.lang.Integer (85));
    _methods.put ("magnitudeMonitor", new java.lang.Integer (86));
    _methods.put ("startDebug", new java.lang.Integer (87));
    _methods.put ("stopDebug", new java.lang.Integer (88));
  }

  public org.omg.CORBA.portable.OutputStream _invoke (String $method,
                                org.omg.CORBA.portable.InputStream in,
                                org.omg.CORBA.portable.ResponseHandler $rh)
  {
    org.omg.CORBA.portable.OutputStream out = null;
    java.lang.Integer __method = (java.lang.Integer)_methods.get ($method);
    if (__method == null)
      throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);

    switch (__method.intValue ())
    {

  // Specific EMIRDas operations
       case 0:  // EMIRDAS/EMIRDas_ifce/setupDetector
       {
         try {
           EMIRTYPES.AcquisitionConfiguration acquisitionConfiguration = EMIRTYPES.AcquisitionConfigurationHelper.read (in);
           this.setupDetector (acquisitionConfiguration);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 1:  // EMIRDAS/EMIRDas_ifce/expose
       {
         try {
           this.expose ();
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 2:  // EMIRDAS/EMIRDas_ifce/setupReadoutMode
       {
         try {
           EMIRTYPES.ReadoutMode readoutmodeval = EMIRTYPES.ReadoutModeHelper.read (in);
           this.setupReadoutMode (readoutmodeval);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 3:  // EMIRDAS/EMIRDas_ifce/setupImageTitle
       {
         try {
           String exposureId = in.read_string ();
           this.setupImageTitle (exposureId);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 4:  // EMIRDAS/EMIRDas_ifce/setupExposureTime
       {
         try {
           int exposureTime = in.read_long ();
           this.setupExposureTime (exposureTime);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 5:  // EMIRDAS/EMIRDas_ifce/setupWindowMode
       {
         try {
           boolean windowmode = in.read_boolean ();
           this.setupWindowMode (windowmode);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 6:  // EMIRDAS/EMIRDas_ifce/setupNSEC
       {
         try {
           int nsec = in.read_long ();
           this.setupNSEC (nsec);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 7:  // EMIRDAS/EMIRDas_ifce/setupNO_EXPOSURES
       {
         try {
           int nexp = in.read_long ();
           this.setupNO_EXPOSURES (nexp);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 8:  // EMIRDAS/EMIRDas_ifce/setupNRDBL
       {
         try {
           int nrdbl = in.read_long ();
           this.setupNRDBL (nrdbl);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 9:  // EMIRDAS/EMIRDas_ifce/setupNRDBL_ND
       {
         try {
           int nrdbl_nd = in.read_long ();
           this.setupNRDBL_ND (nrdbl_nd);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 10:  // EMIRDAS/EMIRDas_ifce/setupNRST
       {
         try {
           int nrst = in.read_long ();
           this.setupNRST (nrst);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 11:  // EMIRDAS/EMIRDas_ifce/setupNPARAM_X
       {
         try {
           int nparam_x = in.read_long ();
           this.setupNPARAM_X (nparam_x);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 12:  // EMIRDAS/EMIRDas_ifce/setupNRDIL
       {
         try {
           int nrdil = in.read_long ();
           this.setupNRDIL (nrdil);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 13:  // EMIRDAS/EMIRDas_ifce/setupNRDIL_ND
       {
         try {
           int nrdil_nd = in.read_long ();
           this.setupNRDIL_ND (nrdil_nd);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 14:  // EMIRDAS/EMIRDas_ifce/setupOPTRST
       {
         try {
           int optrst = in.read_long ();
           this.setupOPTRST (optrst);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 15:  // EMIRDAS/EMIRDas_ifce/setupX_SIZE
       {
         try {
           int x_size = in.read_long ();
           this.setupX_SIZE (x_size);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 16:  // EMIRDAS/EMIRDas_ifce/setupY_SIZE
       {
         try {
           int y_size = in.read_long ();
           this.setupY_SIZE (y_size);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }

       case 17:  // EMIRDAS/EMIRDas_ifce/setupY_SIZE_B
       {
         try {
           int y_size_b = in.read_long ();
           this.setupY_SIZE_B (y_size_b);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  // Monitorable attributes (device magnitudes)
       case 18:  // EMIRDAS/EMIRDas_ifce/_get_isSettingup
       {
         boolean $result = false;
         $result = this.isSettingup ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 19:  // EMIRDAS/EMIRDas_ifce/_get_isConfigurationDone
       {
         boolean $result = false;
         $result = this.isConfigurationDone ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 20:  // EMIRDAS/EMIRDas_ifce/_get_isExposing
       {
         boolean $result = false;
         $result = this.isExposing ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 21:  // EMIRDAS/EMIRDas_ifce/_get_isDetectorControllerIdle
       {
         boolean $result = false;
         $result = this.isDetectorControllerIdle ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 22:  // EMIRDAS/EMIRDas_ifce/_get_currentReadoutMode
       {
         EMIRTYPES.ReadoutMode $result = null;
         $result = this.currentReadoutMode ();
         out = $rh.createReply();
         EMIRTYPES.ReadoutModeHelper.write (out, $result);
         break;
       }

       case 23:  // EMIRDAS/EMIRDas_ifce/_get_currentSDSUControllerStatus
       {
         EMIRTYPES.SDSUControllerStatus $result = null;
         $result = this.currentSDSUControllerStatus ();
         out = $rh.createReply();
         EMIRTYPES.SDSUControllerStatusHelper.write (out, $result);
         break;
       }

       case 24:  // EMIRDAS/EMIRDas_ifce/_get_currentElapsedTime
       {
         int $result = (int)0;
         $result = this.currentElapsedTime ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }

       case 25:  // EMIRDAS/EMIRDas_ifce/_get_currentExposureTime
       {
         int $result = (int)0;
         $result = this.currentExposureTime ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }

       case 26:  // EMIRDAS/EMIRDas_ifce/_get_currentNumberOfImages
       {
         int $result = (int)0;
         $result = this.currentNumberOfImages ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }

       case 27:  // EMIRDAS/EMIRDas_ifce/_get_currentNSEC
       {
         int $result = (int)0;
         $result = this.currentNSEC ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }

       case 28:  // EMIRDAS/EMIRDas_ifce/_get_currentNO_EXPOSURES
       {
         int $result = (int)0;
         $result = this.currentNO_EXPOSURES ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }

       case 29:  // EMIRDAS/EMIRDas_ifce/_get_currentNRDBL
       {
         int $result = (int)0;
         $result = this.currentNRDBL ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }

       case 30:  // EMIRDAS/EMIRDas_ifce/_get_currentNRDBL_ND
       {
         int $result = (int)0;
         $result = this.currentNRDBL_ND ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }

       case 31:  // EMIRDAS/EMIRDas_ifce/_get_currentNPARAM_X
       {
         int $result = (int)0;
         $result = this.currentNPARAM_X ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }

       case 32:  // EMIRDAS/EMIRDas_ifce/_get_currentNRST
       {
         int $result = (int)0;
         $result = this.currentNRST ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }

       case 33:  // EMIRDAS/EMIRDas_ifce/_get_currentNRDIL
       {
         int $result = (int)0;
         $result = this.currentNRDIL ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }

       case 34:  // EMIRDAS/EMIRDas_ifce/_get_currentNRDIL_ND
       {
         int $result = (int)0;
         $result = this.currentNRDIL_ND ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }

       case 35:  // EMIRDAS/EMIRDas_ifce/_get_currentOPTRST
       {
         int $result = (int)0;
         $result = this.currentOPTRST ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }

       case 36:  // EMIRDAS/EMIRDas_ifce/_get_currentX_SIZE
       {
         int $result = (int)0;
         $result = this.currentX_SIZE ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }

       case 37:  // EMIRDAS/EMIRDas_ifce/_get_currentY_SIZE
       {
         int $result = (int)0;
         $result = this.currentY_SIZE ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }

       case 38:  // EMIRDAS/EMIRDas_ifce/_get_currentY_SIZE_B
       {
         int $result = (int)0;
         $result = this.currentY_SIZE_B ();
         out = $rh.createReply();
         out.write_long ($result);
         break;
       }


  // Configurable attributes (device properties)
       case 39:  // EMIRDAS/EMIRDas_ifce/_get_poweronApp
       {
         String $result = null;
         $result = this.poweronApp ();
         out = $rh.createReply();
         out.write_string ($result);
         break;
       }


  // Configurable attributes (device properties)
       case 40:  // EMIRDAS/EMIRDas_ifce/_set_poweronApp
       {
         String newPoweronApp = in.read_string ();
         this.poweronApp (newPoweronApp);
         out = $rh.createReply();
         break;
       }

       case 41:  // EMIRDAS/EMIRDas_ifce/_get_poweroffApp
       {
         String $result = null;
         $result = this.poweroffApp ();
         out = $rh.createReply();
         out.write_string ($result);
         break;
       }

       case 42:  // EMIRDAS/EMIRDas_ifce/_set_poweroffApp
       {
         String newPoweroffApp = in.read_string ();
         this.poweroffApp (newPoweroffApp);
         out = $rh.createReply();
         break;
       }

       case 43:  // EMIRDAS/EMIRDas_ifce/_get_adqApp
       {
         String $result = null;
         $result = this.adqApp ();
         out = $rh.createReply();
         out.write_string ($result);
         break;
       }

       case 44:  // EMIRDAS/EMIRDas_ifce/_set_adqApp
       {
         String newAdqApp = in.read_string ();
         this.adqApp (newAdqApp);
         out = $rh.createReply();
         break;
       }

       case 45:  // EMIRDAS/EMIRDas_ifce/_get_imgDir
       {
         String $result = null;
         $result = this.imgDir ();
         out = $rh.createReply();
         out.write_string ($result);
         break;
       }

       case 46:  // EMIRDAS/EMIRDas_ifce/_set_imgDir
       {
         String newImgDir = in.read_string ();
         this.imgDir (newImgDir);
         out = $rh.createReply();
         break;
       }


  /**
      * Enables the device
      */
       case 47:  // DCF/Device_ifce/enable
       {
         try {
           this.enable ();
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /**
      * Goes to DISABLE state. Only possible from OFF,ON,IDLE,FAULT
      */
       case 48:  // DCF/Device_ifce/disable
       {
         try {
           this.disable ();
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /**
      * Starting procedure
      */
       case 49:  // DCF/Device_ifce/start
       {
         try {
           this.start ();
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /**
      * Initialization procedure
      */
       case 50:  // DCF/Device_ifce/init
       {
         try {
           this.init ();
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /**
      * Halting procedure
      */
       case 51:  // DCF/Device_ifce/halt
       {
         try {
           this.halt ();
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /**
      * Shutdown procedure
      */
       case 52:  // DCF/Device_ifce/shutdown
       {
         try {
           this.shutdown ();
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /**
      * Restore previous situation before FAULT
      */
       case 53:  // DCF/Device_ifce/restore
       {
         try {
           this.restore ();
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /**
      * Stop all activity and goes to FAULT
      */
       case 54:  // DCF/Device_ifce/abort
       {
         try {
           this.abort ();
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /**
      * Resets the current settings, connections, etc of the Device.
      */
       case 55:  // DCF/Device_ifce/reset
       {
         try {
           this.reset ();
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /**
      * Performs some kind of test and returns a report about the results.
      */
       case 56:  // DCF/Device_ifce/test
       {
         try {
           this.test ();
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /**
      * Sets a configuration by name
      */
       case 57:  // DCF/Device_ifce/configure
       {
         try {
           String config_name = in.read_string ();
           this.configure (config_name);
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /**
      * Checks that communication with a Device object is possible via CORBA.
      * In TAO at least, if this method is oneway it only checks for the sanity
      * of the remote ORB, but it does not garantee anything about the concrete 
      * remote object. For this reason the oneway has been removed.
      */
       case 58:  // DCF/Device_ifce/ping
       {
         try {
           this.ping ();
           out = $rh.createReply();
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /**
      * Returns a text reporting the.
      * @param level (SHORT, FULL, NORMAL) 
      */
       case 59:  // DCF/Device_ifce/report
       {
         try {
           short level = in.read_short ();
           String $result = null;
           $result = this.report (level);
           out = $rh.createReply();
           out.write_string ($result);
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /**
      * Returns a text reporting internal implementation details of Device. 
      */
       case 60:  // DCF/Device_ifce/internals
       {
         try {
           String $result = null;
           $result = this.internals ();
           out = $rh.createReply();
           out.write_string ($result);
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /**
      * To be able to ask for a certain state
      */
       case 61:  // DCF/Device_ifce/isOff
       {
         boolean $result = false;
         $result = this.isOff ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 62:  // DCF/Device_ifce/isStarting
       {
         boolean $result = false;
         $result = this.isStarting ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 63:  // DCF/Device_ifce/isShuttingDown
       {
         boolean $result = false;
         $result = this.isShuttingDown ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 64:  // DCF/Device_ifce/isOn
       {
         boolean $result = false;
         $result = this.isOn ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 65:  // DCF/Device_ifce/isInitialising
       {
         boolean $result = false;
         $result = this.isInitialising ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 66:  // DCF/Device_ifce/isHalting
       {
         boolean $result = false;
         $result = this.isHalting ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 67:  // DCF/Device_ifce/isIdle
       {
         boolean $result = false;
         $result = this.isIdle ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 68:  // DCF/Device_ifce/isRunning
       {
         boolean $result = false;
         $result = this.isRunning ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 69:  // DCF/Device_ifce/isDisabled
       {
         boolean $result = false;
         $result = this.isDisabled ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 70:  // DCF/Device_ifce/isFault
       {
         boolean $result = false;
         $result = this.isFault ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }

       case 71:  // DCF/Device_ifce/isResetting
       {
         boolean $result = false;
         $result = this.isResetting ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }


  /**
      * A Device is Ready in IDLE or RUN states. NOT in DISABLED.
      */
       case 72:  // DCF/Device_ifce/isReady
       {
         boolean $result = false;
         $result = this.isReady ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }


  /**
      * A Device is configurable in STARTING, ON, IDLE, FAULT
      */
       case 73:  // DCF/Device_ifce/isConfigurable
       {
         boolean $result = false;
         $result = this.isConfigurable ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }


  /**
      * A Device is composite if has aggregated Devices
      */
       case 74:  // DCF/Device_ifce/isComposite
       {
         boolean $result = false;
         $result = this.isComposite ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }


  /**
      * A Device is INtegrated if is connected to the rest of GCS
      */
       case 75:  // DCF/Device_ifce/isIntegrated
       {
         boolean $result = false;
         $result = this.isIntegrated ();
         out = $rh.createReply();
         out.write_boolean ($result);
         break;
       }


  /**
      * The Device's instance name.
      */
       case 76:  // DCF/Device_ifce/_get_name
       {
         String $result = null;
         $result = this.name ();
         out = $rh.createReply();
         out.write_string ($result);
         break;
       }


  /**
      * The Device's class name.
      */
       case 77:  // DCF/Device_ifce/_get_className
       {
         String $result = null;
         $result = this.className ();
         out = $rh.createReply();
         out.write_string ($result);
         break;
       }


  /**
      * The Device's version.
      */
       case 78:  // DCF/Device_ifce/_get_version
       {
         String $result = null;
         $result = this.version ();
         out = $rh.createReply();
         out.write_string ($result);
         break;
       }


  /**
      * The Device's current state: OFF, ON, STARTING, IDLE, etc.
      */
       case 79:  // DCF/Device_ifce/_get_state
       {
         DGT.State_t $result = null;
         $result = this.state ();
         out = $rh.createReply();
         DGT.State_tHelper.write (out, $result);
         break;
       }


  /**
      * The time when the Device started running.
      */
       case 80:  // DCF/Device_ifce/_get_startTime
       {
         DGT.TimeValue $result = null;
         $result = this.startTime ();
         out = $rh.createReply();
         DGT.TimeValueHelper.write (out, $result);
         break;
       }


  /**
      * The list of monitorable magnitudes names supported by this Device.
      */
       case 81:  // DCF/Device_ifce/_get_magnitudes
       {
         String $result[] = null;
         $result = this.magnitudes ();
         out = $rh.createReply();
         DGT.StringListHelper.write (out, $result);
         break;
       }


  /**
      * The list of property names supported by this Device.
      */
       case 82:  // DCF/Device_ifce/_get_properties
       {
         String $result[] = null;
         $result = this.properties ();
         out = $rh.createReply();
         DGT.StringListHelper.write (out, $result);
         break;
       }


  /**
      * The list of alarm names supported by this Device.
      */
       case 83:  // DCF/Device_ifce/_get_alarms
       {
         String $result[] = null;
         $result = this.alarms ();
         out = $rh.createReply();
         DGT.StringListHelper.write (out, $result);
         break;
       }


  /**
      * Get/set as a whole the current Configuration of the Device.
      */
       case 84:  // DCF/Device_ifce/_get_configuration
       {
         CONFIG.Configuration $result = null;
         $result = this.configuration ();
         out = $rh.createReply();
         CONFIG.ConfigurationHelper.write (out, $result);
         break;
       }


  /**
      * Get/set as a whole the current Configuration of the Device.
      */
       case 85:  // DCF/Device_ifce/_set_configuration
       {
         CONFIG.Configuration newConfiguration = CONFIG.ConfigurationHelper.read (in);
         this.configuration (newConfiguration);
         out = $rh.createReply();
         break;
       }

       case 86:  // DCF/Device_ifce/magnitudeMonitor
       {
         try {
           String magnitude_name = in.read_string ();
           MONITOR.MagnitudeMonitor_ifce $result = null;
           $result = this.magnitudeMonitor (magnitude_name);
           out = $rh.createReply();
           MONITOR.MagnitudeMonitor_ifceHelper.write (out, $result);
         } catch (DGT.GCSException $ex) {
           out = $rh.createExceptionReply ();
           DGT.GCSExceptionHelper.write (out, $ex);
         }
         break;
       }


  /* deprecated */
       case 87:  // DCF/Device_ifce/startDebug
       {
         this.startDebug ();
         out = $rh.createReply();
         break;
       }

       case 88:  // DCF/Device_ifce/stopDebug
       {
         this.stopDebug ();
         out = $rh.createReply();
         break;
       }

       default:
         throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);
    }

    return out;
  } // _invoke

  // Type-specific CORBA::Object operations
  private static String[] __ids = {
    "IDL:gtc/EMIRDAS/EMIRDas_ifce:1.0", 
    "IDL:gtc/DCF/Device_ifce:1.0"};

  public String[] _all_interfaces (org.omg.PortableServer.POA poa, byte[] objectId)
  {
    return (String[])__ids.clone ();
  }

  public EMIRDas_ifce _this() 
  {
    return EMIRDas_ifceHelper.narrow(
    super._this_object());
  }

  public EMIRDas_ifce _this(org.omg.CORBA.ORB orb) 
  {
    return EMIRDas_ifceHelper.narrow(
    super._this_object(orb));
  }


} // class EMIRDas_ifcePOA
