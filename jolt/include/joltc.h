// Copyright � Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#ifndef _JOLT_C_H
#define _JOLT_C_H

#if defined(JPH_SHARED_LIBRARY_BUILD)
#   if defined(_MSC_VER)
#       define _JPH_EXPORT __declspec(dllexport)
#   elif defined(__GNUC__)
#       define _JPH_EXPORT __attribute__((visibility("default")))
#   else
#       define _JPH_EXPORT
#       pragma warning "Unknown dynamic link import/export semantics."
#   endif
#elif defined(VIMAGE_SHARED_LIBRARY_INCLUDE)
#   if defined(_MSC_VER)
#       define _JPH_EXPORT __declspec(dllimport)
#   else
#       define _JPH_EXPORT
#   endif
#else
#   define _JPH_EXPORT
#endif

#ifdef __cplusplus
#    define _JPH_EXTERN extern "C"
#else
#    define _JPH_EXTERN extern
#endif

#ifdef _WIN32
#   define JPH_API_CALL __cdecl
#else
#   define JPH_API_CALL
#endif


#define JPH_CAPI _JPH_EXTERN 

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint32_t JPH_BodyID;
typedef uint32_t JPH_SubShapeID;
typedef uint16_t JPH_ObjectLayer;
typedef uint8_t  JPH_BroadPhaseLayer;

typedef bool (JPH_API_CALL* JPH_ObjectVsBroadPhaseLayerFilter)(JPH_ObjectLayer inLayer1, JPH_BroadPhaseLayer inLayer2);
typedef bool (JPH_API_CALL* JPH_ObjectLayerPairFilter)(JPH_ObjectLayer inLayer1, JPH_ObjectLayer inLayer2);

typedef enum JPH_MotionType {
    JPH_MOTION_TYPE_STATIC = 0,
    JPH_MOTION_TYPE_KINEMATIC = 1,
    JPH_MOTION_TYPE_DYNAMIC = 2,

    _JPH_MOTION_TYPE__NUM,
    _JPH_MOTION_TYPE_FORCEU32 = 0x7fffffff
} JPH_MotionType;

typedef enum JPH_ActivationMode
{
    JPH_ACTIVATION_MODE_ACTIVATE = 0,
    JPH_ACTIVATION_MODE_DONT_ACTIVATE = 1,

    _JPH_ACTIVATION_MODE_NUM,
    _JPH_ACTIVATION_MODE_FORCEU32 = 0x7fffffff
} JPH_ActivationMode;

typedef enum JPH_ValidateResult {
    JPH_VALIDATE_RESULT_ACCEPT_ALL_CONTACTS = 0,
    JPH_VALIDATE_RESULT_ACCEPT_CONTACT = 1,
    JPH_VALIDATE_RESULT_REJECT_CONTACT = 2,
    JPH_VALIDATE_RESULT_REJECT_ALL_CONTACTS = 3,

    _JPH_VALIDATE_RESULT_NUM,
    _JPH_VALIDATE_RESULT_FORCEU32 = 0x7fffffff
} JPH_ValidateResult;

typedef struct JPH_Vec3 {
    float x;
    float y;
    float z;
} JPH_Vec3;

typedef struct JPH_RVec3 {
#ifdef JPH_DOUBLE_PRECISION
    double x;
    double y;
    double z;
#else
    float x;
    float y;
    float z;
#endif
} JPH_RVec3;

typedef struct JPH_Quat {
    float x;
    float y;
    float z;
    float w;
} JPH_Quat;

typedef struct JPH_Triangle {
    JPH_Vec3 v1;
    JPH_Vec3 v2;
    JPH_Vec3 v3;
    uint32_t materialIndex;
} JPH_Triangle;

typedef struct JPH_IndexedTriangleNoMaterial {
    uint32_t i1;
    uint32_t i2;
    uint32_t i3;
} JPH_IndexedTriangleNoMaterial;

