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
// be/be_codegen.cpp:171

#ifndef _TAO_IDL_IDLC___EMIRDFAGENTC_H_
#define _TAO_IDL_IDLC___EMIRDFAGENTC_H_

#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CDR.h"
#include "tao/Environment.h"
#include "tao/Object.h"
#include "tao/Objref_VarOut_T.h"

#include "DCFC.h"
#include "DFAGENTC.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO 

#if defined (TAO_EXPORT_NESTED_CLASSES)
#  if defined (TAO_EXPORT_NESTED_MACRO)
#    undef TAO_EXPORT_NESTED_MACRO
#  endif /* defined (TAO_EXPORT_NESTED_MACRO) */
#  define TAO_EXPORT_NESTED_MACRO 
#endif /* TAO_EXPORT_NESTED_CLASSES */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if defined (__BORLANDC__)
#pragma option push -w-rvl -w-rch -w-ccc -w-inl
#endif /* __BORLANDC__ */

// TAO_IDL - Generated from 
// be/be_visitor_root/root_ch.cpp:63

namespace TAO
{
  class Collocation_Proxy_Broker;
  
  template<typename T> class Narrow_Utils;
  template<typename T> class AbstractBase_Narrow_Utils;
}

// TAO_IDL - Generated from
// be/be_visitor_module/module_ch.cpp:48

namespace EMIRDFAGENT
{
  
  // TAO_IDL - Generated from
  // be/be_visitor_enum/enum_ch.cpp:57
  
  enum ObservationMode
  {
    IMAGE_BIAS,
    IMAGE_DARK,
    IMAGE_FLAT,
    IMAGE_SKY,
    DEAD_PX_MASK,
    IMAGE_FLAT_D,
    MSM_SPECTRAL_FLAT_FIELD,
    SPEC_SKY,
    SPEC_DARK,
    SPEC_FLAT,
    SLIT_TRANSM_CALIBR,
    WAVELENGTH_CALIBR,
    TS_ROUGH_FOCUS,
    TS_FINE_FOCUS,
    EMIR_FOCUS_CTL,
    IMAGE_ACQUISTION,
    MOS_LONG_SLIT,
    TARGET_ACQUISITION,
    MASK_IMAGING,
    MSM_LSM_CHECK,
    STARE_IMAGE,
    NODDED_BEAM_SWITCHED_IMAGE,
    DITHERED_IMAGE,
    MICRODITHERED_IMAGE,
    MOSAICED_IMAGE,
    STARE_SPECTRA,
    DITHERED_NODDED_SPECTRA_ALONG_SLIT,
    OFFSET_SPECTRA_BEYOND_SLIT,
    RASTER_SPECTRA,
    DTU_X_Y_CALIBRATION,
    DTU_Z_CALIBRATION,
    DTU_FLEXURE_COMP,
    CSU2DETECTOR_CALIBRATION,
    LATERAL_COLOUR,
    SPECTRAL_CHARACTERIZATION,
    CENTRE_ROTATION,
    ASTROMETRIC_CALIBRATION,
    PHOTOMETRIC_CALIBRATION,
    SPECTROPHOTOMETRIC_CALIBRATION,
    TEST0,
    TEST1,
    TEST2,
    TEST3,
    TEST4,
    TEST5,
    TEST6,
    TEST7
  };
  
  typedef ObservationMode &ObservationMode_out;
  
  // TAO_IDL - Generated from
  // be/be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_ObservationMode;
  
  // TAO_IDL - Generated from
  // be/be_interface.cpp:611

#if !defined (_EMIRDFAGENT_EMIRDFAGENT_IFCE__VAR_OUT_CH_)
#define _EMIRDFAGENT_EMIRDFAGENT_IFCE__VAR_OUT_CH_
  
  class EMIRDFAgent_ifce;
  typedef EMIRDFAgent_ifce *EMIRDFAgent_ifce_ptr;
  
  typedef
    TAO_Objref_Var_T<
        EMIRDFAgent_ifce
      >
    EMIRDFAgent_ifce_var;
  
  typedef
    TAO_Objref_Out_T<
        EMIRDFAgent_ifce
      >
    EMIRDFAgent_ifce_out;

#endif /* end #if !defined */
  
  // TAO_IDL - Generated from
  // be/be_visitor_interface/interface_ch.cpp:54

#if !defined (_EMIRDFAGENT_EMIRDFAGENT_IFCE_CH_)
#define _EMIRDFAGENT_EMIRDFAGENT_IFCE_CH_
  
