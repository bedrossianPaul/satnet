# Nano Sat Network (NSN)

Author : BEDROSSIAN Paul <paul.bedrossian92@gmail.com>  |  Supervisor : Dr. DHAOU Riadh <br>
Directed by ***IRIT*** & ***CNES***. <br>
Updated at : 29/04/2025

## Description

This repo aim to facilitate the development of network protocols for nano-satelites.

The stack (layer 1 to 3) is based on lwIP (RAW API). But natively on a linux distro you can not change the kernel stack. Please see the documentation to understand how the simulation with lwIP work.

## Overview

***TODO*** Table of content here

## Features

- UDP transport layer

## Work in progress

This section describe the topics currently in development.

- Support of OLSR **(work in progress)**

- Support of TCP transport layer

- Subscribe/publish communication **(not implemented)**

- Point-to-point communication **(not implemented)**

- EBP (Efficient broadcast protocol) **(not implemented)**

## Get started

### On Raspberry Pi

- **Step 1** : Select an image of Raspberry Pi OS (previously Raspian) from the [official website](https://www.raspberrypi.com/software/operating-systems/). (N.B: a desktop env. is not requiered).

- **Step 2** : Flash the image on the SD card (16GB minimum) with the following command. If it's compressed you have to uncompress it. ```sudo dd if=path/to/os_image.img of=path/to/sd_card bs=4M status=progress conv=fsync```

- **Step 3** : Launch the utils/setup.sh script.

- **Step 4** : Turn ON the Raspberry Pi with the SD card inside and connect an ethernet cable between the card and your computer.

- **Step 5** : Launch the utils/sharepoint.sh script.

- **Step 6** : Connect with SSH to the card (Username is **admin** and password is **irit2025** )with ```ssh admin@192.168.0.10```

- **Step 7** : Setup the Raspberry with ```sudo raspi-config``` and change the hostname et activate the wireless by setting up just the country. Then reboot.

- **Step 8** : execute ```export TERM=xterm-256color``` and then modify your *.bashrc* and put the same line at the end. And scp onboard folder.

- **Step 9** : Launch utils/install.sh

- **Step 10** : Start ad-hoc (IBSS) connection and setup for the netstack with utils/setup.sh

- **Step 11** : You can now build and launch the netstack. (onboard/netstack)

## Documentation

### Work of NETSTACK on Raspian OS (Raspberry Pi OS)

In this section, we will describe in a few words how work lwIP in a linux OS.

#### Introduction

Basicly, lwIP is designed to work on *baremetal* programming, but here, we develop a tool using a linux distro, so we will need to have some tricks to simulate the work of lwIP. 

#### Global structure

***TODO*** insert a schema

#### Configuration of interfaces

In the *onboard/utils/setup.sh* script we configure a **tap0** interface (this is the interface used by our lwIP stack) and a bridge (named **lwipbridge**) to allow our **tap0** interface to communicate with the other physical interfaces.
The IP address of **tap0** is *192.168.1.2/24* defined in our network stack implementation and the IP address of **lwipbridge** is *192.168.1.1/24*. Remember that these addresses should be in the same network. Also note that the default gateway for lwIP is set to *192.168.1.1* and this should correspond to the IP address of the lwipbridge.

#### Explaination of the **FORWARDER**

To allow the communication between 192.168.1.2 and the rest of the network (i.e via the physical interface) we have made a tool (part of our netstack) call **forwarder**. This **forwarder** listen on a specific port and redirect packet to the lwIP stack. The **forwarder** is an adapter between lwIP stack (launch in *userland*) and the actual true stack : the linux kernel network stack. If you intent to modify the netstack, please consider to use **forwarder** if you use an new port for your packets.   

*Note* : if you look deep inside the work of the forwarder, you will notice that if our lwIP send a broadcast message, the forwarder re-emit this broadcast message to the emitter (and that was not intendeed). This problem have been solve in the next section (in the **IRU**).

#### Explaination of the **IRU** (*Internal Router Unit*)

The **IRU** is a unit (threaded) designed to manage packet transmission. The lwIP stack in RAW API is not thread safe and work with *callback* style programming. The **IRU** make the interface between the lwIP RAW API and the netstack processus. Netstack processus design all of the processus on the nano-sat that need to communicate (internal communication and/or external communical). Every processus are registrated at the beginning and each processus can call the public API to send and receive packet. Note that lwIP is not *thread safe* and use *callback* style programming but IRU is design on **sequential programming** and is ***thread safe***. The **IRU** can send and receive packet through the network and use it own routing table to reach the destination. It is also in this module that packet are forward to the destination if the IP address of the destination is not the current IP address. If the source IP address is the current IP address, the packet is ignored. 

#### UDP support

The **IRU** support UDP with an adapter called **UDP_COMM**. This module allow the **IRU** to init, send and receive UDP packets with lwIP

#### TCP support

#### Processus

Each part of our nanosat are indepent processus. We can assimilate that with threaded processus in our simulation. Each processus must be registred in the **IRU** to have a UID and use the lwIP stack via the **IRU** public API. Please consider to share data between processus only by using **IRU** to not bypass the simulation.
 
#### Routing protocol

This part is specific to the routing algorithms implemented in our netstack. You will find some documentation about how it work and how to configure it. 

##### OLSR

***TODO***

## Know issues

***TODO***