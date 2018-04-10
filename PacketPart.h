#pragma once

enum PacketPartDir
{
	EMPTY, BOTH, LEFT, RIGHT
};

struct PacketPart
{
	char val;
	PacketPartDir dir;

	PacketPart(char val, PacketPartDir dir) :
		val{ val }, dir{ dir } {}

	void clear() {
		val = 0;
		dir = PacketPartDir::EMPTY;
	}
};

