#pragma once

#include "core/define/types.h"

enum class VulkanError : u16 {

  // Instance
  instanceCreationFailed,
  instanceLayerUnavailable,
  instanceExtensionUnavailable,

  // Physical Device
  noPhysicalDevice,
  unsupportedGPU,
  missingFeature,

  // Device
  deviceCreationFailed,
  queueCreationFailed,

  // Memory
  deviceMemoryAllocationFailed,
  outOfDeviceMemory,

  // Resource
  bufferCreationFailed,
  imageCreationFailed,
  imageViewCreationFailed,
  samplerCreationFailed,

  // Pipeline
  shaderCompilationFailed,
  pipelineCreationFailed,
  descriptorSetCreationFailed,

  // Swapchain
  swapchainCreationFailed,
  swapchainOutOfDate,
  surfaceLost,

  // Command
  commandPoolCreationFailed,
  commandBufferAllocationFailed,
  commandSubmissionFailed,

  // Sync
  fenceCreationFailed,
  semaphoreCreationFailed,

  // Validation
  validationError,

  unknownVulkanException,
};

enum class MemoryError : u16 {
  // Allocation
  memoryLeak,         // No freeing after allocation
  outOfMemory,        // Out of memory
  heapExhaustion,     // Out of heap space
  stackOverflow,      // Out of stack space
  allocationTooLarge, // Allocated too large size
  integerOverflow,    // Size calculation overflowed
  invalidAlignment,   // Invalid alignment, not with power of two, not among {8,
                      // 16, 32, 64, 256, ...}

  // Corruption
  bufferOverflow,      // Write behind allocator buffer
  bufferUnderflow,     // Write ahead of allocator buffer
  headerCorruption,    // Header covered or corrupted
  allocatorCorruption, // Free list ruined or corrupted
  memoryCorruption,    // Random address miswrotten

  // Lifetime
  useAfterFree,            // Use freed memory
  useUninitializedPointer, // Use uninitialized pointer
  readUninitializedMemory, // Read uninitialized memory chunk
  danglingPointer,         // Pointing to a freed object
  wildPointer,             // Random pointer address

  // Pointer
  nullptrDereference,        // Dereference a nullptr
  misalignedPointer,         // Misaligned pointer
  pointerArithmeticOverflow, // Pointer arithmetic overflowed address space

  // Free
  doubleFree,          // Refreeing after freeing
  invalidFree,         // Freeing memory not allocated by a certain allocator
  wrongAllocator,      // Allocated by allocator A, then freed by allocator B
  sizeMismatch,        // Freed size mismatched the allocated size
  interiorPointerFree, // Free called on an address inside an allocation

  // Virtual memory
  guardPageViolation, // Guard page accessed

  // Other
  invalidReallocate, // Invalid reallocation

  unknownMemoryException,
};

enum class ContainerError : u16 {
  // Bounds
  indexOutOfRange, // Index beyond container size
  emptyContainer,  // Pop/peek/back/front on an empty container
  containerFull,   // Capacity exhausted (queue, ring buffer, fixed array)

  // Capacity / size
  invalidCapacity, // Capacity is 0, or growth would overflow
  sizeOverflow,    // Size/offset arithmetic overflowed
  invalidReserve,  // Reserve smaller than current size or invalid growth

  // Lookup / insertion
  keyNotFound,  // HashMap lookup miss
  duplicateKey, // Unique insert with an existing key
  invalidKey,   // Null/invalid key or hash

  // Iteration
  invalidIterator,     // Dereference past end or dangling iterator
  iteratorInvalidated, // Iterator invalidated by rehash/erase

  // String
  invalidEncoding, // String is not valid UTF-8
  stringTooLong,   // Exceeds SSO/allocated capacity

  unknownContainerException,
};

enum class IOError : u16 {
  // File
  fileNotFound,         // File does not exist
  fileOpenFailed,       // Could not open file
  fileReadFailed,       // Read operation failed
  fileWriteFailed,      // Write operation failed
  filePermissionDenied, // Insufficient access rights
  fileAlreadyExists,    // Create with exclusive semantics on existing file

  // Stream
  endOfStream,       // Read past the end of stream
  streamClosed,      // Operation on a closed stream
  streamNotReadable, // Stream does not support reading
  streamNotWritable, // Stream does not support writing
  streamNotSeekable, // Seek on a non-seekable stream

