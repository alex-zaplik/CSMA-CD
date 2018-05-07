#pragma once

#include "Ethernet.h"

class Transmitter
{
private:
	int id;

	Ethernet * medium;
	int entry_point;

	double p;

	int min_packet_time;
	int curr_time = -1;

	int to_send = 0;
	char last_sent = 0;

	bool collision_detected = false;
	int retransmissions = 0;

	void send_next(unsigned int &collison_count);
	int get_retransmission_time();

public:
	Transmitter(int id, Ethernet* medium, int entry_point, double p, int min_packet_time) :
		id{ id }, medium{ medium }, entry_point{ entry_point }, p { p }, min_packet_time{ min_packet_time } {}
	~Transmitter();

	bool tick(unsigned int &collison_count);
	int get_retransmissions();
};

