#include "Transmitter.h"

#include <random>
#include <algorithm>
#include <iostream>
#include <cmath>

Transmitter::~Transmitter()
{
}

bool Transmitter::tick(unsigned int &collison_count)
{
	if (retransmissions > 16)
	{
		return false;
	}

	if (curr_time == -1)
	{
		double r = std::rand() % 10000 / 10000.0;
		if (r <= p)
		{
			curr_time = 0;
		}
	}
	
	if (curr_time == 0)
	{
		send_next(collison_count);
	}
	else if (curr_time > 0 && (retransmissions == 0 || (retransmissions > 0 && medium->get_val(entry_point) == 0)))
	{
		curr_time--;
	}

	return true;
}

void Transmitter::send_next(unsigned int &collison_count)
{
	if(to_send == 0 && last_sent == 0)
	{
		to_send = min_packet_time;
	}

	char entry = medium->get_val(entry_point);
	if (to_send > 0 && (entry == 0 || entry == 'x'))
	{
		last_sent = (collision_detected) ? 'x' : ('A' + id);
		medium->insert(entry_point, last_sent);

		to_send--;
	}

	if (medium->get_val(entry_point) == 'x' && last_sent != 'x')
	{
		if (!collision_detected) {
			std::cout << id << ": Collision detected[" << retransmissions << "]! Sending jamming signal" << std::endl;
			collision_detected = true;
		}
	}
	
	if (to_send == 0)
	{
		last_sent = 0;

		if (!collision_detected)
		{
			curr_time = -1;
			retransmissions = 0;
		}
	}

	if (to_send == 0 && collision_detected)
	{
		curr_time = get_retransmission_time();
		collision_detected = false;
		collison_count++;

		std::cout << id << ": Jamming signal sent. Waiting for " << curr_time << " ticks..." << std::endl;
	}
}

int Transmitter::get_retransmission_time()
{
	retransmissions++;
	int wait_iterval_count = rand() % ((int) std::pow(2.0, std::min(retransmissions, 10)));
	return min_packet_time * wait_iterval_count + 1;
}

int Transmitter::get_retransmissions()
{
	return retransmissions;
}
