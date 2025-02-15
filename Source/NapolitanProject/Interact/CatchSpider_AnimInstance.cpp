// Fill out your copyright notice in the Description page of Project Settings.


#include "CatchSpider_AnimInstance.h"

void UCatchSpider_AnimInstance::PlayDamagedMontage()
{
	if (DamagedMontage)
	{
		Montage_Play(DamagedMontage);
	}
}
