
#import <Foundation/Foundation.h>
#import <UIKit/UIApplication.h>
#import "Bridge.h"
#include "../../sources/defaults.c"
#include "../../framework/core/mtbus.c"
#include "../../framework/core/mtcstr.c"
#include "../../framework/tools/settings.c"

@interface Bridge ( )
{

    SKProductsRequest* request;
    NSMutableArray* products;
    
    SKProduct* productA;
    SKProduct* productB;
    SKProduct* productC;
    
}

@end


@implementation Bridge


- ( void ) openurl : ( char* ) string
{

    NSURL *url = [NSURL URLWithString:[NSString stringWithUTF8String:string]];
    if ([[UIApplication sharedApplication] canOpenURL:url]) {
       [[UIApplication sharedApplication] openURL:url options:[NSDictionary dictionary ] completionHandler:NULL ];
    }
    

}


- ( id ) init
{

    self = [ super init ];
    
    products = [ [ NSMutableArray alloc ] init ];
    
    // setup transaction observer
    
    [ [ SKPaymentQueue defaultQueue ] addTransactionObserver : self ];

    // load donation products from store
    
    [ self validateProductIdentifiers ];

    return self;
    
}

- ( NSUInteger ) itemslength
{
    return [products count];
}

- ( void ) donate_a_pressed
{

    if ( productA != nil )
    {
        SKMutablePayment *payment = [ SKMutablePayment paymentWithProduct : productA ];
        [ [ SKPaymentQueue defaultQueue ] addPayment : payment ];
    }
    
}


- ( void ) donate_b_pressed
{

    if ( productB != nil )
    {
        SKMutablePayment *payment = [ SKMutablePayment paymentWithProduct : productB ];
        [ [ SKPaymentQueue defaultQueue ] addPayment : payment ];
    }
    
}


- ( void ) donate_c_pressed
{

    if ( productC != nil )
    {
        SKMutablePayment *payment = [ SKMutablePayment paymentWithProduct : productC ];
        [ [ SKPaymentQueue defaultQueue ] addPayment : payment ];
    }
    
}


- ( void ) validateProductIdentifiers
{

    NSArray* productIds = @[ @"cortexdonationsmall" , @"cortexdonationmedium" , @"cortexdonationnormal" ];

    request = [ [ SKProductsRequest alloc ] initWithProductIdentifiers : [ NSSet setWithArray : productIds ] ];
    request.delegate = self;
    [ request start ];
    
}


- ( void ) productsRequest : ( SKProductsRequest* ) request
           didReceiveResponse : ( SKProductsResponse* ) response
{

    [ products addObjectsFromArray : response.products ];
    
    for ( SKProduct* product in products )
    {
    
        NSNumberFormatter *numberFormatter = [ [ NSNumberFormatter alloc ] init ];
        [ numberFormatter setFormatterBehavior : NSNumberFormatterBehavior10_4 ];
        [ numberFormatter setNumberStyle : NSNumberFormatterCurrencyStyle ];
        [ numberFormatter setLocale : product.priceLocale ];
        NSString *formattedPrice = [ numberFormatter stringFromNumber : product.price ];

        if ( [ product.productIdentifier isEqualToString : @"cortexdonationsmall" ] )
        {
            productA = product;
            const char* bytes = [ formattedPrice UTF8String ];
            strcpy( defaults.prices[ 0 ], bytes );
        }
        if ( [ product.productIdentifier isEqualToString : @"cortexdonationmedium" ] )
        {
            productB = product;
            const char* bytes = [ formattedPrice UTF8String ];
            strcpy( defaults.prices[ 1 ], bytes );
        }
        if ( [ product.productIdentifier isEqualToString : @"cortexdonationnormal" ] )
        {
            productC = product;
            const char* bytes = [ formattedPrice UTF8String ];
            strcpy( defaults.prices[ 2 ], bytes );
        }

    }

    dispatch_async( dispatch_get_main_queue() , ^{
        defaults.prices_arrived = 1;
        mtbus_notify( "CTL", "SHOWDONATION" , NULL );
    });

    for ( NSString *invalidIdentifier in response.invalidProductIdentifiers )
    {
        NSLog( @"invalid product : %@" , invalidIdentifier );
    }

}


- ( void ) paymentQueue : ( SKPaymentQueue* ) queue
           updatedTransactions : ( NSArray* ) transactions
{

    NSString* result = @"";

    for ( SKPaymentTransaction* transaction in transactions )
    {
        switch ( transaction.transactionState )
        {
            // Call the appropriate custom method for the transaction state.
            case SKPaymentTransactionStatePurchasing :
                result = @"Purchasing";
                break;
                
            case SKPaymentTransactionStateDeferred :
                result = @"Deferred";
                [ [ SKPaymentQueue defaultQueue ] finishTransaction : transaction ];
                break;
                
            case SKPaymentTransactionStateFailed :
                result = @"Failed";
                [ [ SKPaymentQueue defaultQueue ] finishTransaction : transaction ];
                break;
                
            case SKPaymentTransactionStatePurchased :
            {
                result = @"Thank you for your donation!";
                UIAlertController* alert =
                    [ UIAlertController
                        alertControllerWithTitle : @"Thank you"
                        message : result
                        preferredStyle : UIAlertControllerStyleAlert ];
                
                UIAlertAction* defaultAction = [ UIAlertAction
                    actionWithTitle : @"OK"
                    style : UIAlertActionStyleDefault
                    handler : ^(UIAlertAction * action ) { } ];
                
                [ alert addAction : defaultAction ];
                
                // maybe notify ui later about the purchase

                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                break;
            }
            
            case SKPaymentTransactionStateRestored :
                result = @"Restored";
                [ [ SKPaymentQueue defaultQueue ] finishTransaction : transaction ];
                break;
                
            default:
                [ [ SKPaymentQueue defaultQueue ] finishTransaction : transaction ];
                NSLog( @"Unexpected transaction state %@" , @(transaction.transactionState) );
                break;
        }

        NSLog( @"updateTransaction %@ %@" , transaction.error , result );

    }
    
}

@end

Bridge* bridge;

void bridge_init( void )
{
    bridge = [[ Bridge alloc ] init ];
    
    if ( UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone )
    {
        defaults.text_scale = 1.2;
    }
}

void bridge_free( void )
{

}

void bridge_open( char* url )
{
    [ bridge openurl : url ];
}

void bridge_buy( char* item )
{
   for ( int index = 0 ; index < 3 ; index++)
    {
        if ( strcmp( defaults.prices[ index ] , item ) == 0 )
        {
            if ( index == 0 ) [ bridge donate_a_pressed ];
            if ( index == 1 ) [ bridge donate_b_pressed ];
            if ( index == 2 ) [ bridge donate_c_pressed ];
        }
    }
}
