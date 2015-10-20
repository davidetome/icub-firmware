/*
 * Copyright (C) 2015 iCub Facility - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * website: www.robotcub.org
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/


// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _EOTHEMOTIONCONTROLLER_H_
#define _EOTHEMOTIONCONTROLLER_H_

// - doxy begin -------------------------------------------------------------------------------------------------------

/** @file       EOtheMotionController.h
    @brief      this object implements what is required for managing the STRAIN.                
    @author     marco.accame@iit.it
    @date       05/28/2015
**/

/** @defgroup eo_EOtheMotionController Object EOtheMotionController
    ...... 
    @{        
 **/

// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EoProtocol.h"


// - public #define  --------------------------------------------------------------------------------------------------
// empty-section
 
// - declaration of public user-defined types ------------------------------------------------------------------------- 

typedef struct EOtheMotionController_hid EOtheMotionController;



typedef eOresult_t (*eOmotcon_onendofoperation_fun_t) (EOtheMotionController* p, eObool_t operationisok);

enum { eo_motcon_maxJOMOs = 12 };


typedef enum
{
    eo_motcon_mode_NONE         = eomn_serv_NONE,
    eo_motcon_mode_foc          = eomn_serv_MC_foc,
    eo_motcon_mode_mc4          = eomn_serv_MC_mc4,
    eo_motcon_mode_mc4plus      = eomn_serv_MC_mc4plus,
    eo_motcon_mode_mc4plusmais  = eomn_serv_MC_mc4plusmais   
} eOmotioncontroller_mode_t;
   
// - declaration of extern public variables, ...deprecated: better using use _get/_set instead ------------------------
// empty-section

// - declaration of extern public functions ---------------------------------------------------------------------------


extern EOtheMotionController* eo_motioncontrol_Initialise(void);

extern EOtheMotionController* eo_motioncontrol_GetHandle(void);


extern eOmotioncontroller_mode_t eo_motioncontrol_GetMode(EOtheMotionController *p);



// so far we support only eo_motcon_mode_foc and eo_motcon_mode_mc4

extern eOresult_t eo_motioncontrol_Verify(EOtheMotionController *p, const eOmn_serv_configuration_t * servcfg, eOmotcon_onendofoperation_fun_t onverify, eObool_t activateafterverify);

extern eOresult_t eo_motioncontrol_Activate(EOtheMotionController *p, const eOmn_serv_configuration_t * servcfg);

extern eOresult_t eo_motioncontrol_Deactivate(EOtheMotionController *p);


// to be called once before to be ticked
extern eOresult_t eo_motioncontrol_Start(EOtheMotionController *p);

// it ticks every ms of the control-loop
extern eOresult_t eo_motioncontrol_Tick(EOtheMotionController *p);

extern eOresult_t eo_motioncontrol_Stop(EOtheMotionController *p);

/** @}            
    end of group eo_EOtheMotionController
 **/

#endif  // include-guard

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