typedef struct JPH_IndexedTriangle {
    uint32_t i1;
    uint32_t i2;
    uint32_t i3;
    uint32_t materialIndex;
} JPH_IndexedTriangle;

typedef struct JPH_SubShapeIDPair
{
    JPH_BodyID     Body1ID;
    JPH_SubShapeID subShapeID1;
    JPH_BodyID     Body2ID;
    JPH_SubShapeID subShapeID2;
} JPH_SubShapeIDPair;

typedef struct JPH_TempAllocator                JPH_TempAllocator;
typedef struct JPH_JobSystemThreadPool          JPH_JobSystemThreadPool;
typedef struct JPH_BroadPhaseLayerInterface     JPH_BroadPhaseLayerInterface;
typedef struct JPH_PhysicsSystem                JPH_PhysicsSystem;

typedef struct JPH_ShapeSettings                JPH_ShapeSettings;
typedef struct JPH_BoxShapeSettings             JPH_BoxShapeSettings;
typedef struct JPH_SphereShapeSettings          JPH_SphereShapeSettings;
typedef struct JPH_TriangleShapeSettings        JPH_TriangleShapeSettings;
typedef struct JPH_CapsuleShapeSettings         JPH_CapsuleShapeSettings;
typedef struct JPH_CylinderShapeSettings        JPH_CylinderShapeSettings;
typedef struct JPH_ConvexHullShapeSettings      JPH_ConvexHullShapeSettings;
typedef struct JPH_MeshShapeSettings            JPH_MeshShapeSettings;
typedef struct JPH_HeightFieldShapeSettings     JPH_HeightFieldShapeSettings;
typedef struct JPH_TaperedCapsuleShapeSettings  JPH_TaperedCapsuleShapeSettings;

typedef struct JPH_Shape                        JPH_Shape;
typedef struct JPH_ConvexShape                  JPH_ConvexShape;
typedef struct JPH_BoxShape                     JPH_BoxShape;
typedef struct JPH_SphereShape                  JPH_SphereShape;

typedef struct JPH_BodyCreationSettings         JPH_BodyCreationSettings;
typedef struct JPH_BodyInterface                JPH_BodyInterface;
typedef struct JPH_Body                         JPH_Body;

typedef struct JPH_CollideShapeResult           JPH_CollideShapeResult;
typedef struct JPH_ContactListener              JPH_ContactListener;

typedef struct JPH_BodyActivationListener       JPH_BodyActivationListener;

JPH_CAPI bool JPH_Init(void);
JPH_CAPI void JPH_Shutdown(void);

/* JPH_TempAllocator */
JPH_CAPI JPH_TempAllocator* JPH_TempAllocatorMalloc_Create();
JPH_CAPI JPH_TempAllocator* JPH_TempAllocator_Create(uint32_t size);
JPH_CAPI void JPH_TempAllocator_Destroy(JPH_TempAllocator* allocator);

/* JPH_JobSystemThreadPool */
JPH_CAPI JPH_JobSystemThreadPool* JPH_JobSystemThreadPool_Create(uint32_t maxJobs, uint32_t maxBarriers, int inNumThreads);
JPH_CAPI void JPH_JobSystemThreadPool_Destroy(JPH_JobSystemThreadPool* system);

/* JPH_ShapeSettings */
JPH_CAPI void JPH_ShapeSettings_Destroy(JPH_ShapeSettings* settings);

/* BoxShape */
JPH_CAPI JPH_BoxShapeSettings* JPH_BoxShapeSettings_Create(const JPH_Vec3* halfExtent, float convexRadius);
JPH_CAPI JPH_BoxShape* JPH_BoxShape_Create(const JPH_Vec3* halfExtent, float convexRadius);
JPH_CAPI void JPH_BoxShape_GetHalfExtent(const JPH_BoxShape* shape, JPH_Vec3* halfExtent);
JPH_CAPI float JPH_BoxShape_GetVolume(const JPH_BoxShape* shape);
JPH_CAPI float JPH_BoxShape_GetConvexRadius(const JPH_BoxShape* shape);

