// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// TAO_IDL - Generated from
// be/be_codegen.cpp:323


#include "EMIRTYPESC.h"
#include "tao/Typecode.h"
#include "tao/Any_Dual_Impl_T.h"
#include "tao/Any_Basic_Impl_T.h"

#if defined (__BORLANDC__)
#pragma option -w-rvl -w-rch -w-ccc -w-aus -w-sig
#endif /* __BORLANDC__ */

#if !defined (__ACE_INLINE__)
#include "EMIRTYPESC.i"
#endif /* !defined INLINE */

// TAO_IDL - Generated from
// be/be_visitor_arg_traits.cpp:64

// Arg traits specializations.
namespace TAO
{
}


// TAO_IDL - Generated from
// be/be_visitor_typecode/typecode_defn.cpp:284

static const CORBA::Long _oc_EMIRTYPES_ReadoutMode[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  34,
  ACE_NTOHL (0x49444c3a), 
  ACE_NTOHL (0x6774632f), 
  ACE_NTOHL (0x454d4952), 
  ACE_NTOHL (0x54595045), 
  ACE_NTOHL (0x532f5265), 
  ACE_NTOHL (0x61646f75), 
  ACE_NTOHL (0x744d6f64), 
  ACE_NTOHL (0x653a312e), 
  ACE_NTOHL (0x30000000),  // repository ID = IDL:gtc/EMIRTYPES/ReadoutMode:1.0
  12,
  ACE_NTOHL (0x52656164), 
  ACE_NTOHL (0x6f75744d), 
  ACE_NTOHL (0x6f646500),  // name = ReadoutMode
  8, // member count
  7,
  ACE_NTOHL (0x53494e47), 
  ACE_NTOHL (0x4c450000),  // name = SINGLE
  4,
  ACE_NTOHL (0x43445300),  // name = CDS
  7,
  ACE_NTOHL (0x464f574c), 
  ACE_NTOHL (0x45520000),  // name = FOWLER
  5,
  ACE_NTOHL (0x52414d50), 
  ACE_NTOHL (0x0),  // name = RAMP
  11,
  ACE_NTOHL (0x4844525f), 
  ACE_NTOHL (0x53494d50), 
  ACE_NTOHL (0x4c450000),  // name = HDR_SIMPLE
  5,
  ACE_NTOHL (0x42494153), 
  ACE_NTOHL (0x0),  // name = BIAS
  6,
  ACE_NTOHL (0x4d52414d), 
  ACE_NTOHL (0x50000000),  // name = MRAMP
  10,
  ACE_NTOHL (0x554e4445), 
  ACE_NTOHL (0x46494e45), 
  ACE_NTOHL (0x44000000),  // name = UNDEFINED
  };

static CORBA::TypeCode _tc_TAO_tc_EMIRTYPES_ReadoutMode (
    CORBA::tk_enum,
    sizeof (_oc_EMIRTYPES_ReadoutMode),
    (char *) &_oc_EMIRTYPES_ReadoutMode,
    0,
    sizeof (EMIRTYPES::ReadoutMode)
  );

namespace EMIRTYPES
{
  ::CORBA::TypeCode_ptr _tc_ReadoutMode =
    &_tc_TAO_tc_EMIRTYPES_ReadoutMode;
}

// TAO_IDL - Generated from
// be/be_visitor_typecode/typecode_defn.cpp:284

