package com.soomla.cocos2dx.store;

import android.opengl.GLSurfaceView;

import com.soomla.Soomla;
import com.soomla.SoomlaUtils;
import com.soomla.cocos2dx.common.*;
import com.soomla.rewards.VirtualItemReward;
import com.soomla.store.IStoreAssets;
import com.soomla.store.SoomlaStore;
import com.soomla.store.StoreInventory;
import com.soomla.store.data.StoreInfo;
import com.soomla.store.data.StorageManager;
import com.soomla.store.domain.*;
import com.soomla.store.domain.virtualCurrencies.VirtualCurrency;
import com.soomla.store.domain.virtualCurrencies.VirtualCurrencyPack;
import com.soomla.store.domain.virtualGoods.*;
import com.soomla.store.exceptions.InsufficientFundsException;
import com.soomla.store.exceptions.NotEnoughGoodsException;
import com.soomla.store.exceptions.VirtualItemNotFoundException;
import com.soomla.store.purchaseTypes.PurchaseWithMarket;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.lang.ref.WeakReference;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

public class StoreService extends AbstractSoomlaService {

    private static StoreService INSTANCE = null;

    private static IStoreAssets mStoreAssets   = null;
    private boolean inited = false;

    public static StoreService getInstance() {
        if (INSTANCE == null) {
            synchronized (StoreService.class) {
                if (INSTANCE == null) {
                    INSTANCE = new StoreService();
                }
            }
        }
        return INSTANCE;
    }

    @SuppressWarnings("FieldCanBeLocal")
    private StoreEventHandlerBridge storeEventHandlerBridge;

