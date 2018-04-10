#pragma once

#include "Ethernet.h"

class Transmitter
{
private:
	int id;

	Ethernet * medium;
	int entry_point;

	int min_packet_time;
	int min_time;
	int curr_time = 0;

	int to_send = 0;
	char last_sent = 0;

	bool collision_detected = false;
	int retransmissions = 0;

	void send_next();
	int get_wait_time();
	int get_retransmission_time();

public:
	Transmitter(int id, Ethernet* medium, int entry_point, int min_packet_time, int avg_time) :
		id{ id }, medium{ medium }, entry_point{ entry_point },
		min_packet_time{ min_packet_time }, min_time{ avg_time }, curr_time{ get_wait_time() } {}
	~Transmitter();

	bool tick();
	int get_retransmissions();
};

