# ESPservoturret
Little project using ESP's and arduino to control servos wirelessly over the summer

It aims to track user movement and point a laser accurately in the direction the controller is facing by using an orientation sensor and to use its acceleration and gyroscopic features

Using a Firebeetle2, NodeMCU-32S to communicate with each other and arduino nano to get and clean data from input sensor (just in case theres not enough space on the ESP's)

Circuit 1 is interfacing the Nano and NodeMCU with the orientation sensor. The NodeMCU would sent UDP or QUIC datagrams to Circuit 2

Circuit 2 involves the Firebeetle2 alongside 2 servo motors to act as a turret with 180 degrees of horizontal and vertical freedom

Future implementations are to integrate the ESP-CAM to circuit 2 so I can use a gesture based command to switch between orientation sensor and the ESP-CAM.

Also another option is to add in a machine learning algorithm to ESP-CAM but as there would not be enough memory to store it, can use its Wi-Fi capabilities to send it to a cloud server so the algorithm can be used there