/* SphereShapeSettings */
JPH_CAPI JPH_SphereShapeSettings* JPH_SphereShapeSettings_Create(float radius);
JPH_CAPI float JPH_SphereShapeSettings_GetRadius(const JPH_SphereShapeSettings* settings);
JPH_CAPI void JPH_SphereShapeSettings_SetRadius(JPH_SphereShapeSettings* settings, float radius);
JPH_CAPI JPH_SphereShape* JPH_SphereShape_Create(float radius);
JPH_CAPI float JPH_SphereShape_GetRadius(const JPH_SphereShape* shape);

/* TriangleShape */
JPH_CAPI JPH_TriangleShapeSettings* JPH_TriangleShapeSettings_Create(const JPH_Vec3* v1, const JPH_Vec3* v2, const JPH_Vec3* v3, float convexRadius);

/* CapsuleShape */
JPH_CAPI JPH_CapsuleShapeSettings* JPH_CapsuleShapeSettings_Create(float halfHeightOfCylinder, float radius);

/* CylinderShape */
JPH_CAPI JPH_CylinderShapeSettings* JPH_CylinderShapeSettings_Create(float halfHeight, float radius, float convexRadius);

/* ConvexHullShape */
JPH_CAPI JPH_ConvexHullShapeSettings* JPH_ConvexHullShapeSettings_Create(const JPH_Vec3* points, uint32_t pointsCount, float maxConvexRadius);

/* MeshShape */
JPH_CAPI JPH_MeshShapeSettings* JPH_MeshShapeSettings_Create(const JPH_Triangle* triangles, uint32_t triangleCount);
JPH_CAPI JPH_MeshShapeSettings* JPH_MeshShapeSettings_Create2(const JPH_Vec3* vertices, uint32_t verticesCount, const JPH_IndexedTriangle* triangles, uint32_t triangleCount);
JPH_CAPI void JPH_MeshShapeSettings_Sanitize(JPH_MeshShapeSettings* settings);

/* HeightFieldShape */
JPH_CAPI JPH_HeightFieldShapeSettings* JPH_HeightFieldShapeSettings_Create(const float* samples, const JPH_Vec3* offset, const JPH_Vec3* scale, uint32_t sampleCount);
JPH_CAPI void JPH_MeshShapeSettings_DetermineMinAndMaxSample(const JPH_HeightFieldShapeSettings* settings, float* pOutMinValue, float* pOutMaxValue, float* pOutQuantizationScale);
JPH_CAPI uint32_t JPH_MeshShapeSettings_CalculateBitsPerSampleForError(const JPH_HeightFieldShapeSettings* settings, float maxError);

/* TaperedCapsuleShape */
JPH_CAPI JPH_TaperedCapsuleShapeSettings* JPH_TaperedCapsuleShapeSettings_Create(float halfHeightOfTaperedCylinder, float topRadius, float bottomRadius);

/* Shape */
JPH_CAPI void JPH_Shape_Destroy(JPH_Shape* shape);

/* JPH_BodyCreationSettings */
JPH_CAPI JPH_BodyCreationSettings* JPH_BodyCreationSettings_Create();
JPH_CAPI JPH_BodyCreationSettings* JPH_BodyCreationSettings_Create2(JPH_ShapeSettings* settings,
    const JPH_Vec3* position,
    const JPH_Quat* rotation,
    JPH_MotionType motionType,
    JPH_ObjectLayer objectLayer);
JPH_CAPI JPH_BodyCreationSettings* JPH_BodyCreationSettings_Create3(JPH_Shape* shape,
    const JPH_Vec3* position,
    const JPH_Quat* rotation,
    JPH_MotionType motionType,
    JPH_ObjectLayer objectLayer);
JPH_CAPI void JPH_BodyCreationSettings_Destroy(JPH_BodyCreationSettings* settings);