static const CORBA::Long _oc_EMIRTYPES_SDSUControllerStatus[] =
{
  TAO_ENCAP_BYTE_ORDER, // byte order
  43,
  ACE_NTOHL (0x49444c3a), 
  ACE_NTOHL (0x6774632f), 
  ACE_NTOHL (0x454d4952), 
  ACE_NTOHL (0x54595045), 
  ACE_NTOHL (0x532f5344), 
  ACE_NTOHL (0x5355436f), 
  ACE_NTOHL (0x6e74726f), 
  ACE_NTOHL (0x6c6c6572), 
  ACE_NTOHL (0x53746174), 
  ACE_NTOHL (0x75733a31), 
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:gtc/EMIRTYPES/SDSUControllerStatus:1.0
  21,
  ACE_NTOHL (0x53445355), 
  ACE_NTOHL (0x436f6e74), 
  ACE_NTOHL (0x726f6c6c), 
  ACE_NTOHL (0x65725374), 
  ACE_NTOHL (0x61747573), 
  ACE_NTOHL (0x0),  // name = SDSUControllerStatus
  4, // member count
  14,
  ACE_NTOHL (0x44455445), 
  ACE_NTOHL (0x43544f52), 
  ACE_NTOHL (0x5f49444c), 
  ACE_NTOHL (0x45000000),  // name = DETECTOR_IDLE
  15,
  ACE_NTOHL (0x44455445), 
  ACE_NTOHL (0x43544f52), 
  ACE_NTOHL (0x5f455252), 
  ACE_NTOHL (0x4f520000),  // name = DETECTOR_ERROR
  17,
  ACE_NTOHL (0x44455445), 
  ACE_NTOHL (0x43544f52), 
  ACE_NTOHL (0x5f52554e), 
  ACE_NTOHL (0x4e494e47), 
  ACE_NTOHL (0x0),  // name = DETECTOR_RUNNING
  14,
  ACE_NTOHL (0x44455445), 
  ACE_NTOHL (0x43544f52), 
  ACE_NTOHL (0x5f454e44), 
  ACE_NTOHL (0x53000000),  // name = DETECTOR_ENDS
  };

static CORBA::TypeCode _tc_TAO_tc_EMIRTYPES_SDSUControllerStatus (
    CORBA::tk_enum,
    sizeof (_oc_EMIRTYPES_SDSUControllerStatus),
    (char *) &_oc_EMIRTYPES_SDSUControllerStatus,
    0,
    sizeof (EMIRTYPES::SDSUControllerStatus)
  );

namespace EMIRTYPES
{
  ::CORBA::TypeCode_ptr _tc_SDSUControllerStatus =
    &_tc_TAO_tc_EMIRTYPES_SDSUControllerStatus;
}

// TAO_IDL - Generated from
// be/be_visitor_typecode/typecode_defn.cpp:284

static const CORBA::Long _oc_EMIRTYPES_NameValue[] =
{
    TAO_ENCAP_BYTE_ORDER, // byte order
  32,
  ACE_NTOHL (0x49444c3a), 
  ACE_NTOHL (0x6774632f), 
  ACE_NTOHL (0x454d4952), 
  ACE_NTOHL (0x54595045), 
  ACE_NTOHL (0x532f4e61), 
  ACE_NTOHL (0x6d655661), 
  ACE_NTOHL (0x6c75653a), 
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:gtc/EMIRTYPES/NameValue:1.0
    10,
  ACE_NTOHL (0x4e616d65), 
  ACE_NTOHL (0x56616c75), 
  ACE_NTOHL (0x65000000),  // name = NameValue
  2, // member count
    5,
  ACE_NTOHL (0x6e616d65), 
  ACE_NTOHL (0x0),  // name = name
    CORBA::tk_string, 
  0U, // string length
  6,
  ACE_NTOHL (0x76616c75), 
  ACE_NTOHL (0x65000000),  // name = value
    CORBA::tk_any,

};

static CORBA::TypeCode _tc_TAO_tc_EMIRTYPES_NameValue (
    CORBA::tk_struct,
    sizeof (_oc_EMIRTYPES_NameValue),
    (char *) &_oc_EMIRTYPES_NameValue,
    0,
    sizeof (EMIRTYPES::NameValue)
  );

namespace EMIRTYPES
{
  ::CORBA::TypeCode_ptr _tc_NameValue =
    &_tc_TAO_tc_EMIRTYPES_NameValue;
}

// TAO_IDL - Generated from 
// be/be_visitor_structure/structure_cs.cpp:66

void 
EMIRTYPES::NameValue::_tao_any_destructor (
    void *_tao_void_pointer
  )
{
  NameValue *_tao_tmp_pointer =
    ACE_static_cast (NameValue *, _tao_void_pointer);
  delete _tao_tmp_pointer;
}

// TAO_IDL - Generated from 
// be/be_visitor_sequence/sequence_cs.cpp:65

#if !defined (_EMIRTYPES_PARAMLIST_CS_)
#define _EMIRTYPES_PARAMLIST_CS_

