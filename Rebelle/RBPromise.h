//
// This file is part of Rebelle
//  
//  Created by JC on 8/10/13.
// For the full copyright and license information, please view the LICENSE
// file that was distributed with this source code
//

#import <Foundation/Foundation.h>

#import "RBThenable.h"

typedef enum {
   RBPromiseStatePending,
   RBPromiseStateFulfilled,
   RBPromiseStateRejected
} RBPromiseState;

/**
 * Thenable promise
 * - Call "then" if you want to define actions to occur once promise has been resolved
 * - Call ::resolve: to start promise resolving procedure.
 *
 * You can chain calls to then to define multiple promises which must occur after the previous
 * one was realized.
 * \code promise.then(loadUserBlock, nil).then(loadUserLastTweetsBlock, nil) etc. \endcode
 *
 * Promise resolving depend on value type argument:
 * - Any NSExceptions/NSError whill be treated as a promise rejection. Note that NSError objects will be wrapped
 * into a RBErrorException when passed to callback blocks
 * - Any RBPromise will be 1st resolved, then its result will be used as the value argument
 * - Any other value will be considered as a promise fulfillment
 * Once value is determined, it is passed to the fulfill/reject block whose result value is then
 * passed to all promises created by calling "then". If fulfill/reject do not return any value then
 * the one used when calling ::resolve: will be used instead
 *
 * For more information about how a promise work/what it does, check Promises/A+ documentation
 * https://github.com/promises-aplus/promises-spec
 */
@interface RBPromise : NSObject<RBThenable>

/// Setting callback blocks.
/// There are two for this: one using block syntax (more easily chainable) and one
/// using selector syntax (more idiomatic).

/// Set resolve callbacks using block syntax
@property(nonatomic, copy, readonly)RBThenableThen then;
/// Set resolve callbacks using selector syntax
- (RBPromise*) thenOnFulfilled:(RBPromiseFulfilled)fulfilledBlock;
- (RBPromise*) thenOnRejected:(RBPromiseRejected)rejectedBlock;
- (RBPromise*) thenOnFulfilled:(RBPromiseFulfilled)fulfilledBlock onRejected:(RBPromiseRejected)rejectedBlock;

/// the promise state
/// - Pending, resolve has not yet happened or nothing started inside it
/// - Fulfilled, promise was fulfilled, internal resolve process may still need to happen
/// - Rejected, promise was fulfilled, internal resolve process may still need to happen
@property(nonatomic, assign, readonly)RBPromiseState  state;

/// Set to YES if promise is entirely completed, that is state is not longer pending
/// and internal resolve process has been completed
/// Exposed as an attribute so that KVO can be done on it
@property(nonatomic, assign, readonly, getter = isResolved)BOOL   resolved;

- (void)resolve:(id)value;

- (BOOL)isStatePending;

@end

/// Contain all selectors that are considered as protected
/// **MUST** not be used by others
@interface RBPromise (Protected)
- (void)_reject:(NSException *)reason;
- (void)_fulfill:(id)value;
@end
