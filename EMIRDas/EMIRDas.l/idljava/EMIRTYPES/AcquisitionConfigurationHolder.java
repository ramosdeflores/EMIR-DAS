package EMIRTYPES;

/**
* EMIRTYPES/AcquisitionConfigurationHolder.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from EMIRTYPES.idl
* lunes 18 de mayo de 2015 13H54' WEST
*/

public final class AcquisitionConfigurationHolder implements org.omg.CORBA.portable.Streamable
{
  public EMIRTYPES.AcquisitionConfiguration value = null;

  public AcquisitionConfigurationHolder ()
  {
  }

  public AcquisitionConfigurationHolder (EMIRTYPES.AcquisitionConfiguration initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = EMIRTYPES.AcquisitionConfigurationHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    EMIRTYPES.AcquisitionConfigurationHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return EMIRTYPES.AcquisitionConfigurationHelper.type ();
  }

}