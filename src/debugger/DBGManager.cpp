//
// DBGManager.cpp: Debugger information manager
//
// by Jean-Paul Mari
//
// JPM = Jean-Paul Mari <djipi.mari@gmail.com>
//
// WHO  WHEN        WHAT
// ---  ----------  ------------------------------------------------------------
// JPM  12/21/2016  Created this file
// JPM              Various efforts to set the ELF format support
// JPM              Various efforts to set the DWARF format support

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "libelf/libelf.h"
#include "libelf/gelf.h"
#include "log.h"
#include "ELFManager.h"
#include "DwarfManager.h"
#include "DBGManager.h"
#include "HWLABELManager.h"
#include "settings.h"
#include "memory.h"


//
struct Value
{
	union
	{
		char C[10];
		double D;
		float F;
		int32_t SI;
		int64_t SL;
		uint32_t UI;
		uint64_t UL;
	};
}S_Value;


// Common debugger variables
size_t	DBGType;
char value[1000];


// Common debugger initialisation
void DBGManager_Init(void)
{
	DBGType = DBG_NO_TYPE;
	ELFManager_Init();
	DWARFManager_Init();
}


// Common debugger reset
void DBGManager_Reset(void)
{
	if ((DBGType & DBG_DWARF))
	{
		DWARFManager_Reset();
	}

	if ((DBGType & DBG_ELF))
	{
		ELFManager_Reset();
	}

	//DBGType = vjs.displayHWlabels ? DBG_HWLABEL : DBG_NO_TYPE;
	DBGType = DBG_NO_TYPE;
}


// Get debugger type
size_t DBGManager_GetType(void)
{
	return DBGType;
}


// Common debugger set
void DBGManager_SetType(size_t DBGTypeSet)
{
	DBGType |= DBGTypeSet;
}


// Common debugger close
void DBGManager_Close(void)
{
	if ((DBGType & DBG_DWARF))
	{
		DWARFManager_Close();
	}

	if ((DBGType & DBG_ELF))
	{
		ELFManager_Close();
	}
}


// Get source filename based on the memeory address
// return NULL if no source filename
char *DBGManager_GetFullSourceFilenameFromAdr(size_t Adr, bool *Error)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetFullSourceFilenameFromAdr(Adr, Error);
	}
	else
	{
		return	NULL;
	}
}


// Get number of local variables
// Return 0 if none has been found
size_t DBGManager_GetNbLocalVariables(size_t Adr)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetNbLocalVariables(Adr);
	}
	else
	{
		return	0;
	}
}


// Get number of global variables
// Return 0 if none has been found
size_t DBGManager_GetNbGlobalVariables(void)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetNbGlobalVariables();
	}
	else
	{
		return	0;
	}
}


// Get address from symbol name
// Return found address
// Return NULL if no symbol has been found
size_t DBGManager_GetAdrFromSymbolName(char *SymbolName)
{
	if (SymbolName)
	{
		if ((DBGType & DBG_ELF))
		{
			return ELFManager_GetAdrFromSymbolName(SymbolName);
		}
	}

	return 0;
}


// Get global variable's Address based on his Name
// Return found Address
// Return NULL if no Address has been found
size_t DBGManager_GetGlobalVariableAdrFromName(char *VariableName)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetGlobalVariableAdrFromName(VariableName);
	}
	else
	{
		return 0;
	}
}


// Get local variable's type encoding based on his address and Index
// Return the type encoding found
// Return 0 if no type encoding has been found
size_t DBGManager_GetLocalVariableTypeEncoding(size_t Adr, size_t Index)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetLocalVariableTypeEncoding(Adr, Index);
	}
	else
	{
		return	0;
	}
}


//
int DBGManager_GetLocalVariableOffset(size_t Adr, size_t Index)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetLocalVariableOffset(Adr, Index);
	}
	else
	{
		return	0;
	}
}


// Get local variable's type byte size based on his address and Index
// Return the type's byte size found
// Return 0 if no type's byte size has been found
size_t DBGManager_GetLocalVariableTypeByteSize(size_t Adr, size_t Index)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetLocalVariableTypeByteSize(Adr, Index);
	}
	else
	{
		return	0;
	}
}


//
size_t DBGManager_GetLocalVariableTypeTag(size_t Adr, size_t Index)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetLocalVariableTypeTag(Adr, Index);
	}
	else
	{
		return	0;
	}
}


