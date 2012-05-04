
/* @file       EOpid.c
   @brief      This file implements internal implementation of a PID controller.
   @author     alessandro.scalzo@iit.it
   @date       27/03/2012
**/


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h"
#include "EoCommon.h"

#include "EOtheMemoryPool.h"
#include "EOtheErrorManager.h"
#include "EOVtheSystem.h"



// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "EOpid.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "EOpid_hid.h" 


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------
// empty-section



// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const char s_eobj_ownname[] = "EOpid";


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------

extern EOpid* eo_pid_New(void) 
{
    EOpid *o = eo_mempool_GetMemory(eo_mempool_GetHandle(), eo_mempool_align_32bit, sizeof(EOpid), 1);

    if (o)
    {
        o->Ko = 0.0f;
        o->A0 = 0.0f;
        o->A1 = 0.0f;
        o->A2 = 0.0f;
        o->Yn = 0.0f;
        o->Ymax = 0.0f;
        o->En_1 = 0.0f;
        o->Dn   = 0.0f;

        o->initialized = 0;
    }

    return o;
}

extern void eo_pid_Init(EOpid *o, float Kp, float Kd, float Ki, float Ko, float Ymax)
{
    //o->A0 =  Kp + Kd + Ki;
    //o->A1 = -Kp - 2.0f*Kd;
    //o->A2 =  Kd;
        
    o->A2 = -0.1f*Kd;
    o->A1 = -Kp + o->A2;
    o->A0 =  Kp + Ki - o->A2;

    o->Ymax = Ymax;

    o->Yn = o->Ko = Ko;

    o->En_1 = 0.0f;
    o->Dn   = 0.0f;

    o->initialized = 1;
}

extern uint8_t eo_pid_IsInitialized(EOpid *o)
{
    return o->initialized;
}

extern void eo_pid_Reset(EOpid *o)
{
    o->Yn   = o->Ko;
    o->En_1 = 0.0f; 
    o->Dn   = 0.0f; 
}

extern float eo_pid_GetOffset(EOpid *o)
{
    return o->Ko;
}

/*
extern float eo_pid_PWM(EOpid *o, float En)
{
    o->Yn += o->A0 * En + o->A1 * o->En_1 + o->A2 * o->En_2;
    o->En_2 = o->En_1; 
    o->En_1 = En;
    
    if (o->Yn >  o->Ymax) return o->Yn =  o->Ymax;
    
    if (o->Yn < -o->Ymax) return o->Yn = -o->Ymax;
    
    return o->Yn;
}
*/

extern float eo_pid_PWM(EOpid *o, float En)
{   
    o->Yn += o->A0 * En + o->A1 * o->En_1 + o->A2 * o->Dn;
    o->Dn = 0.9f*o->Dn + 0.1f*(En - o->En_1);
    o->En_1 = En;
    
    if (o->Yn >  o->Ymax) return o->Yn =  o->Ymax;
    
    if (o->Yn < -o->Ymax) return o->Yn = -o->Ymax;
    
    return o->Yn;
}

// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------




