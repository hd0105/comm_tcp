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

#define SERVERPORT 9000
#define BUFSIZE 512
#define SLAVEBUF 180
#define REBUFSIZE 9
#define SLAVEDATA 18
#define BUFHEAD 1

using namespace std;

char slave_x[REBUFSIZE] = "";
char X_A[BUFHEAD] = { 'A' };
char slave_y[REBUFSIZE] = "";
char Y_B[BUFHEAD] = { 'B' };
char slave_z[REBUFSIZE] = "";
char Z_C[BUFHEAD] = { 'C' };
char slave_xyz[SLAVEBUF] = "";
char slave_buf[SLAVEBUF] = "";

char CON_X[BUFHEAD] = { 'A' };
char CON_Y[BUFHEAD] = { 'B' };
char CON_Z[BUFHEAD] = { 'C' };
char TH_0[BUFHEAD] = { 'D' };
char TH_1[BUFHEAD] = { 'E' };
char TH_2[BUFHEAD] = { 'F' };
char TH_3[BUFHEAD] = { 'G' };
char TH_4[BUFHEAD] = { 'H' };
char TH_5[BUFHEAD] = { 'I' };

char TAR_X[BUFHEAD] = { 'A' };
char TAR_Y[BUFHEAD] = { 'B' };
char TAR_Z[BUFHEAD] = { 'C' };
char TAR_OX[BUFHEAD] = { 'D' };
char TAR_OY[BUFHEAD] = { 'E' };
char TAR_OZ[BUFHEAD] = { 'F' };
char PRE_X[BUFHEAD] = { 'G' };
char PRE_Y[BUFHEAD] = { 'H' };
char PRE_Z[BUFHEAD] = { 'I' };
char PRE_OX[BUFHEAD] = { 'J' };
char PRE_OY[BUFHEAD] = { 'K' };
char PRE_OZ[BUFHEAD] = { 'L' };
char PRE_X1[BUFHEAD] = { 'M' };
char PRE_Y1[BUFHEAD] = { 'N' };
char PRE_Z1[BUFHEAD] = { 'O' };
char PRE_OX1[BUFHEAD] = { 'P' };
char PRE_OY1[BUFHEAD] = { 'Q' };
char PRE_OZ1[BUFHEAD] = { 'R' };

char slave_contact_x[REBUFSIZE] = "";
char slave_contact_y[REBUFSIZE] = "";
char slave_contact_z[REBUFSIZE] = "";
char slave_th_j0[REBUFSIZE] = "";
char slave_th_j1[REBUFSIZE] = "";
char slave_th_j2[REBUFSIZE] = "";
char slave_th_j3[REBUFSIZE] = "";
char slave_th_j4[REBUFSIZE] = "";
char slave_th_j5[REBUFSIZE] = "";

char AmpX[REBUFSIZE] = "";
char AmpY[REBUFSIZE] = "";
char AmpZ[REBUFSIZE] = "";
char tarOX[REBUFSIZE] = "";
char tarOY[REBUFSIZE] = "";
char tarOZ[REBUFSIZE] = "";
char now_x[REBUFSIZE] = "";
char now_y[REBUFSIZE] = "";
char now_z[REBUFSIZE] = "";
char now_a_x[REBUFSIZE] = "";
char now_a_y[REBUFSIZE] = "";
char now_a_z[REBUFSIZE] = "";
char now_x1[REBUFSIZE] = "";
char now_y1[REBUFSIZE] = "";
char now_z1[REBUFSIZE] = "";
char now_a_x1[REBUFSIZE] = "";
char now_a_y1[REBUFSIZE] = "";
char now_a_z1[REBUFSIZE] = "";

int cnt = 0;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

class Listener {
private:
    double topic_message[18] = { 0 };
public:
    void callback(const std_msgs::Float64MultiArray &msg);
};

