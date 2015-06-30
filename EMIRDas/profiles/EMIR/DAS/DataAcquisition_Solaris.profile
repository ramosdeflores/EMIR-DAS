#--------------------------------------------------------------
# EMIR/DAS/DataAcquisition_1 Device profile
# File generated: 11/6/2013  
#--------------------------------------------------------------

[ Header EMIR/DAS/DataAcquisition_Solaris.header ]
instance   = EMIR/DAS/DataAcquisition_Solaris
class_name = EMIRDas
version    = 0.1
panels     = gtc.AL.Inspector.Panels.Device
tabs       = Commands|Alarms|Configs|Monitors
ifr_id     = IDL:gtc/EMIRDAS/EMIRDas_ifce:1.0

#--------------------------------------------------------------
# Properties
#--------------------------------------------------------------
[ Property EMIR/DAS/DataAcquisition_Solaris.poweronApp ]
description = Path to the power on controller program
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_Solaris.poweroffApp ]
description = Path to the power off controller program
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_Solaris.adqApp ]
description = Path to the adquisition controller program
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_Solaris.imgDir ]
description = Path to the image storage directory
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_Solaris.dataFactoryName ]
description = Data Factory ID
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_Solaris.isDataFactoryEnabled ]
description = Data Factory On/Off
units       = None
type        = short
max_value   = short : 1
min_value   = short : 0

#--------------------------------------------------------------
# Magnitudes
#--------------------------------------------------------------
[ Magnitude EMIR/DAS/DataAcquisition_Solaris.isSettingup ]
description     = It is true if DAS is applying the configuration.
units           = None
type            = boolean
upper_limit     = bool : true
lower_limit     = bool : false
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.isConfigurationDone ]
description     = It is true if DAS is the configuration has been applied.
units           = None
type            = boolean
upper_limit     = bool : true
lower_limit     = bool : false
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.isExposing ]
description     = It is true if the CCD Controller is taking exposures.
units           = None
type            = boolean
upper_limit     = bool : true
lower_limit     = bool : false
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.isDetectorControllerIdle ]
description     = It is true if the Acquisition system is in Idle mode.
units           = None
type            = boolean
upper_limit     = bool : true
lower_limit     = bool : false
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentReadoutMode ]
description     = Readout mode.
units           = None
type            = enum
upper_limit     = ReadoutMode : 5
lower_limit     = ReadoutMode : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentSDSUControllerStatus ]
description     = Current status SDSU Controller
units           = None
type            = enum
upper_limit     = SDSUControllerStatus : 4
lower_limit     = SDSUControllerStatus : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentElapsedTime ]
description     = Current elapsed Time
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentExposureTime ]
description     = Current Exposure Time
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentNumberOfImages ]
description     = Current number of images
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentNSEC ]
description     = Current NSEC controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentNO_EXPOSURES ]
description     = Current NO_EXPOSURES controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentNRDBL ]
description     = Current NRDBL controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentNRDBL_ND ]
description     = Current NRDBL_ND controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentNPARAM_X ]
description     = Current NPARAM_X controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentNRST ]
description     = Current NRST controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentNRDIL ]
description     = Current NRDIL controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentNRDIL_ND ]
description     = Current NRDIL_ND controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentOPTRST ]
description     = Current OPTRST controller value
units           = None
type            = long
upper_limit     = long : 2
lower_limit     = long : 0
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentX_SIZE ]
description     = Current X_SIZE controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentY_SIZE ]
description     = Current Y_SIZE controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_Solaris.currentY_SIZE_B ]
description     = Current Y_SIZE_B controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

#--------------------------------------------------------------
# Alarms
#--------------------------------------------------------------
[ Alarm EMIR/DAS/DataAcquisition_Solaris.deviceFault ]
severity = LEVEL1
req_ack  = Yes
actions  = "Check this Device"

[ Alarm EMIR/DAS/DataAcquisition_Solaris.operationFailed ]
severity = LEVEL1
req_ack  = Yes
actions  = "Check this Device"

[ Alarm EMIR/DAS/DataAcquisition_Solaris.CommunicationFailure ]
severity = Level5
req_ack  = Yes
actions  = "Check SDSU Controller"

#--------------------------------------------------------------
# Default configuration
#--------------------------------------------------------------
[ Configuration EMIR/DAS/DataAcquisition_Solaris.Default ]
poweronApp  = string : /scratch/EMIR/lods/33-11PowerONMUXUnix.lod
poweroffApp = string : /scratch/EMIR/lods/33-11PowerOffENG.lod
adqApp      = string : /scratch/EMIR/lods/33-11ApplicationCodeMUXUnix.lod
imgDir      = string : NONE
dataFactoryName = string : DF/EMIRDFAgent
isDataFactoryEnabled = short : 1
