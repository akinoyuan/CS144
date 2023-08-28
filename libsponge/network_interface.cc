#include "network_interface.hh"

#include "arp_message.hh"
#include "ethernet_frame.hh"

#include <iostream>

// Dummy implementation of a network interface
// Translates from {IP datagram, next hop address} to link-layer frame, and from link-layer frame to IP datagram

// For Lab 5, please replace with a real implementation that passes the
// automated checks run by `make check_lab5`.

// You will need to add private members to the class declaration in `network_interface.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] ethernet_address Ethernet (what ARP calls "hardware") address of the interface
//! \param[in] ip_address IP (what ARP calls "protocol") address of the interface
NetworkInterface::NetworkInterface(const EthernetAddress &ethernet_address, const Address &ip_address)
    : _ethernet_address(ethernet_address), _ip_address(ip_address) {
    cerr << "DEBUG: Network interface has Ethernet address " << to_string(_ethernet_address) << " and IP address "
         << ip_address.ip() << "\n";
}

//! \param[in] dgram the IPv4 datagram to be sent
//! \param[in] next_hop the IP address of the interface to send it to (typically a router or default gateway, but may also be another host if directly connected to the same network as the destination)
//! (Note: the Address type can be converted to a uint32_t (raw 32-bit IP address) with the Address::ipv4_numeric() method.)
void NetworkInterface::send_datagram(const InternetDatagram &dgram, const Address &next_hop) {
    // convert IP address of next hop to raw 32-bit representation (used in ARP header)
    const uint32_t next_hop_ip = next_hop.ipv4_numeric();
    EthernetFrame frame;
    frame.header().src = _ethernet_address;
    if(_table.count(next_hop_ip)) {       //目标以太网地址已知，立即发送
        // find ip mapping, send frame directly
        frame.payload() = dgram.serialize();
        frame.header().type = EthernetHeader::TYPE_IPv4;
        frame.header().dst = _table[next_hop_ip];
    }else if(_map_status.count(next_hop_ip) == 0){ //未知ip地址，广播ARP请求
        ARPMessage msg;
        msg.sender_ethernet_address = _ethernet_address;
        msg.sender_ip_address = _ip_address.ipv4_numeric();
        msg.target_ethernet_address = EthernetAddress{0};
        msg.target_ip_address = next_hop_ip;
        msg.opcode = ARPMessage::OPCODE_REQUEST;
        frame.payload() = msg.serialize();
        frame.header().type = EthernetHeader::TYPE_ARP;
        frame.header().src = _ethernet_address;
        frame.header().dst = EthernetAddress{ETHERNET_BROADCAST};
        _dgrams_queue.push_back({next_hop, dgram});
        _map_status[next_hop_ip] = -1;
    }else if (_map_status.count(next_hop_ip) && _map_status[next_hop_ip] < 0) {
        _dgrams_queue.push_back(make_pair(next_hop, dgram));
        return;
    }
     _frames_out.push(frame);
}

//! \param[in] frame the incoming Ethernet frame
optional<InternetDatagram> NetworkInterface::recv_frame(const EthernetFrame &frame) {
    EthernetHeader header = frame.header();
    //收到IPv4 datagram
    if (header.type == EthernetHeader::TYPE_IPv4 && frame.header().dst == _ethernet_address) {
        InternetDatagram dgram;
        auto state = dgram.parse(frame.payload());
        if (state == ParseResult::NoError)
            return dgram;
    }
    //收到ARP datagram
    if (header.type == EthernetHeader::TYPE_ARP) {
        ARPMessage msg;
        auto state = msg.parse(frame.payload());
        //解析 ARP message
        if (state == ParseResult::NoError) {
            //学习地址映射
            _table[msg.sender_ip_address] = msg.sender_ethernet_address;
            _map_status[msg.sender_ip_address] = 0;
        }
        //收到ARP_REQUEST，发送ARP回复
        if (msg.opcode == ARPMessage::OPCODE_REQUEST && msg.target_ip_address == _ip_address.ipv4_numeric()) {
            msg.target_ethernet_address = msg.sender_ethernet_address;
            msg.target_ip_address = msg.sender_ip_address;
            msg.sender_ethernet_address = _ethernet_address;
            msg.sender_ip_address = _ip_address.ipv4_numeric();
            msg.opcode = ARPMessage::OPCODE_REPLY;
            EthernetFrame back_frame;
            back_frame.payload() = msg.serialize();
            back_frame.header().type = EthernetHeader::TYPE_ARP;
            back_frame.header().src = _ethernet_address;
            back_frame.header().dst = msg.target_ethernet_address;
            _frames_out.push(back_frame);
        }
    }
    _send_dgrams();       //收到回复后,发送能够发送的数据报
    return nullopt;
}

void NetworkInterface::_send_dgrams() {
    int p = 0;
    for (auto i : _dgrams_queue) {
        if (_map_status.count(i.first.ipv4_numeric()) && _map_status[i.first.ipv4_numeric()] >= 0) {
            send_datagram(i.second, i.first);
            _dgrams_queue.erase(_dgrams_queue.begin() + p);
        }
        ++p;
    }
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void NetworkInterface::tick(const size_t ms_since_last_tick) {
    for (auto i : _map_status) {
        if (_map_status.empty())  // Outbound check
            return;
        if (_map_status[i.first] >= 0) {
            // CLOCK>=0, Check if there are any expired mappings
            _map_status[i.first] += ms_since_last_tick;
            if (_map_status[i.first] >= 30000) {
                _map_status.erase(i.first);
                _table.erase(i.first);
            }
        } else {
            // CLOCK<0, Check if there are any expired ARP requests
            _map_status[i.first] -= ms_since_last_tick;
            if (_map_status[i.first] <= -5001)
                _map_status.erase(i.first);
        }
    }
    _send_dgrams();
}
