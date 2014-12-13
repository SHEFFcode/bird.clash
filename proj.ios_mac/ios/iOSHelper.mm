//
//  iOSHelper.mm
//  DodgeBullets
//
//  Created by Rays on 7/05/13.
//
//
#include "iOSHelper.h"
#include "AppController.h"

void iOSHelper::hideAdmobBanner(){
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
    [appDelegate hideAdmobBanner];
}

void iOSHelper::showAdmobBanner(){
    AppController *appDelegate = (AppController *)[[UIApplication sharedApplication] delegate];
    [appDelegate showAdmobBanner];
}