#include "Memory.h"

double Memory::ReadDouble(int* address)
{
	return ReadCacheLine(address).d_data[GetQWORDOffset(address)];
}

void Memory::WriteDouble(int* address, double value)
{
	auto line = ReadCacheLine(address);

	line.address |= DIRTYMASK;
	line.d_data[GetQWORDOffset(address)] = value;

	Write(address, line);
}

long long Memory::ReadLong(int* address)
{
	return ReadCacheLine(address).l_data[GetQWORDOffset(address)];
}

void Memory::WriteLong(int* address, long long value)
{
	auto line = ReadCacheLine(address);

	line.address |= DIRTYMASK;
	line.l_data[GetQWORDOffset(address)] = value;

	Write(address, line);
}

uint Memory::ReadUInt(int* address)
{
	return ReadCacheLine(address).ui_data[GetDWORDOffset(address)];
}

void Memory::WriteUInt(int* address, uint value)
{
	auto line = ReadCacheLine(address);

	line.address |= DIRTYMASK;
	line.ui_data[GetDWORDOffset(address)] = value;

	Write(address, line);
}

// Retrieve an int from the RAM/Cache.
// Reads a cache line, and retrieves the value at the offset of the address from that line's data
int Memory::ReadInt(int * address)
{
	return ReadCacheLine(address).i_data[GetDWORDOffset(address)];
}

void Memory::WriteInt(int * address, int value)
{
	auto line = ReadCacheLine(address);

	line.address |= DIRTYMASK;
	line.i_data[GetDWORDOffset(address)] = value;

	Write(address, line);
}

float Memory::ReadFloat(int* address)
{
	return ReadCacheLine(address).f_data[GetDWORDOffset(address)];
}

void Memory::WriteFloat(int* address, float value)
{
	auto line = ReadCacheLine(address);

	line.address |= DIRTYMASK;
	line.f_data[GetDWORDOffset(address)] = value;

	Write(address, line);
}

int Memory::ReadShort(int* address)
{
	return ReadCacheLine(address).s_data[GetWORDOffset(address)];
}

void Memory::WriteShort(int* address, short value)
{
	auto line = ReadCacheLine(address);

	line.address |= DIRTYMASK;
	line.s_data[GetWORDOffset(address)] = value;

	Write(address, line);
}

char Memory::ReadChar(int* address)
{
	return ReadCacheLine(address).c_data[GetByteOffset(address)];
}

void Memory::WriteChar(int* address, char value)
{
	auto line = ReadCacheLine(address);

	line.address |= DIRTYMASK;
	line.c_data[GetByteOffset(address)] = value;

	Write(address, line);
}

byte Memory::ReadByte(int* address)
{
	return ReadCacheLine(address).b_data[GetByteOffset(address)];
}

void Memory::WriteByte(int* address, byte value)
{
	auto line = ReadCacheLine(address);

	line.address |= DIRTYMASK;
	line.b_data[GetByteOffset(address)] = value;

	Write(address, line);
}