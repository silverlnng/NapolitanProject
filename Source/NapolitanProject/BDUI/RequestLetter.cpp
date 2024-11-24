// Fill out your copyright notice in the Description page of Project Settings.


#include "RequestLetter.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void URequestLetter::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Ticket->OnClicked.AddDynamic(this, &URequestLetter::OnTicketClicked);
	
}

void URequestLetter::OnTicketClicked()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this,MuseumLevel,true);
}
