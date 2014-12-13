#import <UIKit/UIKit.h>

@interface BannerViewController : UIViewController

- (instancetype)initWithContentViewController:(UIViewController *)contentController;
- (void) hideBanner;
- (void) showBanner;

@end
