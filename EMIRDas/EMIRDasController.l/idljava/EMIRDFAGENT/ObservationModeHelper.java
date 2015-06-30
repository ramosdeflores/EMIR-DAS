package EMIRDFAGENT;


/**
* EMIRDFAGENT/ObservationModeHelper.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from EMIRDFAGENT.idl
* lunes 18 de mayo de 2015 13H52' WEST
*/


//  };
abstract public class ObservationModeHelper
{
  private static String  _id = "IDL:gtc/EMIRDFAGENT/ObservationMode:1.0";

  public static void insert (org.omg.CORBA.Any a, EMIRDFAGENT.ObservationMode that)
  {
    org.omg.CORBA.portable.OutputStream out = a.create_output_stream ();
    a.type (type ());
    write (out, that);
    a.read_value (out.create_input_stream (), type ());
  }

  public static EMIRDFAGENT.ObservationMode extract (org.omg.CORBA.Any a)
  {
    return read (a.create_input_stream ());
  }

  private static org.omg.CORBA.TypeCode __typeCode = null;
  synchronized public static org.omg.CORBA.TypeCode type ()
  {
    if (__typeCode == null)
    {
      __typeCode = org.omg.CORBA.ORB.init ().create_enum_tc (EMIRDFAGENT.ObservationModeHelper.id (), "ObservationMode", new String[] { "IMAGE_BIAS", "IMAGE_DARK", "IMAGE_FLAT", "IMAGE_SKY", "DEAD_PX_MASK", "IMAGE_FLAT_D", "MSM_SPECTRAL_FLAT_FIELD", "SPEC_SKY", "SPEC_DARK", "SPEC_FLAT", "SLIT_TRANSM_CALIBR", "WAVELENGTH_CALIBR", "TS_ROUGH_FOCUS", "TS_FINE_FOCUS", "EMIR_FOCUS_CTL", "IMAGE_ACQUISTION", "MOS_LONG_SLIT", "TARGET_ACQUISITION", "MASK_IMAGING", "MSM_LSM_CHECK", "STARE_IMAGE", "NODDED_BEAM_SWITCHED_IMAGE", "DITHERED_IMAGE", "MICRODITHERED_IMAGE", "MOSAICED_IMAGE", "STARE_SPECTRA", "DITHERED_NODDED_SPECTRA_ALONG_SLIT", "OFFSET_SPECTRA_BEYOND_SLIT", "RASTER_SPECTRA", "DTU_X_Y_CALIBRATION", "DTU_Z_CALIBRATION", "DTU_FLEXURE_COMP", "CSU2DETECTOR_CALIBRATION", "LATERAL_COLOUR", "SPECTRAL_CHARACTERIZATION", "CENTRE_ROTATION", "ASTROMETRIC_CALIBRATION", "PHOTOMETRIC_CALIBRATION", "SPECTROPHOTOMETRIC_CALIBRATION", "TEST0", "TEST1", "TEST2", "TEST3", "TEST4", "TEST5", "TEST6", "TEST7"} );
    }
    return __typeCode;
  }

  public static String id ()
  {
    return _id;
  }

  public static EMIRDFAGENT.ObservationMode read (org.omg.CORBA.portable.InputStream istream)
  {
    return EMIRDFAGENT.ObservationMode.from_int (istream.read_long ());
  }

  public static void write (org.omg.CORBA.portable.OutputStream ostream, EMIRDFAGENT.ObservationMode value)
  {
    ostream.write_long (value.value ());
  }

}