  class  EMIRDFAgent_ifce
    : public virtual DFAGENT::DFAgent_ifce
  {
  public:
    friend class TAO::Narrow_Utils<EMIRDFAgent_ifce>;
    typedef EMIRDFAgent_ifce_ptr _ptr_type;
    typedef EMIRDFAgent_ifce_var _var_type;
    
    // The static operations.
    static EMIRDFAgent_ifce_ptr _duplicate (EMIRDFAgent_ifce_ptr obj);
    
    static EMIRDFAgent_ifce_ptr _narrow (
        CORBA::Object_ptr obj
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );
    
    static EMIRDFAgent_ifce_ptr _unchecked_narrow (
        CORBA::Object_ptr obj
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );
    
    static EMIRDFAgent_ifce_ptr _nil (void)
    {
      return (EMIRDFAgent_ifce_ptr)0;
    }
    
    static void _tao_any_destructor (void *);
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setObsMode (
        EMIRDFAGENT::ObservationMode obsMode
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setNumberOfImagesToCollect (
        CORBA::Long value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setNumberOfReadsInAPoint (
        CORBA::Long value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setFilterName (
        const char * filter,
        CORBA::Short position
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setGrismName (
        const char * grism
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setCSUConfFile (
        const char * file
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void endSequence (
        CORBA::Boolean value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setFlip (
        CORBA::Boolean value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setRotatorOffset (
        CORBA::Double value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setReferencePixel (
        CORBA::Double pixelX,
        CORBA::Double pixelY
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setPlateScale (
        CORBA::Double value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setInstrumentPositionAngle (
        CORBA::Double value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setDetectorPositionAngle (
        CORBA::Double value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setDTUPositionAngle (
        CORBA::Double value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setFocalPlanePositionAngle (
        CORBA::Double value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setPixelSizeAtDetector (
        CORBA::Double value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setPixelSizeAtFocalPlane (
        CORBA::Double value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setDTU_FOV (
        CORBA::Long valueX,
        CORBA::Long valueY,
        CORBA::Long valueZ
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setDTU_Factor (
        CORBA::Long valueX,
        CORBA::Long valueY,
        CORBA::Long valueZ
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setDTU_Commanded (
        CORBA::Long valueX,
        CORBA::Long valueY,
        CORBA::Long valueZ
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setCoordinateType (
        const char * value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setCRVAL (
        CORBA::Double alfa,
        CORBA::Double delta
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setDetectorCenter (
        CORBA::Long valueX,
        CORBA::Long valueY
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setNumberCycle (
        const char * value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setCryiostateVersion (
        const char * value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setNumObservingBlock (
        CORBA::Long numTotalObservingBlocks,
        CORBA::Long numActualObservingBlock,
        CORBA::Long numTotalImagesInObsBlock,
        CORBA::Long numActualImageInObsBlock
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setNumTotalObservingBlocks (
        CORBA::Long value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setNumActualObservingBlock (
        CORBA::Long value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setNumTotalImagesInObsBlock (
        CORBA::Long value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void setNumActualImageInObsBlock (
        CORBA::Long value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , DGT::GCSException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual ::EMIRDFAGENT::ObservationMode obsMode (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual CORBA::Boolean isReceivingImageFinished (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual char * observingEngineInstanceName (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void observingEngineInstanceName (
        const char * observingEngineInstanceName
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual char * dfManagerInstanceName (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void dfManagerInstanceName (
        const char * dfManagerInstanceName
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual char * dtuInstanceName (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void dtuInstanceName (
        const char * dtuInstanceName
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual char * emirInstanceName (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void emirInstanceName (
        const char * emirInstanceName
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual char * flatsPath (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void flatsPath (
        const char * flatsPath
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual char * biasPath (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void biasPath (
        const char * biasPath
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual char * darksPath (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void darksPath (
        const char * darksPath
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual char * skyPath (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void skyPath (
        const char * skyPath
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual char * pinholesPath (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void pinholesPath (
        const char * pinholesPath
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual char * latitude (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void latitude (
        const char * latitude
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual char * longitude (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void longitude (
        const char * longitude
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual char * telescopeName (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void telescopeName (
        const char * telescopeName
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual char * originFITS (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void originFITS (
        const char * originFITS
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual char * observatoryName (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void observatoryName (
        const char * observatoryName
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual CORBA::Short recenter (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void recenter (
        CORBA::Short recenter
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual CORBA::Double max_recenter_radius (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_operation/operation_ch.cpp:46
    
    virtual void max_recenter_radius (
        CORBA::Double max_recenter_radius
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
    
    // TAO_IDL - Generated from
    // be/be_visitor_interface/interface_ch.cpp:208
    
    virtual CORBA::Boolean _is_a (
        const char *type_id
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );
    
    virtual const char* _interface_repository_id (void) const;
    virtual CORBA::Boolean marshal (TAO_OutputCDR &cdr);
  private:
    TAO::Collocation_Proxy_Broker *the_TAO_EMIRDFAgent_ifce_Proxy_Broker_;
  
  protected:
    // Concrete interface only.
    EMIRDFAgent_ifce (int collocated = 0);
    
    // These methods travese the inheritance tree and set the
    // parents piece of the given class in the right mode.
    virtual void EMIRDFAGENT_EMIRDFAgent_ifce_setup_collocation (int collocated);
    
    // Concrete non-local interface only.
    EMIRDFAgent_ifce (
        IOP::IOR *ior,
        TAO_ORB_Core *orb_core = 0
      );
    
    // Non-local interface only.
    EMIRDFAgent_ifce (
        TAO_Stub *objref,
        CORBA::Boolean _tao_collocated = 0,
        TAO_Abstract_ServantBase *servant = 0,
        TAO_ORB_Core *orb_core = 0
      );
    
    virtual ~EMIRDFAgent_ifce (void);
  
  private:
    // Private and unimplemented for concrete interfaces.
    EMIRDFAgent_ifce (const EMIRDFAgent_ifce &);
    
    void operator= (const EMIRDFAgent_ifce &);
  };

#endif /* end #if !defined */
  
  // TAO_IDL - Generated from
  // be/be_visitor_typecode/typecode_decl.cpp:44
  
  TAO_NAMESPACE_STORAGE_CLASS ::CORBA::TypeCode_ptr _tc_EMIRDFAgent_ifce;

// TAO_IDL - Generated from
// be/be_visitor_module/module_ch.cpp:66

} // module EMIRDFAGENT

// Proxy Broker Factory function pointer declarations.

// TAO_IDL - Generated from
// be/be_visitor_root/root.cpp:120

extern 
TAO::Collocation_Proxy_Broker *
(*EMIRDFAGENT__TAO_EMIRDFAgent_ifce_Proxy_Broker_Factory_function_pointer) (
    CORBA::Object_ptr obj
  );

// TAO_IDL - Generated from
// be/be_visitor_traits.cpp:59

// Traits specializations.
namespace TAO
{

#if !defined (_EMIRDFAGENT_EMIRDFAGENT_IFCE__TRAITS_CH_)
#define _EMIRDFAGENT_EMIRDFAGENT_IFCE__TRAITS_CH_
  
  ACE_TEMPLATE_SPECIALIZATION
  struct  Objref_Traits<EMIRDFAGENT::EMIRDFAgent_ifce>
  {
    static EMIRDFAGENT::EMIRDFAgent_ifce_ptr tao_duplicate (
        EMIRDFAGENT::EMIRDFAgent_ifce_ptr
      );
    static void tao_release (
        EMIRDFAGENT::EMIRDFAgent_ifce_ptr
      );
    static EMIRDFAGENT::EMIRDFAgent_ifce_ptr tao_nil (void);
    static CORBA::Boolean tao_marshal (
        EMIRDFAGENT::EMIRDFAgent_ifce_ptr p,
        TAO_OutputCDR & cdr
      );
  };

#endif /* end #if !defined */
}

// TAO_IDL - Generated from
// be/be_visitor_enum/any_op_ch.cpp:51

 void operator<<= (CORBA::Any &, EMIRDFAGENT::ObservationMode);
 CORBA::Boolean operator>>= (const CORBA::Any &, EMIRDFAGENT::ObservationMode &);

// TAO_IDL - Generated from
// be/be_visitor_interface/any_op_ch.cpp:52

 void operator<<= (CORBA::Any &, EMIRDFAGENT::EMIRDFAgent_ifce_ptr); // copying
 void operator<<= (CORBA::Any &, EMIRDFAGENT::EMIRDFAgent_ifce_ptr *); // non-copying
 CORBA::Boolean operator>>= (const CORBA::Any &, EMIRDFAGENT::EMIRDFAgent_ifce_ptr &);

// TAO_IDL - Generated from
// be/be_visitor_root/cdr_op.cpp:48

#ifndef __ACE_INLINE__

// TAO_IDL - Generated from
// be/be_visitor_enum/cdr_op_ch.cpp:50

 CORBA::Boolean operator<< (TAO_OutputCDR &, const EMIRDFAGENT::ObservationMode &);
 CORBA::Boolean operator>> (TAO_InputCDR &, EMIRDFAGENT::ObservationMode &);

// TAO_IDL - Generated from
// be/be_visitor_interface/cdr_op_ch.cpp:55

 CORBA::Boolean operator<< (TAO_OutputCDR &, const EMIRDFAGENT::EMIRDFAgent_ifce_ptr );
 CORBA::Boolean operator>> (TAO_InputCDR &, EMIRDFAGENT::EMIRDFAgent_ifce_ptr &);

// TAO_IDL - Generated from
// be/be_visitor_root/cdr_op.cpp:64

#endif /* __ACE_INLINE__ */

// TAO_IDL - Generated from
// be/be_codegen.cpp:978

#if defined (__ACE_INLINE__)
#include "EMIRDFAGENTC.i"
#endif /* defined INLINE */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__BORLANDC__)
#pragma option pop
#endif /* __BORLANDC__ */

#endif /* ifndef */
