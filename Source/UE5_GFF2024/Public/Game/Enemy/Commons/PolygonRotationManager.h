// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UE5_GFF2024_API PolygonRotationManager
{

public:
    FRotator NowRotation = { 0,0,0 };
    FRotator NextRotation = { 0,0,0 };
    FRotator TestRotation = { 0,0,0 };
    FVector NewLocation = { 0,0,0 };
    FVector CenterPosition = { 0,0,0 };

    int32 bottom = 0;
    int32 next = 0;
    int32 FpsCounter = 0;
    const int32 Fps = 30;

    bool IsRotating = false;

    float Speed = 1.f;

public:
    PolygonRotationManager();
    ~PolygonRotationManager();
    // コンストラクタで頂点と面を受け取る
    PolygonRotationManager(const TArray<FVector>& Vertices, const TArray<TArray<int32>>& Faces)
        : Vertices(Vertices), Faces(Faces) {}

    void Init();

    // 各面の中心座標を計算する
    TArray<FVector> GetFaceCenters(const FVector& Scale, const FVector& Position);

    //面の中心の座標を計算する
    FVector GetFaceCenterLocation(const FVector& Scale, const FVector& Position, int FaceNum);

    //頂点座標
    FVector GetVertexLocation(const FVector& Scale, const FVector& Position, int VertexNum);

    // 正多面体の面の中心座標を計算・デバッグ描画
    void DrawPolyhedronFaceCenters(UWorld* World, PolygonRotationManager& Polyhedron, const FVector& Scale, const FVector& Position);

    // 二つのベクトル間の角度を計算する関数
    float CalculateAngleBetweenVectors(const FVector& VectorA, const FVector& VectorB);

    // 二つのベクトル間の角度を計算する関数
    FRotator CalculateRotationBetweenVectors(const FVector& VectorA, const FVector& VectorB);

    //共通する頂点の中心を取得
    FVector GetShareFace(int face1, int face2, const FVector& Scale, const FVector& Position);

    //
    void SetNextBottom(FVector Vector, const FVector& Scale, const FVector& Position);

    void SetBottom(const FVector& Scale, const FVector& Position);

    void CalculateRotationDirection(const FVector& Scale, const FVector& Position);

    void SetNewRotationAndLocation(const FVector& ActorPosition);

    //回転角度設定
    void ShapesRotation(FRotator Rot)
    {
        NowRotation = Rot;
    }

    void SetSpeed(float speed)
    {
        Speed = speed;
    }

    void SetVertices(TArray<FVector>& vertices)
    {
        Vertices = vertices;
    }

    FVector GetNewLocation() { return NewLocation; }
    FRotator GetNowRotation() { return NowRotation; }
    bool GetIsRotating() { return IsRotating; }
    
private:
    TArray<FVector> Vertices;  // 頂点の配列
    TArray<TArray<int32>> Faces;  // 各面の頂点インデックスを保持する配列



    //回転のFPSをデルタタイムから制御してどんなＦＰＳでも違和感をなくす
    //SetActorLocationではなくCharacterMovementを使ってうまいことできないか
};