  // Binary / serializer
  invalidFormat,         // Data layout does not match expected format
  deserializationFailed, // Failed to decode stored data
  serializationFailed,   // Failed to encode data
  checksumMismatch,      // Stored checksum does not match content

  // Console
  consoleReadFailed,  // Failed to read from console
  consoleWriteFailed, // Failed to write to console

  unknownIOException,
};

enum class FileSystemError : u16 {
  // Path
  invalidPath,       // Malformed or empty path
  pathNotFound,      // Path does not exist
  pathAlreadyExists, // Create/rename target already exists
  pathIsFile,        // Expected directory, got file
  pathIsDirectory,   // Expected file, got directory

  // File / directory operations
  permissionDenied,         // Insufficient access rights
  copyFailed,               // Copy operation failed
  moveFailed,               // Rename/move operation failed
  deleteFailed,             // Delete operation failed
  directoryIterationFailed, // Failed to enumerate a directory
  fileNotSupported,         // Operation not supported by this file backend

  // Mounts
  mountFailed,              // Failed to mount a path
  mountPointAlreadyMounted, // Mount point already occupied
  unmountFailed,            // Failed to unmount a path

  // Watcher
  watchFailed,             // Failed to start watching a path
  watchNotificationFailed, // Failed to deliver a change event

  unknownFileSystemException,
};

enum class JobError : u16 {
  // System state
  jobSystemNotInitialized,     // Submit/execute before init
  jobSystemAlreadyInitialized, // Double init
  jobSystemAlreadyShutdown,    // Use after shutdown

  // Queue
  jobQueueFull,        // Enqueue on a full queue
  jobSubmissionFailed, // Failed to push a job
  invalidJobHandle,    // Unknown/cancelled job id
  invalidJobPriority,  // Priority out of supported range

  // Worker
  workerThreadCreationFailed, // Failed to spawn a worker thread
  workerNotRunning,           // Job executed on a stopped worker

  // Synchronization
  deadlockDetected, // Dependency cycle / stuck wait
  jobWaitFailed,    // Failed to wait for job completion

  unknownJobException,
};

enum class InputError : u16 {
  // System state
  inputNotInitialized,     // Query before init
  inputAlreadyInitialized, // Double init
  deviceNotInitialized,    // Specific device (keyboard/mouse) unavailable

  // Key / button
  invalidKeyCode,        // Key code out of supported range
  invalidMouseButton,    // Mouse button out of supported range
  invalidInputAxis,      // Axis id out of supported range
  callbackNotRegistered, // Dispatch to an unregistered callback

  unknownInputException,
};

enum class SceneError : u16 {
  // Scene state
  sceneNotInitialized,     // Use before init
  sceneAlreadyInitialized, // Double init

  // Objects
  objectNotFound,    // Entity id does not exist
  duplicateObject,   // Entity id already in use
  componentNotFound, // Component missing for an object
  invalidParent,     // Attach to a nonexistent/invalid parent

  // Mesh
  meshNotInitialized,   // Use before GPU upload
  invalidMesh,          // Mesh has no vertex/index data
  vertexFormatMismatch, // Vertex layout incompatible with pipeline

  // Material / light / camera
  materialNotFound, // Material handle does not exist
  invalidLight,     // Light type/settings out of range
  invalidCamera,    // Camera has invalid projection parameters

  unknownSceneException,
};

enum class AssetError : u16 {
  // Lookup
  assetNotFound,      // Handle/name does not exist
  assetAlreadyLoaded, // Load requested for an already-loaded asset
  assetNotLoaded,     // Use of an asset still in loading state
  invalidAssetHandle, // Handle out of range or stale

  // Loading / parsing
  assetLoadFailed,      // Async load job failed
  assetParseFailed,     // File content could not be parsed
  unsupportedFormat,    // No importer for this file extension
  importerNotAvailable, // Importer exists but not registered
  assetPathInvalid,     // Missing/empty file path

  // Database / registry
  assetDatabaseNotInitialized, // Registry/dependency graph not wired up
  dependencyCycleDetected,     // Circular dependency between assets
  missingDependency,           // Referenced asset is not registered
  cacheStoreFailed,            // Failed to write the asset cache
  cacheLoadFailed,             // Failed to read the asset cache

