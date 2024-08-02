// Fill out your copyright notice in the Description page of Project Settings.


#include "Database.h"

bool UDatabase::RetrieveItem(FName ItemID, FPickup& OutItem)
{
	bool bItemExists = false;

	if (m_Database.Contains(ItemID))
	{
		OutItem = *m_Database.Find(ItemID);
		return true;
	}
	else
	{
		return false;
	}
}