/* JPH_PhysicsSystem */
JPH_CAPI JPH_PhysicsSystem* JPH_PhysicsSystem_Create(void);
JPH_CAPI void JPH_PhysicsSystem_Destroy(JPH_PhysicsSystem* system);
JPH_CAPI void JPH_PhysicsSystem_Init(JPH_PhysicsSystem* system,
    uint32_t maxBodies, uint32_t numBodyMutexes, uint32_t maxBodyPairs, uint32_t maxContactConstraints,
    JPH_BroadPhaseLayer* layer,
    JPH_ObjectVsBroadPhaseLayerFilter objectVsBroadPhaseLayerFilter,
    JPH_ObjectLayerPairFilter objectLayerPairFilter);

JPH_CAPI void JPH_PhysicsSystem_OptimizeBroadPhase(JPH_PhysicsSystem* system);
JPH_CAPI void JPH_PhysicsSystem_Update(JPH_PhysicsSystem* system, float deltaTime, int collisionSteps, int integrationSubSteps,
    JPH_TempAllocator* tempAlocator,
    JPH_JobSystemThreadPool* jobSystem);

JPH_CAPI JPH_BodyInterface* JPH_PhysicsSystem_GetBodyInterface(JPH_PhysicsSystem* system);
JPH_CAPI void JPH_PhysicsSystem_SetContactListener(JPH_PhysicsSystem* system, JPH_ContactListener* listener);
JPH_CAPI void JPH_PhysicsSystem_SetBodyActivationListener(JPH_PhysicsSystem* system, JPH_BodyActivationListener* listener);

JPH_CAPI uint32_t JPH_PhysicsSystem_GetNumBodies(const JPH_PhysicsSystem* system);
JPH_CAPI uint32_t JPH_PhysicsSystem_GetNumActiveBodies(const JPH_PhysicsSystem* system);
JPH_CAPI uint32_t JPH_PhysicsSystem_GetMaxBodies(const JPH_PhysicsSystem* system);

/* BodyInterface */
JPH_CAPI void JPH_BodyInterface_DestroyBody(JPH_BodyInterface* _interface, JPH_BodyID bodyID);
JPH_CAPI JPH_BodyID JPH_BodyInterface_CreateAndAddBody(JPH_BodyInterface* _interface, JPH_BodyCreationSettings* settings, JPH_ActivationMode activation);
JPH_CAPI JPH_Body* JPH_BodyInterface_CreateBody(JPH_BodyInterface* _interface, JPH_BodyCreationSettings* settings);
JPH_CAPI JPH_Body* JPH_BodyInterface_CreateBodyWithID(JPH_BodyInterface* _interface, JPH_BodyID bodyID, JPH_BodyCreationSettings* settings);
JPH_CAPI JPH_Body* JPH_BodyInterface_CreateBodyWithoutID(JPH_BodyInterface* _interface, JPH_BodyCreationSettings* settings);
JPH_CAPI void JPH_BodyInterface_DestroyBodyWithoutID(JPH_BodyInterface* _interface, JPH_Body* body);
JPH_CAPI bool JPH_BodyInterface_AssignBodyID(JPH_BodyInterface* _interface, JPH_Body* body);
JPH_CAPI bool JPH_BodyInterface_AssignBodyID2(JPH_BodyInterface* _interface, JPH_Body* body, JPH_BodyID bodyID);
JPH_CAPI JPH_Body* JPH_BodyInterface_UnassignBodyID(JPH_BodyInterface* _interface, JPH_BodyID bodyID);

JPH_CAPI void JPH_BodyInterface_AddBody(JPH_BodyInterface* _interface, JPH_BodyID bodyID, JPH_ActivationMode activation);
JPH_CAPI void JPH_BodyInterface_RemoveBody(JPH_BodyInterface* _interface, JPH_BodyID bodyID);
JPH_CAPI bool JPH_BodyInterface_IsActive(JPH_BodyInterface* _interface, JPH_BodyID bodyID);
JPH_CAPI bool JPH_BodyInterface_IsAdded(JPH_BodyInterface* _interface, JPH_BodyID bodyID);

