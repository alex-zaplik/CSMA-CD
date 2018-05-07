#pragma once

#include <vector>
#include "PacketPart.h"

class Ethernet
{
private:
	int length;
	std::vector<PacketPart> cable;

	void single_propagate(std::vector<PacketPart> &curr, std::vector<PacketPart> &tmp, int i, char val, PacketPartDir dir);

public:
	Ethernet(int length);
	void propagate();
	bool insert(int entry_point, char val);
	char get_val(int entry_point);
	void print();
	char clamp_char(char c);
};

