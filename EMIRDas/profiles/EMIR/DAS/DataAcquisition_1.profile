#--------------------------------------------------------------
# EMIR/DAS/DataAcquisition_1 Device profile
# File generated: 11/6/2013  
#--------------------------------------------------------------

[ Header EMIR/DAS/DataAcquisition_1.header ]
instance   = EMIR/DAS/DataAcquisition_1
class_name = EMIRDas
version    = 0.1
panels     = gtc.AL.Inspector.Panels.Device
tabs       = Commands|Alarms|Configs|Monitors
ifr_id     = IDL:gtc/EMIRDAS/EMIRDas_ifce:1.0

#--------------------------------------------------------------
# Properties
#--------------------------------------------------------------
[ Property EMIR/DAS/DataAcquisition_1.poweronApp ]
description = Path to the power on controller program
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_1.poweroffApp ]
description = Path to the power off controller program
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_1.adqApp ]
description = Path to the adquisition controller program
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_1.imgDir ]
description = Path to the image storage directory
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_1.simFile ]
description = Path to the simulated image directory
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_1.dataFactoryName ]
description = Data Factory ID
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_1.isDataFactoryEnabled ]
description = Data Factory On/Off
units       = None
type        = short
max_value   = short : 1
min_value   = short : 0

[ Property EMIR/DAS/DataAcquisition_1.firmware ]
description = Firmware version. Decimal value. The controller stores the value in hex.
units       = None
type        = long
max_value   = long : 10000000
min_value   = long : 0

[ Property EMIR/DAS/DataAcquisition_1.detectorType ]
description = Type of Detector (ENG,SCI,MUX)
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_1.saveFile ]
description = saveFile
units       = None
type        = short
max_value   = short : 1
min_value   = short : 0

[ Property EMIR/DAS/DataAcquisition_1.sendFrame ]
description = sendFrame
units       = None
type        = short
max_value   = short : 1
min_value   = short : 0

[ Property EMIR/DAS/DataAcquisition_1.reorder ]
description = reorder
units       = None
type        = short
max_value   = short : 1
min_value   = short : 0

[ Property EMIR/DAS/DataAcquisition_1.invert ]
description = invert
units       = None
type        = short
max_value   = short : 1
min_value   = short : 0

[ Property EMIR/DAS/DataAcquisition_1.simDetector ]
description = Execute in normal or simulated mode
units       = None
type        = short
max_value   = short : 1
min_value   = short : 0

[ Property EMIR/DAS/DataAcquisition_1.simMode ]
description = Select the simulation mode, 2 = pattern, 1 = file
units       = None
type        = short
max_value   = short : 1
min_value   = short : 0

[ Property EMIR/DAS/DataAcquisition_1.poolSize ]
description = Number of memory slots in the frame queue
units       = None
type        = long
max_value   = long : 37
min_value   = long : 1

[ Property EMIR/DAS/DataAcquisition_1.voffset ]
description = voffset
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_1.vreset ]
description = vreset
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_1.biasgate ]
description = biasgate
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_1.ron ]
description = ron
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

[ Property EMIR/DAS/DataAcquisition_1.gain ]
description = gain
units       = None
type        = string
max_value   = string : 1
min_value   = string : 1

#--------------------------------------------------------------
# Magnitudes
#--------------------------------------------------------------
[ Magnitude EMIR/DAS/DataAcquisition_1.isSettingup ]
description     = It is true if DAS is applying the configuration.
units           = None
type            = boolean
upper_limit     = bool : true
lower_limit     = bool : false
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.isConfigurationDone ]
description     = It is true if DAS is the configuration has been applied.
units           = None
type            = boolean
upper_limit     = bool : true
lower_limit     = bool : false
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.isExposing ]
description     = It is true if the CCD Controller is taking exposures.
units           = None
type            = boolean
upper_limit     = bool : true
lower_limit     = bool : false
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.isDetectorControllerIdle ]
description     = It is true if the Acquisition system is in Idle mode.
units           = None
type            = boolean
upper_limit     = bool : true
lower_limit     = bool : false
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentReadoutMode ]
description     = Readout mode.
units           = None
type            = enum
upper_limit     = ReadoutMode : 5
lower_limit     = ReadoutMode : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentSDSUControllerStatus ]
description     = Current status SDSU Controller
units           = None
type            = enum
upper_limit     = SDSUControllerStatus : 4
lower_limit     = SDSUControllerStatus : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentElapsedTime ]
description     = Current elapsed Time
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentExposureTime ]
description     = Current Exposure Time
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentNumberOfImages ]
description     = Current number of images
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentNSEC ]
description     = Current NSEC controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentNO_EXPOSURES ]
description     = Current NO_EXPOSURES controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentNRDBL ]
description     = Current NRDBL controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentNRDBL_ND ]
description     = Current NRDBL_ND controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentNPARAM_X ]
description     = Current NPARAM_X controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentNRST ]
description     = Current NRST controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentNRDIL ]
description     = Current NRDIL controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentNRDIL_ND ]
description     = Current NRDIL_ND controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentOPTRST ]
description     = Current OPTRST controller value
units           = None
type            = long
upper_limit     = long : 2
lower_limit     = long : 0
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentX_SIZE ]
description     = Current X_SIZE controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentY_SIZE ]
description     = Current Y_SIZE controller value
units           = None
type            = long
upper_limit     = long : 1
lower_limit     = long : 1
default_sampling_period = TimeValue : 0.0s
default_storage_period  = TimeValue : 0.0s

[ Magnitude EMIR/DAS/DataAcquisition_1.currentY_SIZE_B ]
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
[ Alarm EMIR/DAS/DataAcquisition_1.DeviceFault ]
severity = LEVEL1
req_ack  = Yes
actions  = "Check this Device"

[ Alarm EMIR/DAS/DataAcquisition_1.operationFailed ]
severity = LEVEL1
req_ack  = Yes
actions  = "Check this Device"

[ Alarm EMIR/DAS/DataAcquisition_1.CommunicationFailure ]
severity = Level5
req_ack  = Yes
actions  = "Check SDSU Controller"

#--------------------------------------------------------------
# Default configuration
#--------------------------------------------------------------
[ Configuration EMIR/DAS/DataAcquisition_1.Default ]
imgDir      = string : /scratch/EMIR/frames
simFile     = string : /scratch/EMIR/sim/default.img

# Internal
dataFactoryName      = string : DF/EMIRDFAgent
isDataFactoryEnabled = short : 0
poolSize             = short : 37
saveFile             = short : 1
sendFrame            = short : 0
reorder              = short : 1
invert               = short : 0
simDetector          = short : 1
# 1 = File, 2 = Pattern 
simMode              = short : 2

# Detector related
poweronApp           = string : /scratch/EMIR/lods/33-18PowerONMUXUnix.lod
poweroffApp          = string : /scratch/EMIR/lods/33-18PowerOffMUX.lod
adqApp               = string : /scratch/EMIR/lods/33-18ApplicationCodeMUXUnix.lod
firmware             = long   : 77859
#firmware             = long   : 99109
voffset              = double : 0.0
vreset               = double : 0.0
biasgate             = double : 0.0
ron                  = double : 0.0
gain                 = double : 0.0
detectorType         = string : SCI
