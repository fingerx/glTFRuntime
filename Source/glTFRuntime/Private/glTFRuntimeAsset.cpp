// Copyright 2020, Roberto De Ioris.

#include "glTFRuntimeAsset.h"
#include "Animation/AnimSequence.h"

#define GLTF_CHECK_PARSER(RetValue) if (!Parser)\
	{\
		UE_LOG(LogGLTFRuntime, Error, TEXT("No glTF Asset loaded."));\
		return RetValue;\
	}\


bool UglTFRuntimeAsset::LoadFromFilename(const FString Filename)
{
	// asset already loaded ?
	if (Parser)
	{
		return false;
	}

	Parser = FglTFRuntimeParser::FromFilename(Filename);
	if (Parser)
	{
		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UglTFRuntimeAsset, OnErrorProxy));
		Parser->OnError.Add(Delegate);
		Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UglTFRuntimeAsset, OnStaticMeshCreatedProxy));
		Parser->OnStaticMeshCreated.Add(Delegate);
		Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UglTFRuntimeAsset, OnSkeletalMeshCreatedProxy));
		Parser->OnSkeletalMeshCreated.Add(Delegate);
	}
	return Parser != nullptr;
}

bool UglTFRuntimeAsset::LoadFromString(const FString JsonData)
{
	// asset already loaded ?
	if (Parser)
	{
		return false;
	}

	Parser = FglTFRuntimeParser::FromString(JsonData);
	if (Parser)
	{
		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UglTFRuntimeAsset, OnErrorProxy));
		Parser->OnError.Add(Delegate);
		Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UglTFRuntimeAsset, OnStaticMeshCreatedProxy));
		Parser->OnStaticMeshCreated.Add(Delegate);
		Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UglTFRuntimeAsset, OnSkeletalMeshCreatedProxy));
		Parser->OnSkeletalMeshCreated.Add(Delegate);
	}
	return Parser != nullptr;
}

bool UglTFRuntimeAsset::LoadFromData(const TArray64<uint8> Data)
{
	// asset already loaded ?
	if (Parser)
	{
		return false;
	}

	Parser = FglTFRuntimeParser::FromData(Data);
	if (Parser)
	{
		FScriptDelegate Delegate;
		Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UglTFRuntimeAsset, OnErrorProxy));
		Parser->OnError.Add(Delegate);
		Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UglTFRuntimeAsset, OnStaticMeshCreatedProxy));
		Parser->OnStaticMeshCreated.Add(Delegate);
		Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UglTFRuntimeAsset, OnSkeletalMeshCreatedProxy));
		Parser->OnSkeletalMeshCreated.Add(Delegate);
	}
	return Parser != nullptr;
}

void UglTFRuntimeAsset::OnErrorProxy(const FString ErrorContext, const FString ErrorMessage)
{
	if (OnError.IsBound())
	{
		OnError.Broadcast(ErrorContext, ErrorMessage);
	}
}

void UglTFRuntimeAsset::OnStaticMeshCreatedProxy(UStaticMesh* StaticMesh)
{
	if (OnStaticMeshCreated.IsBound())
	{
		OnStaticMeshCreated.Broadcast(StaticMesh);
	}
}

void UglTFRuntimeAsset::OnSkeletalMeshCreatedProxy(USkeletalMesh* SkeletalMesh)
{
	if (OnSkeletalMeshCreated.IsBound())
	{
		OnSkeletalMeshCreated.Broadcast(SkeletalMesh);
	}
}

TArray<FglTFRuntimeScene> UglTFRuntimeAsset::GetScenes()
{
	GLTF_CHECK_PARSER(TArray<FglTFRuntimeScene>());

	TArray<FglTFRuntimeScene> Scenes;
	if (!Parser->LoadScenes(Scenes))
	{
		Parser->AddError("UglTFRuntimeAsset::GetScenes()", "Unable to retrieve Scenes from glTF Asset.");
		return TArray<FglTFRuntimeScene>();
	}
	return Scenes;
}

TArray<FglTFRuntimeNode> UglTFRuntimeAsset::GetNodes()
{
	GLTF_CHECK_PARSER(TArray<FglTFRuntimeNode>());

	TArray<FglTFRuntimeNode> Nodes;
	if (!Parser->GetAllNodes(Nodes))
	{
		Parser->AddError("UglTFRuntimeAsset::GetScenes()", "Unable to retrieve Nodes from glTF Asset.");
		return TArray<FglTFRuntimeNode>();
	}
	return Nodes;
}

bool UglTFRuntimeAsset::GetNode(const int32 NodeIndex, FglTFRuntimeNode& Node)
{
	GLTF_CHECK_PARSER(false);

	return Parser->LoadNode(NodeIndex, Node);
}

bool UglTFRuntimeAsset::GetNodeByName(const FString NodeName, FglTFRuntimeNode& Node)
{
	GLTF_CHECK_PARSER(false);

	return Parser->LoadNodeByName(NodeName, Node);
}

UStaticMesh* UglTFRuntimeAsset::LoadStaticMesh(const int32 MeshIndex, const FglTFRuntimeStaticMeshConfig& StaticMeshConfig)
{
	GLTF_CHECK_PARSER(nullptr);

	return Parser->LoadStaticMesh(MeshIndex, StaticMeshConfig);
}

UStaticMesh* UglTFRuntimeAsset::LoadStaticMeshByName(const FString MeshName, const FglTFRuntimeStaticMeshConfig& StaticMeshConfig)
{
	GLTF_CHECK_PARSER(nullptr);

	return Parser->LoadStaticMeshByName(MeshName, StaticMeshConfig);
}