//
size_t DBGManager_GetGlobalVariableTypeTag(size_t Index)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetGlobalVariableTypeTag(Index);
	}
	else
	{
		return	0;
	}
}


// Get global variable's type name based on his Index
// Return type name's text pointer found
// Return NULL if no type name has been found
char *DBGManager_GetGlobalVariableTypeName(size_t Index)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetGlobalVariableTypeName(Index);
	}
	else
	{
		return	NULL;
	}
}


// Get global variable's Address based on his Index
// Return the Address found
// Return 0 if no Address has been found
size_t DBGManager_GetGlobalVariableAdr(size_t Index)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetGlobalVariableAdr(Index);
	}
	else
	{
		return	0;
	}
}


// Get global variable's type byte size based on his Index
// Return the type's byte size found
// Return 0 if no type's byte size has been found
size_t DBGManager_GetGlobalVariableTypeByteSize(size_t Index)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetGlobalVariableTypeByteSize(Index);
	}
	else
	{
		return	0;
	}
}


// Get global variable's type encoding based on his Index
// Return the type encoding found
// Return 0 if no type encoding has been found
size_t DBGManager_GetGlobalVariableTypeEncoding(size_t Index)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetGlobalVariableTypeEncoding(Index);
	}
	else
	{
		return	0;
	}
}


// Get global variable value based on his Index
// Return value as a text pointer
// Note: Pointer may point on a 0 lenght text
char *DBGManager_GetGlobalVariableValue(size_t Index)
{
	size_t Adr = 0;
	size_t TypeEncoding = DBG_NO_TYPEENCODING;
	size_t TypeByteSize = 0;

	if ((DBGType & DBG_ELFDWARF))
	{
		Adr = DWARFManager_GetGlobalVariableAdr(Index);
		TypeEncoding = DWARFManager_GetGlobalVariableTypeEncoding(Index);
		TypeByteSize = DWARFManager_GetGlobalVariableTypeByteSize(Index);
	}

	return DBGManager_GetVariableValueFromAdr(Adr, TypeEncoding, TypeByteSize);
}


// Get variable value based on his Adresse, Encoding Type and Size
// Return value as a text pointer
// Note: Pointer may point on a 0 lenght text if Adress is NULL
char *DBGManager_GetVariableValueFromAdr(size_t Adr, size_t TypeEncoding, size_t TypeByteSize)
{
	Value V;
	char *Ptrvalue = value;

	value[0] = 0;

	if (Adr)
	{
		memset(&V, 0, sizeof(Value));
#if 0
		for (uint32_t i = 0; i < TypeByteSize; i++)
			jaguarMainRAM[Adr + i] = 0;
			//jaguarMainRAM[Adr + i] = rand();
		jaguarMainRAM[Adr + TypeByteSize - 1] = 0x10;
#endif
#if 1
		for (size_t i = 0, j = TypeByteSize; i < TypeByteSize; i++, j--)
		{
			V.C[i] = jaguarMainRAM[Adr + j - 1];
		}
#endif

		switch (TypeEncoding)
		{
		case DBG_ATE_address:
			break;

		case DBG_ATE_boolean:
			break;

		case DBG_ATE_complex_float:
			break;

		case DBG_ATE_float:
			switch (TypeByteSize)
			{
			case 4:
				sprintf(value, "%F", V.F);
				break;

			case 8:
				//V.D = (double)jaguarMainRAM[Adr];
				//sprintf(value, "%10.10F", V.D);
				sprintf(value, "%F", V.D);
				break;

			default:
				break;
			}
			break;

		case DBG_ATE_signed:
			switch (TypeByteSize)
			{
			case 4:
				sprintf(value, "%i", V.SI);
				break;

			case 8:
				sprintf(value, "%i", V.SL);
				break;

			default:
				break;
			}
			break;

		case DBG_ATE_signed_char:
			break;

		case DBG_ATE_unsigned:
			switch (TypeByteSize)
			{
			case 4:
				sprintf(value, "%u", V.UI);
				break;

			case 8:
				sprintf(value, "%u", V.UL);
				break;

			default:
				break;
			}
			break;

		case DBG_ATE_unsigned_char:
			break;

		case DBG_ATE_ptr:
			switch (TypeByteSize)
			{
			case 4:
				sprintf(value, "0x%06x", V.UI);
				break;

			default:
				break;
			}

		default:
			break;
		}
	}

	return Ptrvalue;
}


