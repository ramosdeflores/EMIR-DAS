package EMIRTYPES;


/**
* EMIRTYPES/ParamListHelper.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from EMIRTYPES.idl
* lunes 18 de mayo de 2015 13H54' WEST
*/

abstract public class ParamListHelper
{
  private static String  _id = "IDL:gtc/EMIRTYPES/ParamList:1.0";

  public static void insert (org.omg.CORBA.Any a, EMIRTYPES.NameValue[] that)
  {
    org.omg.CORBA.portable.OutputStream out = a.create_output_stream ();
    a.type (type ());
    write (out, that);
    a.read_value (out.create_input_stream (), type ());
  }

  public static EMIRTYPES.NameValue[] extract (org.omg.CORBA.Any a)
  {
    return read (a.create_input_stream ());
  }

  private static org.omg.CORBA.TypeCode __typeCode = null;
  synchronized public static org.omg.CORBA.TypeCode type ()
  {
    if (__typeCode == null)
    {
      __typeCode = EMIRTYPES.NameValueHelper.type ();
      __typeCode = org.omg.CORBA.ORB.init ().create_sequence_tc (0, __typeCode);
      __typeCode = org.omg.CORBA.ORB.init ().create_alias_tc (EMIRTYPES.ParamListHelper.id (), "ParamList", __typeCode);
    }
    return __typeCode;
  }

  public static String id ()
  {
    return _id;
  }

  public static EMIRTYPES.NameValue[] read (org.omg.CORBA.portable.InputStream istream)
  {
    EMIRTYPES.NameValue value[] = null;
    int _len0 = istream.read_long ();
    value = new EMIRTYPES.NameValue[_len0];
    for (int _o1 = 0;_o1 < value.length; ++_o1)
      value[_o1] = EMIRTYPES.NameValueHelper.read (istream);
    return value;
  }

  public static void write (org.omg.CORBA.portable.OutputStream ostream, EMIRTYPES.NameValue[] value)
  {
    ostream.write_long (value.length);
    for (int _i0 = 0;_i0 < value.length; ++_i0)
      EMIRTYPES.NameValueHelper.write (ostream, value[_i0]);
  }

}
