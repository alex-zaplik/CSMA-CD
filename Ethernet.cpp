#include "Ethernet.h"

#include <iostream>
#include <vector>

#include "PacketPart.h"

Ethernet::Ethernet(int length) : length{ length }
{
	for (int i = 0; i < length; i++)
	{
		cable.push_back(PacketPart(0, PacketPartDir::EMPTY));
	}
}

void Ethernet::propagate()
{
	std::vector<PacketPart> tmp;
	for (int i = 0; i < length; i++)
	{
		tmp.push_back(PacketPart(0, PacketPartDir::EMPTY));
	}

	for (int i = 0; i < length; i++)
	{
		switch (cable[i].dir)
		{
		case PacketPartDir::BOTH:
			if (i < length - 1)
				single_propagate(tmp, i + 1, cable[i].val, PacketPartDir::RIGHT);
			
			if (i > 0)
				single_propagate(tmp, i - 1, cable[i].val, PacketPartDir::LEFT);
			
			break;

		case PacketPartDir::LEFT:
			if (i > 0)
				single_propagate(tmp, i - 1, cable[i].val, PacketPartDir::LEFT);
			
			break;

		case PacketPartDir::RIGHT:
			if (i < length - 1)
				single_propagate(tmp, i + 1, cable[i].val, PacketPartDir::RIGHT);
			
			break;

		default:
			break;
		}
	}

	cable.swap(tmp);
}

void Ethernet::single_propagate(std::vector<PacketPart> &tmp, int i, char val, PacketPartDir dir)
{
	if (tmp[i].val == 0)
	{
		tmp[i].dir = dir;
	}
	else
	{
		tmp[i].dir = PacketPartDir::BOTH;
	}

	tmp[i].val += val;
}

bool Ethernet::insert(int entry_point, char val)
{
	if (entry_point < 0 && entry_point >= length)
	{
		std::cerr << "Expecter a value in [0; " << length - 1 << "]. Got " << entry_point << std::endl;
		return false;
	}
	
	cable[entry_point].val += val;
	cable[entry_point].dir = PacketPartDir::BOTH;

	return true;
}

char Ethernet::get_val(int entry_point)
{
	if (entry_point < 0 && entry_point >= length)
	{
		std::cerr << "Expecter a value in [0; " << length - 1 << "]. Got " << entry_point << std::endl;
		return 0;
	}

	return cable[entry_point].val;
}

void Ethernet::print()
{
	std::cout << "[";

	for (int i = 0; i < length - 1; i++)
	{
		std::cout << clamp_char(cable[i].val) << "; ";
	}
	std::cout << clamp_char(cable[length - 1].val) << "]" << std::endl;
}

char Ethernet::clamp_char(char c)
{
	if ((c > 'z' || c < 'a') && c != 'X' && c != 0) return '.';
	return c;
}
