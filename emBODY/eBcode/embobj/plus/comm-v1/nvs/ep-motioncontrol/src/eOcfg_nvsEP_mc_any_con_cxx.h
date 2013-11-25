/*
 * Copyright (C) 2011 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
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
#ifndef _EOCFG_NVSEP_MC_ANY_CON_CXX_H_
#define _EOCFG_NVSEP_MC_ANY_CON_CXX_H_




/** @file       eOcfg_nvsEP_mc_any_con_cxx.h
	@brief      This header file gives the ...
	@author     marco.accame@iit.it
	@date       09/06/2011
**/

/** @defgroup eo_sdilsdede234567 Configuation of the ......
    Tcecece 
    
    @{		
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
#include "EoMotionControl.h"
#include "EOnv.h"



// - public #define  --------------------------------------------------------------------------------------------------


// -- max number of network variables in a controller

#define  EOK_cfg_nvsEP_mc_any_con_cxx_maxnumof_nvs_in_controller                            (32)


// -- the index of the network variables inside a controller


#define EOK_cfg_nvsEP_mc_any_con_cxx_cnvindex_cconfig                                       (0)
#define EOK_cfg_nvsEP_mc_any_con_cxx_cnvindex_cconfig__durationofctrlloop                   (1)
#define EOK_cfg_nvsEP_mc_any_con_cxx_cnvindex_cstatus                                       (2)
#define EOK_cfg_nvsEP_mc_any_con_cxx_cnvindex_cstatus__alljomoinitted                       (3)
#define EOK_cfg_nvsEP_mc_any_con_cxx_cnvindex_ccmmnds__go2stateofcontroller                 (4)


// -- the total number of network variables inside a joint

#define EOK_cfg_nvsEP_mc_any_con_cxx_cnvindex_TOTALnumber                                   (5)






// -- the fun and typ of all the network variables in the joint


#define EOK_cfg_nvsEP_mc_any_con_cxx_NVFUNTYP_cconfig                                       EO_nv_FUNTYP(eo_nv_FUN_cfg, eo_nv_TYP_pkd)
#define EOK_cfg_nvsEP_mc_any_con_cxx_NVFUNTYP_cconfig__durationofctrlloop                   EO_nv_FUNTYP(eo_nv_FUN_cfg, eo_nv_TYP_u32)

#define EOK_cfg_nvsEP_mc_any_con_cxx_NVFUNTYP_cstatus                                       EO_nv_FUNTYP(eo_nv_FUN_inp, eo_nv_TYP_pkd)
#define EOK_cfg_nvsEP_mc_any_con_cxx_NVFUNTYP_cstatus__alljomoinitted                       EO_nv_FUNTYP(eo_nv_FUN_inp, eo_nv_TYP_u08)
#define EOK_cfg_nvsEP_mc_any_con_cxx_NVFUNTYP_ccmmnds__go2stateofcontroller                 EO_nv_FUNTYP(eo_nv_FUN_out, eo_nv_TYP_u08)






// - declaration of public user-defined types ------------------------------------------------------------------------- 

EO_VERIFYproposition(eo_cfg_nvsEP_mc_any_con_cxx_check, (EOK_cfg_nvsEP_mc_any_con_cxx_cnvindex_TOTALnumber <= EOK_cfg_nvsEP_mc_any_con_cxx_maxnumof_nvs_in_controller));

    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------

// contains the funtyp ordered by nvindex
extern const uint8_t eo_cfg_nvsEP_mc_any_con_cxx_funtyp[];


// - declaration of extern public functions ---------------------------------------------------------------------------




/** @}            
    end of group eo_sdilsdede234567  
 **/

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------





