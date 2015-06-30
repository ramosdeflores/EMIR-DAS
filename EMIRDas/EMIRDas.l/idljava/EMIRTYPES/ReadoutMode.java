package EMIRTYPES;


/**
* EMIRTYPES/ReadoutMode.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from EMIRTYPES.idl
* lunes 18 de mayo de 2015 13H54' WEST
*/

public class ReadoutMode implements org.omg.CORBA.portable.IDLEntity
{
  private        int __value;
  private static int __size = 8;
  private static EMIRTYPES.ReadoutMode[] __array = new EMIRTYPES.ReadoutMode [__size];

  public static final int _SINGLE = 0;
  public static final EMIRTYPES.ReadoutMode SINGLE = new EMIRTYPES.ReadoutMode(_SINGLE);
  public static final int _CDS = 1;
  public static final EMIRTYPES.ReadoutMode CDS = new EMIRTYPES.ReadoutMode(_CDS);
  public static final int _FOWLER = 2;
  public static final EMIRTYPES.ReadoutMode FOWLER = new EMIRTYPES.ReadoutMode(_FOWLER);
  public static final int _RAMP = 3;
  public static final EMIRTYPES.ReadoutMode RAMP = new EMIRTYPES.ReadoutMode(_RAMP);
  public static final int _HDR_SIMPLE = 4;
  public static final EMIRTYPES.ReadoutMode HDR_SIMPLE = new EMIRTYPES.ReadoutMode(_HDR_SIMPLE);
  public static final int _BIAS = 5;
  public static final EMIRTYPES.ReadoutMode BIAS = new EMIRTYPES.ReadoutMode(_BIAS);
  public static final int _MRAMP = 6;
  public static final EMIRTYPES.ReadoutMode MRAMP = new EMIRTYPES.ReadoutMode(_MRAMP);
  public static final int _UNDEFINED = 7;
  public static final EMIRTYPES.ReadoutMode UNDEFINED = new EMIRTYPES.ReadoutMode(_UNDEFINED);

  public int value ()
  {
    return __value;
  }

  public static EMIRTYPES.ReadoutMode from_int (int value)
  {
    if (value >= 0 && value < __size)
      return __array[value];
    else
      throw new org.omg.CORBA.BAD_PARAM ();
  }

  protected ReadoutMode (int value)
  {
    __value = value;
    __array[__value] = this;
  }
} // class ReadoutMode