JPH_CAPI void JPH_BodyInterface_SetLinearVelocity(JPH_BodyInterface* _interface, JPH_BodyID bodyID, const JPH_Vec3* velocity);
JPH_CAPI void JPH_BodyInterface_GetLinearVelocity(JPH_BodyInterface* _interface, JPH_BodyID bodyID, JPH_Vec3* velocity);
JPH_CAPI void JPH_BodyInterface_GetCenterOfMassPosition(JPH_BodyInterface* _interface, JPH_BodyID bodyID, JPH_RVec3* position);

JPH_CAPI JPH_MotionType JPH_BodyInterface_GetMotionType(JPH_BodyInterface* _interface, JPH_BodyID bodyID);
JPH_CAPI void JPH_BodyInterface_SetMotionType(JPH_BodyInterface* _interface, JPH_BodyID bodyID, JPH_MotionType motionType, JPH_ActivationMode activationMode);

JPH_CAPI float JPH_BodyInterface_GetRestitution(const JPH_BodyInterface* _interface, JPH_BodyID bodyID);
JPH_CAPI void JPH_BodyInterface_SetRestitution(JPH_BodyInterface* _interface, JPH_BodyID bodyID, float restitution);

JPH_CAPI float JPH_BodyInterface_GetFriction(const JPH_BodyInterface* _interface, JPH_BodyID bodyID);
JPH_CAPI void JPH_BodyInterface_SetFriction(JPH_BodyInterface* _interface, JPH_BodyID bodyID, float friction);

/* Body */
JPH_CAPI JPH_Shape *JPH_Body_GetShape(const JPH_Body* body);
JPH_CAPI JPH_BodyID JPH_Body_GetID(const JPH_Body* body);
JPH_CAPI bool JPH_Body_IsActive(const JPH_Body* body);
JPH_CAPI bool JPH_Body_IsStatic(const JPH_Body* body);
JPH_CAPI bool JPH_Body_IsKinematic(const JPH_Body* body);
JPH_CAPI bool JPH_Body_IsDynamic(const JPH_Body* body);
JPH_CAPI bool JPH_Body_IsSensor(const JPH_Body* body);
JPH_CAPI JPH_MotionType JPH_Body_GetMotionType(const JPH_Body* body);
JPH_CAPI void JPH_Body_SetMotionType(JPH_Body* body, JPH_MotionType motionType);
JPH_CAPI float JPH_Body_GetFriction(const JPH_Body* body);
JPH_CAPI void JPH_Body_SetFriction(JPH_Body* body, float friction);
JPH_CAPI float JPH_Body_GetRestitution(const JPH_Body* body);
JPH_CAPI void JPH_Body_SetRestitution(JPH_Body* body, float restitution);
JPH_CAPI void JPH_Body_GetLinearVelocity(JPH_Body* body, JPH_Vec3* velocity);
JPH_CAPI void JPH_Body_SetLinearVelocity(JPH_Body* body, const JPH_Vec3* velocity);
JPH_CAPI void JPH_Body_GetAngularVelocity(JPH_Body* body, JPH_Vec3* velocity);
JPH_CAPI void JPH_Body_SetAngularVelocity(JPH_Body* body, const JPH_Vec3* velocity);
JPH_CAPI void JPH_Body_AddForce(JPH_Body* body, const JPH_Vec3* force);
JPH_CAPI void JPH_Body_AddForceAtPosition(JPH_Body* body, const JPH_Vec3* force, const JPH_RVec3* position);
JPH_CAPI void JPH_Body_AddTorque(JPH_Body* body, const JPH_Vec3* force);
JPH_CAPI void JPH_Body_GetAccumulatedForce(JPH_Body* body, JPH_Vec3* force);
JPH_CAPI void JPH_Body_GetAccumulatedTorque(JPH_Body* body, JPH_Vec3* force);
JPH_CAPI void JPH_Body_AddImpulse(JPH_Body* body, const JPH_Vec3* impulse);
JPH_CAPI void JPH_Body_AddImpulseAtPosition(JPH_Body* body, const JPH_Vec3* impulse, const JPH_RVec3* position);
JPH_CAPI void JPH_Body_AddAngularImpulse(JPH_Body* body, const JPH_Vec3* angularImpulse);

