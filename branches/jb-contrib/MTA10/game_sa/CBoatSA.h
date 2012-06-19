/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        game_sa/CBoatSA.h
*  PURPOSE:     Header file for boat vehicle entity class
*  DEVELOPERS:  Ed Lyons <eai@opencoding.net>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#ifndef __CGAMESA_BOAT
#define __CGAMESA_BOAT

#include <game/CBoat.h>
#include "CVehicleSA.h"

class CBoatSAInterface : public CVehicleSAInterface
{
    // fill this
};
C_ASSERT(sizeof(CBoatSAInterface) == 0x7E8);

class CBoatSA : public virtual CBoat, public virtual CVehicleSA
{
private:
//  CBoatSAInterface        * internalInterface;
public:
                                CBoatSA( CBoatSAInterface * boat );
                                CBoatSA( eVehicleTypes dwModelID, unsigned char ucVariation, unsigned char ucVariation2 );

    virtual                     ~CBoatSA ( void ) {};
};

#endif