EMIRTYPES::ParamList::ParamList (void)
{}

EMIRTYPES::ParamList::ParamList (
    CORBA::ULong max
  )
  : TAO_Unbounded_Sequence<
        EMIRTYPES::NameValue
      >
    (max)
{}

EMIRTYPES::ParamList::ParamList (
    CORBA::ULong max,
    CORBA::ULong length,
    EMIRTYPES::NameValue * buffer,
    CORBA::Boolean release
  )
  : TAO_Unbounded_Sequence<
        EMIRTYPES::NameValue
      >
    (max, length, buffer, release)
{}

EMIRTYPES::ParamList::ParamList (
    const ParamList &seq
  )
  : TAO_Unbounded_Sequence<
        EMIRTYPES::NameValue
      >
    (seq)
{}

EMIRTYPES::ParamList::~ParamList (void)
{}

void EMIRTYPES::ParamList::_tao_any_destructor (
    void * _tao_void_pointer
  )
{
  ParamList * _tao_tmp_pointer =
    ACE_static_cast (ParamList *, _tao_void_pointer);
  delete _tao_tmp_pointer;
}

#endif /* end #if !defined */

// TAO_IDL - Generated from
// be/be_visitor_typecode/typecode_defn.cpp:284

static const CORBA::Long _oc_EMIRTYPES_ParamList[] =
{
    TAO_ENCAP_BYTE_ORDER, // byte order
  32,
  ACE_NTOHL (0x49444c3a), 
  ACE_NTOHL (0x6774632f), 
  ACE_NTOHL (0x454d4952), 
  ACE_NTOHL (0x54595045), 
  ACE_NTOHL (0x532f5061), 
  ACE_NTOHL (0x72616d4c), 
  ACE_NTOHL (0x6973743a), 
  ACE_NTOHL (0x312e3000),  // repository ID = IDL:gtc/EMIRTYPES/ParamList:1.0
    10,
  ACE_NTOHL (0x50617261), 
  ACE_NTOHL (0x6d4c6973), 
  ACE_NTOHL (0x74000000),  // name = ParamList
    CORBA::tk_sequence, // typecode kind
  112, // encapsulation length
    TAO_ENCAP_BYTE_ORDER, // byte order
    CORBA::tk_struct, // typecode kind
    96, // encapsulation length
      TAO_ENCAP_BYTE_ORDER, // byte order
      32,
      ACE_NTOHL (0x49444c3a), 
      ACE_NTOHL (0x6774632f), 
      ACE_NTOHL (0x454d4952), 
      ACE_NTOHL (0x54595045), 
      ACE_NTOHL (0x532f4e61), 
      ACE_NTOHL (0x6d655661), 
      ACE_NTOHL (0x6c75653a), 
      ACE_NTOHL (0x312e3000),  // repository ID = IDL:gtc/EMIRTYPES/NameValue:1.0
            10,
      ACE_NTOHL (0x4e616d65), 
      ACE_NTOHL (0x56616c75), 
      ACE_NTOHL (0x65000000),  // name = NameValue
      2, // member count
            5,
      ACE_NTOHL (0x6e616d65), 
      ACE_NTOHL (0x0),  // name = name
            CORBA::tk_string, 
      0U, // string length
      6,
      ACE_NTOHL (0x76616c75), 
      ACE_NTOHL (0x65000000),  // name = value
            CORBA::tk_any,


    0U,

};

static CORBA::TypeCode _tc_TAO_tc_EMIRTYPES_ParamList (
    CORBA::tk_alias,
    sizeof (_oc_EMIRTYPES_ParamList),
    (char *) &_oc_EMIRTYPES_ParamList,
    0,
    sizeof (EMIRTYPES::ParamList)
  );

namespace EMIRTYPES
{
  ::CORBA::TypeCode_ptr _tc_ParamList =
    &_tc_TAO_tc_EMIRTYPES_ParamList;
}

// TAO_IDL - Generated from
// be/be_visitor_typecode/typecode_defn.cpp:284

