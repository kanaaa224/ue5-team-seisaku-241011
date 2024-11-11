// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/Commons/PolygonRotationManager.h"
#include "Kismet/KismetMathLibrary.h"

PolygonRotationManager::PolygonRotationManager()
{
}

PolygonRotationManager::~PolygonRotationManager()
{
}

TArray<FVector> PolygonRotationManager::GetFaceCenters(const FVector& Scale, const FVector& Position)
{
    TArray<FVector> FaceCenters;

    // 各面について計算
    for (const TArray<int32>& Face : Faces)
    {
        FVector FaceCenter(0, 0, 0);

        // 面を構成する各頂点の座標を合計
        for (int32 VertexIndex : Face)
        {
            FaceCenter += Vertices[VertexIndex];
        }

        // 頂点の平均を取る
        FaceCenter /= Face.Num();

        // スケールと位置を適用
        FaceCenter = FaceCenter * Scale;
        FaceCenter += Position;

        // 結果を追加
        FaceCenters.Add(FaceCenter);
    }

    return FaceCenters;
}

FVector PolygonRotationManager::GetFaceCenterLocation(const FVector& Scale, const FVector& Position, int FaceNum)
{
    FVector FaceCenter(0, 0, 0);

    // 面を構成する各頂点の座標を合計
    for (int32 VertexIndex : Faces[FaceNum])
    {
        FaceCenter += Vertices[VertexIndex];
    }

    // 頂点の平均を取る
    FaceCenter /= Faces[FaceNum].Num();

    // スケールと位置を適用
    FaceCenter = FaceCenter * Scale;
    FaceCenter += Position;

    return FaceCenter;
}

FVector PolygonRotationManager::GetVertexLocation(const FVector& Scale, const FVector& Position, int VertexNum)
{
    FVector Vertex;
    Vertex = Vertices[VertexNum];

    Vertex = Vertex * Scale;
    Vertex += Position;

    return Vertex;
}

void PolygonRotationManager::DrawPolyhedronFaceCenters(UWorld* World, PolygonRotationManager& Polyhedron, const FVector& Scale, const FVector& Position)
{
    TArray<FVector> FaceCenters = Polyhedron.GetFaceCenters(Scale, Position);

    for (const FVector& FaceCenter : FaceCenters)
    {
        DrawDebugSphere(World, FaceCenter, 10.0f, 12, FColor::Green, false, 0.1f);
    }

    ////回転テスト
    //FVector v = FaceCenters[2];
    //FVector a;

    ////回転の中心を箱の原点にするためにこのPositionの使い方をしている。しないとマップの中心を回転の中心にしちゃう
    //a = NowRotation.RotateVector(v - Position);
    DrawDebugSphere(World, CenterPosition, 15.0f, 12, FColor::Red, false, 2.0f);
}

float PolygonRotationManager::CalculateAngleBetweenVectors(const FVector& VectorA, const FVector& VectorB)
{
    // ベクトルを正規化
    FVector NormalizedA = VectorA.GetSafeNormal();
    FVector NormalizedB = VectorB.GetSafeNormal();

    // 内積を計算
    float DotProduct = FVector::DotProduct(NormalizedA, NormalizedB);

    // 逆余弦を使って角度を計算（ラジアン）
    float AngleRadians = FMath::Acos(DotProduct);

    // ラジアンを度に変換して返す
    return FMath::RadiansToDegrees(AngleRadians);
}

FRotator PolygonRotationManager::CalculateRotationBetweenVectors(const FVector& VectorA, const FVector& VectorB)
{
    // ベクトルAとベクトルBを正規化（長さ1にする）
    FVector NormalizedA = VectorA.GetSafeNormal();
    FVector NormalizedB = VectorB.GetSafeNormal();

    // 内積を使って角度を計算（ラジアン）
    float DotProduct = FVector::DotProduct(NormalizedA, NormalizedB);

    // ベクトル間の回転軸を計算する（外積）
    FVector RotationAxis = FVector::CrossProduct(NormalizedA, NormalizedB);

    // 角度（ラジアン）を計算
    float AngleRadians = FMath::Acos(DotProduct);

    // ラジアンを度に変換
    float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

    // 回転軸と角度から回転クォータニオンを作成
    FQuat RotationQuat = FQuat(RotationAxis, FMath::DegreesToRadians(AngleDegrees));

    // クォータニオンをFRotatorに変換
    FRotator RotationResult = RotationQuat.Rotator();

    return RotationResult;
}

FVector PolygonRotationManager::GetShareFace(int face1, int face2, const FVector& Scale, const FVector& Position)
{
    TArray<int32> Face1 = Faces[face1];//底面
    TArray<int32> Face2 = Faces[face2];//次の面

    TArray<int32> ShareVertices;//共通する頂点

    //頂点の数分ループする
    for (int i = 0; i < Face1.Num(); i++)
    {
        for (int j = 0; j < Face2.Num(); j++)
        {
            //共通する頂点を保存
            if (Face1[i] == Face2[j])
            {
                ShareVertices.Add(Face1[i]);
                continue;
            }
        }
    }

    //頂点座標の合計
    FVector Total = { 0,0,0 };

    //二つの面が持つ共通した頂点の座標の平均が回転の中心になる
    for (int i = 0; i < ShareVertices.Num(); i++)
    {
        Total += GetVertexLocation(Scale, Position, ShareVertices[i]) - Position;
    }

    //このCenterを求めるとこまで同一関数にするフラグはこの関数に入るかの条件にする
    FVector Center = (Total / ShareVertices.Num()) + Position;

    CenterPosition = Center;

    return Center;
}

