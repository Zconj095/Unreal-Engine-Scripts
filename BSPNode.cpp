#include "BSPNode.h"

FBSPNode::FBSPNode(const FBox& InBounds, const TArray<FNanovoxel>& InVoxels)
	: Bounds(InBounds)
	, Voxels(InVoxels)
{
}

FBSPNode::FBSPNode(const FBox& InBounds, TArray<FNanovoxel>&& InVoxels)
	: Bounds(InBounds)
	, Voxels(MoveTemp(InVoxels))
{
}

void FBSPNode::Subdivide()
{
	if (!bIsLeaf || Voxels.Num() <= 1)
	{
		return;
	}

	bIsLeaf = false;

	const FVector Size = Bounds.GetSize();
	const FVector Center = Bounds.GetCenter();
	FBox LeftBounds(EForceInit::ForceInit);
	FBox RightBounds(EForceInit::ForceInit);

	if (Size.X >= Size.Y && Size.X >= Size.Z)
	{
		const FVector HalfExtent(Size.X * 0.25f, Size.Y * 0.5f, Size.Z * 0.5f);
		const FVector Offset(Size.X * 0.25f, 0.0f, 0.0f);
		const FVector LeftCenter = Center - Offset;
		const FVector RightCenter = Center + Offset;

		LeftBounds = FBox(LeftCenter - HalfExtent, LeftCenter + HalfExtent);
		RightBounds = FBox(RightCenter - HalfExtent, RightCenter + HalfExtent);
	}
	else if (Size.Y >= Size.X && Size.Y >= Size.Z)
	{
		const FVector HalfExtent(Size.X * 0.5f, Size.Y * 0.25f, Size.Z * 0.5f);
		const FVector Offset(0.0f, Size.Y * 0.25f, 0.0f);
		const FVector LeftCenter = Center - Offset;
		const FVector RightCenter = Center + Offset;

		LeftBounds = FBox(LeftCenter - HalfExtent, LeftCenter + HalfExtent);
		RightBounds = FBox(RightCenter - HalfExtent, RightCenter + HalfExtent);
	}
	else
	{
		const FVector HalfExtent(Size.X * 0.5f, Size.Y * 0.5f, Size.Z * 0.25f);
		const FVector Offset(0.0f, 0.0f, Size.Z * 0.25f);
		const FVector LeftCenter = Center - Offset;
		const FVector RightCenter = Center + Offset;

		LeftBounds = FBox(LeftCenter - HalfExtent, LeftCenter + HalfExtent);
		RightBounds = FBox(RightCenter - HalfExtent, RightCenter + HalfExtent);
	}

	TArray<FNanovoxel> LeftVoxels;
	TArray<FNanovoxel> RightVoxels;
	LeftVoxels.Reserve(Voxels.Num());
	RightVoxels.Reserve(Voxels.Num());

	for (const FNanovoxel& Voxel : Voxels)
	{
		if (LeftBounds.IsInside(Voxel.Position))
		{
			LeftVoxels.Add(Voxel);
		}
		else if (RightBounds.IsInside(Voxel.Position))
		{
			RightVoxels.Add(Voxel);
		}
	}

	LeftChild = MakeUnique<FBSPNode>(LeftBounds, MoveTemp(LeftVoxels));
	RightChild = MakeUnique<FBSPNode>(RightBounds, MoveTemp(RightVoxels));

	LeftChild->Subdivide();
	RightChild->Subdivide();

	Voxels.Reset();
}
