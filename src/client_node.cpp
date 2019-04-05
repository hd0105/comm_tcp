#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "std_msgs/String.h"
#include <std_msgs/Float64.h> //메세지를 보내는 형태
#include <std_msgs/Float64MultiArray.h> //메세지를 보내는 형태

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

using namespace std;

int cnt = 0;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

class Listener {
private:
    char topic_message[BUFSIZE] = { 0 };
public:
    void callback(const std_msgs::String::ConstPtr& msg);
    char* getMessageValue();
};

void Listener::callback(const std_msgs::String::ConstPtr& msg) {
    memset(topic_message, 0, BUFSIZE+1);
    strcpy(topic_message, msg->data.c_str());
    // ROS_INFO("I heard:[%s]\n", msg->data.c_str());
}

char* Listener::getMessageValue() {
    return topic_message;
}

int main (int argc, char** argv)
{
  argc = 0;
  argv = NULL;
  int retval;
  Listener listener;
  ros::init(argc, argv, "server_node");
  ros::NodeHandle nh;
  ros::Publisher server_pub = nh.advertise<std_msgs::String>("/server_messages/", 1000);
  ros::Subscriber server_sub = nh.subscribe("slave_messages", 1, &Listener::callback, &listener);

  // Socket Create
  int client_socket;
  // int sockfd, newsockfd, portno; //Socket file descriptors and port number
  // client_socket = socket(AF_INET, SOCK_STREAM, 0); // TCP
  client_socket = socket(PF_INET, SOCK_DGRAM, 0); // UDP
  if (client_socket < 0){
    error("ERROR opening socket");
  }
  else {
    printf("socket create success\n");
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = PF_INET;
  server_addr.sin_addr.s_addr = inet_addr(SERVERIP);
  server_addr.sin_port = htons(SERVERPORT);

  // Data communication
  socklen_t addrlen;
  char buf_read[BUFSIZE+1];
  std_msgs::String message;
  std::stringstream ss;

  char udpsend[20] = "hello! udp\n";

  while(ros::ok()) {
    // data write
    sendto(client_socket, udpsend, sizeof(udpsend), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    // retval = write(cli_socket, slave_buf, SLAVEBUF);
    // if (retval < 0) {
		// 	printf("data send failed! %d\n",cnt);
		// }
		// else {
		// 	// printf("[Slave] %d byte send!\n", retval);
    //   printf("[Slave] %s\n", slave_buf);
		// }
    ss.str(std::string()); //Clear contents of string stream
    bzero(buf_read, BUFSIZE+1);
    addrlen = sizeof(server_addr);
    retval = recvfrom(client_socket, buf_read, BUFSIZE, 0, (struct sockaddr *)&server_addr, &addrlen);
    // retval = read(cli_socket, buf, BUFSIZE);
    printf("recv success? retval = %d\n", retval);
    // if (retval < 0) {
    // 	printf("data receive failed!");
    // 	// break;
    // }
    // else if (retval == 0) {
    // 	printf("retval == 0\n");
    // 	// break;
    // }
    buf_read[retval] = '\0';
    // printf("[Master/%s:%d] %s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), buf);
    // printf("Here is the message: %s\n",buf);
    ss << buf_read;
    message.data = ss.str();
    // ROS_INFO("%s", message.data.c_str());
    server_pub.publish(message);
    ros::spinOnce();
  }
  close(client_socket);
  return 0;
}
