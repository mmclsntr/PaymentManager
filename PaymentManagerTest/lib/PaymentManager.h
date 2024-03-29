//
//  PaymentManager.h
//  PaymentManagerTest
//
//  Created by user on 2014/10/29.
//  Copyright (c) 2014年 yamasaki. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

#import "VerifyStoreReceipt.h"

/** 購入処理状況 */
typedef enum {
    /** 購入処理状況 購入処理開始 */
    PaymentStatusPurchasing,
    /** 購入処理状況 購入処理完了 */
    PaymentStatusPurchased,
    /** 購入処理状況 失敗 */
    PaymentStatusFailed,
    /** 購入処理状況 リストア(復元)の完了 */
    PaymentStatusRestored,
    /** 購入処理状況 レシート更新中 */
    PaymentStatusReceiptRefleshing,
    /** 購入処理状況 レシート更新完了 */
    PaymentStatusReceiptRefleshed
} PaymentStatus;

/** 購入処理エラー */
typedef enum {
    /** 購入処理エラー プロダクト情報の取得エラー */
    PaymentErrorResponsedProductInfo,
    /** 購入処理エラー クライアントが許可されていない処理を行おうとした */
    PaymentErrorClientInvalid,
    /** 購入処理エラー キャンセルを行った */
    PaymentErrorCancelled,
    /** 購入処理エラー リクエストが不正 */
    PaymentErrorInvalid,
    /** 購入処理エラー アプリ内課金が制限されている */
    PaymentErrorNotAllowed,
    /** 購入処理エラー リストアの失敗 */
    PaymentErrorFailedRestore,
    /** 購入処理エラー 不明 */
    PaymentErrorUnknown
} PaymentError;

/** アプリ内課金管理デリゲート */
@protocol PaymentManagerDelegate <NSObject>

/** 
 プロダクト情報のリクエスト結果通知
 
 @param products 情報取得に成功したプロダクト。SKProduct型
 @param invalidProducts 情報取得に失敗したプロダクト一覧。NSString型
 */
- (void) responseProductInfo: (NSArray *) products InvalidProducts: (NSArray *) invalidProducts;

/**
 購入完了通知
 
 @param transaction 完了した購入処理情報
 */
- (void) completePayment: (SKPaymentTransaction *) transaction;

/**
 購入処理状況通知
 
 @param status 購入処理状況
 */
- (void) onPaymentStatus:(PaymentStatus) status;

/** 
 購入処理エラー通知 
 
 @param error 購入処理エラー
 */
- (void) onPaymentError:(PaymentError) error;

@end




/** 
 アプリ内課金管理クラス
 */
@interface PaymentManager : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver, SKRequestDelegate> {
    /// レシート更新用
    SKReceiptRefreshRequest *receiptRequest;
}

/** 
 アプリ内課金状況通知デリゲート 
 */
@property(nonatomic) id<PaymentManagerDelegate> delegate;

/** 
 インスタンスの共有
 複数のクラスから同一のインスタンスを扱うことをできるようにするためシングルトンにする
 */
+ (PaymentManager *)sharedInstance;

/**
 プロダクト情報の要求
 
 リクエスト結果は、PaymentManagerDelegateの[responseProductInfo:products InvalidProducts:invalidProducts]へ通知される
 
 @param productIds 要求するプロダクトのID一覧
 */
- (SKProductsRequest *)requestProductInfo: (NSSet *) productIds;

/**
 プロダクトの購入
 
 購入が完了すると、PaymentManagerDelegateの[completePayment: transaction]が呼び出される
 
 @param product 購入するプロダクト
 */
- (BOOL) buyProduct: (SKProduct *) product;

/**
 プロダクトの購入
 
 購入が完了すると、PaymentManagerDelegateの[completePayment: transaction]が呼び出される
 
 @param product 購入するプロダクト
 @param quantity 購入量
 */
- (BOOL) buyProduct: (SKProduct *) product WithQuantity: (NSInteger) quantity;

/**
 レシートの確認
 
 @param productId プロダクトのID
 @return プロダクトの有効期限
 */
- (NSTimeInterval) checkReceipt: (NSString *) productId;

/**
 レシートの更新
 
 レシートの更新が開始または完了すると、 PaymentManagerDelegate の [onPaymentStatus:status]が呼び出される
 
 更新開始 : PaymentStatusReceiptRefleshing
 
 更新完了 : PaymentStatusReceiptRefleshed
 */
- (void) refreshReceipt;

/** 
 リストアの開始
 
 購入済みのプロダクトを再提供
 
 リストアが完了すると、 PaymentManagerDelegate の [onPaymentStatus:status]が呼び出される
 
 リストア完了 : PaymentStatusRestored
 */
- (BOOL) startRestore;

@end
