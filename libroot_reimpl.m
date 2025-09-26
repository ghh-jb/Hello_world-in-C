#import <Foundation/Foundation.h>

char* jbrootpath() {
    NSString* preboot = @"/private/preboot/";
    NSArray* dirs = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:preboot error:NULL];
    for (NSString *sub in dirs) {
        if ([sub length] > 20) {
            NSString* bootUUID = [preboot stringByAppendingString:sub];
            bootUUID = [bootUUID stringByAppendingString:@"/"];

            NSArray* bootUUIDManager = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:bootUUID error:NULL];

            for (NSString *inBoot in bootUUIDManager) {
                if ([inBoot hasPrefix:@"dopamine-"]) {

                    NSString* dopamine = [bootUUID stringByAppendingString:inBoot];
                    NSString* jbroot = [dopamine stringByAppendingString:@"/procursus"];

                    return [jbroot UTF8String];
                }
            }
            break;
        }
    }
    return "";
}

char* getItemInJBROOT(char* item) {
    char* jbroot = jbrootpath();
    strcat(jbroot, item);
    return jbroot;
}
