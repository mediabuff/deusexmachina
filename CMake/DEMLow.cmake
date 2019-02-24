set(DEM_L1_LOW_HEADERS
	DEM/Low/src/StdCfg.h
	DEM/Low/src/StdDEM.h
	DEM/Low/src/Animation/AnimClip.h
	DEM/Low/src/Animation/AnimFwd.h
	DEM/Low/src/Animation/AnimTask.h
	DEM/Low/src/Animation/EventTrack.h
	DEM/Low/src/Animation/KeyframeClip.h
	DEM/Low/src/Animation/KeyframeClipLoaderKFA.h
	DEM/Low/src/Animation/KeyframeTrack.h
	DEM/Low/src/Animation/MocapClip.h
	DEM/Low/src/Animation/MocapClipLoaderNAX2.h
	DEM/Low/src/Animation/MocapTrack.h
	DEM/Low/src/Animation/NodeControllerKeyframe.h
	DEM/Low/src/Animation/NodeControllerMocap.h
	DEM/Low/src/Core/Application.h
	DEM/Low/src/Core/ApplicationState.h
	DEM/Low/src/Core/CoreServer.h
	DEM/Low/src/Core/Factory.h
	DEM/Low/src/Core/Object.h
	DEM/Low/src/Core/RTTI.h
	DEM/Low/src/Core/RTTIBaseClass.h
	DEM/Low/src/Core/TimeSource.h
	DEM/Low/src/Data/Array.h
	DEM/Low/src/Data/Array2.h
	DEM/Low/src/Data/ArrayUtils.h
	DEM/Low/src/Data/Buffer.h
	DEM/Low/src/Data/Data.h
	DEM/Low/src/Data/DataArray.h
	DEM/Low/src/Data/DataScheme.h
	DEM/Low/src/Data/Dictionary.h
	DEM/Low/src/Data/DynamicEnum.h
	DEM/Low/src/Data/FixedArray.h
	DEM/Low/src/Data/Flags.h
	DEM/Low/src/Data/FourCC.h
	DEM/Low/src/Data/HandleManager.h
	DEM/Low/src/Data/Hash.h
	DEM/Low/src/Data/HashPairT.h
	DEM/Low/src/Data/HashTable.h
	DEM/Low/src/Data/HRDParser.h
	DEM/Low/src/Data/KeyList.h
	DEM/Low/src/Data/LineBuffer.h
	DEM/Low/src/Data/List.h
	DEM/Low/src/Data/MurmurHash3.h
	DEM/Low/src/Data/PairT.h
	DEM/Low/src/Data/Param.h
	DEM/Low/src/Data/Params.h
	DEM/Low/src/Data/ParamsUtils.h
	DEM/Low/src/Data/Ptr.h
	DEM/Low/src/Data/QuadTree.h
	DEM/Low/src/Data/RAMData.h
	DEM/Low/src/Data/RefCounted.h
	DEM/Low/src/Data/Regions.h
	DEM/Low/src/Data/RingBuffer.h
	DEM/Low/src/Data/Singleton.h
	DEM/Low/src/Data/Stack.h
	DEM/Low/src/Data/String.h
	DEM/Low/src/Data/StringID.h
	DEM/Low/src/Data/StringIDStorage.h
	DEM/Low/src/Data/StringTokenizer.h
	DEM/Low/src/Data/StringUtils.h
	DEM/Low/src/Data/SuperFastHash.h
	DEM/Low/src/Data/TableT.h
	DEM/Low/src/Data/Type.h
	DEM/Low/src/Data/XMLDocument.h
	DEM/Low/src/Debug/DebugDraw.h
	DEM/Low/src/Debug/DebugServer.h
	DEM/Low/src/Debug/LuaConsole.h
	DEM/Low/src/Debug/Profiler.h
	DEM/Low/src/Debug/WatcherWindow.h
	DEM/Low/src/Events/Event.h
	DEM/Low/src/Events/EventBase.h
	DEM/Low/src/Events/EventDispatcher.h
	DEM/Low/src/Events/EventHandler.h
	DEM/Low/src/Events/EventID.h
	DEM/Low/src/Events/EventNative.h
	DEM/Low/src/Events/EventServer.h
	DEM/Low/src/Events/EventsFwd.h
	DEM/Low/src/Events/Subscription.h
	DEM/Low/src/Frame/NodeAttrAmbientLight.h
	DEM/Low/src/Frame/NodeAttrCamera.h
	DEM/Low/src/Frame/NodeAttrLight.h
	DEM/Low/src/Frame/NodeAttrRenderable.h
	DEM/Low/src/Frame/NodeAttrSkin.h
	DEM/Low/src/Frame/RenderPath.h
	DEM/Low/src/Frame/RenderPathLoaderRP.h
	DEM/Low/src/Frame/RenderPhase.h
	DEM/Low/src/Frame/RenderPhaseGeometry.h
	DEM/Low/src/Frame/RenderPhaseGlobalSetup.h
	DEM/Low/src/Frame/SceneNodeUpdateInSPS.h
	DEM/Low/src/Frame/SceneNodeValidateResources.h
	DEM/Low/src/Frame/View.h
	DEM/Low/src/Input/ControlLayout.h
	DEM/Low/src/Input/InputCondition.h
	DEM/Low/src/Input/InputConditionComboEvent.h
	DEM/Low/src/Input/InputConditionComboState.h
	DEM/Low/src/Input/InputConditionDown.h
	DEM/Low/src/Input/InputConditionHold.h
	DEM/Low/src/Input/InputConditionMove.h
	DEM/Low/src/Input/InputConditionPressed.h
	DEM/Low/src/Input/InputConditionReleased.h
	DEM/Low/src/Input/InputConditionSequence.h
	DEM/Low/src/Input/InputConditionUp.h
	DEM/Low/src/Input/InputDevice.h
	DEM/Low/src/Input/InputEvents.h
	DEM/Low/src/Input/InputFwd.h
	DEM/Low/src/Input/InputTranslator.h
	DEM/Low/src/IO/BinaryReader.h
	DEM/Low/src/IO/BinaryWriter.h
	DEM/Low/src/IO/BTFile.h
	DEM/Low/src/IO/FileSystem.h
	DEM/Low/src/IO/FSBrowser.h
	DEM/Low/src/IO/HRDWriter.h
	DEM/Low/src/IO/IOFwd.h
	DEM/Low/src/IO/IOServer.h
	DEM/Low/src/IO/PathUtils.h
	DEM/Low/src/IO/Stream.h
	DEM/Low/src/IO/StreamReader.h
	DEM/Low/src/IO/StreamWriter.h
	DEM/Low/src/IO/TextReader.h
	DEM/Low/src/IO/FS/FileSystemNative.h
	DEM/Low/src/IO/FS/FileSystemNPK.h
	DEM/Low/src/IO/FS/NpkTOC.h
	DEM/Low/src/IO/FS/NpkTOCEntry.h
	DEM/Low/src/IO/Streams/FileStream.h
	DEM/Low/src/IO/Streams/MemStream.h
	DEM/Low/src/IO/Streams/ScopedStream.h
	DEM/Low/src/Math/AABB.h
	DEM/Low/src/Math/EnvelopeCurve.h
	DEM/Low/src/Math/Euler.h
	DEM/Low/src/Math/EulerAngles.h
	DEM/Low/src/Math/Line.h
	DEM/Low/src/Math/Math.h
	DEM/Low/src/Math/Matrix33.h
	DEM/Low/src/Math/Matrix44.h
	DEM/Low/src/Math/MatrixDefs.h
	DEM/Low/src/Math/PackedNormal.h
	DEM/Low/src/Math/Plane.h
	DEM/Low/src/Math/Polar.h
	DEM/Low/src/Math/Quaternion.h
	DEM/Low/src/Math/Sphere.h
	DEM/Low/src/Math/TransformSRT.h
	DEM/Low/src/Math/Triangle.h
	DEM/Low/src/Math/Vector2.h
	DEM/Low/src/Math/Vector3.h
	DEM/Low/src/Math/Vector3EnvelopeCurve.h
	DEM/Low/src/Math/Vector4.h
	DEM/Low/src/Physics/BulletConv.h
	DEM/Low/src/Physics/ClosestNotMeRayResultCallback.h
	DEM/Low/src/Physics/CollisionObjMoving.h
	DEM/Low/src/Physics/CollisionObjStatic.h
	DEM/Low/src/Physics/CollisionShape.h
	DEM/Low/src/Physics/CollisionShapeLoader.h
	DEM/Low/src/Physics/HeightfieldShape.h
	DEM/Low/src/Physics/MotionStateDynamic.h
	DEM/Low/src/Physics/MotionStateKinematic.h
	DEM/Low/src/Physics/NodeAttrCollision.h
	DEM/Low/src/Physics/NodeControllerRigidBody.h
	DEM/Low/src/Physics/PhysicsDebugDraw.h
	DEM/Low/src/Physics/PhysicsLevel.h
	DEM/Low/src/Physics/PhysicsObject.h
	DEM/Low/src/Physics/PhysicsServer.h
	DEM/Low/src/Physics/RigidBody.h
	DEM/Low/src/Physics/StaticMeshShape.h
	DEM/Low/src/Render/CDLODData.h
	DEM/Low/src/Render/CDLODDataLoader.h
	DEM/Low/src/Render/ConstantBuffer.h
	DEM/Low/src/Render/ConstantBufferSet.h
	DEM/Low/src/Render/DepthStencilBuffer.h
	DEM/Low/src/Render/DisplayDriver.h
	DEM/Low/src/Render/DisplayMode.h
	DEM/Low/src/Render/Effect.h
	DEM/Low/src/Render/GPUDriver.h
	DEM/Low/src/Render/ImageUtils.h
	DEM/Low/src/Render/IndexBuffer.h
	DEM/Low/src/Render/Light.h
	DEM/Low/src/Render/Material.h
	DEM/Low/src/Render/Mesh.h
	DEM/Low/src/Render/MeshData.h
	DEM/Low/src/Render/MeshGenerators.h
	DEM/Low/src/Render/MeshLoaderNVX2.h
	DEM/Low/src/Render/Model.h
	DEM/Low/src/Render/ModelRenderer.h
	DEM/Low/src/Render/Renderable.h
	DEM/Low/src/Render/Renderer.h
	DEM/Low/src/Render/RenderFwd.h
	DEM/Low/src/Render/RenderNode.h
	DEM/Low/src/Render/RenderState.h
	DEM/Low/src/Render/RenderStateDesc.h
	DEM/Low/src/Render/RenderTarget.h
	DEM/Low/src/Render/Sampler.h
	DEM/Low/src/Render/SamplerDesc.h
	DEM/Low/src/Render/Shader.h
	DEM/Low/src/Render/ShaderConstant.h
	DEM/Low/src/Render/ShaderLibrary.h
	DEM/Low/src/Render/ShaderLibraryLoaderSLB.h
	DEM/Low/src/Render/ShaderMetadata.h
	DEM/Low/src/Render/SkinInfo.h
	DEM/Low/src/Render/SkinInfoLoaderSKN.h
	DEM/Low/src/Render/Skybox.h
	DEM/Low/src/Render/SkyboxRenderer.h
	DEM/Low/src/Render/SwapChain.h
	DEM/Low/src/Render/Technique.h
	DEM/Low/src/Render/Terrain.h
	DEM/Low/src/Render/TerrainRenderer.h
	DEM/Low/src/Render/Texture.h
	DEM/Low/src/Render/TextureData.h
	DEM/Low/src/Render/TextureLoaderCDLOD.h
	DEM/Low/src/Render/TextureLoaderDDS.h
	DEM/Low/src/Render/TextureLoaderTGA.h
	DEM/Low/src/Render/VertexBuffer.h
	DEM/Low/src/Render/VertexComponent.h
	DEM/Low/src/Render/VertexLayout.h
	DEM/Low/src/Render/VideoDriverFactory.h
	DEM/Low/src/Render/D3D11/D3D11ConstantBuffer.h
	DEM/Low/src/Render/D3D11/D3D11DepthStencilBuffer.h
	DEM/Low/src/Render/D3D11/D3D11DisplayDriver.h
	DEM/Low/src/Render/D3D11/D3D11DriverFactory.h
	DEM/Low/src/Render/D3D11/D3D11GPUDriver.h
	DEM/Low/src/Render/D3D11/D3D11IndexBuffer.h
	DEM/Low/src/Render/D3D11/D3D11RenderState.h
	DEM/Low/src/Render/D3D11/D3D11RenderTarget.h
	DEM/Low/src/Render/D3D11/D3D11Sampler.h
	DEM/Low/src/Render/D3D11/D3D11Shader.h
	DEM/Low/src/Render/D3D11/D3D11SwapChain.h
	DEM/Low/src/Render/D3D11/D3D11Texture.h
	DEM/Low/src/Render/D3D11/D3D11VertexBuffer.h
	DEM/Low/src/Render/D3D11/D3D11VertexLayout.h
	DEM/Low/src/Render/D3D11/USMShaderConstant.h
	DEM/Low/src/Render/D3D11/USMShaderMetadata.h
	DEM/Low/src/Render/D3D9/D3D9ConstantBuffer.h
	DEM/Low/src/Render/D3D9/D3D9DepthStencilBuffer.h
	DEM/Low/src/Render/D3D9/D3D9DisplayDriver.h
	DEM/Low/src/Render/D3D9/D3D9DriverFactory.h
	DEM/Low/src/Render/D3D9/D3D9Fwd.h
	DEM/Low/src/Render/D3D9/D3D9GPUDriver.h
	DEM/Low/src/Render/D3D9/D3D9IndexBuffer.h
	DEM/Low/src/Render/D3D9/D3D9RenderState.h
	DEM/Low/src/Render/D3D9/D3D9RenderTarget.h
	DEM/Low/src/Render/D3D9/D3D9Sampler.h
	DEM/Low/src/Render/D3D9/D3D9Shader.h
	DEM/Low/src/Render/D3D9/D3D9SwapChain.h
	DEM/Low/src/Render/D3D9/D3D9Texture.h
	DEM/Low/src/Render/D3D9/D3D9VertexBuffer.h
	DEM/Low/src/Render/D3D9/D3D9VertexLayout.h
	DEM/Low/src/Render/D3D9/SM30ShaderConstant.h
	DEM/Low/src/Render/D3D9/SM30ShaderMetadata.h
	DEM/Low/src/Resources/Resource.h
	DEM/Low/src/Resources/ResourceCreator.h
	DEM/Low/src/Resources/ResourceLoader.h
	DEM/Low/src/Resources/ResourceManager.h
	DEM/Low/src/Resources/ResourceObject.h
	DEM/Low/src/Scene/LODGroup.h
	DEM/Low/src/Scene/NodeAttribute.h
	DEM/Low/src/Scene/NodeController.h
	DEM/Low/src/Scene/NodeControllerComposite.h
	DEM/Low/src/Scene/NodeControllerPriorityBlend.h
	DEM/Low/src/Scene/NodeControllerStatic.h
	DEM/Low/src/Scene/NodeControllerThirdPerson.h
	DEM/Low/src/Scene/NodeVisitor.h
	DEM/Low/src/Scene/SceneFwd.h
	DEM/Low/src/Scene/SceneNode.h
	DEM/Low/src/Scene/SceneNodeLoaderSCN.h
	DEM/Low/src/Scene/SceneNodeRenderDebug.h
	DEM/Low/src/Scene/SPS.h
	DEM/Low/src/Scripting/EventHandlerScript.h
	DEM/Low/src/Scripting/ScriptObject.h
	DEM/Low/src/Scripting/ScriptServer.h
	DEM/Low/src/SI/SI_L1.h
	DEM/Low/src/System/Memory.h
	DEM/Low/src/System/OSFileSystem.h
	DEM/Low/src/System/OSWindow.h
	DEM/Low/src/System/Platform.h
	DEM/Low/src/System/System.h
	DEM/Low/src/System/Allocators/PoolAllocator.h
	DEM/Low/src/System/Win32/InputDeviceWin32.h
	DEM/Low/src/System/Win32/KeyboardWin32.h
	DEM/Low/src/System/Win32/MouseWin32.h
	DEM/Low/src/System/Win32/OSFileSystemWin32.h
	DEM/Low/src/System/Win32/OSWindowWin32.h
	DEM/Low/src/System/Win32/PlatformWin32.h
	DEM/Low/src/UI/RenderPhaseGUI.h
	DEM/Low/src/UI/UIContext.h
	DEM/Low/src/UI/UIFwd.h
	DEM/Low/src/UI/UIServer.h
	DEM/Low/src/UI/UIWindow.h
	DEM/Low/src/UI/CEGUI/DEMGeometryBuffer.h
	DEM/Low/src/UI/CEGUI/DEMLogger.h
	DEM/Low/src/UI/CEGUI/DEMRenderer.h
	DEM/Low/src/UI/CEGUI/DEMRenderTarget.h
	DEM/Low/src/UI/CEGUI/DEMResourceProvider.h
	DEM/Low/src/UI/CEGUI/DEMShaderWrapper.h
	DEM/Low/src/UI/CEGUI/DEMTexture.h
	DEM/Low/src/UI/CEGUI/DEMTextureTarget.h
	DEM/Low/src/UI/CEGUI/DEMViewportTarget.h
	DEM/Low/src/UI/CEGUI/FmtLbTextItem.h
	DEM/Low/src/Util/MD5.h
	DEM/Low/src/Util/PFLoop.h
	DEM/Low/src/Util/PFLoopAngular.h
	DEM/Low/src/Util/PFLoopIntDrv.h
	DEM/Low/src/Util/PFLoopQuat.h
	DEM/Low/src/Util/UtilFwd.h
	DEM/Low/src/Video/OGGTheoraPlayer.h
	DEM/Low/src/Video/VideoPlayer.h
	DEM/Low/src/Video/VideoServer.h
)

