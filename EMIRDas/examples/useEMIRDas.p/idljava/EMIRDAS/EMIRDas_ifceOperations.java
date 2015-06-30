package EMIRDAS;


/**
* EMIRDAS/EMIRDas_ifceOperations.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from EMIRDAS.idl
* jueves 11 de julio de 2013 19H47' WEST
*/

public interface EMIRDas_ifceOperations  extends DCF.Device_ifceOperations
{

  // Specific EMIRDas operations
  void setupDetector (EMIRTYPES.AcquisitionConfiguration acquisitionConfiguration) throws DGT.GCSException;
  void expose () throws DGT.GCSException;
  void setupReadoutMode (EMIRTYPES.ReadoutMode readoutmodeval) throws DGT.GCSException;
  void setupImageTitle (String exposureId) throws DGT.GCSException;
  void setupExposureTime (int exposureTime) throws DGT.GCSException;
  void setupWindowMode (boolean windowmode) throws DGT.GCSException;
  void setupNSEC (int nsec) throws DGT.GCSException;
  void setupNO_EXPOSURES (int nexp) throws DGT.GCSException;
  void setupNRDBL (int nrdbl) throws DGT.GCSException;
  void setupNRDBL_ND (int nrdbl_nd) throws DGT.GCSException;
  void setupNRST (int nrst) throws DGT.GCSException;
  void setupNPARAM_X (int nparam_x) throws DGT.GCSException;
  void setupNRDIL (int nrdil) throws DGT.GCSException;
  void setupNRDIL_ND (int nrdil_nd) throws DGT.GCSException;
  void setupOPTRST (int optrst) throws DGT.GCSException;
  void setupX_SIZE (int x_size) throws DGT.GCSException;
  void setupY_SIZE (int y_size) throws DGT.GCSException;
  void setupY_SIZE_B (int y_size_b) throws DGT.GCSException;

  // Monitorable attributes (device magnitudes)
  boolean isSettingup ();
  boolean isConfigurationDone ();
  boolean isExposing ();
  boolean isDetectorControllerIdle ();
  EMIRTYPES.ReadoutMode currentReadoutMode ();
  EMIRTYPES.SDSUControllerStatus currentSDSUControllerStatus ();
  int currentElapsedTime ();
  int currentExposureTime ();
  int currentNumberOfImages ();
  int currentNSEC ();
  int currentNO_EXPOSURES ();
  int currentNRDBL ();
  int currentNRDBL_ND ();
  int currentNPARAM_X ();
  int currentNRST ();
  int currentNRDIL ();
  int currentNRDIL_ND ();
  int currentOPTRST ();
  int currentX_SIZE ();
  int currentY_SIZE ();
  int currentY_SIZE_B ();

  // Configurable attributes (device properties)
  String poweronApp ();

  // Configurable attributes (device properties)
  void poweronApp (String newPoweronApp);
  String poweroffApp ();
  void poweroffApp (String newPoweroffApp);
  String adqApp ();
  void adqApp (String newAdqApp);
  String imgDir ();
  void imgDir (String newImgDir);
} // interface EMIRDas_ifceOperations
