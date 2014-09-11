
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  iOS User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

#import "ReginaDocument.h"
#import "TSMessage.h"
#import "packet/ncontainer.h"
#import "packet/ntext.h"
#import "snappea/nsnappeatriangulation.h"
#import <boost/iostreams/device/array.hpp>
#import <boost/iostreams/stream.hpp>
#import <sstream>

// TODO: Perhaps override the hooks disableEditing / enableEditing, to make the UI read-only?

static NSURL* docsDir_ = nil;

@interface ReginaDocument () {
    enum DocType {
        /**
         * Indicates a native Regina data file, residing in the usual documents directory.
         */
        DOC_NATIVE,
        /**
         * Indicates a native Regina data file, residing in a read-only location.  Such a
         * file might be an example file, or census data.
         * When modified, a file of this type must be copied into the documents directory
         * before it can be saved.
         */
        DOC_READONLY,
        /**
         * Indicates a file in a foreign data format.
         * When modified, a file of this type must be saved in the documents directory under
         * a different name, using Regina's native file format.
         */
        DOC_FOREIGN
    };
    
    NSString* description;
    DocType type;
}
@end

@implementation ReginaDocument

- (id)initWithExample:(Example *)e
{
    NSString* path = [[NSBundle mainBundle] pathForResource:e.file ofType:nil inDirectory:@"examples"];
    if (! path) {
        NSLog(@"Could not find example resource: %@", e.file);
        return nil;
    }
    
    self = [super initWithFileURL:[NSURL fileURLWithPath:path]];
    if (self) {
        description = e.desc;
        _tree = 0;
        type = DOC_READONLY;
    }
    return self;
}

+ (id)documentWithExample:(Example *)e
{
    return [[ReginaDocument alloc] initWithExample:e];
}

- (id)initWithInboxURL:(NSURL *)u preferredName:(NSURL *)name
{
    if (! [u isFileURL]) {
        NSLog(@"Inbox URL is not a file URL: %@", u);
        return nil;
    }

    // Move from the inbox into the documents folder.
    NSLog(@"Moving file out of inbox: %@", u);
    BOOL moveOk = NO;
    NSURL* docURL = [ReginaDocument uniqueDocURLFor:(name ? name : u)];
    if (docURL)
        moveOk = [[NSFileManager defaultManager] moveItemAtURL:u toURL:docURL error:nil];
    if (moveOk) {
        NSLog(@"New URL: %@", docURL);
        u = docURL;
    } else
        NSLog(@"Could not move to new URL: %@", docURL);

    self = [super initWithFileURL:u];
    if (self) {
        description = nil;
        _tree = 0;
        type = (moveOk ? DOC_NATIVE : DOC_READONLY);
    }
    return self;
}

+ (id)documentWithInboxURL:(NSURL *)u preferredName:(NSURL *)name
{
    return [[ReginaDocument alloc] initWithInboxURL:u preferredName:name];
}


- (id)initWithURL:(NSURL *)u
{
    if (! [u isFileURL]) {
        NSLog(@"Attempt to create a non-file document URL: %@", u);
        return nil;
    }

    self = [super initWithFileURL:u];
    if (self) {
        description = nil;
        _tree = 0;
        type = DOC_NATIVE;
    }
    return self;
}

+ (id)documentWithURL:(NSURL *)u
{
    return [[ReginaDocument alloc] initWithURL:u];
}

- (id)initNewWithCompletionHandler:(void (^)(ReginaDocument*))completionHandler
{
    NSURL* url = [ReginaDocument uniqueDocURLFor:[NSURL URLWithString:@"Untitled.rga" relativeToURL:[ReginaDocument docsDir]]];
    if (! url) {
        completionHandler(nil);
        return nil;
    }
    
    self = [super initWithFileURL:url];
    if (self) {
        _tree = new regina::NContainer();
        regina::NText* text = new regina::NText("TODO: A welcome.");
        text->setPacketLabel("Read me");
        _tree->insertChildLast(text);
        type = DOC_NATIVE;
        
        [self saveToURL:url forSaveOperation:UIDocumentSaveForCreating completionHandler:^(BOOL success) {
            if (success)
                completionHandler(self);
            else
                completionHandler(nil);
        }];
        return self;
    } else {
        completionHandler(nil);
        return nil;
    }
}

+ (id)documentNewWithCompletionHandler:(void (^)(ReginaDocument*))completionHandler
{
    return [[ReginaDocument alloc] initNewWithCompletionHandler:completionHandler];
}

- (void)dealloc {
    if (_tree) {
        delete _tree;
        NSLog(@"Deleted packet tree.");
    }
}

- (NSString *)localizedName {
    if (description)
        return description;
    else
        return [super localizedName];
}

