//
//  CInventory.hpp
//  Termite
//
//  Created by Milan Toth on 2018. 11. 13..
//  Copyright © 2018. milgra. All rights reserved.
//

#ifndef CInventory_hpp
#define CInventory_hpp

#include <stdio.h>
#include "steam/steam_api.h"

void steam_init( void );
void steam_buy( char* item );

class CInventory final
{

private:

    void (*OnPrices)(void);
    void ShowNoPrices(int error);

public:

    SteamItemDef_t itemDefs[3];
    uint64 itemPrices[3];
    char itemLabels[3][512];

    CInventory();
    ~CInventory();

    void GetPrices( void (*onprices)(void) );
    void BuyItem( char* name );

    STEAM_CALLBACK( CInventory, OnItemsReceived, SteamInventoryResultReady_t,
        m_CallbackItemsReceived );

    void OnStartPurchaseResult( SteamInventoryStartPurchaseResult_t *pParam, bool bIOFailure );
    CCallResult<CInventory, SteamInventoryStartPurchaseResult_t> m_SteamCallResultStartPurchase;

    void OnRequestPricesResult( SteamInventoryRequestPricesResult_t *pParam, bool bIOFailure );
    CCallResult<CInventory, SteamInventoryRequestPricesResult_t> m_SteamCallResultRequestPrices;
};

#endif /* CInventory_hpp */
