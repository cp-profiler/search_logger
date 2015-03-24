#include <zmq.hpp>
#include <string>
#include <iostream>
#include <cstdio>
#include <iomanip>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

using std::setw;

enum MsgType {
  NODE_DATA = 1,
  DONE_SENDING = 2,
  START_SENDING = 3
};

struct Message {
  static const int LABEL_SIZE = 32;

  MsgType type;
  int sid;
  int parent;
  int alt;
  int kids;
  int status;
  int restart_id;
  unsigned long long time;
  char thread;
  char label[LABEL_SIZE];
  float domain;
};

int main() {

	zmq::context_t context(1);
	zmq::socket_t receiver (context, ZMQ_PULL);
	receiver.bind("tcp://*:6565");
  std::cout << "listening to port: 6565\n";
	int total = 0;

  char raw_data[80] = {0};

	while (true) {
	
    zmq::message_t msg;
  
		receiver.recv(&msg);

    // std::memcpy(raw_data, msg.data(), sizeof(msg.data()));
    std::memcpy(raw_data, msg.data(), 80);
		Message *tr = reinterpret_cast<Message*>(msg.data());

    // std::cout << "raw: ";

    // for (int i = 0; i < 80; i++)
    // {
    //     if (i > 0) printf(":");
    //     printf("%02X", raw_data[i]);
    // }

    // std::cout << std::endl;

		switch (tr->type) {
			case NODE_DATA:
				std::cout << std::left << "Node: " << setw(8) << tr->sid << " " << setw(8) << tr->parent
                  << " " << setw(2) << tr->alt << " " << tr->kids << " " << tr->status
                  << "  thread: " << setw(2) << (int)tr->thread 
                  << "  restart: " << setw(2) << static_cast<int>(tr->restart_id)
                  << "  time: " << setw(9) << tr->time
                  << "  domain: " << setw(6) << std::setprecision(4) << tr->domain
                  << "  label: " << tr->label << std::endl;
			break;
			case DONE_SENDING:
				std::cout << "Done receiving\n";
			break;
      case START_SENDING:
        std::cout << "Start recieving, restart: " << tr->restart_id << "\n";
      break;
		}
			
	}
	return 0;
}