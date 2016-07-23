#import <Foundation/Foundation.h>

#import "LSSpan.h"
#import "OTTracer.h"

/**
 * An implementation of the OTTracer protocol.
 *
 * Either pass the resulting id<OTTracer> around your application explicitly or use the OTGlobal singleton mechanism.
 *
 * LSTracer is thread-safe.
 *
 * @see OTGlobal
 */
@interface LSTracer : NSObject<OTTracer>

#pragma mark - LSTracer initialization

/**
 * @see `+[LSTracer initWithToken:componentName:hostport]` for parameter details.
 *
 * @return An `LSTracer` instance that's ready to create spans and logs.
 */
- (instancetype) initWithToken:(NSString*)accessToken;

/**
 * @see `+[LSTracer initWithToken:componentName:hostport]` for parameter details.
 *
 * @return An `LSTracer` instance that's ready to create spans and logs.
 */
- (instancetype) initWithToken:(NSString*)accessToken
                 componentName:(NSString*)componentName;

/**
 * Initialize an LSTracer instance. Either pass the resulting LSTracer* around your application explicitly or use the OTGlobal singleton mechanism.
 *
 * @param accessToken the access token.
 * @param componentName the "component name" to associate with spans from this process; e.g., the name of your iOS app or the bundle name.
 * @param hostport the collector's host and (TLS) port as a single string (e.g.  @"collector.lightstep.com:443").
 *
 * @return An `LSTracer` instance that's ready to create spans and logs.
 *
 * @see OTGlobal
 */
- (instancetype) initWithToken:(NSString*)accessToken
                 componentName:(NSString*)componentName
                      hostport:(NSString*)hostport;

#pragma mark - OpenTracing API

- (id<OTSpan>)startSpan:(NSString*)operationName;

- (id<OTSpan>)startSpan:(NSString*)operationName
		   tags:(NSDictionary*)tags;

- (id<OTSpan>)startSpan:(NSString*)operationName
		childOf:(id<OTSpanContext>)parentSpan;

- (id<OTSpan>)startSpan:(NSString*)operationName
		childOf:(id<OTSpanContext>)parentSpan
		   tags:(NSDictionary*)tags;

- (id<OTSpan>)startSpan:(NSString*)operationName
		childOf:(id<OTSpanContext>)parentSpan
		   tags:(NSDictionary*)tags
	      startTime:(NSDate*)startTime;

- (id<OTSpan>)startSpan:(NSString*)operationName
	     references:(NSArray*)references
		   tags:(NSDictionary*)tags
	      startTime:(NSDate*)startTime;

- (bool)inject:(id<OTSpanContext>)span format:(NSString*)format carrier:(id)carrier;
- (bool)inject:(id<OTSpanContext>)span format:(NSString*)format carrier:(id)carrier error:(NSError* __autoreleasing *)outError;

- (id<OTSpanContext>)extract:(NSString*)format carrier:(id)carrier;
- (id<OTSpanContext>)extract:(NSString*)format carrier:(id)carrier error:(NSError* __autoreleasing *)outError;

#pragma mark - LightStep extensions and internal methods

/**
 * The remote service URL string (as derived from `sharedInstancWithServiceHostport:token:`).
 */
@property (nonatomic, readonly) NSString* serviceUrl;

/**
 * The `LSTracer` instance's globally unique id ("guid"), which is both immutable and assigned automatically by LightStep.
 */
@property (nonatomic, readonly) NSString* runtimeGuid;


/**
 * The `LSTracer` instance's maximum number of records to buffer between reports.
 */
@property (atomic) NSUInteger maxLogRecords;

/**
 * The `LSTracer` instance's maximum number of records to buffer between reports.
 */
@property (atomic) NSUInteger maxSpanRecords;

/**
 * Maximum string length of any single JSON payload.
 */
@property (atomic) NSUInteger maxPayloadJSONLength;

/**
 * Approximate interval to use for reporting buffered data to the collector.
 */
@property (atomic) NSUInteger flushIntervalSeconds;

/**
 * Returns true if the library is currently buffering and reporting data.
 */
- (bool) enabled;

/**
 * Returns the Tracer's access token.
 */
- (NSString*) accessToken;

/**
 * Flush any buffered data to the collector.
 */
- (void)flush;

@end