JPH_CAPI JPH_Vec3 JPH_Body_GetPosition(JPH_Body* body);
JPH_CAPI void JPH_Body_SetPosition(JPH_Body* body, const JPH_Vec3* position);
JPH_CAPI JPH_Quat JPH_Body_GetRotation(JPH_Body* body);
JPH_CAPI void JPH_Body_SetRotation(JPH_Body* body, const JPH_Quat* rotation);
JPH_CAPI uint32_t JPH_Body_GetUserData(JPH_Body* body);
JPH_CAPI void JPH_Body_SetUserData(JPH_Body* body, uint32_t data);
JPH_CAPI JPH_Vec3 JPH_Body_GetCenterOfMassPosition(JPH_Body* body);

/* JPH_BroadPhaseLayer */
typedef struct JPH_BroadPhaseLayerInterface_Procs {
    uint32_t(JPH_API_CALL* GetNumBroadPhaseLayers)(const JPH_BroadPhaseLayerInterface* _interface);
    JPH_BroadPhaseLayer(JPH_API_CALL* GetBroadPhaseLayer)(const JPH_BroadPhaseLayerInterface* _interface, JPH_ObjectLayer layer);

    const char* (*GetBroadPhaseLayerName)(const JPH_BroadPhaseLayerInterface* _interface, JPH_BroadPhaseLayer layer);
} JPH_BroadPhaseLayerInterface_Procs;

JPH_CAPI void JPH_BroadPhaseLayerInterface_SetProcs(JPH_BroadPhaseLayerInterface_Procs procs);
JPH_CAPI JPH_BroadPhaseLayerInterface* JPH_BroadPhaseLayerInterface_Create();
JPH_CAPI void JPH_BroadPhaseLayerInterface_Destroy(JPH_BroadPhaseLayerInterface* layer);

/* Contact listener */
typedef struct JPH_ContactListener_Procs {
    JPH_ValidateResult (JPH_API_CALL *OnContactValidate)(JPH_ContactListener* listener,
        const JPH_Body* body1,
        const JPH_Body* body2,
        const JPH_RVec3* baseOffset,
        const JPH_CollideShapeResult* collisionResult);

    void(JPH_API_CALL* OnContactAdded)(JPH_ContactListener* listener,
        const JPH_Body* body1,
        const JPH_Body* body2);

    void(JPH_API_CALL* OnContactPersisted)(JPH_ContactListener* listener,
        const JPH_Body* body1,
        const JPH_Body* body2);

    void(JPH_API_CALL* OnContactRemoved)(JPH_ContactListener* listener,
        const JPH_SubShapeIDPair* subShapePair
        );
} JPH_ContactListener_Procs;

JPH_CAPI void JPH_ContactListener_SetProcs(JPH_ContactListener_Procs procs);
JPH_CAPI JPH_ContactListener* JPH_ContactListener_Create();
JPH_CAPI void JPH_ContactListener_Destroy(JPH_ContactListener* listener);

/* BodyActivationListener */
typedef struct JPH_BodyActivationListener_Procs {
    void(JPH_API_CALL* OnBodyActivated)(JPH_BodyActivationListener* listener, JPH_BodyID bodyID, uint64_t bodyUserData);
    void(JPH_API_CALL* OnBodyDeactivated)(JPH_BodyActivationListener* listener, JPH_BodyID bodyID, uint64_t bodyUserData);
} JPH_BodyActivationListener_Procs;

JPH_CAPI void JPH_BodyActivationListener_SetProcs(JPH_BodyActivationListener_Procs procs);
JPH_CAPI JPH_BodyActivationListener* JPH_BodyActivationListener_Create();
JPH_CAPI void JPH_BodyActivationListener_Destroy(JPH_BodyActivationListener* listener);

#endif /* _JOLT_C_H */
