#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>

#include "PacketPart.h"
#include "Ethernet.h"
#include "Transmitter.h"

int main() {
	using namespace std::chrono_literals;

	int max_retransmissions = 0;

	int length = 40;
	int min_wait = length;
	int extra_wait = (int) (0.25 * length);

	Ethernet* medium = new Ethernet(length);
	bool network_fine = true;

	std::vector<Transmitter> transmitters;
	transmitters.push_back(Transmitter(0, medium, 0, length * 2, min_wait + rand() % extra_wait));
	// transmitters.push_back(Transmitter(1, medium, length / 2, length * 2, min_wait + rand() % extra_wait));
	transmitters.push_back(Transmitter(2, medium, length - 1 , length * 2, min_wait + rand() % extra_wait));

	medium->print();

	int i;
	for (i = 0; i < 1000000 && network_fine; i++)
	{
		// std::cin.get();

		medium->propagate();

		for (auto it = transmitters.begin(); it != transmitters.end(); it++)
		{
			if (!(*it).tick())
			{
				std::cout << "Network error!" << std::endl;
				network_fine = false;
				break;
			}

			max_retransmissions = std::max(max_retransmissions, (*it).get_retransmissions());
		}

		medium->print();

		std::this_thread::sleep_for(25ms);
	}

	std::cout << "\n\nMaximum number of retransmissions: " << max_retransmissions << std::endl;
	std::cout << "Iterations done: " << i << std::endl;
	std::cin.get();

	delete medium;
}