set(DEM_L1_LOW_SOURCES
	DEM/Low/src/Animation/AnimClip.cpp
	DEM/Low/src/Animation/AnimTask.cpp
	DEM/Low/src/Animation/KeyframeClip.cpp
	DEM/Low/src/Animation/KeyframeClipLoaderKFA.cpp
	DEM/Low/src/Animation/KeyframeTrack.cpp
	DEM/Low/src/Animation/MocapClip.cpp
	DEM/Low/src/Animation/MocapClipLoaderNAX2.cpp
	DEM/Low/src/Animation/MocapTrack.cpp
	DEM/Low/src/Animation/NodeControllerKeyframe.cpp
	DEM/Low/src/Animation/NodeControllerMocap.cpp
	DEM/Low/src/Core/Application.cpp
	DEM/Low/src/Core/ApplicationState.cpp
	DEM/Low/src/Core/CoreServer.cpp
	DEM/Low/src/Core/Factory.cpp
	DEM/Low/src/Core/Object.cpp
	DEM/Low/src/Core/RTTIBaseClass.cpp
	DEM/Low/src/Core/TimeSource.cpp
	DEM/Low/src/Data/Buffer.cpp
	DEM/Low/src/Data/Data.cpp
	DEM/Low/src/Data/DataArray.cpp
	DEM/Low/src/Data/DataScheme.cpp
	DEM/Low/src/Data/HandleManager.cpp
	DEM/Low/src/Data/HRDParser.cpp
	DEM/Low/src/Data/MurmurHash3.cpp
	DEM/Low/src/Data/Params.cpp
	DEM/Low/src/Data/ParamsUtils.cpp
	DEM/Low/src/Data/RAMData.cpp
	DEM/Low/src/Data/String.cpp
	DEM/Low/src/Data/StringID.cpp
	DEM/Low/src/Data/StringIDStorage.cpp
	DEM/Low/src/Data/StringUtils.cpp
	DEM/Low/src/Data/Loaders/StringTableFromExcelXML.cpp
	DEM/Low/src/Debug/DebugDraw.cpp
	DEM/Low/src/Debug/DebugServer.cpp
	DEM/Low/src/Debug/LuaConsole.cpp
	DEM/Low/src/Debug/WatcherWindow.cpp
	DEM/Low/src/Events/Event.cpp
	DEM/Low/src/Events/EventDispatcher.cpp
	DEM/Low/src/Events/EventNative.cpp
	DEM/Low/src/Events/EventServer.cpp
	DEM/Low/src/Events/Subscription.cpp
	DEM/Low/src/Frame/NodeAttrAmbientLight.cpp
	DEM/Low/src/Frame/NodeAttrCamera.cpp
	DEM/Low/src/Frame/NodeAttrLight.cpp
	DEM/Low/src/Frame/NodeAttrRenderable.cpp
	DEM/Low/src/Frame/NodeAttrSkin.cpp
	DEM/Low/src/Frame/RenderPath.cpp
	DEM/Low/src/Frame/RenderPathLoaderRP.cpp
	DEM/Low/src/Frame/RenderPhase.cpp
	DEM/Low/src/Frame/RenderPhaseGeometry.cpp
	DEM/Low/src/Frame/RenderPhaseGlobalSetup.cpp
	DEM/Low/src/Frame/SceneNodeUpdateInSPS.cpp
	DEM/Low/src/Frame/SceneNodeValidateResources.cpp
	DEM/Low/src/Frame/View.cpp
	DEM/Low/src/Input/ControlLayout.cpp
	DEM/Low/src/Input/Input.cpp
	DEM/Low/src/Input/InputCondition.cpp
	DEM/Low/src/Input/InputConditionComboEvent.cpp
	DEM/Low/src/Input/InputConditionComboState.cpp
	DEM/Low/src/Input/InputConditionDown.cpp
	DEM/Low/src/Input/InputConditionHold.cpp
	DEM/Low/src/Input/InputConditionMove.cpp
	DEM/Low/src/Input/InputConditionPressed.cpp
	DEM/Low/src/Input/InputConditionReleased.cpp
	DEM/Low/src/Input/InputConditionSequence.cpp
	DEM/Low/src/Input/InputConditionUp.cpp
	DEM/Low/src/Input/InputEvents.cpp
	DEM/Low/src/Input/InputTranslator.cpp
	DEM/Low/src/IO/BinaryReader.cpp
	DEM/Low/src/IO/BinaryWriter.cpp
	DEM/Low/src/IO/BTFile.cpp
	DEM/Low/src/IO/FSBrowser.cpp
	DEM/Low/src/IO/HRDWriter.cpp
	DEM/Low/src/IO/IOServer.cpp
	DEM/Low/src/IO/PathUtils.cpp
	DEM/Low/src/IO/Stream.cpp
	DEM/Low/src/IO/TextReader.cpp
	DEM/Low/src/IO/FS/FileSystemNative.cpp
	DEM/Low/src/IO/FS/FileSystemNPK.cpp
	DEM/Low/src/IO/Streams/FileStream.cpp
	DEM/Low/src/IO/Streams/MemStream.cpp
	DEM/Low/src/IO/Streams/ScopedStream.cpp
	DEM/Low/src/Math/AABB.cpp
	DEM/Low/src/Math/Math.cpp
	DEM/Low/src/Math/Matrix33.cpp
	DEM/Low/src/Math/Matrix44.cpp
	DEM/Low/src/Math/Quaternion.cpp
	DEM/Low/src/Math/TransformSRT.cpp
	DEM/Low/src/Math/Vector2.cpp
	DEM/Low/src/Math/Vector3.cpp
	DEM/Low/src/Math/Vector4.cpp
	DEM/Low/src/Physics/CollisionObjMoving.cpp
	DEM/Low/src/Physics/CollisionObjStatic.cpp
	DEM/Low/src/Physics/CollisionShape.cpp
	DEM/Low/src/Physics/CollisionShapeLoader.cpp
	DEM/Low/src/Physics/HeightfieldShape.cpp
	DEM/Low/src/Physics/NodeAttrCollision.cpp
	DEM/Low/src/Physics/NodeControllerRigidBody.cpp
	DEM/Low/src/Physics/PhysicsDebugDraw.cpp
	DEM/Low/src/Physics/PhysicsLevel.cpp
	DEM/Low/src/Physics/PhysicsObject.cpp
	DEM/Low/src/Physics/PhysicsServer.cpp
	DEM/Low/src/Physics/RigidBody.cpp
	DEM/Low/src/Physics/StaticMeshShape.cpp
	DEM/Low/src/Render/CDLODData.cpp
	DEM/Low/src/Render/CDLODDataLoader.cpp
	DEM/Low/src/Render/ConstantBufferSet.cpp
	DEM/Low/src/Render/DepthStencilBuffer.cpp
	DEM/Low/src/Render/Effect.cpp
	DEM/Low/src/Render/GPUDriver.cpp
	DEM/Low/src/Render/ImageUtils.cpp
	DEM/Low/src/Render/Light.cpp
	DEM/Low/src/Render/LoadMaterialFromPRM.cpp
	DEM/Low/src/Render/Material.cpp
	DEM/Low/src/Render/Mesh.cpp
	DEM/Low/src/Render/MeshData.cpp
	DEM/Low/src/Render/MeshGenerators.cpp
	DEM/Low/src/Render/MeshLoaderNVX2.cpp
	DEM/Low/src/Render/Model.cpp
	DEM/Low/src/Render/ModelRenderer.cpp
	DEM/Low/src/Render/Renderable.cpp
	DEM/Low/src/Render/Renderer.cpp
	DEM/Low/src/Render/RenderStateDesc.cpp
	DEM/Low/src/Render/RenderTarget.cpp
	DEM/Low/src/Render/SamplerDesc.cpp
	DEM/Low/src/Render/ShaderLibrary.cpp
	DEM/Low/src/Render/ShaderLibraryLoaderSLB.cpp
	DEM/Low/src/Render/ShaderMetadata.cpp
	DEM/Low/src/Render/SkinInfo.cpp
	DEM/Low/src/Render/SkinInfoLoaderSKN.cpp
	DEM/Low/src/Render/Skybox.cpp
	DEM/Low/src/Render/SkyboxRenderer.cpp
	DEM/Low/src/Render/SwapChain.cpp
	DEM/Low/src/Render/Technique.cpp
	DEM/Low/src/Render/Terrain.cpp
	DEM/Low/src/Render/TerrainRenderer.cpp
	DEM/Low/src/Render/Texture.cpp
	DEM/Low/src/Render/TextureData.cpp
	DEM/Low/src/Render/TextureLoaderCDLOD.cpp
	DEM/Low/src/Render/TextureLoaderDDS.cpp
	DEM/Low/src/Render/TextureLoaderTGA.cpp
	DEM/Low/src/Render/VertexLayout.cpp
	DEM/Low/src/Render/D3D11/D3D11ConstantBuffer.cpp
	DEM/Low/src/Render/D3D11/D3D11DepthStencilBuffer.cpp
	DEM/Low/src/Render/D3D11/D3D11DisplayDriver.cpp
	DEM/Low/src/Render/D3D11/D3D11DriverFactory.cpp
	DEM/Low/src/Render/D3D11/D3D11GPUDriver.cpp
	DEM/Low/src/Render/D3D11/D3D11IndexBuffer.cpp
	DEM/Low/src/Render/D3D11/D3D11RenderState.cpp
	DEM/Low/src/Render/D3D11/D3D11RenderTarget.cpp
	DEM/Low/src/Render/D3D11/D3D11Sampler.cpp
	DEM/Low/src/Render/D3D11/D3D11Shader.cpp
	DEM/Low/src/Render/D3D11/D3D11SwapChain.cpp
	DEM/Low/src/Render/D3D11/D3D11Texture.cpp
	DEM/Low/src/Render/D3D11/D3D11VertexBuffer.cpp
	DEM/Low/src/Render/D3D11/D3D11VertexLayout.cpp
	DEM/Low/src/Render/D3D11/USMShaderConstant.cpp
	DEM/Low/src/Render/D3D11/USMShaderMetadata.cpp
	DEM/Low/src/Render/D3D9/D3D9ConstantBuffer.cpp
	DEM/Low/src/Render/D3D9/D3D9DepthStencilBuffer.cpp
	DEM/Low/src/Render/D3D9/D3D9DisplayDriver.cpp
	DEM/Low/src/Render/D3D9/D3D9DriverFactory.cpp
	DEM/Low/src/Render/D3D9/D3D9GPUDriver.cpp
	DEM/Low/src/Render/D3D9/D3D9IndexBuffer.cpp
	DEM/Low/src/Render/D3D9/D3D9RenderState.cpp
	DEM/Low/src/Render/D3D9/D3D9RenderTarget.cpp
	DEM/Low/src/Render/D3D9/D3D9Sampler.cpp
	DEM/Low/src/Render/D3D9/D3D9Shader.cpp
	DEM/Low/src/Render/D3D9/D3D9SwapChain.cpp
	DEM/Low/src/Render/D3D9/D3D9Texture.cpp
	DEM/Low/src/Render/D3D9/D3D9VertexBuffer.cpp
	DEM/Low/src/Render/D3D9/D3D9VertexLayout.cpp
	DEM/Low/src/Render/D3D9/SM30ShaderConstant.cpp
	DEM/Low/src/Render/D3D9/SM30ShaderMetadata.cpp
	DEM/Low/src/Resources/Resource.cpp
	DEM/Low/src/Resources/ResourceManager.cpp
	DEM/Low/src/Resources/ResourceObject.cpp
	DEM/Low/src/Scene/LODGroup.cpp
	DEM/Low/src/Scene/NodeAttribute.cpp
	DEM/Low/src/Scene/NodeController.cpp
	DEM/Low/src/Scene/NodeControllerComposite.cpp
	DEM/Low/src/Scene/NodeControllerPriorityBlend.cpp
	DEM/Low/src/Scene/NodeControllerThirdPerson.cpp
	DEM/Low/src/Scene/SceneNode.cpp
	DEM/Low/src/Scene/SceneNodeLoaderSCN.cpp
	DEM/Low/src/Scene/SceneNodeRenderDebug.cpp
	DEM/Low/src/Scene/SPS.cpp
	DEM/Low/src/Scripting/EventHandlerScript.cpp
	DEM/Low/src/Scripting/ScriptObject.cpp
	DEM/Low/src/Scripting/ScriptServer.cpp
	DEM/Low/src/SI/SIEventServer.cpp
	DEM/Low/src/SI/SIGlobals.cpp
	DEM/Low/src/SI/SITime.cpp
	DEM/Low/src/System/Memory.cpp
	DEM/Low/src/System/System.cpp
	DEM/Low/src/System/Win32/KeyboardWin32.cpp
	DEM/Low/src/System/Win32/MouseWin32.cpp
	DEM/Low/src/System/Win32/OSFileSystemWin32.cpp
	DEM/Low/src/System/Win32/OSWindowWin32.cpp
	DEM/Low/src/System/Win32/PlatformWin32.cpp
	DEM/Low/src/System/Win32/SystemWin32.cpp
	DEM/Low/src/UI/RenderPhaseGUI.cpp
	DEM/Low/src/UI/UIContext.cpp
	DEM/Low/src/UI/UIServer.cpp
	DEM/Low/src/UI/UIWindow.cpp
	DEM/Low/src/UI/CEGUI/DEMGeometryBuffer.cpp
	DEM/Low/src/UI/CEGUI/DEMLogger.cpp
	DEM/Low/src/UI/CEGUI/DEMRenderer.cpp
	DEM/Low/src/UI/CEGUI/DEMRenderTarget.cpp
	DEM/Low/src/UI/CEGUI/DEMResourceProvider.cpp
	DEM/Low/src/UI/CEGUI/DEMShaderWrapper.cpp
	DEM/Low/src/UI/CEGUI/DEMTexture.cpp
	DEM/Low/src/UI/CEGUI/DEMTextureTarget.cpp
	DEM/Low/src/UI/CEGUI/DEMViewportTarget.cpp
	DEM/Low/src/UI/CEGUI/FmtLbTextItem.cpp
	DEM/Low/src/Util/CRC.cpp
	DEM/Low/src/Util/MD5.cpp
	DEM/Low/src/Util/PerlinNoise.cpp
	DEM/Low/src/Video/OGGTheoraPlayer.cpp
	DEM/Low/src/Video/VideoPlayer.cpp
	DEM/Low/src/Video/VideoServer.cpp
)

