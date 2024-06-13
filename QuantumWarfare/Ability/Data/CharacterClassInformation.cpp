// @2023 All rights reversed by Reverse-Alpha Studios

#include "CharacterClassInformation.h"

FCharacterClassDefaultInformation UCharacterClassInformation::GetClassDefaultInformation(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}