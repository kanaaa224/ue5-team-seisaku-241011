// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Enemy/3/Enemy3AttackAllCollsion.h"
#include "Game/Player_Cube.h"
#include "Kismet/GameplayStatics.h"
#include "Game/Enemy/3/Enemy3Character.h"
#include "Game/Enemy/3/AIC_Enemy3.h"

// Sets default values
AEnemy3AttackAllCollsion::AEnemy3AttackAllCollsion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* BoxComponentを作成する */
	AttackCollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	/* BoxComponentをルートに設定 */
	RootComponent = AttackCollisionMesh;

	/* コリジョン設定 */
	/* UBoxComponentのコリジョン設定 */
	AttackCollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	/* UBoxComponentのコリジョンチャンネル（種類）を動的オブジェクトとして設定 */
	AttackCollisionMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	/* UBoxComponentのすべてのコリジョンチャンネルに対する反応を一括で無視に設定 */
	AttackCollisionMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	/* UBoxComponentのPawnチャンネルに対してOverlapが発動する */
	//AttackCollisionMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	/* Enemy3Attack用のCollision OnComponentBeginOverlapをBindする */
	AttackCollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemy3AttackAllCollsion::OnBoxBeginOverlap);
	/* Enemy3Attack用のCollision OnComponentEndOverlapをBindする */
	AttackCollisionMesh->OnComponentEndOverlap.AddDynamic(this, &AEnemy3AttackAllCollsion::OnBoxEndOverlap);
}

// Called when the game starts or when spawned
void AEnemy3AttackAllCollsion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy3AttackAllCollsion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy3AttackAllCollsion::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("Enemy3 Beam OnBoxBeginOverlap"));

	/* OtherActorに変なものが入っていないか確認 */
	if (OtherActor)
	{
		/* 引数がCaracter型かどうか確認 違うならnullptr */
		ACharacter* character = Cast<ACharacter>(OtherActor);
		if (character)
		{
			/* characterがPlayerであるかどうかを判別 */
			if (character->GetController() && character->GetController()->IsPlayerController())
			{
				//UE_LOG(LogTemp, Warning, TEXT("Enemy3 BeamAttack <Player Character overlapped!>"));

				/* デフォルトのダメージタイプを使用 */
				TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass();
				/* ダメージを引き起こした側のコントローラー Controllerが間接的に取得できないため直接取得 */
				AController* AIController = character->GetController();
				/* ダメージの原因となったオブジェクト */
				AEnemy3Character* Enemy3 = nullptr;

				/* TargetActor */
				/* ダメージを受ける対象（AActor*）。ここではPlayerや敵キャラクターなど。*/
				/* DamageAmount */
				/* 与えるダメージ量（float）。*/
				/* InstigatorController */
				/*  ダメージを引き起こした側のコントローラー（AController*）。通常は攻撃者のコントローラー。*/
				/* DamageCauser */
				/* ダメージの原因となったオブジェクト（AActor*）。攻撃者や武器など。*/
				/* DamageTypeClass */
				/* ダメージの種類（TSubclassOf<UDamageType>）。ダメージの追加情報を提供するために使用します。*/
				UGameplayStatics::ApplyDamage(character, 10.0f, AIController, Enemy3, DamageTypeClass);
			}
		}
	}
}

void AEnemy3AttackAllCollsion::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}