USkeletalMesh* UglTFRuntimeAsset::LoadSkeletalMesh(const int32 MeshIndex, const int32 SkinIndex, const FglTFRuntimeSkeletalMeshConfig SkeletalMeshConfig)
{
	GLTF_CHECK_PARSER(nullptr);

	return Parser->LoadSkeletalMesh(MeshIndex, SkinIndex, SkeletalMeshConfig);
}

USkeleton* UglTFRuntimeAsset::LoadSkeleton(const int32 SkinIndex, const FglTFRuntimeSkeletonConfig SkeletonConfig)
{
	GLTF_CHECK_PARSER(nullptr);

	return Parser->LoadSkeleton(SkinIndex, SkeletonConfig);
}

UAnimSequence* UglTFRuntimeAsset::LoadSkeletalAnimation(USkeletalMesh* SkeletalMesh, const int32 AnimationIndex, const FglTFRuntimeSkeletalAnimationConfig SkeletalAnimationConfig)
{
	GLTF_CHECK_PARSER(nullptr);

	return Parser->LoadSkeletalAnimation(SkeletalMesh, AnimationIndex, SkeletalAnimationConfig);
}

UAnimSequence* UglTFRuntimeAsset::LoadSkeletalAnimationByName(USkeletalMesh* SkeletalMesh, const FString AnimationName, const FglTFRuntimeSkeletalAnimationConfig SkeletalAnimationConfig)
{
	GLTF_CHECK_PARSER(nullptr);

	return Parser->LoadSkeletalAnimationByName(SkeletalMesh, AnimationName, SkeletalAnimationConfig);
}

bool UglTFRuntimeAsset::BuildTransformFromNodeBackward(const int32 NodeIndex, FTransform& Transform)
{
	GLTF_CHECK_PARSER(false);

	Transform = FTransform::Identity;

	FglTFRuntimeNode Node;
	Node.ParentIndex = NodeIndex;

	while (Node.ParentIndex != INDEX_NONE)
	{
		if (!Parser->LoadNode(Node.ParentIndex, Node))
		{
			return false;
		}
		Transform *= Node.Transform;
	}

	return true;
}

bool UglTFRuntimeAsset::NodeIsBone(const int32 NodeIndex)
{
	GLTF_CHECK_PARSER(false);

	return Parser->NodeIsBone(NodeIndex);
}

bool UglTFRuntimeAsset::BuildTransformFromNodeForward(const int32 NodeIndex, const int32 LastNodeIndex, FTransform& Transform)
{
	GLTF_CHECK_PARSER(false);

	Transform = FTransform::Identity;

	TArray<FTransform> NodesTree;

	FglTFRuntimeNode Node;
	Node.ParentIndex = LastNodeIndex;

	bool bFoundNode = false;

	while (Node.ParentIndex != INDEX_NONE)
	{
		if (!Parser->LoadNode(Node.ParentIndex, Node))
			return false;
		NodesTree.Add(Node.Transform);
		if (Node.Index == NodeIndex)
		{
			bFoundNode = true;
			break;
		}
	}

	if (!bFoundNode)
		return false;

	for (int32 ChildIndex = NodesTree.Num() - 1; ChildIndex >= 0; ChildIndex--)
	{
		FTransform& ChildTransform = NodesTree[ChildIndex];
		Transform *= ChildTransform;
	}

	return true;
}

UAnimMontage* UglTFRuntimeAsset::LoadSkeletalAnimationAsMontage(USkeletalMesh* SkeletalMesh, const int32 AnimationIndex, const FString SlotNodeName, const FglTFRuntimeSkeletalAnimationConfig AnimationConfig)
{
	UAnimSequence* AnimSequence = LoadSkeletalAnimation(SkeletalMesh, AnimationIndex, AnimationConfig);
	if (!AnimSequence)
	{
		return nullptr;
	}

	UAnimMontage* AnimMontage = UAnimMontage::CreateSlotAnimationAsDynamicMontage(AnimSequence, FName(SlotNodeName), 0, 0, 1);
	if (!AnimMontage)
	{
		return nullptr;
	}

	AnimMontage->SetPreviewMesh(SkeletalMesh);

	return AnimMontage;
}

UglTFRuntimeAnimationCurve* UglTFRuntimeAsset::LoadNodeAnimationCurve(const int32 NodeIndex)
{
	GLTF_CHECK_PARSER(nullptr);

	return Parser->LoadNodeAnimationCurve(NodeIndex);
}

TArray<UglTFRuntimeAnimationCurve*> UglTFRuntimeAsset::LoadAllNodeAnimationCurves(const int32 NodeIndex)
{
	GLTF_CHECK_PARSER(TArray<UglTFRuntimeAnimationCurve*>());

	return Parser->LoadAllNodeAnimationCurves(NodeIndex);
}

UAnimSequence* UglTFRuntimeAsset::LoadNodeSkeletalAnimation(USkeletalMesh* SkeletalMesh, const int32 NodeIndex, const FglTFRuntimeSkeletalAnimationConfig SkeletalAnimationConfig)
{
	GLTF_CHECK_PARSER(nullptr);

	return Parser->LoadNodeSkeletalAnimation(SkeletalMesh, NodeIndex, SkeletalAnimationConfig);
}

bool UglTFRuntimeAsset::FindNodeByNameInArray(const TArray<int32> NodeIndices, const FString NodeName, FglTFRuntimeNode& Node)
{
	GLTF_CHECK_PARSER(false);

	for (int32 NodeIndex : NodeIndices)
	{
		FglTFRuntimeNode CurrentNode;
		if (Parser->LoadNode(NodeIndex, CurrentNode))
		{
			if (CurrentNode.Name == NodeName)
			{
				Node = CurrentNode;
				return true;
			}
		}
	}
	return false;
}