    public StoreService() {
        storeEventHandlerBridge = new StoreEventHandlerBridge();

        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_MARKET_ITEM, MarketItem.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_VIRTUAL_CATEGORY, VirtualCategory.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_VIRTUAL_CURRENCY, VirtualCurrency.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_VIRTUAL_CURRENCY_PACK, VirtualCurrencyPack.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_EQUIPPABLE_VG, EquippableVG.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_LIFETIME_VG, LifetimeVG.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_SINGLE_USE_PACK_VG, SingleUsePackVG.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_SINGLE_USE_VG, SingleUseVG.class);
        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_UPGRADE_VG, UpgradeVG.class);

        DomainHelper.getInstance().registerTypeWithClassName(StoreConsts.JSON_JSON_TYPE_ITEM, VirtualItemReward.class);

        final NdkGlue ndkGlue = NdkGlue.getInstance();

        ndkGlue.registerCallHandler("CCStoreAssets::init", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                init();
                int version = params.getInt("version");
                JSONObject storeAssetsJson = params.getJSONObject("storeAssets");
                StoreInfo.setStoreAssets(version, storeAssetsJson.toString());
            }
        });

        ndkGlue.registerCallHandler("CCStoreService::init", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                // Compatibility
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::buyMarketItem", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String productId = params.getString("productId");
                String payload = params.getString("payload");
                SoomlaUtils.LogDebug("SOOMLA", "buyWithMarket is called from java with productId: " + productId + "!");
                PurchasableVirtualItem pvi = StoreInfo.getPurchasableItem(productId);
                if(pvi.getPurchaseType() instanceof PurchaseWithMarket) {
                    SoomlaStore.getInstance().buyWithMarket(((PurchaseWithMarket)pvi.getPurchaseType()).getMarketItem(), payload);
                } else {
                    throw new VirtualItemNotFoundException("productId", productId);
                }
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::startIabServiceInBg", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                SoomlaUtils.LogDebug("SOOMLA", "startIabServiceInBg is called from java!");
                SoomlaStore.getInstance().startIabServiceInBg();
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::stopIabServiceInBg", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                SoomlaUtils.LogDebug("SOOMLA", "stopIabServiceInBg is called from java!");
                SoomlaStore.getInstance().stopIabServiceInBg();
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::restoreTransactions", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                SoomlaUtils.LogDebug("SOOMLA", "restoreTransactions is called from java!");
                SoomlaStore.getInstance().restoreTransactions();
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::refreshMarketItemsDetails", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                SoomlaUtils.LogDebug("SOOMLA", "refreshMarketItemsDetails is called from java!");
                SoomlaStore.getInstance().refreshMarketItemsDetails();
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::transactionsAlreadyRestored", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                throw new UnsupportedOperationException("transactionsAlreadyRestored has no use in Android");
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::refreshInventory", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                SoomlaUtils.LogDebug("SOOMLA", "refreshInventory is called from java!");
                SoomlaStore.getInstance().refreshInventory();
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::setAndroidPublicKey", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                try {
                    String publicKey = params.getString("androidPublicKey");

                    Class googlePlayClass = Class.forName("com.soomla.store.billing.google.GooglePlayIabService");
                    Method factoryMethod = googlePlayClass.getDeclaredMethod("getInstance");
                    Object singleton = factoryMethod.invoke(null, null);
                    Method setPKMethod = googlePlayClass.getDeclaredMethod("setPublicKey", String.class);
                    setPKMethod.invoke(singleton, publicKey);
                } catch (Exception e) {
                    SoomlaUtils.LogError("StoreService JNI", "Something happened while we were trying to run CCSoomlaStore::setAndroidPublicKey. error: " + e.getLocalizedMessage());
                    e.printStackTrace();
                }

            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::setTestPurchases", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                try {
                    boolean testPurchases = params.getBoolean("testPurchases");
                    Class googlePlayClass = Class.forName("com.soomla.store.billing.google.GooglePlayIabService");
                    Field testPurchasesField = googlePlayClass.getDeclaredField("AllowAndroidTestPurchases");
                    testPurchasesField.set(null, testPurchases);
                } catch (Exception e) {
                    SoomlaUtils.LogError("StoreService JNI", "Something happened while we were trying to run CCSoomlaStore::setTestPurchases. error: " + e.getLocalizedMessage());
                    e.printStackTrace();
                }
            }
        });

        ndkGlue.registerCallHandler("CCSoomlaStore::loadBillingService", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                SoomlaUtils.LogDebug("SOOMLA", "refreshInventory is called from java!");
                SoomlaStore.getInstance().loadBillingService();
            }
        });

        ndkGlue.registerCallHandler("CCStoreInfo::loadFromDB", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                StoreInfo.loadFromDB();
            }
        });

        ndkGlue.registerCallHandler("CCNativeVirtualCurrencyStorage::getBalance", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                int outBalance = StorageManager.getVirtualCurrencyStorage().getBalance(itemId);
                retParams.put("return", outBalance);
            }
        });

        ndkGlue.registerCallHandler("CCNativeVirtualCurrencyStorage::setBalance", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                int balance = params.getInt("balance");
                boolean notify = params.getBoolean("notify");
                int outBalance = StorageManager.getVirtualCurrencyStorage().setBalance(itemId, balance, notify);
                retParams.put("return", outBalance);
            }
        });

        ndkGlue.registerCallHandler("CCNativeVirtualCurrencyStorage::add", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                int amount = params.getInt("amount");
                boolean notify = params.getBoolean("notify");
                int outBalance = StorageManager.getVirtualCurrencyStorage().add(itemId, amount, notify);
                retParams.put("return", outBalance);
            }
        });

        ndkGlue.registerCallHandler("CCNativeVirtualCurrencyStorage::remove", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                int amount = params.getInt("amount");
                boolean notify = params.getBoolean("notify");
                int outBalance = StorageManager.getVirtualCurrencyStorage().remove(itemId, amount, notify);
                retParams.put("return", outBalance);
            }
        });

        ndkGlue.registerCallHandler("CCNativeVirtualGoodsStorage::getBalance", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                int outBalance = StorageManager.getVirtualGoodsStorage().getBalance(itemId);
                retParams.put("return", outBalance);
            }
        });

        ndkGlue.registerCallHandler("CCNativeVirtualGoodsStorage::setBalance", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                int balance = params.getInt("balance");
                boolean notify = params.getBoolean("notify");
                int outBalance = StorageManager.getVirtualGoodsStorage().setBalance(itemId, balance, notify);
                retParams.put("return", outBalance);
            }
        });

        ndkGlue.registerCallHandler("CCNativeVirtualGoodsStorage::add", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                int amount = params.getInt("amount");
                boolean notify = params.getBoolean("notify");
                int outBalance = StorageManager.getVirtualGoodsStorage().add(itemId, amount, notify);
                retParams.put("return", outBalance);
            }
        });

        ndkGlue.registerCallHandler("CCNativeVirtualGoodsStorage::remove", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                int amount = params.getInt("amount");
                boolean notify = params.getBoolean("notify");
                int outBalance = StorageManager.getVirtualGoodsStorage().remove(itemId, amount, notify);
                retParams.put("return", outBalance);
            }
        });

        ndkGlue.registerCallHandler("CCNativeVirtualGoodsStorage::removeUpgrades", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                boolean notify = params.getBoolean("notify");
                StorageManager.getVirtualGoodsStorage().removeUpgrades(itemId, notify);
            }
        });

        ndkGlue.registerCallHandler("CCNativeVirtualGoodsStorage::assignCurrentUpgrade", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                String upgradeItemId = params.getString("upgradeItemId");
                boolean notify = params.getBoolean("notify");
                StorageManager.getVirtualGoodsStorage().assignCurrentUpgrade(itemId, upgradeItemId, notify);
            }
        });

        ndkGlue.registerCallHandler("CCNativeVirtualGoodsStorage::getCurrentUpgrade", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                String upgradeItemId = StorageManager.getVirtualGoodsStorage().getCurrentUpgrade(itemId);
                retParams.put("return", upgradeItemId);
            }
        });

        ndkGlue.registerCallHandler("CCNativeVirtualGoodsStorage::isEquipped", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                boolean res = StorageManager.getVirtualGoodsStorage().isEquipped(itemId);
                retParams.put("return", res);
            }
        });

        ndkGlue.registerCallHandler("CCNativeVirtualGoodsStorage::equip", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                boolean notify = params.getBoolean("notify");
                StorageManager.getVirtualGoodsStorage().equip(itemId, notify);
            }
        });

        ndkGlue.registerCallHandler("CCNativeVirtualGoodsStorage::unequip", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                boolean notify = params.getBoolean("notify");
                StorageManager.getVirtualGoodsStorage().unequip(itemId, notify);
            }
        });

        ndkGlue.registerCallHandler("CCStoreEventDispatcher::pushOnItemPurchased", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                String payload = params.getString("payload");
                storeEventHandlerBridge.pushOnItemPurchased(itemId, payload);
            }
        });

        ndkGlue.registerCallHandler("CCStoreEventDispatcher::pushOnItemPurchaseStarted", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String itemId = params.getString("itemId");
                storeEventHandlerBridge.pushOnItemPurchaseStarted(itemId);
            }
        });

        ndkGlue.registerCallHandler("CCStoreEventDispatcher::pushOnUnexpectedErrorInStore", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                String errorMessage = params.getString("errorMessage");
                storeEventHandlerBridge.pushOnUnexpectedErrorInStore(errorMessage);
            }
        });

        ndkGlue.registerCallHandler("CCStoreEventDispatcher::pushOnSoomlaStoreInitialized", new NdkGlue.CallHandler() {
            @Override
            public void handle(JSONObject params, JSONObject retParams) throws Exception {
                storeEventHandlerBridge.pushOnSoomlaStoreInitialized();
            }
        });


        final NdkGlue.ExceptionHandler exceptionHandler = new NdkGlue.ExceptionHandler() {
            @Override
            public void handle(Exception exception, JSONObject params, JSONObject retParams) throws Exception {
                retParams.put("errorInfo", exception.getClass().getName());
            }
        };

        ndkGlue.registerExceptionHandler(VirtualItemNotFoundException.class.getName(), exceptionHandler);
        ndkGlue.registerExceptionHandler(InsufficientFundsException.class.getName(), exceptionHandler);
        ndkGlue.registerExceptionHandler(NotEnoughGoodsException.class.getName(), exceptionHandler);
    }

    public void init() {
        final GLSurfaceView glSurfaceView = glSurfaceViewRef.get();
        if (glSurfaceView != null) {
            storeEventHandlerBridge.setGlSurfaceView(glSurfaceView);
        }

        inited = true;
    }

    public void setGlSurfaceView(GLSurfaceView glSurfaceView) {
        if (inited) {
            storeEventHandlerBridge.setGlSurfaceView(glSurfaceView);
        } else {
            glSurfaceViewRef = new WeakReference<GLSurfaceView>(glSurfaceView);
        }
    }
}
