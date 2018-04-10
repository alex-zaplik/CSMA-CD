#include "Transmitter.h"

#include <random>
#include <algorithm>
#include <iostream>
#include <cmath>

Transmitter::~Transmitter()
{
}

bool Transmitter::tick()
{
	if (retransmissions > 16)
	{
		return false;
	}

	if (curr_time == 0)
	{
		send_next();
	}
	else
	{
		curr_time--;
	}

	if (last_sent != 0 && medium->get_val(entry_point) != last_sent)
	{
		if (!collision_detected) std::cout << id << ": Collision detected[" << retransmissions << "]! Sending jamming signal" << std::endl;
		collision_detected = true;
	}

	if (collision_detected)
	{
		// std::cout << id << ": " << to_send << " left..." << std::endl;
	}
}

void Transmitter::send_next()
{
	if(to_send == 0 && last_sent == 0)
	{
		to_send = min_packet_time * (std::rand() % 5 + 1) - 1;
	}

	if (to_send > 0 && (medium->get_val(entry_point) == 0 || collision_detected))
	{
		last_sent = (collision_detected) ? 'X' : ('a' + (rand() % ('z' - 'a' + 1)));
		medium->insert(entry_point, last_sent);

		to_send--;
	}
	
	if (to_send == 0)
	{
		last_sent = 0;

		if (collision_detected)
		{
			curr_time = get_retransmission_time();
			collision_detected = false;

			std::cout << id << ": Jamming signal sent. Waiting for " << curr_time << " ticks..." << std::endl;
		}
		else
		{
			curr_time = get_wait_time();
			retransmissions = 0;
		}
	}
}

int Transmitter::get_wait_time()
{
	return (int) (min_time * ((double) ((std::rand() % 1000) / 200.0 + 1)));
}

int Transmitter::get_retransmission_time()
{
	retransmissions++;
	int wait_iterval_count = rand() % ((int) std::pow(2.0, std::min(retransmissions, 10)));
	return min_packet_time * wait_iterval_count;
}

int Transmitter::get_retransmissions()
{
	return retransmissions;
}
