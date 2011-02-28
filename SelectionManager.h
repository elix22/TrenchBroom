//
//  SelectionManager.h
//  TrenchBroom
//
//  Created by Kristian Duske on 30.01.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

extern NSString* const SelectionAdded;
extern NSString* const SelectionRemoved;

extern NSString* const SelectionEntities;
extern NSString* const SelectionBrushes;
extern NSString* const SelectionFaces;

typedef enum {
    SM_UNDEFINED,
    SM_FACES,
    SM_GEOMETRY
} ESelectionMode;

@protocol Face;
@protocol Brush;
@protocol Entity;

@interface SelectionManager : NSObject {
    @private
    NSMutableSet* faces;
    NSMutableSet* brushes;
    NSMutableSet* entities;
    ESelectionMode mode;
}

- (void)addFace:(id <Face>)face;
- (void)addFaces:(NSSet *)theFaces;
- (void)addBrush:(id <Brush>)brush;
- (void)addBrushes:(NSSet *)theBrushes;
- (void)addEntity:(id <Entity>)entity;
- (void)addEntities:(NSSet *)theEntities;

- (ESelectionMode)mode;
- (BOOL)isFaceSelected:(id <Face>)face;
- (BOOL)isBrushSelected:(id <Brush>)brush;
- (BOOL)isEntitySelected:(id <Entity>)entity;
- (BOOL)hasSelectedFaces:(id <Brush>)brush;

- (NSSet *)selectedEntities;
- (NSSet *)selectedBrushes;
- (NSSet *)selectedFaces;
- (NSSet *)selectedBrushFaces;

- (BOOL)hasSelection;
- (BOOL)hasSelectedEntities;
- (BOOL)hasSelectedBrushes;
- (BOOL)hasSelectedFaces;

- (void)removeFace:(id <Face>)face;
- (void)removeBrush:(id <Brush>)brush;
- (void)removeEntity:(id <Entity>)entity;

- (void)removeAll;

@end