static const CORBA::Long _oc_EMIRTYPES_AcquisitionConfiguration[] =
{
    TAO_ENCAP_BYTE_ORDER, // byte order
  47,
  ACE_NTOHL (0x49444c3a), 
  ACE_NTOHL (0x6774632f), 
  ACE_NTOHL (0x454d4952), 
  ACE_NTOHL (0x54595045), 
  ACE_NTOHL (0x532f4163), 
  ACE_NTOHL (0x71756973), 
  ACE_NTOHL (0x6974696f), 
  ACE_NTOHL (0x6e436f6e), 
  ACE_NTOHL (0x66696775), 
  ACE_NTOHL (0x72617469), 
  ACE_NTOHL (0x6f6e3a31), 
  ACE_NTOHL (0x2e300000),  // repository ID = IDL:gtc/EMIRTYPES/AcquisitionConfiguration:1.0
    25,
  ACE_NTOHL (0x41637175), 
  ACE_NTOHL (0x69736974), 
  ACE_NTOHL (0x696f6e43), 
  ACE_NTOHL (0x6f6e6669), 
  ACE_NTOHL (0x67757261), 
  ACE_NTOHL (0x74696f6e), 
  ACE_NTOHL (0x0),  // name = AcquisitionConfiguration
  18, // member count
    6,
  ACE_NTOHL (0x7469746c), 
  ACE_NTOHL (0x65000000),  // name = title
    CORBA::tk_string, 
  0U, // string length
  15,
  ACE_NTOHL (0x72656164), 
  ACE_NTOHL (0x6f75746d), 
  ACE_NTOHL (0x6f646576), 
  ACE_NTOHL (0x616c0000),  // name = readoutmodeval
    CORBA::tk_enum, // typecode kind
  164, // encapsulation length
TAO_ENCAP_BYTE_ORDER, // byte order
    34,
    ACE_NTOHL (0x49444c3a), 
    ACE_NTOHL (0x6774632f), 
    ACE_NTOHL (0x454d4952), 
    ACE_NTOHL (0x54595045), 
    ACE_NTOHL (0x532f5265), 
    ACE_NTOHL (0x61646f75), 
    ACE_NTOHL (0x744d6f64), 
    ACE_NTOHL (0x653a312e), 
    ACE_NTOHL (0x30000000),  // repository ID = IDL:gtc/EMIRTYPES/ReadoutMode:1.0
    12,
    ACE_NTOHL (0x52656164), 
    ACE_NTOHL (0x6f75744d), 
    ACE_NTOHL (0x6f646500),  // name = ReadoutMode
    8, // member count
    7,
    ACE_NTOHL (0x53494e47), 
    ACE_NTOHL (0x4c450000),  // name = SINGLE
    4,
    ACE_NTOHL (0x43445300),  // name = CDS
    7,
    ACE_NTOHL (0x464f574c), 
    ACE_NTOHL (0x45520000),  // name = FOWLER
    5,
    ACE_NTOHL (0x52414d50), 
    ACE_NTOHL (0x0),  // name = RAMP
    11,
    ACE_NTOHL (0x4844525f), 
    ACE_NTOHL (0x53494d50), 
    ACE_NTOHL (0x4c450000),  // name = HDR_SIMPLE
    5,
    ACE_NTOHL (0x42494153), 
    ACE_NTOHL (0x0),  // name = BIAS
    6,
    ACE_NTOHL (0x4d52414d), 
    ACE_NTOHL (0x50000000),  // name = MRAMP
    10,
    ACE_NTOHL (0x554e4445), 
    ACE_NTOHL (0x46494e45), 
    ACE_NTOHL (0x44000000),  // name = UNDEFINED
    
  13,
  ACE_NTOHL (0x6578706f), 
  ACE_NTOHL (0x73757265), 
  ACE_NTOHL (0x74696d65), 
  ACE_NTOHL (0x0),  // name = exposuretime
    CORBA::tk_long,

  7,
  ACE_NTOHL (0x785f7369), 
  ACE_NTOHL (0x7a650000),  // name = x_size
    CORBA::tk_long,

  7,
  ACE_NTOHL (0x795f7369), 
  ACE_NTOHL (0x7a650000),  // name = y_size
    CORBA::tk_long,

  11,
  ACE_NTOHL (0x77696e64), 
  ACE_NTOHL (0x6f774d6f), 
  ACE_NTOHL (0x64650000),  // name = windowMode
    CORBA::tk_boolean,

  19,
  ACE_NTOHL (0x6170706c), 
  ACE_NTOHL (0x79436f6e), 
  ACE_NTOHL (0x66696775), 
  ACE_NTOHL (0x72617469), 
  ACE_NTOHL (0x6f6e0000),  // name = applyConfiguration
    CORBA::tk_boolean,

  19,
  ACE_NTOHL (0x6170706c), 
  ACE_NTOHL (0x7953696d), 
  ACE_NTOHL (0x756c6174), 
  ACE_NTOHL (0x65496d61), 
  ACE_NTOHL (0x67650000),  // name = applySimulateImage
    CORBA::tk_boolean,

  11,
  ACE_NTOHL (0x73617665), 
  ACE_NTOHL (0x496d6167), 
  ACE_NTOHL (0x65730000),  // name = saveImages
    CORBA::tk_boolean,

  5,
  ACE_NTOHL (0x6e736563), 
  ACE_NTOHL (0x0),  // name = nsec
    CORBA::tk_long,

  7,
  ACE_NTOHL (0x6f707472), 
  ACE_NTOHL (0x73740000),  // name = optrst
    CORBA::tk_long,

  5,
  ACE_NTOHL (0x6e727374), 
  ACE_NTOHL (0x0),  // name = nrst
    CORBA::tk_long,

  6,
  ACE_NTOHL (0x6e726462), 
  ACE_NTOHL (0x6c000000),  // name = nrdbl
    CORBA::tk_long,

  9,
  ACE_NTOHL (0x6e726462), 
  ACE_NTOHL (0x6c5f6e64), 
  ACE_NTOHL (0x0),  // name = nrdbl_nd
    CORBA::tk_long,

  7,
  ACE_NTOHL (0x6e756d45), 
  ACE_NTOHL (0x78700000),  // name = numExp
    CORBA::tk_long,

  6,
  ACE_NTOHL (0x6e726469), 
  ACE_NTOHL (0x6c000000),  // name = nrdil
    CORBA::tk_long,

  9,
  ACE_NTOHL (0x6e726469), 
  ACE_NTOHL (0x6c5f6e64), 
  ACE_NTOHL (0x0),  // name = nrdil_nd
    CORBA::tk_long,

  9,
  ACE_NTOHL (0x6e706172), 
  ACE_NTOHL (0x616d5f78), 
  ACE_NTOHL (0x0),  // name = nparam_x
    CORBA::tk_long,

};

