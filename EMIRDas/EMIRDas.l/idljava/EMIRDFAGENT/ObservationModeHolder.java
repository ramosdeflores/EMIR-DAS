package EMIRDFAGENT;

/**
* EMIRDFAGENT/ObservationModeHolder.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from EMIRDFAGENT.idl
* lunes 18 de mayo de 2015 13H54' WEST
*/


//  };
public final class ObservationModeHolder implements org.omg.CORBA.portable.Streamable
{
  public EMIRDFAGENT.ObservationMode value = null;

  public ObservationModeHolder ()
  {
  }

  public ObservationModeHolder (EMIRDFAGENT.ObservationMode initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = EMIRDFAGENT.ObservationModeHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    EMIRDFAGENT.ObservationModeHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return EMIRDFAGENT.ObservationModeHelper.type ();
  }

}