- (BOOL)loadFromContents:(id)contents ofType:(NSString *)typeName error:(NSError *__autoreleasing *)outError
{
    if (! [contents isKindOfClass:[NSData class]]) {
        NSLog(@"File data is not of type NSData.  Perhaps you tried to open a directory?");
        return NO;
    }
    NSData* data = static_cast<NSData*>(contents);
    
    // We decide on the file type not based on typeName, but by actually examing the file contents.
    //
    // Note: Regina data files should start with either:
    // "\x1f\x8b", for compressed XML, or
    // "<?xml", for uncompressed XML.
    
    char prefix[6];
    [data getBytes:prefix length:5];
    if (strncmp(prefix, "% Tri", 5) == 0) {
        // Looks like it belongs to SnapPea/SnapPy.
        std::string str(static_cast<const char*>(data.bytes), data.length);
        regina::NSnapPeaTriangulation* tri = new regina::NSnapPeaTriangulation(str);
        if ((! tri) || tri->isNull()) {
            delete tri;
            return NO;
        }
        _tree = new regina::NContainer();
        _tree->setPacketLabel("SnapPea import");
        _tree->insertChildLast(tri);
        
        type = DOC_FOREIGN;
    } else {
        boost::iostreams::stream<boost::iostreams::array_source> s(static_cast<const char*>(data.bytes), data.length);
        _tree = regina::open(s);
    }
    
    if (_tree)
        return YES;
    else {
        // TODO: outError = [NSError errorWithDomain:<#(NSString *)#> code:<#(NSInteger)#> userInfo:<#(NSDictionary *)#>]
        return NO;
    }
}

- (id)contentsForType:(NSString *)typeName error:(NSError *__autoreleasing *)outError
{
    if (! _tree) {
        // TODO: outError.
        return nil;
    }
    
    NSLog(@"Preparing to save: %@", self.fileURL);
    
    std::ostringstream s;
    if (_tree->save(s)) {
        const std::string& str = s.str();
        return [NSData dataWithBytes:str.c_str() length:str.length()];
    } else {
        // TODO: outError.
        return nil;
    }
}

- (void)setDirty
{
    if (type == DOC_READONLY) {
        NSLog(@"Copying to documents directory.");
        NSURL* newURL = [ReginaDocument uniqueDocURLFor:self.fileURL];
        if ([[NSFileManager defaultManager] copyItemAtURL:self.fileURL toURL:newURL error:nil]) {
            [self presentedItemDidMoveToURL:newURL];
            [TSMessage showNotificationWithTitle:@"Copied to documents folder:"
                                        subtitle:newURL.lastPathComponent
                                            type:TSMessageNotificationTypeMessage];

            description = nil;
            type = DOC_NATIVE;
            
            [self updateChangeCount:UIDocumentChangeDone];
        } else {
            UIAlertView* alert = [[UIAlertView alloc]
                                  initWithTitle:@"Changes will be lost"
                                  message:@"I was unable to copy this example into your documents folder.  This means that any changes you make here will be lost."
                                  delegate:nil
                                  cancelButtonTitle:@"Close"
                                  otherButtonTitles:nil];
            [alert show];
        }
    } else if (type == DOC_FOREIGN) {
        NSLog(@"Converting to Regina's native format.");
        NSURL* newURL = [ReginaDocument uniqueRgaURLFor:self.fileURL];
        [self presentedItemDidMoveToURL:newURL];
        [self saveToURL:newURL forSaveOperation:UIDocumentSaveForCreating completionHandler:^(BOOL success) {
            if (success) {
                [TSMessage showNotificationWithTitle:@"Converted to Regina document:"
                                            subtitle:newURL.lastPathComponent
                                                type:TSMessageNotificationTypeMessage];
            } else {
                UIAlertView* alert = [[UIAlertView alloc]
                                      initWithTitle:@"Changes will be lost"
                                      message:@"I was unable to save this document in Regina's native file format.  This means that any changes you make here will be lost."
                                      delegate:nil
                                      cancelButtonTitle:@"Close"
                                      otherButtonTitles:nil];
                [alert show];
            }
        }];
        
        description = nil;
        type = DOC_NATIVE;
    } else {
        [self updateChangeCount:UIDocumentChangeDone];
    }
}

+ (NSURL *)docsDir
{
    if (! docsDir_)
        docsDir_ = [[NSFileManager defaultManager] URLForDirectory:NSDocumentDirectory inDomain:NSUserDomainMask appropriateForURL:nil create:YES error:nil];
    return docsDir_;
}

+ (NSURL *)uniqueDocURLFor:(NSURL *)url
{
    // Note: [NSURL URLByAppendingPathComponent] handles all necessary character escaping for us.
    NSString* filename = [url lastPathComponent];

    NSFileManager* f = [NSFileManager defaultManager];
    NSURL* dir = [ReginaDocument docsDir];

    url = [dir URLByAppendingPathComponent:filename];
    if (! [f fileExistsAtPath:url.path])
        return url;
    
    NSString* basename = [filename stringByDeletingPathExtension];
    NSString* extension = [url pathExtension];
    
    int i = 1;
    while (true) {
        filename = [NSString stringWithFormat:@"%@ %d.%@", basename, i, extension];
        url = [dir URLByAppendingPathComponent:filename];
        if (! [f fileExistsAtPath:url.path])
            return url;
        
        ++i;
    }
}

+ (NSURL *)uniqueRgaURLFor:(NSURL *)url
{
    // Note: [NSURL URLByAppendingPathComponent] handles all necessary character escaping for us.
    NSString* basename = [[url lastPathComponent] stringByDeletingPathExtension];
    NSString* filename = [basename stringByAppendingString:@".rga"];
    
    NSFileManager* f = [NSFileManager defaultManager];
    NSURL* dir = [ReginaDocument docsDir];
    
    url = [dir URLByAppendingPathComponent:filename];
    if (! [f fileExistsAtPath:url.path])
        return url;
    
    int i = 1;
    while (true) {
        filename = [NSString stringWithFormat:@"%@ %d.rga", basename, i];
        url = [dir URLByAppendingPathComponent:filename];
        if (! [f fileExistsAtPath:url.path])
            return url;
        
        ++i;
    }
}

@end