  unknownAssetException,
};

enum class RenderError : u16 {
  // System state
  rendererNotInitialized,     // Draw before init
  rendererAlreadyInitialized, // Double init
  rendererNotRunning,         // Render after shutdown

  // Descriptor
  descriptorLayoutMismatch,      // Set layout incompatible with pipeline
  descriptorSetAllocationFailed, // Pool exhausted or allocator error
  uniformBufferUpdateFailed,     // Failed to upload UBO data

  // Pipeline
  pipelineNotCreated,   // Draw with no bound pipeline
  pipelineIncompatible, // Pipeline state incompatible with render pass
  shaderNotCompiled,    // SPIR-V module not compiled/loaded

  // Framebuffer / frame
  framebufferCreationFailed, // Framebuffer incomplete or create failed
  frameResourceExhausted,    // All frames in flight still busy

  // Resource
  resourceNotFound,      // Buffer/texture handle does not exist
  textureLoadFailed,     // Texture decode/upload failed
  textureNotFound,       // Sampler/texture lookup miss
  bufferAccessViolation, // Map/unmap or binding misuse

  unknownRenderException,
};

enum class WindowError : u16 {
  // GLFW
  glfwInitializationFailed, // glfwInit failed
  glfwNotInitialized,       // Use before glfwInit
  windowCreationFailed,     // GLFW window creation failed
  windowNotInitialized,     // Use window before creation
  windowAlreadyInitialized, // Double create
  windowDestroyFailed,      // Failed to destroy the window

  // Context
  contextCreationFailed, // Failed to create the window surface/context
  contextLost,           // Window context lost

  unknownWindowException,
};

enum class PlatformError : u16 {
  // Threading
  threadCreationFailed,    // Failed to spawn a thread
  threadJoinFailed,        // Failed to join a thread
  threadDetachFailed,      // Failed to detach a thread
  mutexCreationFailed,     // Failed to create a mutex
  mutexOperationFailed,    // Lock/unlock/trylock failed
  conditionVariableFailed, // Wait/signal failed
  atomicOperationFailed,   // Compare-exchange or fence failed

  // Memory
  memoryMappingFailed,   // mmap/MapViewOfFile failed
  memoryUnmappingFailed, // munmap/UnmapViewOfFile failed
  pageCommitFailed,      // Failed to commit/reserve virtual pages

  // Socket
  socketCreationFailed, // socket() failed
  socketBindFailed,     // bind() failed
  socketListenFailed,   // listen() failed
  socketConnectFailed,  // connect() failed
  socketAcceptFailed,   // accept() failed
  socketSendFailed,     // send() failed
  socketReceiveFailed,  // recv() failed
  socketTimeout,        // Socket operation timed out
  socketNotConnected,   // Operation on an unconnected socket

  // File / console / time
  fileOperationFailed,    // Platform-level file call failed
  consoleOperationFailed, // Platform console call failed
  timeQueryFailed,        // Failed to query the clock

  unknownPlatformException,
};

enum class EventError : u16 {
  // System state
  eventSystemNotInitialized,     // Dispatch before init
  eventSystemAlreadyInitialized, // Double init

  // Registration
  eventAlreadyRegistered, // Duplicate handler for an event type
  eventNotRegistered,     // No handler for this event type
  invalidEventType,       // Event id out of supported range

  // Queue
  eventQueueFull,      // Event queue capacity exhausted
  eventDispatchFailed, // Handler propagation failed

  unknownEventException,
};

enum class AppError : u16 {
  appInitializationFailed, // init() step failed
  appAlreadyInitialized,   // Double init
  appNotInitialized,       // run/destroy before init
  appRunFailed,            // Main loop failed
  appShutdownFailed,       // destroy() step failed
  appNotRunning,           // Unexpected stop of the main loop

  unknownAppException,
};

enum class UtilsError : u16 {
  invalidArgument,       // Function argument out of contract
  conversionFailed,      // Value could not be converted/parsed
  operationNotSupported, // Feature not implemented for this input
  resultOutOfRange,      // Computed result overflows the output type

  unknownUtilsException,
};