static CORBA::TypeCode _tc_TAO_tc_EMIRTYPES_AcquisitionConfiguration (
    CORBA::tk_struct,
    sizeof (_oc_EMIRTYPES_AcquisitionConfiguration),
    (char *) &_oc_EMIRTYPES_AcquisitionConfiguration,
    0,
    sizeof (EMIRTYPES::AcquisitionConfiguration)
  );

namespace EMIRTYPES
{
  ::CORBA::TypeCode_ptr _tc_AcquisitionConfiguration =
    &_tc_TAO_tc_EMIRTYPES_AcquisitionConfiguration;
}

// TAO_IDL - Generated from 
// be/be_visitor_structure/structure_cs.cpp:66

void 
EMIRTYPES::AcquisitionConfiguration::_tao_any_destructor (
    void *_tao_void_pointer
  )
{
  AcquisitionConfiguration *_tao_tmp_pointer =
    ACE_static_cast (AcquisitionConfiguration *, _tao_void_pointer);
  delete _tao_tmp_pointer;
}

// TAO_IDL - Generated from
// be/be_visitor_enum/any_op_cs.cpp:52

void operator<<= (
    CORBA::Any &_tao_any,
    EMIRTYPES::ReadoutMode _tao_elem
  )
{
  TAO::Any_Basic_Impl_T<EMIRTYPES::ReadoutMode>::insert (
      _tao_any,
      EMIRTYPES::_tc_ReadoutMode,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (
    const CORBA::Any &_tao_any,
    EMIRTYPES::ReadoutMode &_tao_elem
  )
{
  return
    TAO::Any_Basic_Impl_T<EMIRTYPES::ReadoutMode>::extract (
        _tao_any,
        EMIRTYPES::_tc_ReadoutMode,
        _tao_elem 
      );
}

// TAO_IDL - Generated from
// be/be_visitor_enum/any_op_cs.cpp:52

void operator<<= (
    CORBA::Any &_tao_any,
    EMIRTYPES::SDSUControllerStatus _tao_elem
  )
{
  TAO::Any_Basic_Impl_T<EMIRTYPES::SDSUControllerStatus>::insert (
      _tao_any,
      EMIRTYPES::_tc_SDSUControllerStatus,
      _tao_elem
    );
}

CORBA::Boolean operator>>= (
    const CORBA::Any &_tao_any,
    EMIRTYPES::SDSUControllerStatus &_tao_elem
  )
{
  return
    TAO::Any_Basic_Impl_T<EMIRTYPES::SDSUControllerStatus>::extract (
        _tao_any,
        EMIRTYPES::_tc_SDSUControllerStatus,
        _tao_elem 
      );
}

// TAO_IDL - Generated from 
// be/be_visitor_structure/any_op_cs.cpp:54

// Copying insertion.
void operator<<= (
    CORBA::Any &_tao_any,
    const EMIRTYPES::NameValue &_tao_elem
  )
{
  TAO::Any_Dual_Impl_T<EMIRTYPES::NameValue>::insert_copy (
      _tao_any,
      EMIRTYPES::NameValue::_tao_any_destructor,
      EMIRTYPES::_tc_NameValue,
      _tao_elem
    );
}

// Non-copying insertion.
void operator<<= (
    CORBA::Any &_tao_any,
    EMIRTYPES::NameValue *_tao_elem
  )
{
  TAO::Any_Dual_Impl_T<EMIRTYPES::NameValue>::insert (
      _tao_any,
      EMIRTYPES::NameValue::_tao_any_destructor,
      EMIRTYPES::_tc_NameValue,
      _tao_elem
    );
}

// Extraction to non-const pointer (deprecated).
CORBA::Boolean operator>>= (
    const CORBA::Any &_tao_any,
    EMIRTYPES::NameValue *&_tao_elem
  )
{
  return _tao_any >>= ACE_const_cast (
      const EMIRTYPES::NameValue *&,
      _tao_elem
    );
}

// Extraction to const pointer.
CORBA::Boolean operator>>= (
    const CORBA::Any &_tao_any,
    const EMIRTYPES::NameValue *&_tao_elem
  )
{
  return
    TAO::Any_Dual_Impl_T<EMIRTYPES::NameValue>::extract (
        _tao_any,
        EMIRTYPES::NameValue::_tao_any_destructor,
        EMIRTYPES::_tc_NameValue,
        _tao_elem
      );
}

// TAO_IDL - Generated from 
// be/be_visitor_sequence/any_op_cs.cpp:54

// Copying insertion.
void operator<<= (
    CORBA::Any &_tao_any,
    const EMIRTYPES::ParamList &_tao_elem
  )
{
  TAO::Any_Dual_Impl_T<EMIRTYPES::ParamList>::insert_copy (
      _tao_any,
      EMIRTYPES::ParamList::_tao_any_destructor,
      EMIRTYPES::_tc_ParamList,
      _tao_elem
    );
}

// Non-copying insertion.
void operator<<= (
    CORBA::Any &_tao_any,
    EMIRTYPES::ParamList *_tao_elem
  )
{
  TAO::Any_Dual_Impl_T<EMIRTYPES::ParamList>::insert (
      _tao_any,
      EMIRTYPES::ParamList::_tao_any_destructor,
      EMIRTYPES::_tc_ParamList,
      _tao_elem
    );
}

// Extraction to non-const pointer (deprecated).
CORBA::Boolean operator>>= (
    const CORBA::Any &_tao_any,
    EMIRTYPES::ParamList *&_tao_elem
  )
{
  return _tao_any >>= ACE_const_cast (
      const EMIRTYPES::ParamList *&,
      _tao_elem
    );
}

// Extraction to const pointer.
CORBA::Boolean operator>>= (
    const CORBA::Any &_tao_any,
    const EMIRTYPES::ParamList *&_tao_elem
  )
{
  return
    TAO::Any_Dual_Impl_T<EMIRTYPES::ParamList>::extract (
        _tao_any,
        EMIRTYPES::ParamList::_tao_any_destructor,
        EMIRTYPES::_tc_ParamList,
        _tao_elem
      );
}

// TAO_IDL - Generated from 
// be/be_visitor_structure/any_op_cs.cpp:54

// Copying insertion.
void operator<<= (
    CORBA::Any &_tao_any,
    const EMIRTYPES::AcquisitionConfiguration &_tao_elem
  )
{
  TAO::Any_Dual_Impl_T<EMIRTYPES::AcquisitionConfiguration>::insert_copy (
      _tao_any,
      EMIRTYPES::AcquisitionConfiguration::_tao_any_destructor,
      EMIRTYPES::_tc_AcquisitionConfiguration,
      _tao_elem
    );
}

// Non-copying insertion.
void operator<<= (
    CORBA::Any &_tao_any,
    EMIRTYPES::AcquisitionConfiguration *_tao_elem
  )
{
  TAO::Any_Dual_Impl_T<EMIRTYPES::AcquisitionConfiguration>::insert (
      _tao_any,
      EMIRTYPES::AcquisitionConfiguration::_tao_any_destructor,
      EMIRTYPES::_tc_AcquisitionConfiguration,
      _tao_elem
    );
}

// Extraction to non-const pointer (deprecated).
CORBA::Boolean operator>>= (
    const CORBA::Any &_tao_any,
    EMIRTYPES::AcquisitionConfiguration *&_tao_elem
  )
{
  return _tao_any >>= ACE_const_cast (
      const EMIRTYPES::AcquisitionConfiguration *&,
      _tao_elem
    );
}

// Extraction to const pointer.
CORBA::Boolean operator>>= (
    const CORBA::Any &_tao_any,
    const EMIRTYPES::AcquisitionConfiguration *&_tao_elem
  )
{
  return
    TAO::Any_Dual_Impl_T<EMIRTYPES::AcquisitionConfiguration>::extract (
        _tao_any,
        EMIRTYPES::AcquisitionConfiguration::_tao_any_destructor,
        EMIRTYPES::_tc_AcquisitionConfiguration,
        _tao_elem
      );
}

// TAO_IDL - Generated from
// be/be_visitor_sequence/cdr_op_cs.cpp:95

#if !defined _TAO_CDR_OP_EMIRTYPES_ParamList_CPP_
#define _TAO_CDR_OP_EMIRTYPES_ParamList_CPP_

CORBA::Boolean operator<< (
    TAO_OutputCDR &strm,
    const EMIRTYPES::ParamList &_tao_sequence
  )
{
  CORBA::ULong _tao_seq_len = _tao_sequence.length ();
  
  if (strm << _tao_seq_len)
    {
      // Encode all elements.
      CORBA::Boolean _tao_marshal_flag = 1;
      
      for (CORBA::ULong i = 0; i < _tao_seq_len && _tao_marshal_flag; ++i)
        {
          _tao_marshal_flag = (strm << _tao_sequence[i]);
        }
      
      return _tao_marshal_flag;
    }
  
  return 0;
}

CORBA::Boolean operator>> (
    TAO_InputCDR &strm,
    EMIRTYPES::ParamList &_tao_sequence
  )
{
  CORBA::ULong _tao_seq_len;
  
  if (strm >> _tao_seq_len)
    {
      // Add a check to the length of the sequence
      // to make sure it does not exceed the length
      // of the stream. (See bug 58.)
      if (_tao_seq_len > strm.length ())
        {
          return 0;
        }
      
      // Set the length of the sequence.
      _tao_sequence.length (_tao_seq_len);
      
      // If length is 0 we return true.
      if (0 >= _tao_seq_len) 
        {
          return 1;
        }
      
      // Retrieve all the elements.
      CORBA::Boolean _tao_marshal_flag = 1;
      
      for (CORBA::ULong i = 0; i < _tao_seq_len && _tao_marshal_flag; ++i)
        {
          _tao_marshal_flag = (strm >> _tao_sequence[i]);
        }
      
      return _tao_marshal_flag;
    
    }
  
  return 0;
}

#endif /* _TAO_CDR_OP_EMIRTYPES_ParamList_CPP_ */

