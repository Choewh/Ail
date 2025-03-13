#pragma once
#include "CoreMinimal.h"
#include "StaticMeshAttributes.h"
#include "MeshDescription.h"
#include "StaticMeshOperations.h"
#include "GeometryProcessing/DynamicMeshToMeshDescription.h"
#include "GeometryProcessing/MeshDescriptionToDynamicMesh.h"
#include "GeometryProcessing/DynamicMesh/DynamicMesh3.h"
#include "GeometryProcessing/UVLayout/UVLayout.h"

class FMeshUtils
{
public:
    static UStaticMesh* UMyStaticMeshUtils::GenerateRuntimeUVs(UStaticMesh* StaticMesh)
    {
        if (!StaticMesh)
        {
            return nullptr;
        }

        // 메시 설명 가져오기
        FMeshDescription* MeshDescription = StaticMesh->GetMeshDescription(0);
        if (!MeshDescription)
        {
            return nullptr;
        }

        // 메시 설명을 다이내믹 메시로 변환
        FDynamicMesh3 DynamicMesh;
        UE::Geometry::FMeshDescriptionToDynamicMesh Converter;
        Converter.Convert(MeshDescription, DynamicMesh);

        // UV 언래핑
        UE::Geometry::FUVLayout UVLayout;
        UVLayout.Apply(DynamicMesh);

        // 다이내믹 메시를 메시 설명으로 변환
        UE::Geometry::FDynamicMeshToMeshDescription Converter2;
        Converter2.Convert(&DynamicMesh, *MeshDescription);

        // 스태틱 메시 빌드
        StaticMesh->Build();

        return StaticMesh;
    }
};