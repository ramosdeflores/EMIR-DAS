#include "SimpleProcManager.h"

////////////////// HAY QUE MODIFICAR PARA QUE LEA DE FIXDATA EN VEZ DE USAR OPER CON LOGICAL REORDER. VER SIMPLESTORAGE

#ifndef VXWORKS
#define INT16	short
#define UINT16	unsigned short
#endif


void SimpleProcManager::bTransform() {

   int ccdx[4], ccdy[4], ccdbase[4];
   int line, chanel, ccd, pix;
   int chanelStep, schanel, baseChAdd;
   int i;

   schanel = 128; // NPIX / CCDS / CHANELS 

   ccdbase[0] = COLS / 2 - 1;           ccdx[0] = -1;    ccdy[0] = +COLS;
   ccdbase[1] = SDSU_SIZE / 2;          ccdx[1] = +COLS; ccdy[1] = +1;
   ccdbase[2] = SDSU_SIZE - (COLS / 2); ccdx[2] = +1;    ccdy[2] = -COLS;
   ccdbase[3] = SDSU_SIZE / 2 - 1;      ccdx[3] = -COLS; ccdy[3] = -1;

   i = 0;
   for (line = 0; line < NLINES; ++line) {
      for (pix = 0; pix < schanel; ++pix) {
         for (ccd = 0; ccd < CCDS; ++ccd) {
            chanelStep = schanel * ccdx[ccd];
            baseChAdd = ccdbase[ccd] + line * ccdy[ccd] + pix * ccdx[ccd];
            for (chanel = 0; chanel < CHANELS; ++chanel)
               tIndex[i++] = baseChAdd + chanel * chanelStep;
         }
      }
   }
}

SimpleProcManager::SimpleProcManager(staticData data)
{

  std::cout << "CREATING MANAGER " << endl;

  fixData = data;

  if ( (tIndex = (long int *)malloc(SDSU_SIZE * sizeof(long int))) == NULL ) {
     std::cout << "SimpleProcManager ERROR ALLOCATING MEMORY FOR tIndex" << endl;
     fixData.logAgent_->logError("SimpleProManager() FAILED TO ALLOCATE MEMORY FOR INDEX MATRIX");
  }

  if ( (Matrix1 = (UINT16 *)malloc(SDSU_SIZE * sizeof(UINT16))) == NULL ) {
     std::cout << "SimpleProcManager ERROR ALLOCATING MEMORY FOR Matrix1" << endl;
     fixData.logAgent_->logError("SimpleProManager() FAILED TO ALLOCATE MEMORY FOR TEMP MATRIX");
  }

  trace_ = new Trace("SimpleProcManager");
;
}

SimpleProcManager::~SimpleProcManager()
{
  if ( tIndex != NULL ) free(tIndex);
  if ( Matrix1 != NULL ) free(Matrix1);
;
}


int SimpleProcManager::initFrame()
{
   trace_->out ("initFrame() starts");

   bTransform();

   trace_->out ("intFrame() ends");

   return DAS::OPERATION_OK;
}

int SimpleProcManager::procFrame(EMIRFrame *emirFrame) {
   UINT16 *Matrix0;
   TimeValue ttamp0, ttamp1, ttamp2, utc;
   int i0, i1;

   trace_->out ("procFrame() starts");

   Matrix0 = (UINT16 *)(emirFrame->DetectorData + SDSU_HEADER_SIZE / 2);

   ttamp0 = TimeService::getTAI();
   // Reorder the frame 
   if ( fixData.reorder ) {
      trace_->out ("procFrame() reordering frame");
      for (i0 = 0; i0 < SDSU_SIZE; ++i0 )
         Matrix1[tIndex[i0]] = Matrix0[i0];
   } else {
      trace_->out ("procFrame() REORDER copying frame");
      memcpy(Matrix1, Matrix0, SDSU_SIZE * sizeof(UINT16));
   }
   ttamp1 = TimeService::getTAI();


   // Invert the frame
   if ( fixData.invert ) {
      trace_->out ("procFrame() inverting frame");
      for ( i0 = 0, i1 = SDSU_SIZE; i0 < SDSU_SIZE; ++i0, --i1)
         Matrix0[i0] = Matrix1[i1];
   } else {
      trace_->out("procFrame() INVER copying frame");
      memcpy(Matrix0, Matrix1, SDSU_SIZE * sizeof(UINT16));
   }
   ttamp2 = TimeService::getTAI();

   std::cout << "FRAME REORDERING = " << ttamp1.toFloat() - ttamp0.toFloat() << " FRAME INVERT = " << ttamp2.toFloat() - ttamp1.toFloat() << endl;

   trace_->out ("procFrame() ends");

   return DAS::OPERATION_OK;
}