void Listener::callback(const std_msgs::Float64MultiArray &msg) {
    memset(topic_message, 0, SLAVEDATA+1);
    memset(slave_xyz, 0, BUFSIZE+1);
    for(int i = 0; i < SLAVEDATA; i++) {
      topic_message[i] = msg.data[i];
    }
    sprintf(AmpX, "%f", topic_message[0]);
    sprintf(AmpY, "%f", topic_message[1]);
    sprintf(AmpZ, "%f", topic_message[2]);
    sprintf(tarOX, "%f", topic_message[3]);
    sprintf(tarOY, "%f", topic_message[4]);
    sprintf(tarOZ, "%f", topic_message[5]);
    sprintf(now_x, "%f", topic_message[6]);
    sprintf(now_y, "%f", topic_message[7]);
    sprintf(now_z, "%f", topic_message[8]);
    sprintf(now_a_x, "%f", topic_message[9]);
    sprintf(now_a_y, "%f", topic_message[10]);
    sprintf(now_a_z, "%f", topic_message[11]);
    sprintf(now_x1, "%f", topic_message[12]);
    sprintf(now_y1, "%f", topic_message[13]);
    sprintf(now_z1, "%f", topic_message[14]);
    sprintf(now_a_x1, "%f", topic_message[15]);
    sprintf(now_a_y1, "%f", topic_message[16]);
    sprintf(now_a_z1, "%f", topic_message[17]);
    strncat(slave_xyz, TAR_X, BUFHEAD);
    strncat(slave_xyz, AmpX, REBUFSIZE);
    strncat(slave_xyz, TAR_Y, BUFHEAD);
    strncat(slave_xyz, AmpY, REBUFSIZE);
    strncat(slave_xyz, TAR_Z, BUFHEAD);
    strncat(slave_xyz, AmpZ, REBUFSIZE);
    strncat(slave_xyz, TAR_OX, BUFHEAD);
    strncat(slave_xyz, tarOX, REBUFSIZE);
    strncat(slave_xyz, TAR_OY, BUFHEAD);
    strncat(slave_xyz, tarOY, REBUFSIZE);
    strncat(slave_xyz, TAR_OZ, BUFHEAD);
    strncat(slave_xyz, tarOZ, REBUFSIZE);
    strncat(slave_xyz, PRE_X, BUFHEAD);
    strncat(slave_xyz, now_x, REBUFSIZE);
    strncat(slave_xyz, PRE_Y, BUFHEAD);
    strncat(slave_xyz, now_y, REBUFSIZE);
    strncat(slave_xyz, PRE_Z, BUFHEAD);
    strncat(slave_xyz, now_z, REBUFSIZE);
    strncat(slave_xyz, PRE_OX, BUFHEAD);
    strncat(slave_xyz, now_a_x, REBUFSIZE);
    strncat(slave_xyz, PRE_OY, BUFHEAD);
    strncat(slave_xyz, now_a_y, REBUFSIZE);
    strncat(slave_xyz, PRE_OZ, BUFHEAD);
    strncat(slave_xyz, now_a_z, REBUFSIZE);
    strncat(slave_xyz, PRE_X1, BUFHEAD);
    strncat(slave_xyz, now_x1, REBUFSIZE);
    strncat(slave_xyz, PRE_Y1, BUFHEAD);
    strncat(slave_xyz, now_y1, REBUFSIZE);
    strncat(slave_xyz, PRE_Z1, BUFHEAD);
    strncat(slave_xyz, now_z1, REBUFSIZE);
    strncat(slave_xyz, PRE_OX1, BUFHEAD);
    strncat(slave_xyz, now_a_x1, REBUFSIZE);
    strncat(slave_xyz, PRE_OY1, BUFHEAD);
    strncat(slave_xyz, now_a_y1, REBUFSIZE);
    strncat(slave_xyz, PRE_OZ1, BUFHEAD);
    strncat(slave_xyz, now_a_z1, REBUFSIZE);
    strncpy(slave_buf, slave_xyz, SLAVEBUF);
    // ROS_INFO("I heard:[%s]\n", slave_buf);
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
  int server_socket;
  server_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); // UDP
  if (server_socket < 0){
    error("ERROR opening socket");
  }
  else {
    printf("socket create success\n");
  }

  // Bind
  struct sockaddr_in serveraddr;
  memset(&serveraddr, 0, sizeof(serveraddr));
  serveraddr.sin_family = PF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons(SERVERPORT);
  retval = bind(server_socket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
  if (retval < 0){
    error("ERROR binding failed");
  }
  else {
    printf("bind success!\n");
  }

  // Data communication
  struct sockaddr_in cliaddr;
  int cli_socket;
  int k = 0;
  socklen_t addrlen;
  char buf[BUFSIZE+1];
  char buf_read[BUFSIZE+1];
  std_msgs::String message;
  std::stringstream ss;

  // char udpsend[20] = "hello! udp\n";

  ros::Rate loop_rate(1000);

  while(ros::ok()) {

    while(ros::ok()){
      addrlen = sizeof(cliaddr);

      ss.str(std::string()); //Clear contents of string stream
      bzero(buf, BUFSIZE+1);
      retval = recvfrom(server_socket, buf, BUFSIZE, 0, (struct sockaddr *)&cliaddr, &addrlen);

      // printf("recv success? retval = %d\n", retval);
  		if (retval < 0) {
  			printf("data receive failed!");
  			break;
  		}
  		else if (retval == 0) {
  			printf("retval == 0\n");
  			break;
  		}
      buf[retval] = '\0';
  		printf("[Master/%s:%d]\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
      printf("[Master] %d byte recv!\n",retval);
      printf("[Master] %s\n",buf);
      ss << buf;
      message.data = ss.str();
      // ROS_INFO("%s", message.data.c_str());
      server_pub.publish(message);

      // data write
      retval = sendto(server_socket, slave_buf, SLAVEBUF, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
      if (retval < 0) {
  			printf("data send failed!\n");
  		}
  		else {
  			printf("[Slave] %d byte send!\n", retval);
        printf("[Slave] %s\n", slave_buf);
  		}
      loop_rate.sleep();
      ros::spinOnce();
    }
    close(cli_socket);
    printf("[TCP server] client connect terminate : IP Address = %s, PORT = %d \n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
  }

  // server Terminate
  close(server_socket);

  return 0;
}
