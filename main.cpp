#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <ctime>

#include "PacketPart.h"
#include "Ethernet.h"
#include "Transmitter.h"

int main() {
	int length = 75;
	double prob = 1.0 / length;
	int packet_length = (int)(length * 2.0);

	using namespace std::chrono_literals;

	std::srand(unsigned(std::time(0)));
	int max_retransmissions = 0;
	bool network_fine = true;

	Ethernet* medium = new Ethernet(length);
	std::vector<Transmitter> transmitters;
	transmitters.push_back(Transmitter(0, medium, 0,          prob, packet_length));
	// transmitters.push_back(Transmitter(1, medium, length / 2, prob, packet_length));
	transmitters.push_back(Transmitter(2, medium, length - 1, prob, packet_length));

	// medium->print();

	unsigned int collison_count = 0;
	int i;
	for (i = 0; i < 2000000 && network_fine; i++)
	{
		// std::cin.get();

		medium->propagate();

		for (auto it = transmitters.begin(); it != transmitters.end(); it++)
		{
			if (!(*it).tick(collison_count))
			{
				std::cout << "Network error!" << std::endl;
				network_fine = false;
				break;
			}

			max_retransmissions = std::max(max_retransmissions, (*it).get_retransmissions());
		}

		medium->print();
		std::this_thread::sleep_for(50ms);
	}

	std::cout << "\n\nMaximum number of retransmissions: " << max_retransmissions << std::endl;
	std::cout << "Iterations done: " << i << std::endl;
	std::cout << "Collision count: " << collison_count << std::endl;
	std::cin.get();

	delete medium;
}