void PolygonRotationManager::SetNextBottom(FVector Vector, const FVector& Scale, const FVector& Position)
{
    if (!IsRotating && Vector.Length() > 0.) {
        //進行方向ベクトルとベクトルの長さ（最初はめっちゃ長くしてる）
        FVector tmp = Vector * Scale;
        FVector length = tmp * 1000;

        SetBottom(Scale, Position);

        for (int i = 0; i < Faces.Num(); i++)
        {
            if (i == bottom) continue;//自分自身
            //底面と上面とか隣り合わない奴らの場合はスキップ
            if (bottom % 2 == 0)
            {
                if (i == bottom + 1) continue;
            }
            else {
                if (i == bottom - 1) continue;
            }

            FVector len;
            len = GetFaceCenterLocation(Scale, Position, i) - Position;//相対座標
            len = tmp - len;

            //進行方向のベクトルに近い面を次の面とする
            if (length.Length() >= len.Length())
            {
                length = len;

                next = i;
            }
        }

        CalculateRotationDirection(Scale, Position);
        if (FpsCounter == 0) GetShareFace(bottom, next, Scale, Position);
    }
}

void PolygonRotationManager::SetBottom(const FVector& Scale, const FVector& Position)
{
    //底面の番号を選出
    for (int i = 0; i < Faces.Num(); i++)
    {
        FVector tmp;
        tmp = GetFaceCenterLocation(Scale, Position, i);

        //面の座標の中で最も高度が低い奴が底面
        if (tmp.Z <= GetFaceCenterLocation(Scale, Position, bottom).Z)
        {
            bottom = i;
        }
    }
}

void PolygonRotationManager::CalculateRotationDirection(const FVector& Scale, const FVector& Position)
{
    FRotator r = CalculateRotationBetweenVectors(GetFaceCenterLocation(Scale, Position, bottom) - Position,
        GetFaceCenterLocation(Scale, Position, next) - Position);

    NextRotation = r;
}

void PolygonRotationManager::SetNewRotationAndLocation(const FVector& ActorPosition)
{
    //メッシュの回転
    //box->SetRelativeRotation(-1 * rot_test);
    if ((int)NextRotation.Pitch >= 180)
    {
        NextRotation.Pitch = 0.;
    }
    if ((int)NextRotation.Yaw >= 180)
    {
        NextRotation.Yaw = 0.;
    }
    if ((int)NextRotation.Roll >= 180)
    {
        NextRotation.Roll = 0.;
    }

    /*if (abs((int)NextRotation.Pitch) >= 90)
    {
        NextRotation.Pitch = 0.;
    }
    if (abs((int)NextRotation.Yaw) >= 90)
    {
        NextRotation.Yaw = 0.;
    }
    if (abs((int)NextRotation.Roll) >= 90)
    {
        NextRotation.Roll = 0.;
    }*/

    int32 speed = Fps * Speed;

    //30はFPSの数字 -> 変数Fpsに切り替える
    //NowRotation += TestRotation;
    //NowRotation += {NextRotation.Pitch / Fps, NextRotation.Yaw / Fps, NextRotation.Roll / Fps};     //SetActorLocationを使う場合はこっち
    NowRotation = {NextRotation.Pitch / Fps, NextRotation.Yaw / Fps, NextRotation.Roll / Fps};    //AddActorRotationを使う場合はこっち
   
    //NowRotation += {NextRotation.Pitch / speed, NextRotation.Yaw / speed, NextRotation.Roll / speed};
    //とりあえず一秒で回転
    if (FpsCounter++ >= /*speed*/Fps - 1)
    {
        FpsCounter = 0;
        //NowRotation = { 0,0,0 };
    }

    // オブジェクトの現在位置を取得
    FVector ActorLocation = ActorPosition;

    // ピボットポイント（回転中心）からの相対的な位置を計算
    FVector Direction = ActorLocation - CenterPosition;

    // 指定された角度でベクトルを回転
    FQuat QuatRotation = FQuat({ -NextRotation.Pitch / Fps, -NextRotation.Yaw / Fps, -NextRotation.Roll / Fps });	//y z x

    //FQuat QuatRotation = FQuat({ -NextRotation.Pitch / speed, -NextRotation.Yaw / speed, -NextRotation.Roll / speed });	//y z x
    //{1,0,0} pitch {0,1,0} yaw {0,0,1} roll
    
    //回転後の相対座標
    FVector NewDirection = QuatRotation.RotateVector(Direction);

    // 新しい位置を計算（回転後のベクトルにピボットポイントを足す）
    NewLocation = CenterPosition + NewDirection;

    //回転中かの判定
    if (FpsCounter == 0) IsRotating = false;
    else IsRotating = true;
}
