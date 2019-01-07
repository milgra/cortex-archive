//
#include "CInventory.hpp"
#include "steam/steam_api.h"

extern "C"
{
    #include "../../sources/defaults.c"
    #include "../../framework/core/mtbus.c"
    #include "../../framework/core/mtcstr.c"
    #include "../../framework/tools/settings.c"
}


CInventory::CInventory():
    m_CallbackItemsReceived( this, &CInventory::OnItemsReceived )
{


}


void CInventory::GetItems( void )
{

    SteamInventory()->GetAllItems( NULL );

}

void CInventory::GetPrices( void (*OnPrices)(void) )
{

    this->OnPrices = OnPrices;

    SteamAPICall_t hSteamAPICall = SteamInventory( )->RequestPrices( );
    
    m_SteamCallResultRequestPrices.Set(
        hSteamAPICall ,
        this ,
        &CInventory::OnRequestPricesResult );

    printf( "Requesting prices...\n" );

}


void CInventory::ShowNoPrices( int error )
{

    sprintf( itemLabels[ 0 ] , "STORE ERROR %i" , error );
    sprintf( itemLabels[ 1 ] , "" );
    sprintf( itemLabels[ 2 ] , "" );

}


void CInventory::OnRequestPricesResult( SteamInventoryRequestPricesResult_t *pParam, bool bIOFailure )
{
    // we may get callbacks for other games' stats arriving, ignore them
    printf( "Prices arrived, result : %i\n" , pParam->m_result );

    if ( pParam->m_result == k_EResultOK )
    {
        char currency[ 4 ];

        strncpy(
            currency,
            pParam->m_rgchCurrency ,
            sizeof( currency ) );

        uint32 itemCount = SteamInventory( )->GetNumItemsWithPrices( );
        
        if ( itemCount < 4 )
        {

            if ( SteamInventory( )->GetItemsWithPrices( itemDefs , itemPrices , itemCount ) )
            {
            
                for ( uint32 index = 0;
                             index < itemCount ;
                             index++ )
                {

                    if ( index < 3 )
                    {
                    
                        sprintf( itemLabels[ index ] ,
                                 "%0.2f %s" ,
                                 float( itemPrices[ index ] ) / 100 ,
                                 currency );
                        
                        printf( "%i %llu %s\n", itemDefs[ index ] , itemPrices[ index ] , itemLabels[ index ] );
                        
                    }
                    
                }
                
            }
            else ShowNoPrices( 801 );
        
        }
        else ShowNoPrices( 800 );
        
    }
    else ShowNoPrices( ( int ) pParam->m_result );

    this->OnPrices( );

}


void CInventory::BuyItem( char* label )
{

    SteamItemDef_t newitems[ 1 ] = { 0 };
    uint32 quantities[ 1 ] = { 1 };

    for ( int index = 0 ;
              index < 3 ;
              index++)
    {
    
        if ( strcmp( itemLabels[ index ] , label ) == 0 )
        {
        
            newitems[ 0 ] = itemDefs[ index ];
            
        }
        
    }
    
    printf( "ButItem %s %i\n" , label , newitems[0] );

    SteamAPICall_t hSteamAPICall = SteamInventory()->StartPurchase(
        newitems ,
        quantities ,
        1 );

    m_SteamCallResultStartPurchase.Set(
        hSteamAPICall,
        this,
        &CInventory::OnStartPurchaseResult );

}


void CInventory::OnStartPurchaseResult( SteamInventoryStartPurchaseResult_t *pParam , bool bIOFailure )
{

    printf( "OnStartPurchaseResult %i %i\n" , pParam->m_result , bIOFailure );

    SteamInventory()->GetAllItems( NULL );

}


void CInventory::OnItemsReceived( SteamInventoryResultReady_t* callback )
{

    printf( "OnItemsReceived %i\n" , callback->m_result );

    if ( callback->m_result == k_EResultOK )
    {

        uint32 count = 0;

        if ( SteamInventory()->GetResultItems( callback->m_handle, NULL, &count ) )
        {
			
        	SteamItemDetails_t items[ count ];

        	bool bGotResult = false;

            bGotResult = SteamInventory()->GetResultItems(
                callback->m_handle,
                items,
                &count );
			
			if ( bGotResult )
			{

				for ( int index = 0 ;
						  index < count ;
						  index++ )
				{

					SteamItemDetails_t detail = items[ index ];

					printf( "id %llu def %i quantity %i\n" ,
							detail.m_itemId ,
							detail.m_iDefinition ,
							detail.m_unQuantity  );

					defaults.donation_arrived = 1;
					
					// SteamInventory()->ConsumeItem( NULL , detail.m_itemId , detail.m_unQuantity );

				}

			}

        }

    }

    // We're not hanging on the the result after processing it.
    SteamInventory()->DestroyResult( callback->m_handle );

}


CInventory* inv;

void steam_storeprices( )
{

    for ( int index = 0 ;
              index < 3 ;
              index++ )
    {
    
        strcpy( defaults.prices[ index ] ,
                inv->itemLabels[ index ] );
        
        defaults.prices_arrived = 1;
        
    }
    
    mtbus_notify( "CTL", "SHOWDONATION" , NULL );
    
}

void steam_init( )
{
    bool success = SteamAPI_Init( );
    
    printf( "Steam Init Success %i\n" , success );

    if ( success )
    {
        inv = new CInventory( );
        inv->GetItems( );
        inv->GetPrices( &steam_storeprices );
    }
}

void steam_buy( char* item )
{
    inv->BuyItem( item );
}