// Get local variable's type name based on his Index
// Return type name's text pointer found
// Return NULL if no type name has been found
char *DBGManager_GetLocalVariableTypeName(size_t Adr, size_t Index)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetLocalVariableTypeName(Adr, Index);
	}
	else
	{
		return	NULL;
	}
}


// Get local variable Op based on his Index
// Return variable Op's found
// Return 0 if no variable Op has been found
size_t DBGManager_GetLocalVariableOp(size_t Adr, size_t Index)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetLocalVariableOp(Adr, Index);
	}
	else
	{
		return	0;
	}
}


// Get local variable name based on his Index
// Return variable name's text pointer found
// Return NULL if no variable name has been found
char *DBGManager_GetLocalVariableName(size_t Adr, size_t Index)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetLocalVariableName(Adr, Index);
	}
	else
	{
		return	NULL;
	}
}


// Get global variable name based on his Index
// Return variable name's text pointer found
// Return NULL if no variable name has been found
char *DBGManager_GetGlobalVariableName(size_t Index)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetGlobalVariableName(Index);
	}
	else
	{
		return	NULL;
	}
}


// Get function name from address
// Return function name found
// Return NULL if no function name has been found
char *DBGManager_GetFunctionName(size_t Adr)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetFunctionName(Adr);
	}
	else
	{
		return	NULL;
	}
}


// Get line number from address and his tag
// Return line number on the symbol name found
// Return 0 if no symbol name has been found
size_t DBGManager_GetNumLineFromAdr(size_t Adr, size_t Tag)
{
	if ((DBGType & DBG_ELFDWARF))
	{
		return DWARFManager_GetNumLineFromAdr(Adr, Tag);
	}
	else
	{
		return	0;
	}
}


// Get symbol name from address
// Return text pointer on the symbol name found
// Return NULL if no symbol name has been found
char *DBGManager_GetSymbolNameFromAdr(size_t Adr)
{
	char *Symbolname;

	//if ((DBGType & DBG_HWLABEL) || vjs.displayHWlabels)
	if (vjs.displayHWlabels)
	{
		Symbolname = HWLABELManager_GetSymbolnameFromAdr(Adr);
	}
	else
	{
		Symbolname = NULL;
	}
#ifdef _MSC_VER
#pragma message("Warning: !!! Need to set the DBG_HWLABEL in DBGType instead to use the setting value !!!")
#else
	#warning "!!! Need to set the DBG_HWLABEL in DBGType instead to use the setting value !!!"
#endif // _MSC_VER

	if (Symbolname == NULL)
	{
		if ((DBGType & DBG_ELFDWARF))
		{
			Symbolname = DWARFManager_GetSymbolnameFromAdr(Adr);
		}

		if ((DBGType & DBG_ELF) && (Symbolname == NULL))
		{
			Symbolname = ELFManager_GetSymbolnameFromAdr(Adr);
		}
	}

	return	Symbolname;
}


// Get source line based on the Address and his Tag
// Return text pointer on the source line found
// Return NULL if no source line has been found
char *DBGManager_GetLineSrcFromAdr(size_t Adr, size_t Tag)
{
	char *Symbolname = NULL;

	if ((DBGType & DBG_ELFDWARF))
	{
		Symbolname = DWARFManager_GetLineSrcFromAdr(Adr, Tag);
	}

	return	Symbolname;
}


// Get text line from source based on address and num line (starting by 1)
// Return NULL if no text line has been found
char *DBGManager_GetLineSrcFromAdrNumLine(size_t Adr, size_t NumLine)
{
	char *Symbolname = NULL;

	if ((DBGType & DBG_ELFDWARF))
	{
		Symbolname = DWARFManager_GetLineSrcFromAdrNumLine(Adr, NumLine);
	}

	return	Symbolname;
}


// Get text line from source based on address and num line (starting by 1)
// Return NULL if no text line has been found
char *DBGManager_GetLineSrcFromNumLineBaseAdr(size_t Adr, size_t NumLine)
{
	char *Symbolname = NULL;

	if ((DBGType & DBG_ELFDWARF))
	{
		Symbolname = DWARFManager_GetLineSrcFromNumLineBaseAdr(Adr, NumLine);
	}

	return	Symbolname;
}

