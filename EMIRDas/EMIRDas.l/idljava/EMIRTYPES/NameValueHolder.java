package EMIRTYPES;

/**
* EMIRTYPES/NameValueHolder.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from EMIRTYPES.idl
* lunes 18 de mayo de 2015 13H54' WEST
*/

public final class NameValueHolder implements org.omg.CORBA.portable.Streamable
{
  public EMIRTYPES.NameValue value = null;

  public NameValueHolder ()
  {
  }

  public NameValueHolder (EMIRTYPES.NameValue initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = EMIRTYPES.NameValueHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    EMIRTYPES.NameValueHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return EMIRTYPES.NameValueHelper.type ();
  }

}